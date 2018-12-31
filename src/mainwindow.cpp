#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //  inicijalizuje se scena
    scene = new QGraphicsScene(this);
    ui->view->setScene(scene);

    //  inicijalizuje se objekat za snimanje zvuka

    recorder = new QAudioRecorder();
    QAudioProbe *probe = new QAudioProbe;

    QAudioEncoderSettings audioSettings;
    audioSettings.setCodec("audio/PCM");
    audioSettings.setQuality(QMultimedia::HighQuality);

    recorder->setContainerFormat("mp3");
    recorder->setEncodingSettings(audioSettings);

    //recorder->setOutputLocation(QUrl::fromLocalFile("proba_qt_snimak.mp3"));

    //  funkcija processBuffer se poziva kad recorder ima nove podatke
    probe->setSource(recorder);
    connect(probe, SIGNAL(audioBufferProbed(QAudioBuffer)), this, SLOT(processAudioBuffer(QAudioBuffer)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_recordPushButton_clicked()
{
    // pocinje snimanje zvuka
    recorder->record();
}

void MainWindow::on_stopPushButton_clicked()
{
    // zaustavlja se snimanje zvuka
    recorder->stop();
}

void MainWindow::on_cleanPushButton_clicked()
{
    // brisu se objekti sa scene
    scene->clear();
    scene->views()[0]->update();

    //  brojac se vraca na pocetnu vrednost
    buffer_count = 0;
}

//  funkcija processBuffer se poziva kad recorder ima nove podatke i iscrtava visinu tona
void MainWindow::processAudioBuffer(QAudioBuffer buffer)
{
    std::cout << "----------------------- QAudioBuffer broj " << buffer_count << " ------------------------------ "   << std::endl;

    int n = buffer.frameCount();
    QAudioBuffer::S16U *frames = buffer.data<QAudioBuffer::S16U>();

    //  obradjuju se vrednosti za desni kanal (za levi se dobijaju slicne vrednosti)

    //  prolazak kroz sve okvire i cuvanje max i min ( da bi moglo posle da se skalira )
    double max = frames[0].right;
    double min = frames[0].right;

    for (int i=0; i < n; i++)
    {
        max = (frames[i].right > max) ? frames[i].right : max;
        min = (frames[i].right < min) ? frames[i].right : min;
    }

    //  brza furijeova transformacija (FFT) nad podacima u baferu:

    double input[n];        // ulaz za fft
    fftw_complex output[n]; // izlaz iz fft

    for (int i=0; i<n; i++) {
        double hannWindow = 0.5 * (1 - cos((2 * M_PI * i) / (n - 1)));
        // podaci se skaliraju na [-1, 1] * window function
        input[i] = ( 2/(max-min)*frames[i].right-1)*hannWindow;
     }

    // izvrsava se fft
    fftw_plan myPlan = fftw_plan_dft_r2c_1d(n, input, output, FFTW_ESTIMATE);
    fftw_execute(myPlan);

    double current_magnitude;

    double all_magnitudes_max = 0;
    int all_magn_max_index = 0;

    double part_mag_max = 0;
    double part_mag_min = DBL_MAX;


//    za prikaz svih vrednosti iz izlaza fft:
//    int bin_count = 0;
//    scene->clear();
//    scene->views()[0]->update();

    //  obrada izlaza iz fft
    for(int i=2; i<n/2; i++)    // u prvom je posebna vrednost (DC)
    {
        //Formula:  magnitude = sqrt(re*re+im*im)
        current_magnitude = sqrt(output[i][REAL]*output[i][REAL] + output[i][IMAG]*output[i][IMAG]);

        if (current_magnitude > all_magnitudes_max){
            all_magnitudes_max = current_magnitude;
            all_magn_max_index = i;
        }

        // posmatra se samo mali deo oko sredine, sto je visi ton vece su razlike
        if(i > n*2/16 && i < n*3/16){
            part_mag_max = (current_magnitude > part_mag_max) ? current_magnitude : part_mag_max;
            part_mag_min = (current_magnitude < part_mag_min) ? current_magnitude : part_mag_min;
        }

//        za prikaz svih vrednosti iz izlaza fft:
//        bin_count += 2;
//        scene->addLine(bin_count, 0, bin_count, -5*current_magnitude, QPen(Qt::magenta, 1));

    }

    fftw_destroy_plan(myPlan);
    // iscrtava se razlika iz ovog bafera
    scene->addLine(buffer_count, -(part_mag_max-part_mag_min)*SCALING_MAG, buffer_count, -(part_mag_max-part_mag_min)*SCALING_MAG+1, QPen(Qt::cyan, 1));

    //  azurira se poslednjih 30 razlika
    if (buffer_count > 30)
        recent_magnitudes.erase(recent_magnitudes.begin());
    recent_magnitudes.push_back(part_mag_max-part_mag_min);

    //  iscrtava se prosek poslednjih 30 razlika
    double average_magn = std::accumulate(recent_magnitudes.begin(), recent_magnitudes.end(), 0.0);
    average_magn /= recent_magnitudes.size();

    scene->addLine(buffer_count, -average_magn*SCALING_MAG, buffer_count, -average_magn*SCALING_MAG+1, QPen(Qt::darkBlue, 2));


    //  Formula za frekvenciju:  freq = i_max * sampleRate / n
    //  ne radi dobro
    double frequency =  ((double)buffer.format().sampleRate())*all_magn_max_index/(n); //*(M_PI/2); ako je u rad/s

    std::cout << "Frekvencija?: \t"   << frequency  << std::endl;
    std::cout << "Maksimun i minimum na manjem delu: \t" << part_mag_max << "\t" << part_mag_min << std::endl;
    std::cout << "Maksimum svih:       \t"   << all_magnitudes_max   << std::endl;
    std::cout << "Indeks maksimuma:    \t"   << all_magn_max_index   << std::endl;

    std::cout << "sample rate   \t"   << buffer.format().sampleRate() << std::endl;
    std::cout << "chanel count  \t"   << buffer.format().channelCount() << std::endl;
    std::cout << "byteCount     \t" << buffer.byteCount() << std::endl;
    std::cout << "sample count  \t" << buffer.sampleCount() << std::endl;
    std::cout << "duration      \t" << buffer.duration() <<std::endl;
    std::cout << "frame count   \t" << buffer.frameCount() << std::endl;

    buffer_count++;

}


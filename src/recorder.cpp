#include "recorder.h"

Recorder::Recorder(QObject *parent = nullptr, QGraphicsScene *scene = nullptr)
: QObject(parent), m_scene(scene)
{
    recorder = new QAudioRecorder();
    QAudioProbe *probe = new QAudioProbe;

    QAudioEncoderSettings audioSettings;
//    u slucaju da mikrofon ne prepoznaje zakomentarisati liniju ispod
    audioSettings.setCodec("audio/PCM");
    audioSettings.setQuality(QMultimedia::HighQuality);
//    u slucaju da mikrofon ne prepoznaje zakomentarisati liniju ispod
    recorder->setContainerFormat("mp3");
    recorder->setEncodingSettings(audioSettings);

    //recorder->setOutputLocation(QUrl::fromLocalFile("proba_qt_snimak.mp3"));

    //  funkcija processBuffer se poziva kad recorder ima nove podatke
    probe->setSource(recorder);
    QObject::connect(probe, SIGNAL(audioBufferProbed(QAudioBuffer)), this, SLOT(processAudioBuffer(QAudioBuffer)));

}

void Recorder::startRecording(double x, double y)
{
    player_x = x;
    player_y = y;
    recorder->record();
    is_recording = true;
}

std::vector<QPoint> Recorder::stopRecording()
{
    recorder->stop();
    buffer_count = 0;
    recent_magnitudes.resize(0);
    is_recording = false;
    start_magnitude = 0;
    // buffer na pocetku unosa je los pa ga ignorisemo
    int skip = 20;
    int _ = 0;
    for (QPoint point: line_dots){
        if (_ < skip){
        _++;
        }
        else{
            m_scene->addLine(point.x(), point.y(), point.x(), point.y()+1, QPen(Qt::red, 3));
        }
    }
    return line_dots;
}

bool Recorder::get_is_recording()
{
    return is_recording;
}

void Recorder::set_is_recording(bool b)
{
    is_recording = b;
}

void Recorder::processAudioBuffer(QAudioBuffer buffer)
{
    qDebug() << "-----------------------QAudioBuffer br  " << buffer_count << " ------------------------------ ";

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
    double part_mag_min = 2000;


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

    if (buffer_count > 20)
        m_scene->addLine(player_x + buffer_count, -start_magnitude*SCALING_MAG+(part_mag_max)*SCALING_MAG, player_x + buffer_count, -start_magnitude*SCALING_MAG+(part_mag_max)*SCALING_MAG+1, QPen(Qt::cyan, 1));

    //  pamti se poslednjih 30
    if (buffer_count > 30)
        recent_magnitudes.erase(recent_magnitudes.begin());
    recent_magnitudes.push_back(part_mag_max);


    //  iscrtava se prosek poslednjih 30
    double average_magn = std::accumulate(recent_magnitudes.begin(), recent_magnitudes.end(), 0.0);
    average_magn /= recent_magnitudes.size();

    recent_avg_magn.push_back(average_magn);

    if (buffer_count == 20)
        start_magnitude = average_magn;
    if (buffer_count > 20)
        m_scene->addLine(player_x + buffer_count, -start_magnitude*SCALING_MAG+average_magn*SCALING_MAG, player_x + buffer_count, -start_magnitude*SCALING_MAG+average_magn*SCALING_MAG+1, QPen(Qt::darkBlue, 2));

    // dodaje se nova tacka za liniju kretanja

    double max_magn_line = *std::max_element(recent_avg_magn.begin(), recent_avg_magn.end());
    int i_max_magn_line = recent_avg_magn.end() -std::max_element(recent_avg_magn.begin(), recent_avg_magn.end());
    qDebug() << "tacka za liniju br: " << i_max_magn_line;

//    if (buffer_count > 30 && buffer_count % LINE_EPOCH == 0 ){
    line_dots.push_back(QPoint(player_x + buffer_count - i_max_magn_line, max_magn_line*SCALING_MAG-start_magnitude*SCALING_MAG));
    recent_avg_magn.resize(0);
//    }


    //  Formula za frekvenciju:  freq = i_max * sampleRate / n
    //  ne radi dobro
    double frequency =  ((double)buffer.format().sampleRate())*all_magn_max_index/(n); //*(M_PI/2); ako je u rad/s

    qDebug() << "Frekvencija?: \t"   << frequency;
    qDebug() << "Maksimun i minimum na manjem delu: \t" << part_mag_max << "\t" << part_mag_min;
    qDebug() << "Maksimum svih       \t"   << all_magnitudes_max;
    qDebug() << "Indeks maksimuma    \t"   << all_magn_max_index;

    qDebug() << "sample rate   \t"   << buffer.format().sampleRate();
    qDebug() << "chanel count  \t"   << buffer.format().channelCount();
    qDebug() << "byteCount     \t" << buffer.byteCount();
    qDebug() << "sample count  \t" << buffer.sampleCount();
    qDebug() << "duration      \t" << buffer.duration();
    qDebug() << "frame count   \t" << buffer.frameCount();

    buffer_count++;

}


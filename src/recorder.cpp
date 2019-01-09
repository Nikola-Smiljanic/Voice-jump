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

    // brisu se sve linije sa scene
    QList<QGraphicsItem *> all = m_scene->items();
    for(QGraphicsItem *i : all){
        if (i->type() == QGraphicsLineItem::Type){
            m_scene->removeItem(i);
            delete(i);
        }
    }
    // vraca se linija za x osu
    m_scene->addLine(-200, 0, 5000, 0, QPen(Qt::black, 1));

    std::vector<QPoint> move_dots = line_dots;
    line_dots.resize(0);

    std::vector<QPoint>::iterator it;
    for (it = move_dots.begin()+20; it+5 < move_dots.end(); it+=5)
        if ((*it).y()>=0){
            m_scene->addLine((*it).x(), (*it).y(), (*(it+5)).x(), (*(it+5)).y(), QPen(Qt::magenta, 2, Qt::DotLine));
        }
        else{
            // u slucaju da je frekv duboka crtaj ispod po x osi
            m_scene->addLine((*it).x(), 0, (*(it+5)).x(), 0, QPen(Qt::magenta, 2, Qt::DotLine));
        }

    //  dodaje se tacka koja na kraju linije vraca igraca do x ose
    if(move_dots.back().y() != 0)
        move_dots.push_back(QPoint(move_dots.back().x(), 0));
    m_scene->addLine(it->x(), it->y(), move_dots.rbegin()->x(), move_dots.rbegin()->y(), QPen(Qt::magenta, 2, Qt::DotLine));

    return move_dots;
}

bool Recorder::get_is_recording()
{
    return is_recording;
}

void Recorder::set_is_recording(bool b)
{
    is_recording = b;
}


//  brza furijeova transformacija (FFT):
double Recorder::fft_calculation(double input[], int n){

    fftw_complex output[n]; // izlaz iz fft

    // izvrsava se fft
    fftw_plan myPlan = fftw_plan_dft_r2c_1d(n, input, output, FFTW_ESTIMATE);
    fftw_execute(myPlan);

    double current_magnitude;

    double part_mag_max = 0;
    double part_mag_min = 2000;

    //  obrada izlaza iz fft
    for(int i=2; i<n/2; i++)    // u prvom je posebna vrednost (DC)
    {
        // posmatra se samo mali deo oko sredine, sto je visi ton vece su razlike
        if(i > n*2/16 && i < n*3/16){

            //Formula:  magnitude = sqrt(re*re+im*im)
            current_magnitude = sqrt(output[i][REAL]*output[i][REAL] + output[i][IMAG]*output[i][IMAG]);

            part_mag_max = (current_magnitude > part_mag_max) ? current_magnitude : part_mag_max;
            part_mag_min = (current_magnitude < part_mag_min) ? current_magnitude : part_mag_min;
        }

    }

    fftw_destroy_plan(myPlan);

    return part_mag_max;
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

    double input[n];    // ulaz za fft

    //  priprema podataka za fft

    for (int i=0; i<n; i++) {
        double hannWindow = 0.5 * (1 - cos((2 * M_PI * i) / (n - 1)));
        // podaci se skaliraju na [-1, 1] * window function
        input[i] = ( 2/(max-min)*frames[i].right-1)*hannWindow;
     }

    //  fft
    double magnitudes_max = fft_calculation(input, n);

    //  prikazuje se magnitudes_max za ovaj QAudioBuffer, samo za proveru, obrisati na kraju:
    if (buffer_count > 20)
        m_scene->addLine(player_x + buffer_count, (magnitudes_max-start_magnitude)*SCALING_MAG,
                         player_x + buffer_count, (magnitudes_max-start_magnitude)*SCALING_MAG + 1,
                         QPen(Qt::cyan, 1));


    //  pamti se poslednjih 30 magnitudes_max
    if (buffer_count > 30)
        recent_magnitudes.erase(recent_magnitudes.begin());
    recent_magnitudes.push_back(magnitudes_max);

    //  racuna se prosek poslednjih 30
    double average_magn = std::accumulate(recent_magnitudes.begin(), recent_magnitudes.end(), 0.0);
    average_magn /= recent_magnitudes.size();

    //  20-a vrednost (iz 20-og QAudioBuffer-a) se uzima kao pocetna i ona se oduzima od ostalih (uvek krece od 0)
    if (buffer_count == 20)
        start_magnitude = average_magn;

    //  iscrtava se linija od prethodne tacke do ove
    if (buffer_count > 20){
        if(line_dots.back().y()>0){
            m_scene->addLine(line_dots.back().x(), line_dots.back().y(),
                             player_x + buffer_count, (average_magn-start_magnitude)*SCALING_MAG+1,
                             QPen(Qt::blue, 1));
        }
        else{
            // frek. ispod x ose boji crvenom bojom
            m_scene->addLine(line_dots.back().x(), line_dots.back().y(),
                             player_x + buffer_count, (average_magn-start_magnitude)*SCALING_MAG+1,
                             QPen(Qt::red, 1));

        }
    }
    // dodaje se nova tacka za liniju kretanja
    line_dots.push_back(QPoint(player_x + buffer_count, (average_magn-start_magnitude)*SCALING_MAG));

    qDebug() << "Maksimum na manjem delu: \t" << magnitudes_max;
    qDebug() << "sample rate   \t"   << buffer.format().sampleRate();
    qDebug() << "chanel count  \t"   << buffer.format().channelCount();
    qDebug() << "duration      \t" << buffer.duration();
    qDebug() << "frame count   \t" << buffer.frameCount();

    buffer_count++;

}


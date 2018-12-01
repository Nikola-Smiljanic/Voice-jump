#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    ui->view->setScene(scene);

    //prvi nacin prikazivanja (dugme record1)

    recorder = new QAudioRecorder();
    QAudioProbe *probe = new QAudioProbe;

    QAudioEncoderSettings audioSettings;
    audioSettings.setCodec("audio/PCM");

    recorder->setContainerFormat("mp3");
    audioSettings.setQuality(QMultimedia::HighQuality);
    recorder->setEncodingSettings(audioSettings);
    recorder->setOutputLocation(QUrl::fromLocalFile("proba_qt_snimak.mp3"));

    connect(probe, SIGNAL(audioBufferProbed(QAudioBuffer)), this, SLOT(processBuffer(QAudioBuffer)));

    probe->setSource(recorder);


    //drugi nacin prikazivanja (dugme record2)

    QAudioDeviceInfo deviceInfo = QAudioDeviceInfo::defaultInputDevice();

    QAudioFormat formatAudio;
    formatAudio.setSampleRate(8000);
    formatAudio.setChannelCount(1);
    formatAudio.setSampleSize(8);
    formatAudio.setCodec("audio/pcm");
    formatAudio.setByteOrder(QAudioFormat::LittleEndian);
    formatAudio.setSampleType(QAudioFormat::UnSignedInt);
    m_audioInput = new QAudioInput(deviceInfo, formatAudio, this);

    m_device = new XYSeriesIODevice(this, scene);
    m_device->open(QIODevice::WriteOnly);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_rec1PushButton_clicked()
{
    recorder->record();
}

void MainWindow::on_stop1PushButton_clicked()
{
    recorder->stop();
}

void MainWindow::on_rec2PushButton_clicked()
{
    m_audioInput->start(m_device);
}

void MainWindow::on_stop2PushButton_clicked()
{
    m_audioInput->stop();

}

void MainWindow::on_cleanPushButton_clicked()
{
    scene->clear();
    scene->views()[0]->update();
    x_count = 0;
}


void MainWindow::processBuffer(const QAudioBuffer &buffer){

    // TODO prikaz frekvencije a ne proseka vrednosti
    if(buffer.isValid())
        std::cout << "*" << std::endl;
    std::cout << buffer.byteCount() << std::endl;
    std::cout << buffer.sampleCount() << std::endl;
    std::cout << buffer.duration() <<std::endl;
    std::cout << buffer.frameCount() << std::endl;

    const quint16 *data = buffer.constData<quint16>();
    qint16 br = 0;
    quint16 sum = 0;

    for(int i=0; i< (buffer.sampleCount()/16); i++, data+=16){
        std::cout << "_";
        br = *data;
        std::cout << br;
        sum+=br;
    }
    std::cout << std::endl << "prosek: " << sum/(buffer.sampleCount()/16) << std::endl;

    // iscrtava se prosek vrednosti kao uspravna linija
    scene->addLine( x_count, 0, x_count, -sum/(buffer.sampleCount()/4), QPen(Qt::blue));
    x_count++;
}


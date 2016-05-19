#include <QCoreApplication>
#include <QAudioOutput>
#include <QFile>
#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <QAudio>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QFile sourceFile;   // class member.
    QAudioOutput* audio; // class member.
    {
        sourceFile.setFileName("C:/Users/VAsilisk/Desktop/ENG_1/Scorpions.mp3");
        sourceFile.open(QIODevice::ReadOnly);

        QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
        //        qWarning() << info.supportedCodecs();

        QAudioFormat format;
        // Set up the format, eg.
        format.setSampleRate(8000);
        format.setChannelCount(1);
        format.setSampleSize(8);

        format.setCodec("audio/pcm");
        format.setByteOrder(QAudioFormat::LittleEndian);
        format.setSampleType(QAudioFormat::UnSignedInt);

        if (!info.isFormatSupported(format))
        {
            qWarning() << "Raw audio format not supported by backend, cannot play audio.";
            return 0;
        }

        audio = new QAudioOutput(format);
        //connect(audio, SIGNAL(stateChanged(QAudio::State)), this, SLOT(handleStateChanged(QAudio::State)));
        audio->start(&sourceFile);
    }

    return a.exec();
}

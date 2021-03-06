#ifndef PROG1_H
#define PROG1_H
#include <QString>
#include <QIODevice>
#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <QAudioOutput>

class Generator : public QIODevice
{
    Q_OBJECT

public:
    Generator(const QAudioFormat &format, qint64 durationUs, int sampleRate, QObject *parent);
    ~Generator();

    void start();
    void stop();
signals:
    //void stateChanged(QAudio::State newState);
    void finished();
public:
    qint64 readData(char *data, qint64 maxlen);
    qint64 writeData(const char *data, qint64 len);
    qint64 bytesAvailable() const;

private:
    void generateData(const QAudioFormat &format, qint64 durationUs, int sampleRate);

private:
    qint64 m_freq;
    qint64 m_pos;
    QByteArray m_buffer;
    int  m_sampleRate;
    qint64  m_durationUs;
    const QAudioFormat & m_format;
};



class Player : public QAudioOutput
{
    Q_OBJECT
public:
    Player();
    //~Player();

private:
    void initializeAudio();

private:

    QAudioDeviceInfo m_device;
    Generator *m_generator_0;
    Generator *m_generator_1;
    QAudioOutput *m_audioOutput;
    QIODevice *m_output; // not owned
    QAudioFormat m_format;
    QByteArray m_buffer;
public slots:
    void nextBit();
    void stateChanged(QAudio::State newState);
public:
     Player(QString sequence);
     int bitIndex;
     void play();

private:
    QString str;

};
#endif //PROG1_H

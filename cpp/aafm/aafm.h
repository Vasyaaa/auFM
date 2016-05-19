#ifndef AAFM_H
#define AAFM_H
#include <QString>
#include <QAudioFormat>
#include <QIODevice>
class Generator : public QIODevice
{


public:
    Generator(const QAudioFormat &format, qint64 durationUs, int sampleRate, QObject *parent);
    ~Generator();

    void start();
    void stop();

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



class Player
{
public:
     Player(QString sequence);
     void play();

private:
    QString str;




};
#endif // AAFM_H

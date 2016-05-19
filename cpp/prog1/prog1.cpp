
#include <iostream>
#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <QAudioOutput>
#include <QDebug>
#include <qmath.h>
#include <qendian.h>

#include "prog1.h"

using namespace std;



const int DurationSeconds = 1000000*1;
const int ToneSampleRateHz0 = 1400;
const int ToneSampleRateHz1 = 2100;
const int DataSampleRateHz = 42000;
const int BufferSize      = 3276800;


Generator::Generator(const QAudioFormat &format, qint64 durationUs,int sampleRate, QObject *parent)

    :   QIODevice(parent)
    ,   m_pos(0)
    ,   m_sampleRate(sampleRate)
    ,   m_durationUs(durationUs)
    ,   m_format(format)
{
    if (format.isValid())
        generateData(format, m_durationUs, m_sampleRate);
}

Generator::~Generator()
{

}

void Generator::start()
{
    open(QIODevice::ReadOnly);
    m_pos = 0;
}

void Generator::stop()
{
    m_pos = 0;
    close();
}

void Generator::generateData(const QAudioFormat &format, qint64 durationUs, int sampleRate)
{
    const int channelBytes = format.sampleSize() / 8;//разрядность
    const int sampleBytes = format.channelCount() * channelBytes;//количество байт умнож на кол-во каналов

    qint64 length = (format.sampleRate() * format.channelCount() * (format.sampleSize() / 8))
                        * durationUs / 1000000.0;

    Q_ASSERT(length % sampleBytes == 0);
    Q_UNUSED(sampleBytes) // suppress warning in release builds

    m_buffer.resize(length);
    unsigned char *ptr = reinterpret_cast<unsigned char *>(m_buffer.data());
    int sampleIndex = 0;

    while (length)
    {
        const qreal x = qSin(2 * M_PI * sampleRate * qreal(sampleIndex % format.sampleRate()) / format.sampleRate());
        for (int i=0; i<format.channelCount(); ++i) {
            if (format.sampleSize() == 8 && format.sampleType() == QAudioFormat::UnSignedInt) {
                const quint8 value = static_cast<quint8>((1.0 + x) / 2 * 255);
                *reinterpret_cast<quint8*>(ptr) = value;
            } else if (format.sampleSize() == 8 && format.sampleType() == QAudioFormat::SignedInt) {
                const qint8 value = static_cast<qint8>(x * 127);
                *reinterpret_cast<quint8*>(ptr) = value;
            } else if (format.sampleSize() == 16 && format.sampleType() == QAudioFormat::UnSignedInt) {
                quint16 value = static_cast<quint16>((1.0 + x) / 2 * 65535);
                if (format.byteOrder() == QAudioFormat::LittleEndian)
                    qToLittleEndian<quint16>(value, ptr);
                else
                    qToBigEndian<quint16>(value, ptr);
            } else if (format.sampleSize() == 16 && format.sampleType() == QAudioFormat::SignedInt) {
                qint16 value = static_cast<qint16>(x * 32767);
                if (format.byteOrder() == QAudioFormat::LittleEndian)
                    qToLittleEndian<qint16>(value, ptr);
                else
                    qToBigEndian<qint16>(value, ptr);
            }

            ptr += channelBytes;
            length -= channelBytes;
        }
        ++sampleIndex;
    }
}

qint64 Generator::readData(char *data, qint64 len)
{
    qint64 total = 0;
    if (!m_buffer.isEmpty())
    {
        while (len - total > 0)
        {
            const qint64 chunk = qMin((m_buffer.size() - m_pos), len - total);

            memcpy(data + total, m_buffer.constData() + m_pos, chunk);
            m_pos = (m_pos + chunk);
            total += chunk;

            if (chunk < len)
               emit finished();
        }
        return total;
    }
}

qint64 Generator::writeData(const char *data, qint64 len)
{
    Q_UNUSED(data);
    Q_UNUSED(len);

    return 0;
}

qint64 Generator::bytesAvailable() const
{
    return m_buffer.size() + QIODevice::bytesAvailable();
}

Player::Player(QString sequence)

    :   QAudioOutput()
    ,   m_device(QAudioDeviceInfo::defaultOutputDevice())
    ,   m_generator_0(0)
    ,   m_generator_1(0)
    ,   m_audioOutput(0)
    ,   m_output(0)
    ,   m_buffer(BufferSize, 0)
    {
        str = sequence;
        initializeAudio();

    }

    void Player::initializeAudio()
    {
        m_format.setSampleRate(DataSampleRateHz);
        m_format.setChannelCount(1);
        m_format.setSampleSize(16);
        m_format.setCodec("audio/pcm");
        m_format.setByteOrder(QAudioFormat::LittleEndian);
        m_format.setSampleType(QAudioFormat::SignedInt);

        QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());//проверка воспроизведения
        if (!info.isFormatSupported(m_format))
        {
            qWarning() << "Default format not supported - trying to use nearest";
            m_format = info.nearestFormat(m_format);
        }

        m_generator_0 = new Generator(m_format, DurationSeconds, ToneSampleRateHz0, NULL);
        m_generator_1 = new Generator(m_format, DurationSeconds, ToneSampleRateHz1, NULL);
        connect(m_generator_0, SIGNAL(finished()), this, SLOT(nextBit()));
        connect(m_generator_1, SIGNAL(finished()), this, SLOT(nextBit()));

        m_audioOutput = 0;
        m_audioOutput = new QAudioOutput(m_device, m_format);
        connect(m_audioOutput, SIGNAL(stateChanged(QAudio::State)), this, SLOT(stateChanged(QAudio::State)));

    }

    void Player::stateChanged(QAudio::State state)
    {
        int tmp = 0;
    }
        void Player::nextBit()
        {
          m_audioOutput->stop();
          m_generator_0->stop();
          m_generator_1->stop();


          if (bitIndex < str.size())
          {
               qDebug() << "generating data" << str.at(bitIndex);
            if (str.at(bitIndex) == '0')
            {
                m_generator_0->start();
                m_audioOutput->start(m_generator_0);

            }
            if (str.at(bitIndex) == '1')
            {
                m_generator_1->start();
                m_audioOutput->start(m_generator_1);
               //для каждого генератора
            }
          }
            else
                qDebug() << "stop";
            bitIndex++;

        }
//_________________________________
void Player::play()
{

    bitIndex = 0;
    nextBit();

}

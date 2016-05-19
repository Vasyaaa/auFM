#include <iostream>
#include "aafm.h"

#include <QAudioOutput>
#include <qmath.h>
#include <qendian.h>

using namespace std;

const int DurationSeconds = 100000;
const int ToneSampleRateHz0 = 1400;//как мне подставить их? пока разбираюсь с этим 
const int ToneSampleRateHz1 = 2100;
const int DataSampleRateHz = 42000;
const int BufferSize      = 32768;

Generator::Generator(const QAudioFormat &format, qint64 durationUs, int sampleRate, QObject *parent)
    :   QIODevice(parent)
    ,   m_pos(0) // 1) не знаю что эта за функция 
    ,   m_sampleRate(sampleRate) // 2) частота  дискретизации или сигнала? 
    ,   m_durationUs(durationUs) // 3) длительность в микросек
    ,   m_format(format) // сколько байт в одно одной дискрете
{
    if (format.isValid()) // метод,  который выполняет фактическую проверку допустимости
        generateData(format, m_durationUs, m_sampleRate);
}

Generator::~Generator() //деструктор
{

}

void Generator::start()
{
    open(QIODevice::ReadOnly);
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
                        * durationUs / 1000000;

    Q_ASSERT(length % sampleBytes == 0);
    Q_UNUSED(sampleBytes) // suppress warning in release builds

    m_buffer.resize(length);
    unsigned char *ptr = reinterpret_cast<unsigned char *>(m_buffer.data());
    int sampleIndex = 0;

    while (length)
    {
        const qreal x = qSin(2 * M_PI * sampleRate * qreal(sampleIndex % format.sampleRate()) / format.sampleRate());
        for (int i=0; i<format.channelCount(); ++i)
        {
            if (format.sampleSize() == 8 && format.sampleType() == QAudioFormat::UnSignedInt)
            {
                const quint8 value = static_cast<quint8>((1.0 + x) / 2 * 255);
                *reinterpret_cast<quint8*>(ptr) = value;
            }
            else if (format.sampleSize() == 8 && format.sampleType() == QAudioFormat::SignedInt)
            {
                const qint8 value = static_cast<qint8>(x * 127);
                *reinterpret_cast<quint8*>(ptr) = value;
            }
            else if (format.sampleSize() == 16 && format.sampleType() == QAudioFormat::UnSignedInt)
            {
                quint16 value = static_cast<quint16>((1.0 + x) / 2 * 65535);
                if (format.byteOrder() == QAudioFormat::LittleEndian)
                    qToLittleEndian<quint16>(value, ptr);
                else
                    qToBigEndian<quint16>(value, ptr);
            }
            else if (format.sampleSize() == 16 && format.sampleType() == QAudioFormat::SignedInt)
            {
                qint16 value = static_cast<qint16>(x * 32767);
                if (format.byteOrder() == QAudioFormat::LittleEndian)
                    qToLittleEndian<qint16>(value, ptr);
                else
                    qToBigEndian<qint16>(value, ptr);
            }

            ptr += channelBytes;//кол-во каналов
            length -= channelBytes;
        }
        ++sampleIndex;
    }
}
//____________________________________

qint64 Generator::readData(char *data, qint64 len) // y
{
    qint64 total = 0;
    if (!m_buffer.isEmpty()) {
        while (len - total > 0) {
            const qint64 chunk = qMin((m_buffer.size() - m_pos), len - total);
            memcpy(data + total, m_buffer.constData() + m_pos, chunk);
            m_pos = (m_pos + chunk) % m_buffer.size();
            total += chunk;
        }
    }
    return total;
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


//____________________________________


Player::Player(QString sequence)
{
    str = sequence;
}


void Player::play()
{
    for (int i = 0; i < str.size(); ++i)
    {
        if (str.at(i) == '0')
            cout  << "play 1400Hz \n" << endl;
        if (str.at(i) == '1')
            cout  << "play 2100Hz \n " << endl;
    }



}


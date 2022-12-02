#include "waveformthread.h"

WaveformThread::WaveformThread()
{
    m_abort = false;
}

WaveformThread::~WaveformThread()
{
    mutex.lock();
    m_abort = true;
    mutex.unlock();

    wait();
}

void WaveformThread::storeSamples(const QVector<double> &samples)
{
    m_samplesL = samples;
}

void WaveformThread::storeSamples(const QVector<double> &samplesR, const QVector<double> &samplesL)
{
    m_samplesR = samplesR;
    m_samplesL = samplesL;
}

void WaveformThread::deleteSamples()
{
    stopProcess();
    m_samplesL.clear();
    m_samplesR.clear();
}

void WaveformThread::setChannelCount(int channels)
{
    m_channelCount = channels;
}

void WaveformThread::processSamples(qreal window_wdith, qreal window_height, QColor wave_color, QColor background_color)
{

    m_width = window_wdith;
    m_heihgt = window_height;
    m_wave_color = wave_color;
    m_background_color = background_color;

    m_abort = false;
    start();
}

void WaveformThread::run()
{
    int numberOfSamples = m_samplesL.size();
    float xScale = (float)m_width / (numberOfSamples);
    float center = (float)m_heihgt / 2;
    QImage waveImage = QImage(m_width, m_heihgt, QImage::Format_RGBA64);
    waveImage.fill(m_background_color);
    QPainter painter(&waveImage);
    painter.setPen(QPen(m_wave_color, 1, Qt::SolidLine, Qt::RoundCap));
    int counter = 0;

    for(int i = 0; i < numberOfSamples; i++){
        if (m_abort)
            break;
        if (m_channelCount == 1)
            painter.drawRect(i * xScale, center - (m_samplesL[i] * center), 2, (m_samplesL[i] * center) * 2);
        else if (m_channelCount == 2)
        {
            painter.drawRect(i * xScale, (center / 2) - (m_samplesL[i] * (center / 2)), 2, (m_samplesL[i] * (center / 2)) * 2);
            painter.drawRect(i * xScale, ((center / 2) * 3) - (m_samplesL[i] * (center / 2)), 2, (m_samplesL[i] * (center / 2)) * 2);
        }
        counter++;
    }
    emit waveformReady(waveImage);
}

void WaveformThread::stopProcess()
{
    mutex.lock();
    m_abort = true;
    mutex.unlock();
}

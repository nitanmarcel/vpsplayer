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
    m_samples = samples;
}

void WaveformThread::deleteSamples()
{
    stopProcess();
    m_samples.clear();
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
    int numberOfSamples = m_samples.size();
    float xScale = (float)m_width / (numberOfSamples);
    float center = (float)m_heihgt / 2;
    QImage waveImage = QImage(m_width, m_heihgt, QImage::Format_RGB16);
    waveImage.fill(m_background_color);
    QPainter painter(&waveImage);
    painter.setPen(QPen(m_wave_color, 1, Qt::SolidLine, Qt::RoundCap));

    for(int i = 0; i < numberOfSamples; i++){
        if (m_abort)
            break;
        painter.drawRect(i * xScale, center - (m_samples[i] * center), 2, (m_samples[i] * center) * 2);
    }
    emit waveformReady(waveImage);
}

void WaveformThread::stopProcess()
{
    mutex.lock();
    m_abort = true;
    mutex.unlock();
}

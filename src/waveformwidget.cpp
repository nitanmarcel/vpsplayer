#include "waveformwidget.h"

WaveformWidget::WaveformWidget()
{
    waveformReady = false;
    m_pixLabel = new QLabel(this);
    m_pixLabel->show();
    this->m_paintTimer = new QTimer(this);
    connect(this->m_paintTimer, &QTimer::timeout, this, &WaveformWidget::drawWave);
    m_paintTimer->setInterval(100);
    m_paintTimer->start();
}

WaveformWidget::~WaveformWidget()
{

}

void WaveformWidget::setFormat(QAudioFormat format)
{
    audioFormat = format;
}

void WaveformWidget::setReady(bool ready)
{
    waveformReady = ready;
}

void WaveformWidget::appendSamples(QAudioBuffer buffer)
{
    QAudioBuffer::S32F *data = buffer.data<QAudioBuffer::S32F>();
    qreal peakValue = qreal(1.00003); // sample type is always float
    int count = buffer.sampleCount() / 2;
    for (int i = 0; i < count; i += 1102){
        double val = data[i].left/peakValue;
        samples.append(val);
    }
}

void WaveformWidget::clearSamples()
{
    samples.clear();
}

void WaveformWidget::drawWave()
    {
        if (!waveformReady)
            return;
        int numberOfSamples = samples.size();
        int count = numberOfSamples / 2;
        float xScale = (float)width() / count;
        float center = (float)height() / 2;
        m_pixMap = QPixmap(size());
        m_pixMap.scaled(size());
        m_pixMap.fill(Qt::black);
        QPainter painter(&m_pixMap);
        painter.setPen(Qt::red);
        int currIndex = m_pixMap.rect().x();
        for(int i = 0; i < numberOfSamples; ++i){
            painter.drawRect(i * xScale, center - (samples[i] * center), 2, (samples[i] * center) * 2);
            currIndex++;
        }

        m_pixMap.scaled(width(), height());
        m_pixLabel->setPixmap(m_pixMap);
        m_pixLabel->resize(width(), height());
    }

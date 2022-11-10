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
    qreal peak = getPeakValue(buffer.format());
    const qint16 *data = buffer.constData<qint16>();
    int count = buffer.sampleCount() / 2;
    for (int i = 0; i < count; i += 1200){ // This works fine for 48000Hz but not for 441000 and
        double val = data[i]/peak;
        samples.append(val);
    }
}

void WaveformWidget::clearSamples()
{
    samples.clear();
}

qreal WaveformWidget::getPeakValue(const QAudioFormat& format)
{
    // Note: Only the most common sample formats are supported
    if (!format.isValid())
        return qreal(0);

    if (format.codec() != "audio/pcm")
        return qreal(0);

    switch (format.sampleType()) {
    case QAudioFormat::Unknown:
        break;
    case QAudioFormat::Float:
        if (format.sampleSize() != 32) // other sample formats are not supported
            return qreal(0);
        return qreal(1.00003);
    case QAudioFormat::SignedInt:
        if (format.sampleSize() == 32)
#ifdef Q_OS_WIN
            return qreal(INT_MAX);
#endif
#ifdef Q_OS_UNIX
            return qreal(SHRT_MAX);
#endif
        if (format.sampleSize() == 16)
            return qreal(SHRT_MAX);
        if (format.sampleSize() == 8)
            return qreal(CHAR_MAX);
        break;
    case QAudioFormat::UnSignedInt:
        if (format.sampleSize() == 32)
            return qreal(UINT_MAX);
        if (format.sampleSize() == 16)
            return qreal(USHRT_MAX);
        if (format.sampleSize() == 8)
            return qreal(UCHAR_MAX);
        break;
    }

    return qreal(0);
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

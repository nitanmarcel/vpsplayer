#include "waveformwidget.h"
#include "math.h"

WaveformWidget::WaveformWidget()
{

    m_isClickable = false;
    m_areSamplesReady = false;
    m_pixLabel = new QLabel(this);
    m_pixLabel->show();

    thread = new WaveformThread();
    connect(thread, &WaveformThread::waveformReady, this, &WaveformWidget::setWaveImage);


    this->m_paintTimer = new QTimer(this);
    m_paintTimer->setInterval(100);
    connect(this->m_paintTimer, &QTimer::timeout, this, &WaveformWidget::drawWave);

    m_paintTimer->start();
}

WaveformWidget::~WaveformWidget()
{

}

void WaveformWidget::setClickable(bool clickable)
{
  if (clickable)
    setCursor(Qt::PointingHandCursor);
  else {
    setCursor(Qt::ForbiddenCursor);
    if (underMouse())
      QToolTip::hideText();
  }

  setMouseTracking(clickable);

  m_isClickable = clickable;
}

void WaveformWidget::mousePressEvent(QMouseEvent *event)
{
  if ((event->button() == Qt::RightButton) && m_isClickable)
      if ((event->x() > this->m_breakPointPos + 3) || (event->x() < this->m_breakPointPos - 3))
      {
          m_breakPointPos = event->x();
          this->m_hasBreakPoint = true;
          emit breakPointSet(QStyle::sliderValueFromPosition(minimum(), maximum(), event->x(), width()));
          this->m_updateBreakPointRequired = true;
      }
      else
       {
          this->m_hasBreakPoint = false;
          this->m_breakPointPos = 0;
          emit breakPointRemoved();
          this->m_updateBreakPointRequired = true;
      }
  else if ((event->button() == Qt::LeftButton) && m_isClickable)
      emit barClicked(event->x() > 5 ? QStyle::sliderValueFromPosition(minimum(), maximum(), event->x(), width()) : 0);

  event->accept();
}

void WaveformWidget::mouseMoveEvent(QMouseEvent *event)
{
  event->accept();
}

void WaveformWidget::resetBreakPoint()
{
    m_updateBreakPointRequired = true;
    m_hasBreakPoint = false;
}

void WaveformWidget::setBreakPoint(int pos)
{
    m_breakPointPos = pos / (maximum() / width());
    m_updateBreakPointRequired = true;
}

int WaveformWidget::getBreakPoint()
{
    return m_breakPointPos * (maximum() / width());
}

void WaveformWidget::appendSamples(QAudioBuffer buffer)
{
    QAudioBuffer::S32F *data = buffer.data<QAudioBuffer::S32F>();
    qreal peakValue = getPeakValue(buffer.format()); // sample type is always float
    int count = buffer.sampleCount() / 2;
    int sampleIncrement = 2;
    if (buffer.format().sampleRate() == 48000)
        sampleIncrement = 1200; // 40 samples/second
    if (buffer.format().sampleRate() == 44100)
        sampleIncrement = 1050; // 42 samples/second
    if (buffer.format().sampleRate() == 22050)
        sampleIncrement = 525; // 42 samples/second

    for (int i = 0; i < buffer.frameCount(); i++){
        double val = data[i].left/peakValue;
        m_samples.append(val);
    }
}

void WaveformWidget::clearSamples()
{
    m_areSamplesReady = false;
    m_samples.clear();
    thread->deleteSamples();
}

void WaveformWidget::setSamplesReady()
{
    m_areSamplesReady = true;
    thread->storeSamples(m_samples);
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

void WaveformWidget::setWaveImage(QImage waveform)
{
    m_waveImage = waveform;
}


void WaveformWidget::drawWave()
    {
        m_pixMap = QPixmap(size());
        m_pixMap.fill(m_waveformBackgroundColor);

        if (m_samples.empty())
        {
            m_waveImage = QImage(size(), QImage::Format_RGB16);
            m_waveImage.fill(m_waveformBackgroundColor);
        }

        m_pixMap.convertFromImage(m_waveImage);
        if (m_areSamplesReady)
        {
            thread->processSamples(width(), height(), m_waveformColor, m_waveformBackgroundColor);
        }


        QPainter painter(&m_pixMap);

        if (this->m_breakPointPos > 0 && this->m_hasBreakPoint)
        {
            painter.setPen(QPen(Qt::darkGray, 2, Qt::SolidLine, Qt::RoundCap));
            painter.drawLine(m_breakPointPos, 0, m_breakPointPos, 1000);
        }

        if (value() > 0)
        {
            painter.setPen(QPen(m_progressColor, 2, Qt::SolidLine, Qt::RoundCap));
            painter.drawLine(QStyle::sliderPositionFromValue(minimum(), maximum(), value(), width()), 0, QStyle::sliderPositionFromValue(minimum(), maximum(), value(), width()), 1000);
        }

        m_pixMap.scaled(width(), height());
        m_pixLabel->setPixmap(m_pixMap);
        m_pixLabel->resize(width(), height());
    }

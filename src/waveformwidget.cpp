#include "waveformwidget.h"
#include "math.h"

WaveformWidget::WaveformWidget()
{
    qRegisterMetaType<QVector<float> >("QVector<float>");

    is_clickable = false;
    m_pixLabel = new QLabel(this);
    m_pixLabel->show();

    builder = new WaveformBuilder();


    this->m_paintTimer = new QTimer(this);
    m_paintTimer->setInterval(100);
    connect(this->m_paintTimer, &QTimer::timeout, this, &WaveformWidget::paint);
    connect(builder, &WaveformBuilder::dataReady, this, &WaveformWidget::processData);

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

  is_clickable = clickable;
}

void WaveformWidget::mousePressEvent(QMouseEvent *event)
{
  if ((event->button() == Qt::RightButton) && is_clickable)
      if ((event->x() > breakpoint_pos + 3) || (event->x() < breakpoint_pos - 3))
      {
          breakpoint_pos = event->x();
          this->has_breakpoint = true;
          emit breakPointSet(QStyle::sliderValueFromPosition(minimum(), maximum(), event->x(), width()));
          this->update_breakpoint = true;
      }
      else
       {
          this->has_breakpoint = false;
          this->breakpoint_pos = 0;
          emit breakPointRemoved();
          this->update_breakpoint = true;
      }
  else if ((event->button() == Qt::LeftButton) && is_clickable)
      emit barClicked(event->x() > 5 ? QStyle::sliderValueFromPosition(minimum(), maximum(), event->x(), width()) - 50 : 0);

  event->accept();
}

void WaveformWidget::mouseMoveEvent(QMouseEvent *event)
{
  emit barClicked(event->x() > 5 ? QStyle::sliderValueFromPosition(minimum(), maximum(), event->x(), width()) - 50 : 0);
  event->accept();
}

void WaveformWidget::resizeEvent(QResizeEvent *e)
{
    QAbstractSlider::resizeEvent(e);
    builder->setProprieties(-1, -1, -1, width(), height());
}

void WaveformWidget::resetBreakPoint()
{
    update_breakpoint = true;
    has_breakpoint = false;
}

void WaveformWidget::setBreakPoint(int pos)
{
    breakpoint_pos = pos / (maximum() / width());
    update_breakpoint = true;
}

int WaveformWidget::getBreakPoint()
{
    if (has_breakpoint)
        return QStyle::sliderValueFromPosition(minimum(), maximum(), breakpoint_pos, width()) - 50;
    return 0;
}

void WaveformWidget::onBufferReady(QAudioBuffer buffer)
{
    builder->processBuffer(buffer, reset);
    reset = false;
}

void WaveformWidget::onDecodingFinished(int channel_count, long sample_rate, long duration)
{
    builder->setProprieties(channel_count, sample_rate, duration, width(), height());
    reset = true; // make sure we reset next time
}

void WaveformWidget::processData(QVector<double> left_rms, QVector<double> right_rms, QVector<double>  left_average, QVector<double>  right_average, int channel_count)
{
    rms_left = left_rms;
    rms_right = right_rms;
    average_left = left_average;
    average_right = right_average;
    channels = channel_count;
}

double WaveformWidget::calcScareFactor(QVector<double>& wf1, const QVector<double>& wf2)
{
    // Find the maximum value of the two waveforms
    double maxValue = std::max(
        *std::max_element(wf1.begin(), wf1.end()),
        *std::max_element(wf2.begin(), wf2.end())
    );

    // Calculate the scare factor as the maximum value of the two waveforms
    return maxValue;
}

void WaveformWidget::paint()
{
    size_t nb_data = rms_left.size();
    if (nb_data == 0)
        return;

    m_pixMap = QPixmap(size());
    m_pixMap.fill(waveform_background_color);
    QPainter painter(&m_pixMap);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    QVector<double> rmsL = rms_left;
    QVector<double> rmsR = rms_right;

    QVector<double> avrL = average_left;
    QVector<double> avrR = average_right;

    long center = height() / 2;

    double avr_scale_factor = calcScareFactor(avrL, avrR);
    double rms_scale_factor = calcScareFactor(rmsL, rmsR);

    int counter = 0;

    for (int i = 0; i < avrL.size(); ++i)
    {
        if (channels == 1)
        {
            // LEFT

            if (QStyle::sliderValueFromPosition(minimum(), maximum(), i, width()) < value())
            {
                painter.setPen(QPen(waveform_progress_color, 1, Qt::SolidLine, Qt::FlatCap));
            }
            else
            {
                painter.setPen(QPen(waveform_color, 1, Qt::SolidLine, Qt::FlatCap));
            }

            painter.drawLine(counter, center, counter, center - avrL[i] / avr_scale_factor * center / 2);
            painter.drawLine(counter, center, counter, center + avrL[i] / avr_scale_factor * center / 2);

            // RMS
            if (QStyle::sliderValueFromPosition(minimum(), maximum(), i, width()) < value())
            {
                painter.setPen(QPen(waveform_progress_color.lighter(), 1, Qt::SolidLine, Qt::FlatCap));
            }
            else
            {
                painter.setPen(QPen(waveform_color.lighter(), 1, Qt::SolidLine, Qt::FlatCap));
            }

            painter.drawLine(counter, center, counter, center - rmsL[i] / rms_scale_factor * center / 6);
            painter.drawLine(counter, center, counter, center + rmsL[i] / rms_scale_factor * center / 6);
        }
        else if (channels == 2)
        {
            // LEFT

            if (QStyle::sliderValueFromPosition(minimum(), maximum(), i, width()) < value())
            {
                painter.setPen(QPen(waveform_progress_color, 1, Qt::SolidLine, Qt::FlatCap));
            }
            else
            {
                painter.setPen(QPen(waveform_color, 1, Qt::SolidLine, Qt::FlatCap));
            }

            painter.drawLine(i, (center / 2), i, (center / 2) - avrL[i] / avr_scale_factor * center / 2);
            painter.drawLine(i, (center / 2), i, (center / 2) + avrL[i] / avr_scale_factor * center / 2);

            // RMS
            if (QStyle::sliderValueFromPosition(minimum(), maximum(), i, width()) < value())
            {
                painter.setPen(QPen(waveform_progress_color.lighter(), 1, Qt::SolidLine, Qt::FlatCap));
            }
            else
            {
                painter.setPen(QPen(waveform_color.lighter(), 1, Qt::SolidLine, Qt::FlatCap));
            }

            painter.drawLine(i, (center / 2), i, (center / 2) - rmsL[i] / rms_scale_factor * center / 6);
            painter.drawLine(i, (center / 2), i, (center / 2) + rmsL[i] / rms_scale_factor * center / 6);


            // RIGHT

            if (QStyle::sliderValueFromPosition(minimum(), maximum(), i, width()) < value())
            {
                painter.setPen(QPen(waveform_progress_color, 1, Qt::SolidLine, Qt::FlatCap));
            }
            else
            {
                painter.setPen(QPen(waveform_color, 1, Qt::SolidLine, Qt::FlatCap));
            }

            painter.drawLine(i, (center / 2) * 3, i, (center / 2) * 3 - avrR[i] / avr_scale_factor * center / 2);
            painter.drawLine(i, (center / 2) * 3, i, (center / 2) * 3 + avrR[i] / avr_scale_factor * center / 2);

            // RMS
            if (QStyle::sliderValueFromPosition(minimum(), maximum(), i, width()) < value())
            {
                painter.setPen(QPen(waveform_progress_color.lighter(), 1, Qt::SolidLine, Qt::FlatCap));
            }
            else
            {
                painter.setPen(QPen(waveform_color.lighter(), 1, Qt::SolidLine, Qt::FlatCap));
            }

            painter.drawLine(i, (center / 2) * 3, i, (center / 2) * 3 - rmsR[i] / rms_scale_factor * center / 6);
            painter.drawLine(i, (center / 2) * 3, i, (center / 2) * 3 + rmsR[i] / rms_scale_factor * center / 6);
        }
        counter++;
    }

    if (value() > 0)
    {
        painter.setPen(QPen(waveform_progress_color, 1, Qt::SolidLine, Qt::FlatCap));
        painter.drawLine(QStyle::sliderPositionFromValue(minimum(), maximum(), value(), width()), 0, QStyle::sliderPositionFromValue(minimum(), maximum(), value(), width()), 1000);
    }

    if (breakpoint_pos > 0 && has_breakpoint)
    {
        painter.setPen(QPen(Qt::darkGray, 2, Qt::SolidLine, Qt::RoundCap));
        painter.drawLine(breakpoint_pos, 0, breakpoint_pos, 1000);
    }

    m_pixMap.scaled(width(), height());
    m_pixLabel->setPixmap(m_pixMap);
    m_pixLabel->resize(width(), height());

    update_breakpoint = false;
}

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
      emit barClicked(event->x() > 5 ? QStyle::sliderValueFromPosition(minimum(), maximum(), event->x(), width()) : 0);

  event->accept();
}

void WaveformWidget::mouseMoveEvent(QMouseEvent *event)
{
  event->accept();
}

void WaveformWidget::resizeEvent(QResizeEvent *e)
{
    QAbstractSlider::resizeEvent(e);
    auto i = builder->wait();
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
        return breakpoint_pos * (maximum() / width());
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

void WaveformWidget::processData(QVector<float> left_rms, QVector<float> right_rms, QVector<float>  left_average, QVector<float>  right_average, int channel_count)
{
    rms_left = left_rms;
    rms_right = right_rms;
    average_left = left_average;
    average_right = right_average;
    channels = channel_count;
}

double WaveformWidget::getMaxPeak(QVector<float> v)
{
    double max = v.at(0);

    if (v.size() == 0)
        return max;
    for (int i = 0; i < v.size(); i++)
    {
        if (v[i] > max)
            max = v[i];
    }
    return max;
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

    QVector<float> rmsL = rms_left;
    QVector<float> rmsR = rms_right;

    QVector<float> avrL = average_left;
    QVector<float> avrR = average_right;

    long center = height() / 2;

    double peak = getMaxPeak(avrL);
    double scale_factor = 1.0/peak;
    double padding = 0.4;
    if (channels == 2)
        padding = 0.5;
    scale_factor = scale_factor - scale_factor / padding;

    int counter = 0;

    // FIXME

    painter.setPen(QPen(waveform_color, 2, Qt::SolidLine, Qt::FlatCap));

    if (channels == 1)
    {
        painter.drawLine(counter, center, width(), center);
    }
    else if (channels == 2)
    {
        QLineF lines[2] = {
            QLineF(counter, center - (center / 2), width(), center - (center / 2)),
            QLineF(counter, center + (center / 2), width(), center + (center / 2))
        };

        painter.drawLines(lines, 2);
    }

    //

    for (int i = 0; i < avrL.size(); ++i)
    {
       if (QStyle::sliderValueFromPosition(minimum(), maximum(), i, width()) < value())
       {
           painter.setPen(QPen(waveform_progress_color.lighter(), 1, Qt::SolidLine, Qt::FlatCap));
       }
       else
       {
           painter.setPen(QPen(waveform_color.lighter(), 1, Qt::SolidLine, Qt::FlatCap));
       }

      if (channels == 1)
      {
          QLineF pointsAVRGL[2] = {
              QLineF(counter, center, counter, center+((this->height()/4)*avrL[i]*scale_factor)),
              QLineF(counter, center, counter, center-((this->height()/4)*avrL[i]*scale_factor))
          };

          QLineF pointsRMSL[3] = {
              QLineF(counter, center, counter, center+((this->height()/4)*rmsL[i]*scale_factor / 2)),
              QLineF(counter, center, counter, center-((this->height()/4)*rmsL[i]*scale_factor / 2)),
          };

          if (QStyle::sliderValueFromPosition(minimum(), maximum(), i, width()) < value())
          {

              // Draw avearge Point
              painter.setPen(QPen(waveform_progress_color, 1, Qt::SolidLine, Qt::FlatCap));

              // FIXME
              painter.setPen(QPen(waveform_progress_color.lighter(), 2, Qt::SolidLine, Qt::FlatCap));
              painter.drawLine(counter - 1, center - (center / 2), counter, center - (center / 2));
              //

              painter.drawLines(pointsAVRGL, 3);

              // Draw RMS
              painter.setPen(QPen(waveform_progress_color.lighter(), 1, Qt::SolidLine, Qt::FlatCap));
              painter.drawLines(pointsRMSL, 2);
          }
          else
          {
              // Draw avearge Point
              painter.setPen(QPen(waveform_color, 1, Qt::SolidLine, Qt::FlatCap));
              painter.drawLines(pointsAVRGL, 3);

              // Draw RMS
              painter.setPen(QPen(waveform_color.lighter(), 1, Qt::SolidLine, Qt::FlatCap));
              painter.drawLines(pointsRMSL, 2);
          }
      }
      if (channels == 2)
      {
                QLineF pointsAVRGL[2] = {
                    QLineF(counter, center - (center / 2), counter, center - (center / 2)+((this->height()/4)*avrL[i]*scale_factor)),
                    QLineF(counter, center - (center / 2), counter, center - (center / 2)-((this->height()/4)*avrL[i]*scale_factor))
                };

                QLineF pointsRMSL[2] = {
                    QLineF(counter, center - (center / 2), counter, center - (center / 2)+((this->height()/4)*rmsL[i]*scale_factor / 2)),
                    QLineF(counter, center - (center / 2), counter, center - (center / 2)-((this->height()/4)*rmsL[i]*scale_factor / 2)),
                };

                if (QStyle::sliderValueFromPosition(minimum(), maximum(), i, width()) < value())
                {
                    // Draw avearge Point
                    painter.setPen(QPen(waveform_progress_color, 1, Qt::SolidLine, Qt::FlatCap));
                    painter.drawLines(pointsAVRGL, 2);

                    // FIXME
                    painter.setPen(QPen(waveform_progress_color.lighter(), 2, Qt::SolidLine, Qt::FlatCap));
                    painter.drawLine(counter - 1, center - (center / 2), counter, center - (center / 2));
                    //

                    // Draw RMS
                    painter.setPen(QPen(waveform_progress_color.lighter(), 1, Qt::SolidLine, Qt::FlatCap));
                    painter.drawLines(pointsRMSL, 2);
                }
                else
                {
                    // Draw avearge Point
                    painter.setPen(QPen(waveform_color, 1, Qt::SolidLine, Qt::FlatCap));
                    painter.drawLines(pointsAVRGL, 2);

                    // Draw RMS
                    painter.setPen(QPen(waveform_color.lighter(), 1, Qt::SolidLine, Qt::FlatCap));
                    painter.drawLines(pointsRMSL, 2);
                }

                QLineF pointsAVRGR[2] = {
                    QLineF(counter, center + (center / 2), counter, center + (center / 2)+((this->height()/4)*avrR[i]*scale_factor)),
                    QLineF(counter, center + (center / 2), counter, center + (center / 2)-((this->height()/4)*avrR[i]*scale_factor))
                };

                QLineF pointsRMSR[2] = {
                    QLineF(counter, center + (center / 2), counter, center + (center / 2)+((this->height()/4)*rmsR[i]*scale_factor / 2)),
                    QLineF(counter, center + (center / 2), counter, center + (center / 2)-((this->height()/4)*rmsR[i]*scale_factor / 2))
                };

                if (QStyle::sliderValueFromPosition(minimum(), maximum(), i, width()) < value())
                {

                    // Draw avearge Point
                    painter.setPen(QPen(waveform_progress_color, 1, Qt::SolidLine, Qt::FlatCap));
                    painter.drawLines(pointsAVRGR, 2);

                    // FIXME
                    painter.setPen(QPen(waveform_progress_color.lighter(), 2, Qt::SolidLine, Qt::FlatCap));
                    painter.drawLine(counter - 1, center + (center / 2), counter, center + (center / 2));
                    //

                    // Draw RMS
                    painter.setPen(QPen(waveform_progress_color.lighter(), 1, Qt::SolidLine, Qt::FlatCap));
                    painter.drawLines(pointsRMSR, 2);
                }
                else
                {
                    // Draw avearge Point
                    painter.setPen(QPen(waveform_color, 1, Qt::SolidLine, Qt::FlatCap));
                    painter.drawLines(pointsAVRGR, 2);

                    // Draw RMS
                    painter.setPen(QPen(waveform_color.lighter(), 1, Qt::SolidLine, Qt::FlatCap));
                    painter.drawLines(pointsRMSR, 2);
                }

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

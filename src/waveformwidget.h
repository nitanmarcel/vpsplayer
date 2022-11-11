#ifndef WAVEFORMWIDGET_H
#define WAVEFORMWIDGET_H

#include <QAbstractSlider>
#include <QPainter>
#include <QPixmap>
#include <QLabel>
#include <QTimer>
#include <QObject>
#include <QtGlobal>
#include <QAudioFormat>
#include <QAudioDecoder>
#include <QAudioDeviceInfo>
#include <QAudioBuffer>
#include <QToolTip>
#include <QMouseEvent>
#include "QStyle"

class WaveformWidget : public QAbstractSlider
{
    Q_PROPERTY( QColor waveformColor MEMBER m_waveformColor );
    Q_PROPERTY( QColor waveformProgressColor MEMBER m_progressColor );
    Q_PROPERTY( QColor waveformBackgroundColor MEMBER m_waveformBackgroundColor );

    Q_OBJECT
public:
    WaveformWidget();
    ~WaveformWidget();
    void setFormat(QAudioFormat format);
    void appendSamples(QAudioBuffer buffer);
    void clearSamples();
    void setClickable(bool clickable);
    void resetBreakPoint();
    void setBreakPoint(int pos);
    int getBreakPoint();
private:
    QPixmap m_pixMap;
    QLabel *m_pixLabel;
    QTimer *m_paintTimer ;
    QVector<double> m_samples;

    bool m_isClickable;
    int m_drawingIndex;
    bool m_updateBreakPointRequired;
    bool m_hasBreakPoint;
    int m_breakPointPos;

    void drawWave();
    qreal getPeakValue(const QAudioFormat& format);

    // colors
    QColor m_waveformColor { Qt::blue };
    QColor m_progressColor { QColor(246, 134, 86) };
    QColor m_waveformBackgroundColor { Qt::transparent };
protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event);
signals:
    void barClicked(int);
    void breakPointRemoved();
    int breakPointSet(int position);
};

#endif // WAVEFORMWIDGET_H

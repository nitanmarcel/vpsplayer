#ifndef WAVEFORMWIDGET_H
#define WAVEFORMWIDGET_H

#include <QAbstractSlider>
#include <QPainter>
#include <QPixmap>
#include <QImage>
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
#include <QtConcurrent>
#include <QMetaType>
#include <QResizeEvent>

#include "waveformbuilder.h"
#include <algorithm>

class WaveformWidget : public QAbstractSlider
{
    Q_PROPERTY( QColor waveformColor MEMBER waveform_color );
    Q_PROPERTY( QColor waveformProgressColor MEMBER waveform_progress_color );
    Q_PROPERTY( QColor waveformBackgroundColor MEMBER waveform_background_color );

    Q_OBJECT
public:
    WaveformWidget();
    ~WaveformWidget();

    void onBufferReady(QAudioBuffer buffer);
    void onDecodingFinished(int channel_count, long sample_rate, long duration);
    void setProprieties(int _channel_count, long _sample_rate, long _duration);

    void setClickable(bool clickable);
    void resetBreakPoint();
    void setBreakPoint(int pos);
    int getBreakPoint();

private:
    QPixmap m_pixMap;
    QLabel *m_pixLabel;
    QTimer *m_paintTimer;

    QVector<double> rms_left;
    QVector<double> rms_right;

    QVector<double> average_left;
    QVector<double> average_right;

    int channels;

    WaveformBuilder *builder;

    bool is_clickable;
    bool is_breakpoint_changed;

    int breakpoint_position;

    bool update_breakpoint;
    int breakpoint_pos;
    bool has_breakpoint;

    bool reset = false;

    QVector<double> samples_left;
    QVector<double> samples_right;

    QColor waveform_color { QColor(50,50,200) };
    QColor waveform_progress_color { QColor(246, 134, 86) };
    QColor waveform_background_color { QColor(192,192,192) };

    Q_PROPERTY( QColor waveformColor MEMBER waveform_color );
    Q_PROPERTY( QColor waveformProgressColor MEMBER waveform_progress_color );
    Q_PROPERTY( QColor waveformBackgroundColor MEMBER waveform_background_color );

    void processData(QVector<double> left_rms, QVector<double> right_rms, QVector<double>  left_average, QVector<double>  right_average, int channel_count);
    void paint();

    double getMaxPeak(QVector<float> v);
    double calcScareFactor(QVector<double>& wf1, const QVector<double>& wf2);
protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    virtual void resizeEvent(QResizeEvent *);
signals:
    void barClicked(int);
    void breakPointRemoved();
    int breakPointSet(int position);
};

#endif // WAVEFORMWIDGET_H

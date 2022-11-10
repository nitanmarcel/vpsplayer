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

class WaveformWidget : public QAbstractSlider
{
    Q_OBJECT
public:
    WaveformWidget();
    ~WaveformWidget();
    void setFormat(QAudioFormat format);
    void setReady(bool ready);
    void appendSamples(QAudioBuffer buffer);
    void clearSamples();
private:
    QPixmap m_pixMap;
    QLabel *m_pixLabel;
    QTimer *m_paintTimer ;
    QVector<double> samples;
    QAudioFormat audioFormat;

    void drawWave();
    qreal getPeakValue(const QAudioFormat& format);

    bool waveformReady;
};

#endif // WAVEFORMWIDGET_H

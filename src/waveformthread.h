#ifndef WAVEFORMTHREAD_H
#define WAVEFORMTHREAD_H

#include <QMutex>
#include <QThread>
#include <QWidget>
#include <QImage>
#include <QVector>
#include <QPainter>

class WaveformThread : public QThread
{
    Q_OBJECT
public:
    WaveformThread();
    ~WaveformThread();
    void storeSamples(const QVector<double> &samples);
    void deleteSamples();
    void processSamples(qreal window_wdith, qreal window_height, QColor wave_color, QColor background_color);
    void stopProcess();
private:
    QMutex mutex;
    bool m_abort;

    QVector<double> m_samples;
    qreal m_width;
    qreal m_heihgt;
    QColor m_wave_color;
    QColor m_background_color;
protected:
    void run();
signals:
    void waveformReady(QImage);
};

#endif // WAVEFORMTHREAD_H

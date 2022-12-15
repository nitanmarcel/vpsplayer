#ifndef WAVEFORMBUILDER_H
#define WAVEFORMBUILDER_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QAudioBuffer>
#include <QAudioFormat>

#include "math.h"

class WaveformBuilder : public QThread
{
    Q_OBJECT
public:
    WaveformBuilder(QObject *parent = nullptr);
    ~WaveformBuilder();
    void setProprieties(int _channel_count, long _sample_rate, long _duration, long _window_width, long _window_height);
    void processBuffer(QAudioBuffer buffer, bool reset_before_processing);
    void stopProcess();
    void reset();
private:

    float qsqrt(float n);

    std::vector<double> samples_left;
    std::vector<double> samples_right;

    int channel_count = 0;
    long sample_rate = 0;
    long duration = 0;
    long window_width = 0;
    long window_height = 0;

    QMutex mutex;
    bool abort = false;
    bool recalculate = false;
protected:
    void run();

signals:
    void dataReady(QVector<double>  left_rms, QVector<double>  right_rms, QVector<double>  left_average, QVector<double>  right_average, int channel_count);
};

#endif // WAVEFORMBUILDER_H

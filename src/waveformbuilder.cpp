#include "waveformbuilder.h"

#include <QDebug>

WaveformBuilder::WaveformBuilder(QObject *parent)
    : QThread{parent}
{
    mutex.lock();
    abort = false;
    mutex.unlock();
}

WaveformBuilder::~WaveformBuilder()
{
    mutex.lock();
    abort = true;
    mutex.unlock();
    wait();

}

void WaveformBuilder::setProprieties(int _channel_count, long _sample_rate, long _duration, long _window_width, long _window_height)
{
    if (_channel_count > -1)
        channel_count = _channel_count;
    if (_sample_rate > -1)
        sample_rate = _sample_rate;
    if (_duration > -1)
        duration = _duration;
    if (_window_width > -1)
        window_width = _window_width;
    if (_window_height > -1)
        window_height = _window_height;
    start();
}

void WaveformBuilder::processBuffer(QAudioBuffer buffer, bool reset_before_processing)
{
    if (reset_before_processing == true)
    {
        samples_left.clear();
        samples_right.clear();
    }
    QAudioFormat format = buffer.format();
    QAudioBuffer::S32F *data = buffer.data<QAudioBuffer::S32F>();
    double peak_value = double(1.00003);
    for (int i = 0; i < buffer.frameCount(); i++) {
        samples_left.push_back(abs(data[i].left)/peak_value);
        samples_right.push_back(abs(data[i].right)/peak_value);
    }
}

void WaveformBuilder::stopProcess()
{
    mutex.lock();
    abort = true;
    mutex.unlock();

}

void WaveformBuilder::reset()
{
    samples_left.clear();
    samples_right.clear();
}

void WaveformBuilder::run()
{
    QVector<double> rms_left;
    QVector<double> rms_right;

    QVector<double> average_left;
    QVector<double> average_right;

    const long samples_width = static_cast<long>((duration / 1000) * sample_rate);
    const long long samples_per_pixel = static_cast<int>(samples_width / window_width);

    int nb_samples = samples_left.size();

    for (int i = 0; i < nb_samples; i+=samples_per_pixel)
    {
        if (abort == true)
        {
            abort = false;
            break;
        }
        float sum_left = 0;
        float sum_right = 0;

        float squared_sum_left = 0;
        float squared_sum_right = 0;

        double* sample_chunk_left = &samples_left[i];
        double* sample_chunk_right = &samples_right[i];

        for (int j = 0; j < samples_per_pixel; j++)
        {
            if (sample_chunk_left[j] < 0)
                sum_left += -sample_chunk_left[j];
            else
                sum_left += sample_chunk_left[j];
            squared_sum_left += sample_chunk_left[j] * sample_chunk_left[j];

            if (channel_count == 2)
            {
                if (sample_chunk_right[j] < 0)
                    sum_right += -sample_chunk_right[j];
                else
                    sum_right += sample_chunk_right[j];
                squared_sum_right += sample_chunk_right[j] * sample_chunk_right[j];
            }
        }

        double mean_left = squared_sum_left / samples_per_pixel;
        double mean_right = squared_sum_right / samples_per_pixel;

        double rms_point_left = qsqrt(mean_left);
        double rms_point_right = qsqrt(mean_right);

        rms_left.push_back(rms_point_left);
        rms_right.push_back(rms_point_right);

        double average_point_left = (sum_left * 2) / samples_per_pixel;
        double average_point_right = (sum_right * 2) / samples_per_pixel;

        average_left.push_back(average_point_left);
        average_right.push_back(average_point_right);
    }
    emit dataReady(rms_left, rms_right, average_left, average_right, channel_count);
}

float WaveformBuilder::qsqrt(float n)
{
    // QUAKE 3
    const float xhalf = 0.5f*n;

    union // get bits for floating value
    {
      float n;
      int i;
    } u;
    u.n = n;
    u.i = 0x5f3759df - (u.i >> 1);  // gives initial guess y0
    return n*u.n*(1.5f - xhalf*u.n*u.n);// Newton step, repeating increases accuracy
}

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
    mutex.lock();
    QVector<float> rms_left;
    QVector<float> rms_right;

    QVector<float> average_left;
    QVector<float> average_right;

    const long samples_width = static_cast<long>((duration / 1000) * sample_rate);
    const long long samples_per_pixel = static_cast<int>(samples_width / window_width);

    int nb_samples = samples_left.size();

    int nr = 0;

    for (int i = 0; i < nb_samples; i+=samples_per_pixel)
    {
        if (abort == true)
            break;
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

        float mean_left = squared_sum_left / samples_per_pixel;
        float mean_right = squared_sum_right / samples_per_pixel;

        float rms_point_left = std::sqrt(mean_left);
        float rms_point_right = std::sqrt(mean_right);

        rms_left.push_back(rms_point_left);
        rms_right.push_back(rms_point_right);

        float average_point_left = (sum_left * 2) / samples_per_pixel;
        float average_point_right = (sum_right * 2) / samples_per_pixel;

        average_left.push_back(average_point_left);
        average_right.push_back(average_point_right);

        nr+= samples_per_pixel;
    }
    emit dataReady(rms_left, rms_right, average_left, average_right, channel_count);
    mutex.unlock();
}

float WaveformBuilder::rsqrt(float n)
{
    // https://en.wikipedia.org/wiki/Fast_inverse_square_root
    long i;
    float x2, y;
    const float threehalfs = 1.5f;

    x2 = n * 0.5F;
    y  = n;
    i  = * ( long * ) &y;
    i  = 0x5f3759df - ( i >> 1 );
    y  = * ( float * ) &i;
    y  = y * ( threehalfs - ( x2 * y * y ) );

    return y;
}

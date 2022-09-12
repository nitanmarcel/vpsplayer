#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QObject>
#include <QSettings>
#include <QStringList>

class AppSettings : public QObject
{
    Q_OBJECT
public:
    AppSettings(QObject *parent = nullptr);
    ~AppSettings();
    void setHighQuality(bool enabled);
    void setPerserveFormatShape(bool enabled);
    void setEngineIndex(int index);
    void setShowWaveform(bool enabled);
    void setSpeedModifierValue(int value);
    void setPitchModifierValue(int value);
    void setPauseKey(int key);
    void setPitchSliderKeyPlus(int key);
    void setPitchSliderKeyMinus(int key);
    void setSpeedSliderKeyPlus(int key);
    void setSpeedSliderKeyMinus(int key);
    void setPlaybackSliderKeyPlus(int key);
    void setPlaybackSliderKeyMinus(int key);
    bool getHighQuality();
    bool getPerserveFormatShape();
    bool getShowWaveform();
    int getEngineIndex();
    int getPitchModifierValue();
    int getSpeedModifierValue();
    int getPauseKey();
    int getPitchSliderKeyPlus();
    int getPitchSliderKeyMinus();
    int getSpeedSliderKeyPlus();
    int getSpeedSliderKeyMinus();
    int getPlaybackSliderKeyPlus();
    int getPlaybackSliderKeyMinus();

private:
    QSettings *settings;
    int engineIndex;
    bool highQuality;
    bool perserveFormatShape;
    bool showWaveform;
    int pitchModifierValue;
    int speedModifierValue;
    int pauseKey;
    int pitchSliderKeyPlus;
    int pitchSliderKeyMinus;
    int speedSliderKeyPlus;
    int speedSliderKeyMinus;
    int playbackSliderKeyPlus;
    int playbackSliderKeyMinus;
};

#endif // APPSETTINGS_H

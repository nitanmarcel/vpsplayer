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
    void setGeometry(QByteArray geometry);
    void setHighQuality(bool enabled);
    void setPerserveFormatShape(bool enabled);
    void setEngineIndex(int index);
    void setShowWaveform(bool enabled);
    void setSpeedModifierValue(int value);
    void setPitchModifierValue(int value);
    void setModifierKey(int key);
    void setPauseKey(int key);
    void setPauseKeyAlt(int key);
    void setPitchSliderKeyPlus(int key);
    void setPitchSliderKeyMinus(int key);
    void setSpeedSliderKeyPlus(int key);
    void setSpeedSliderKeyMinus(int key);
    void setPlaybackSliderKeyPlus(int key);
    void setPlaybackSliderKeyMinus(int key);
    QByteArray getGeometry();
    bool getHighQuality();
    bool getPerserveFormatShape();
    bool getShowWaveform();
    int getEngineIndex();
    int getPitchModifierValue();
    int getSpeedModifierValue();
    int getModifierKey();
    int getPauseKey();
    int getPauseKeyAlt();
    int getPitchSliderKeyPlus();
    int getPitchSliderKeyMinus();
    int getSpeedSliderKeyPlus();
    int getSpeedSliderKeyMinus();
    int getPlaybackSliderKeyPlus();
    int getPlaybackSliderKeyMinus();

private:
    QSettings *settings;
    QByteArray geometry;
    int engineIndex;
    bool highQuality;
    bool perserveFormatShape;
    bool showWaveform;
    int pitchModifierValue;
    int speedModifierValue;
    int modifierKey;
    int pauseKey;
    int pauseKeyAlt;
    int pitchSliderKeyPlus;
    int pitchSliderKeyMinus;
    int speedSliderKeyPlus;
    int speedSliderKeyMinus;
    int playbackSliderKeyPlus;
    int playbackSliderKeyMinus;
};

#endif // APPSETTINGS_H

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
    bool getHighQuality();
    bool getPerserveFormatShape();
    bool getShowWaveform();
    int getEngineIndex();
    int getPitchModifierValue();
    int getSpeedModifierValue();

private:
    QSettings *settings;
    int engineIndex;
    bool highQuality;
    bool perserveFormatShape;
    bool showWaveform;
    int pitchModifierValue;
    int speedModifierValue;
};

#endif // APPSETTINGS_H

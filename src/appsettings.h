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
    bool getHighQuality();
    bool getPerserveFormatShape();
    bool getShowWaveform();
    int getEngineIndex();

private:
    QSettings *settings;
    int engineIndex;
    bool highQuality;
    bool perserveFormatShape;
    bool showWaveform;
};

#endif // APPSETTINGS_H

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
    void setConvertMono(bool value);
    void setFfmpegPath(QString value);
    void setHighQuality(bool enabled);
    void setPerserveFormatShape(bool enabled);
    void setEngineIndex(int index);
    void setShowWaveform(bool enabled);
    bool getConvertMono();
    bool getHighQuality();
    bool getPerserveFormatShape();
    bool getShowWaveform();
    int getEngineIndex();
    QString getFfmpegPath();

private:
    QSettings *settings;
    bool convertMono;
    QString ffmpegPath;
    int engineIndex;
    bool highQuality;
    bool perserveFormatShape;
    bool showWaveform;
};

#endif // APPSETTINGS_H

#include "appsettings.h"
#include <QStandardPaths>
#include <QDir>

AppSettings::AppSettings(QObject *parent)
    : QObject{parent}
{
    QString settingsPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QDir::separator() + "vpsplayer" + QDir::separator();
    settings =  new QSettings(settingsPath + "settings.ini", QSettings::IniFormat);
    convertMono = settings->value("convert_mono", false).value<bool>();
    ffmpegPath = settings->value("ffmpeg_path", "").value<QString>();
    engineIndex = settings->value("engine_index", 1).value<int>();
    highQuality = settings->value("high_quality", true).value<bool>();
    perserveFormatShape = settings->value("perserve_formant_shape", true).value<bool>();
}

AppSettings::~AppSettings()
{
}

void AppSettings::setConvertMono(bool value)
{
    convertMono = value;
    settings->setValue("convert_mono", value);
    settings->sync();
}

void AppSettings::setFfmpegPath(QString value)
{
    ffmpegPath = value;
    settings->setValue("ffmpeg_path", value);
    settings->sync();
}

void AppSettings::setEngineIndex(int index)
{
    engineIndex = index;
    settings->setValue("engine_index", index);
    settings->sync();
}

void AppSettings::setHighQuality(bool enabled)
{
    highQuality = enabled;
    settings->setValue("high_quality", enabled);
    settings->sync();
}

void AppSettings::setPerserveFormatShape(bool enabled)
{
    perserveFormatShape = enabled;
    settings->setValue("perserve_formant_shape", enabled);
    settings->sync();
}

bool AppSettings::getConvertMono()
{
    return convertMono;
}

QString AppSettings::getFfmpegPath()
{
    return ffmpegPath;
}

int AppSettings::getEngineIndex()
{
    return engineIndex;
}

bool AppSettings::getHighQuality()
{
    return highQuality;
}

bool AppSettings::getPerserveFormatShape()
{
    return perserveFormatShape;
}

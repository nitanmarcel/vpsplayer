#include "appsettings.h"
#include <QStandardPaths>
#include <QDir>

AppSettings::AppSettings(QObject *parent)
    : QObject{parent}
{
    QString settingsPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QDir::separator() + "vpsplayer" + QDir::separator();
    settings =  new QSettings(settingsPath + "settings.ini", QSettings::IniFormat);
    engineIndex = settings->value("engine_index", 1).value<int>();
    highQuality = settings->value("high_quality", true).value<bool>();
    perserveFormatShape = settings->value("preserve_formant_shape", true).value<bool>();
    showWaveform = settings->value("show_waveform", false).value<bool>();
    pitchModifierValue = settings->value("pitch_modifier_value", 5).value<int>();
    speedModifierValue = settings->value("speed_modifier_value", 5).value<int>();

}

AppSettings::~AppSettings()
{
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

void AppSettings::setShowWaveform(bool enabled)
{
    showWaveform = enabled;
    settings->setValue("show_waveform", enabled);
    settings->sync();
}

void AppSettings::setPitchModifierValue(int value)
{
    pitchModifierValue = value;
    settings->setValue("pitch_modifier_value", value);
    settings->sync();
}

void AppSettings::setSpeedModifierValue(int value)
{
    speedModifierValue = value;
    settings->setValue("speed_modifier_value", value);
    settings->sync();
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

bool AppSettings::getShowWaveform()
{
    return showWaveform;
}

int AppSettings::getPitchModifierValue()
{
    return pitchModifierValue;
}

int AppSettings::getSpeedModifierValue()
{
    return speedModifierValue;
}

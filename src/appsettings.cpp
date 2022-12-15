#include "appsettings.h"
#include <QStandardPaths>
#include <QDir>

AppSettings::AppSettings(QObject *parent)
    : QObject{parent}
{
    QString settingsPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QDir::separator() + "vpsplayer" + QDir::separator();
    settings =  new QSettings(settingsPath + "settings.ini", QSettings::IniFormat);

    geometry = settings->value("geometry", QByteArray()).value<QByteArray>();
    engineIndex = settings->value("engine_index", 1).value<int>();
    highQuality = settings->value("high_quality", true).value<bool>();
    perserveFormatShape = settings->value("preserve_formant_shape", true).value<bool>();
    showWaveform = settings->value("show_waveform", true).value<bool>();
    pitchModifierValue = settings->value("pitch_modifier_value", 2).value<int>();
    speedModifierValue = settings->value("speed_modifier_value", 2).value<int>();

    modifierKey = settings->value("modifier_key", Qt::Key_Shift).value<int>();
    pauseKey = settings->value("pause_key", Qt::Key_Space).value<int>();
    pauseKeyAlt = settings->value("pause_key_alt", Qt::Key_Alt).value<int>();
    pitchSliderKeyPlus = settings->value("pitch_slider_key_plus", Qt::Key_W).value<int>();
    pitchSliderKeyMinus = settings->value("pitch_slider_key_minus", Qt::Key_S).value<int>();
    speedSliderKeyPlus = settings->value("speed_slider_key_plus", Qt::Key_D).value<int>();
    speedSliderKeyMinus = settings->value("speed_slider_key_minus", Qt::Key_A).value<int>();
    playbackSliderKeyPlus = settings->value("playback_slider_key_plus", Qt::Key_E).value<int>();
    playbackSliderKeyMinus = settings->value("playback_slider_key_minus", Qt::Key_Q).value<int>();

}

AppSettings::~AppSettings()
{
}

void AppSettings::setGeometry(QByteArray _geometry)
{
    geometry = _geometry;
    settings->setValue("geometry", geometry);
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

void AppSettings::setModifierKey(int key)
{
    modifierKey = key;
    settings->setValue("modifier_key", key);
}

void AppSettings::setPauseKey(int key)
{
    pauseKey = key;
    settings->setValue("pause_key", key);
    settings->sync();
}

void AppSettings::setPauseKeyAlt(int key)
{
    pauseKey = key;
    settings->setValue("pause_key_alt", key);
    settings->sync();
}

void AppSettings::setPitchSliderKeyPlus(int key)
{
    pitchSliderKeyMinus = key;
    settings->setValue("pitch_slider_key_plus", key);
    settings->sync();
}

void AppSettings::setPitchSliderKeyMinus(int key)
{
    pitchSliderKeyMinus = key;
    settings->setValue("pitch_slider_key_minus", key);
    settings->sync();
}

void AppSettings::setSpeedSliderKeyPlus(int key)
{
    speedSliderKeyMinus = key;
    settings->setValue("speed_slider_key_plus", key);
    settings->sync();
}

void AppSettings::setSpeedSliderKeyMinus(int key)
{
    speedSliderKeyMinus = key;
    settings->setValue("speed_slider_key_minus", key);
    settings->sync();
}

void AppSettings::setPlaybackSliderKeyPlus(int key)
{
    playbackSliderKeyMinus = key;
    settings->setValue("playback_slider_key_plus", key);
    settings->sync();
}

void AppSettings::setPlaybackSliderKeyMinus(int key)
{
    playbackSliderKeyMinus = key;
    settings->setValue("playback_slider_key_minus", key);
    settings->sync();
}

QByteArray AppSettings::getGeometry()
{
    return geometry;
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

int AppSettings::getModifierKey()
{
    return modifierKey;
}

int AppSettings::getPauseKey()
{
    return pauseKey;
}

int AppSettings::getPauseKeyAlt()
{
    return pauseKeyAlt;
}

int AppSettings::getPitchSliderKeyPlus()
{
    return pitchSliderKeyPlus;
}

int AppSettings::getPitchSliderKeyMinus()
{
    return pitchSliderKeyMinus;
}

int AppSettings::getSpeedSliderKeyPlus()
{
    return speedSliderKeyPlus;
}

int AppSettings::getSpeedSliderKeyMinus()
{
    return speedSliderKeyMinus;
}

int AppSettings::getPlaybackSliderKeyPlus()
{
    return playbackSliderKeyPlus;
}

int AppSettings::getPlaybackSliderKeyMinus()
{
    return playbackSliderKeyMinus;
}

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QCheckBox>
#include <QKeyEvent>
#include "appsettings.h"
#include "keyedit.h"

class SettingsDialog : public QDialog
{
    Q_OBJECT
private:
    QComboBox *combobox_engine;
    QCheckBox *check_high_quality;
    QCheckBox *check_enable_waveform;
    QWidget *widget_settings;
    AppSettings *app_settings;

    KeyEdit *key_settings_modifier_linedit;
    KeyEdit *key_settings_pause_linedit;
    KeyEdit *key_settings_pause_alt_linedit;
    KeyEdit *key_settings_pitch_plus_linedit;
    KeyEdit *key_settings_pitch_minus_linedit;
    KeyEdit *key_settings_playback_plus_linedit;
    KeyEdit *key_settings_playback_minus_linedit;
    KeyEdit *key_settings_speed_plus_linedit;
    KeyEdit *key_settings_speed_minus_linedit;


    bool waitForKeyInput;
    QString keyInputReciever;

    void emitIndexOptionUseR3EngineChanged(int index);
    void emitCheckUseHighQualityChanged(bool enabled);
    void emitCheckFormantPreservedChanged(bool enabled);
    void emitCheckEnableWaveformChanged(bool enabled);
    void emitPitchModifierValueChanged(int value);
    void emitSpeedModifierValueChanged(int value);
    void emitModifierKeyChanged(int key);
    void emitPauseKey(int key);
    void emitPauseKeyAltChanged(int key);
    void emitPitchSliderKeyPlus(int key);
    void emitPitchSliderKeyMinus(int key);
    void emitSpeedSliderKeyPlus(int key);
    void emitSpeedSliderKeyMinus(int key);
    void emitPlaybackSliderKeyPlus(int key);
    void emitPlaybackSliderKeyMinus(int key);

protected:
    void keyPressEvent(QKeyEvent *event);

public:
    SettingsDialog();
    ~SettingsDialog(); // Destructor

    void open();

signals:
    int indexOptionUseR3EngineChanged(int);
    bool checkUseHighQualityChanged(bool);
    bool checkFormantPreservedChanged(bool);
    bool checkEnableWaveformChanged(bool enabled);
    int pitchModifierValueChanged(int value);
    int speedModifierValueChanged(int value);
    int modifierKeyChanged(int key);
    int pauseKeyChanged(int key);
    int pauseKeyAltChanged(int key);
    int pitchSliderKeyPlusChanged(int key);
    int pitchSliderKeyMinusChanged(int key);
    int speedSliderKeyPlusChanged(int key);
    int speedSliderKeyMinusChanged(int key);
    int playbackSliderKeyPlusChanged(int key);
    int playbackSliderKeyMinusChanged(int key);

};

#endif // SETTINGSDIALOG_H

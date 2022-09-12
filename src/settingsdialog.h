#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include "appsettings.h"

class SettingsDialog : public QDialog
{
    Q_OBJECT
private:
    QComboBox *combobox_engine;
    QCheckBox *check_high_quality;
    QCheckBox *check_enable_waveform;
    QWidget *widget_settings;
    AppSettings *app_settings;

    void emitIndexOptionUseR3EngineChanged(int index);
    void emitCheckUseHighQualityChanged(bool enabled);
    void emitCheckFormantPreservedChanged(bool enabled);
    void emitCheckEnableWaveformChanged(bool enabled);
    void emitPitchModifierValueChanged(int value);
    void emitSpeedModifierValueChanged(int value);
public:
    SettingsDialog();
    ~SettingsDialog(); // Destructor

signals:
    int indexOptionUseR3EngineChanged(int);
    bool checkUseHighQualityChanged(bool);
    bool checkFormantPreservedChanged(bool);
    bool checkEnableWaveformChanged(bool enabled);
    int pitchModifierValueChanged(int value);
    int speedModifierValueChanged(int value);
};

#endif // SETTINGSDIALOG_H

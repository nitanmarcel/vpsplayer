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
public:
    SettingsDialog();
    ~SettingsDialog(); // Destructor

signals:
    int indexOptionUseR3EngineChanged(int);
    bool checkUseHighQualityChanged(bool);
    bool checkFormantPreservedChanged(bool);
    bool checkEnableWaveformChanged(bool enabled);
};

#endif // SETTINGSDIALOG_H

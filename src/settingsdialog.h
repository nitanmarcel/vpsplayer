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
    QLineEdit *ffmpeg_path;
    QCheckBox *check_convert_mono;
    QWidget *widget_settings;
    AppSettings *app_settings;

    void emitIndexOptionUseR3EngineChanged(int index);
    void emitCheckUseHighQualityChanged(bool enabled);
    void emitCheckFormantPreservedChanged(bool enabled);
    void emitFfmpegPathChanged(QString path);
    void emitCheckConvertMonoChanged(bool enabled);
public:
    SettingsDialog(AppSettings *settings);
    ~SettingsDialog(); // Destructor
signals:
    int indexOptionUseR3EngineChanged(int);
    bool checkUseHighQualityChanged(bool);
    bool checkFormantPreservedChanged(bool);
    QString ffmpegPathChanged(QString path);
    bool checkConvertMonoChanged(bool enabled);
};

#endif // SETTINGSDIALOG_H

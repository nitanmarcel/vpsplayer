#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QCheckBox>

class SettingsDialog : public QDialog
{
    Q_OBJECT
private:
    QComboBox *combobox_engine;
    QCheckBox *check_high_quality;
    QWidget *settings;
public:
    SettingsDialog();
    ~SettingsDialog(); // Destructor
signals:
    int indexOptionUseR3EngineChanged(int);
    bool checkUseHighQualityChanged(bool);
    bool checkFormantPreservedChanged(bool);
};

#endif // SETTINGSDIALOG_H

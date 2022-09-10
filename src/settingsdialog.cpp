#include "settingsdialog.h"
#include "appsettings.h"
#include <QLabel>
#include <QLineEdit>
#include <QGroupBox>
#include <QHBoxLayout>

#include <QDebug>
SettingsDialog::SettingsDialog()
{

    app_settings = new AppSettings(this);

    QGroupBox *groupbox_general_settings = new QGroupBox("General");
    QLabel *label_engine = new QLabel("Engine");
    combobox_engine = new QComboBox;
    combobox_engine->addItem("Rubber Band R2 (faster)");
    combobox_engine->addItem("Rubber Band R3 (finer)");
    combobox_engine->setToolTip("R3 engine produces higher-quality results than the R2 engine for most material, especially complex mixes, vocals and other sounds that have soft onsets and smooth pitch changes, and music with substantial bass content. However, it uses much more CPU power than the R2 engine.");
    QHBoxLayout *layout_engine = new QHBoxLayout;
    layout_engine->addWidget(label_engine);
    layout_engine->addWidget(combobox_engine);
    layout_engine->addStretch();

    QCheckBox *check_formant_preserved = new QCheckBox("Preserve formant shape (spectral envelope)");
    check_formant_preserved->setToolTip("Preserve the spectral envelope of the original signal. This permits shifting the note frequency without so substantially affecting the perceived pitch profile of the voice or instrument.");

    check_high_quality = new QCheckBox("High quality (uses more CPU)");
    check_high_quality->setToolTip("Use the highest quality method for pitch shifting. This method may use much more CPU, especially for large pitch shift.");

    check_enable_waveform = new QCheckBox("Enable waveform (affects performance / requires restart)");
    check_enable_waveform->setToolTip("Displays an waveform that acts as a progress bar. (Affects performance");

    QVBoxLayout *layout_general_settings = new QVBoxLayout;
    layout_general_settings->addLayout(layout_engine);
    layout_general_settings->addWidget(check_high_quality);
    layout_general_settings->addWidget(check_formant_preserved);
    layout_general_settings->addWidget(check_enable_waveform);
    combobox_engine->setCurrentIndex(app_settings->getEngineIndex());
    check_high_quality->setChecked(app_settings->getHighQuality());
    check_formant_preserved->setChecked(app_settings->getPerserveFormatShape());
    groupbox_general_settings->setLayout(layout_general_settings);

    QVBoxLayout *layout_main = new QVBoxLayout;
    layout_main->addWidget(groupbox_general_settings);

    setLayout(layout_main);

    combobox_engine->setCurrentIndex(app_settings->getEngineIndex());
    check_high_quality->setChecked(app_settings->getHighQuality());
    check_formant_preserved->setChecked(app_settings->getPerserveFormatShape());
    check_enable_waveform->setChecked(app_settings->getShowWaveform());


    connect(combobox_engine, qOverload<int>(&QComboBox::currentIndexChanged), [this](int index){ emitIndexOptionUseR3EngineChanged(index); });
    connect(check_high_quality, &QAbstractButton::toggled, [this](bool checked){ emitCheckUseHighQualityChanged(checked); });
    connect(check_formant_preserved, &QAbstractButton::toggled, [this](bool checked){  emitCheckFormantPreservedChanged(checked); });
    connect(check_enable_waveform, &QAbstractButton::toggled, [this](bool checked){  emitCheckEnableWaveformChanged(checked); });

    emitIndexOptionUseR3EngineChanged(app_settings->getEngineIndex());
    emitCheckUseHighQualityChanged(app_settings->getHighQuality());
    emitCheckFormantPreservedChanged(app_settings->getPerserveFormatShape());
}

// Destructor
SettingsDialog::~SettingsDialog()
{

}

void SettingsDialog::emitIndexOptionUseR3EngineChanged(int index)
{
    app_settings->setEngineIndex(index);
    emit indexOptionUseR3EngineChanged(index);
}

void SettingsDialog::emitCheckUseHighQualityChanged(bool enabled)
{
    app_settings->setHighQuality(enabled);
    emit checkUseHighQualityChanged(enabled);
}

void SettingsDialog::emitCheckFormantPreservedChanged(bool enabled)
{
    app_settings->setPerserveFormatShape(enabled);
    emit checkFormantPreservedChanged(enabled);
}

void SettingsDialog::emitCheckEnableWaveformChanged(bool enabled)
{
    app_settings->setShowWaveform(enabled);
    emit checkEnableWaveformChanged(enabled);
}

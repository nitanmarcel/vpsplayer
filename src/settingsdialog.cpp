#include "settingsdialog.h"
#include <QLabel>
#include <QHBoxLayout>
SettingsDialog::SettingsDialog()
{
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

    QVBoxLayout *layout_settings = new QVBoxLayout;
    layout_settings->addLayout(layout_engine);
    layout_settings->addWidget(check_high_quality);
    layout_settings->addWidget(check_formant_preserved);

    // Update status
    //combobox_engine->setEnabled(enable_options);
    //check_high_quality->setEnabled(enable_options);
    check_formant_preserved->setChecked(true);

    combobox_engine->setCurrentIndex(1);
    check_high_quality->setChecked(true);

    settings = new QWidget;
    settings->setLayout(layout_settings);
    setLayout(layout_settings);

    connect(combobox_engine, qOverload<int>(&QComboBox::currentIndexChanged), [this](int index){ emit indexOptionUseR3EngineChanged(index); });
    connect(check_high_quality, &QAbstractButton::toggled, [this](bool checked){ emit checkUseHighQualityChanged(checked); });
    connect(check_formant_preserved, &QAbstractButton::toggled, [this](bool checked){  emit checkUseHighQualityChanged(checked); });
}

// Destructor
SettingsDialog::~SettingsDialog()
{

}

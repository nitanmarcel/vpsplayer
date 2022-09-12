#include "settingsdialog.h"
#include "appsettings.h"
#include <QLabel>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QSpinBox>

#include <QDebug>
SettingsDialog::SettingsDialog()
{

    app_settings = new AppSettings(this);
    clearFocus();
    setFocusPolicy(Qt::NoFocus);

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


    QGroupBox *groupbox_keys_settings = new QGroupBox("Keybinds");
    QVBoxLayout *layout_keys_settings = new QVBoxLayout;

    QHBoxLayout *layout_key_settings_pause = new QHBoxLayout;
    QLabel *key_settings_pitch_pause_label = new QLabel("Pause");
    key_settings_pause_linedit = new KeyEdit;
    key_settings_pause_linedit->setMaximumWidth(100);
    key_settings_pause_linedit->setReadOnly(true);
    key_settings_pause_linedit->setText(QKeySequence(app_settings->getPauseKey()).toString());
    layout_key_settings_pause->addWidget(key_settings_pitch_pause_label);
    layout_key_settings_pause->addWidget(key_settings_pause_linedit);
    layout_keys_settings->addLayout(layout_key_settings_pause);

    QHBoxLayout *layout_key_settings_pitch_plus = new QHBoxLayout;
    QLabel *key_settings_pitch_plus_label = new QLabel("Pitch+");
    key_settings_pitch_plus_linedit = new KeyEdit;
    key_settings_pitch_plus_linedit->setMaximumWidth(100);
    key_settings_pitch_plus_linedit->setReadOnly(true);
    key_settings_pitch_plus_linedit->setText(QKeySequence(app_settings->getPitchSliderKeyPlus()).toString());
    layout_key_settings_pitch_plus->addWidget(key_settings_pitch_plus_label);
    layout_key_settings_pitch_plus->addWidget(key_settings_pitch_plus_linedit);
    layout_keys_settings->addLayout(layout_key_settings_pitch_plus);

    QHBoxLayout *layout_key_settings_pitch_minus = new QHBoxLayout;
    QLabel *key_settings_pitch_minus_label = new QLabel("Pitch-");
    key_settings_pitch_minus_linedit = new KeyEdit;
    key_settings_pitch_minus_linedit->setMaximumWidth(100);
    key_settings_pitch_minus_linedit->setReadOnly(true);
    key_settings_pitch_minus_linedit->setText(QKeySequence(app_settings->getPitchSliderKeyMinus()).toString());
    layout_key_settings_pitch_minus->addWidget(key_settings_pitch_minus_label);
    layout_key_settings_pitch_minus->addWidget(key_settings_pitch_minus_linedit);
    layout_keys_settings->addLayout(layout_key_settings_pitch_minus);

    QHBoxLayout *layout_key_settings_speed_plus = new QHBoxLayout;
    QLabel *key_settings_speed_plus_label = new QLabel("Speed+");
    key_settings_speed_plus_linedit = new KeyEdit;
    key_settings_speed_plus_linedit->setMaximumWidth(100);
    key_settings_speed_plus_linedit->setReadOnly(true);
    key_settings_speed_plus_linedit->setText(QKeySequence(app_settings->getSpeedSliderKeyPlus()).toString());
    layout_key_settings_speed_plus->addWidget(key_settings_speed_plus_label);
    layout_key_settings_speed_plus->addWidget(key_settings_speed_plus_linedit);
    layout_keys_settings->addLayout(layout_key_settings_speed_plus);

    QHBoxLayout *layout_key_settings_speed_minus = new QHBoxLayout;
    QLabel *key_settings_speed_minus_label = new QLabel("Speed-");
    key_settings_speed_minus_linedit = new KeyEdit;
    key_settings_speed_minus_linedit->setReadOnly(true);
    key_settings_speed_minus_linedit->setMaximumWidth(100);
    key_settings_speed_minus_linedit->setText(QKeySequence(app_settings->getSpeedSliderKeyMinus()).toString());
    layout_key_settings_speed_minus->addWidget(key_settings_speed_minus_label);
    layout_key_settings_speed_minus->addWidget(key_settings_speed_minus_linedit);
    layout_keys_settings->addLayout(layout_key_settings_speed_minus);
    groupbox_keys_settings->setLayout(layout_keys_settings);

    QHBoxLayout *layout_key_settings_playback_plus = new QHBoxLayout;
    QLabel *key_settings_playback_plus_label = new QLabel("Playback+");
    key_settings_playback_plus_linedit = new KeyEdit;
    key_settings_playback_plus_linedit->setMaximumWidth(100);
    key_settings_playback_plus_linedit->setReadOnly(true);
    key_settings_playback_plus_linedit->setText(QKeySequence(app_settings->getPlaybackSliderKeyPlus()).toString());
    layout_key_settings_playback_plus->addWidget(key_settings_playback_plus_label);
    layout_key_settings_playback_plus->addWidget(key_settings_playback_plus_linedit);
    layout_keys_settings->addLayout(layout_key_settings_playback_plus);

    QHBoxLayout *layout_key_settings_playback_minus = new QHBoxLayout;
    QLabel *key_settings_playback_minus_label = new QLabel("Playback-");
    key_settings_playback_minus_linedit = new KeyEdit;
    key_settings_playback_minus_linedit->setMaximumWidth(100);
    key_settings_playback_minus_linedit->setReadOnly(true);
    key_settings_playback_minus_linedit->setText(QKeySequence(app_settings->getPlaybackSliderKeyMinus()).toString());
    layout_key_settings_playback_minus->addWidget(key_settings_playback_minus_label);
    layout_key_settings_playback_minus->addWidget(key_settings_playback_minus_linedit);
    layout_keys_settings->addLayout(layout_key_settings_playback_minus);
    groupbox_keys_settings->setLayout(layout_keys_settings);

    QGroupBox *groupbox_modifier_settings = new QGroupBox("Modifiers (Shift + KEY)");
    QVBoxLayout *layout_modifier_settings = new QVBoxLayout;
    QHBoxLayout *layout_modifier_settings_pitch = new QHBoxLayout;
    QLabel *modifier_settings_pitch_label = new QLabel("Pitch modifier value");
    QSpinBox *modifier_settings_pitch_spinbox = new QSpinBox;
    modifier_settings_pitch_spinbox->setMaximum(10);
    modifier_settings_pitch_spinbox->setMinimum(0);
    if (app_settings->getPitchModifierValue() > 0 && app_settings->getPitchModifierValue() < 10)
        modifier_settings_pitch_spinbox->setValue(app_settings->getPitchModifierValue());
    layout_modifier_settings_pitch->addWidget(modifier_settings_pitch_label);
    layout_modifier_settings_pitch->addWidget(modifier_settings_pitch_spinbox);

    QHBoxLayout *layout_modifier_settings_speed = new QHBoxLayout;
    QLabel *modifier_settings_speed_label = new QLabel("Pitch modifier value");
    QSpinBox *modifier_settings_speed_spinbox = new QSpinBox;
    modifier_settings_speed_spinbox->setMaximum(10);
    modifier_settings_speed_spinbox->setMinimum(0);
    if (app_settings->getSpeedModifierValue() > 0 && app_settings->getSpeedModifierValue() < 10)
        modifier_settings_speed_spinbox->setValue(app_settings->getSpeedModifierValue());
    layout_modifier_settings_speed->addWidget(modifier_settings_speed_label);
    layout_modifier_settings_speed->addWidget(modifier_settings_speed_spinbox);

    layout_modifier_settings->addLayout(layout_modifier_settings_pitch);
    layout_modifier_settings->addLayout(layout_modifier_settings_speed);
    groupbox_modifier_settings->setLayout(layout_modifier_settings);


    QVBoxLayout *layout_main = new QVBoxLayout;
    layout_main->addWidget(groupbox_general_settings);
    layout_main->addWidget(groupbox_modifier_settings);
    layout_main->addWidget(groupbox_keys_settings);

    setLayout(layout_main);

    combobox_engine->setCurrentIndex(app_settings->getEngineIndex());
    check_high_quality->setChecked(app_settings->getHighQuality());
    check_formant_preserved->setChecked(app_settings->getPerserveFormatShape());
    check_enable_waveform->setChecked(app_settings->getShowWaveform());


    connect(this, &QDialog::rejected, [this](){ releaseKeyboard(); });

    connect(combobox_engine, qOverload<int>(&QComboBox::currentIndexChanged), [this](int index){ emitIndexOptionUseR3EngineChanged(index); });
    connect(check_high_quality, &QAbstractButton::toggled, [this](bool checked){ emitCheckUseHighQualityChanged(checked); });
    connect(check_formant_preserved, &QAbstractButton::toggled, [this](bool checked){  emitCheckFormantPreservedChanged(checked); });
    connect(check_enable_waveform, &QAbstractButton::toggled, [this](bool checked){  emitCheckEnableWaveformChanged(checked); });
    connect(modifier_settings_pitch_spinbox, qOverload<int>(&QSpinBox::valueChanged), [this](int value){ emitPitchModifierValueChanged(value); });
    connect(modifier_settings_speed_spinbox, qOverload<int>(&QSpinBox::valueChanged), [this](int value){ emitSpeedModifierValueChanged(value); });

    connect(key_settings_pause_linedit, &KeyEdit::focussed, [this](bool hasFocus){keyInputReciever = "pause_key"; waitForKeyInput = hasFocus; });
    connect(key_settings_pitch_plus_linedit, &KeyEdit::focussed, [this](bool hasFocus){keyInputReciever = "pitch_plus"; waitForKeyInput = hasFocus; });
    connect(key_settings_pitch_minus_linedit, &KeyEdit::focussed, [this](bool hasFocus){keyInputReciever = "pitch_minus"; waitForKeyInput = hasFocus; });
    connect(key_settings_speed_plus_linedit, &KeyEdit::focussed, [this](bool hasFocus){keyInputReciever = "speed_plus"; waitForKeyInput = hasFocus; });
    connect(key_settings_speed_minus_linedit, &KeyEdit::focussed, [this](bool hasFocus){keyInputReciever = "speed_minus"; waitForKeyInput = hasFocus; });
    connect(key_settings_playback_plus_linedit, &KeyEdit::focussed, [this](bool hasFocus){keyInputReciever = "playback_plus"; waitForKeyInput = hasFocus; });
    connect(key_settings_playback_minus_linedit, &KeyEdit::focussed, [this](bool hasFocus){keyInputReciever = "playback_minus"; waitForKeyInput = hasFocus; });

    emitIndexOptionUseR3EngineChanged(app_settings->getEngineIndex());
    emitCheckUseHighQualityChanged(app_settings->getHighQuality());
    emitCheckFormantPreservedChanged(app_settings->getPerserveFormatShape());
}

// Destructor
SettingsDialog::~SettingsDialog()
{

}

void SettingsDialog::open()
{
    grabKeyboard();
    show();
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

void SettingsDialog::emitPitchModifierValueChanged(int value)
{
    app_settings->setPitchModifierValue(value);
    emit pitchModifierValueChanged(value);
}

void SettingsDialog::emitSpeedModifierValueChanged(int value)
{
    app_settings->setSpeedModifierValue(value);
    emit speedModifierValueChanged(value);
}

void SettingsDialog::emitPauseKey(int key)
{
    app_settings->setPauseKey(key);
    emit pauseKeyChanged(key);
}

void SettingsDialog::emitPitchSliderKeyPlus(int key)
{
    app_settings->setPitchSliderKeyPlus(key);
    emit pitchSliderKeyPlusChanged(key);
}

void SettingsDialog::emitPitchSliderKeyMinus(int key)
{
    app_settings->setPitchSliderKeyMinus(key);
    emit pitchSliderKeyMinusChanged(key);
}

void SettingsDialog::emitSpeedSliderKeyPlus(int key)
{
    app_settings->setSpeedSliderKeyPlus(key);
    emit speedSliderKeyPlusChanged(key);
}

void SettingsDialog::emitSpeedSliderKeyMinus(int key)
{
    app_settings->setSpeedSliderKeyMinus(key);
    emit speedSliderKeyMinusChanged(key);
}

void SettingsDialog::emitPlaybackSliderKeyPlus(int key)
{
    app_settings->setPlaybackSliderKeyPlus(key);
    emit playbackSliderKeyPlusChanged(key);
}

void SettingsDialog::emitPlaybackSliderKeyMinus(int key)
{
    app_settings->setPitchSliderKeyMinus(key);
    emit playbackSliderKeyMinusChanged(key);
}

void SettingsDialog::keyPressEvent(QKeyEvent *event)
{
    if (!waitForKeyInput)
        return;
    if (keyInputReciever == "pause_key")
    {
        key_settings_pause_linedit->setText(QKeySequence(event->key()).toString());
        key_settings_pause_linedit->clearFocus();
        emitPauseKey(event->key());
    }
    if (keyInputReciever == "pitch_plus")
    {
        key_settings_pitch_plus_linedit->setText(QKeySequence(event->key()).toString());
        key_settings_pitch_plus_linedit->clearFocus();
        emitPitchSliderKeyPlus(event->key());
    }

    if (keyInputReciever == "pitch_minus")
    {
        key_settings_pitch_minus_linedit->setText(QKeySequence(event->key()).toString());
        key_settings_pitch_minus_linedit->clearFocus();
        emitPitchSliderKeyMinus(event->key());
    }

    if (keyInputReciever == "speed_plus")
    {
        key_settings_speed_plus_linedit->setText(QKeySequence(event->key()).toString());
        key_settings_speed_plus_linedit->clearFocus();
        emitSpeedSliderKeyPlus(event->key());
    }

    if (keyInputReciever == "speed_minus")
    {
        key_settings_speed_minus_linedit->setText(QKeySequence(event->key()).toString());
        key_settings_speed_minus_linedit->clearFocus();
        emitSpeedSliderKeyMinus(event->key());
    }

    if (keyInputReciever == "playback_plus")
    {
        key_settings_playback_plus_linedit->setText(QKeySequence(event->key()).toString());
        key_settings_playback_plus_linedit->clearFocus();
        emitPlaybackSliderKeyPlus(event->key());
    }

    if (keyInputReciever == "playback_minus")
    {
        key_settings_playback_minus_linedit->setText(QKeySequence(event->key()).toString());
        key_settings_playback_minus_linedit->clearFocus();
        emitPlaybackSliderKeyMinus(event->key());
    }
    waitForKeyInput = false;
    keyInputReciever = "";
}

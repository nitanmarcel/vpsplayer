// Copyright 2018-2020 François CROLLET

// This file is part of VPS Player.
// VPS Player is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 3 of the License, or (at your option) any later version.
// VPS Player is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with VPS Player. If not, see <http://www.gnu.org/licenses/>.

#include <QtMath>
#include <QAudio>
#include <QDir>
#include <QFileDialog>
#include <QFont>
#include <QGridLayout>
#include <QComboBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QKeySequence>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QSlider>
#include <QStandardPaths>
#include <QStatusBar>
#include <QStringList>
#include <QVBoxLayout>
#include <QMainWindow>
#include <QMimeData>
#include <QMimeDatabase>
#include <QDragEnterEvent>
#include <WaveformWidget.h>

#include "Player_window.h"
#include "settingsdialog.h"
#include "tools.h"


// Constructor
PlayerWindow::PlayerWindow(const QIcon &app_icon, const QString &filename)
{
  clearFocus();
  setFocusPolicy(Qt::NoFocus);
  audio_player = new AudioPlayer(this);
  key_modifier = false;

  const QIcon open_icon(QStringLiteral(":/open-32.png"));
  const QIcon backward_icon(QStringLiteral(":/backward-32.png"));
  const QIcon forward_icon(QStringLiteral(":/forward-32.png"));
  const QFont fixed_font(QStringLiteral("monospace"));
  
  QMenu *menu_file = menuBar()->addMenu("&File");
  QMenu *menu_help = menuBar()->addMenu(QStringLiteral("&?"));
  action_open = new QAction(open_icon, "&Open", this);
  action_open->setShortcut(QKeySequence(QStringLiteral("Ctrl+O")));
  QAction *action_settings = new QAction("&Settings", this);
  QAction *action_quit = new QAction(QIcon(QStringLiteral(":/quit-32.png")), "&Quit", this);
  action_quit->setShortcut(QKeySequence(QStringLiteral("Ctrl+Q")));
  QAction *action_about = new QAction(app_icon, "&About", this);
  QAction *action_about_qt = new QAction(QIcon(QStringLiteral(":/qt-32.png")), "About Q&t", this);
  menu_file->addAction(action_open);
  menu_file->addSeparator();
  menu_file->addAction(action_quit);
  menu_file->addAction(action_settings);
  menu_help->addAction(action_about);
  menu_help->addAction(action_about_qt);

  
  label_status = new QLabel;
  label_loading_progress = new QLabel;
  label_loading_progress->setFont(fixed_font);
  statusBar()->addWidget(label_status, 1);
  statusBar()->addPermanentWidget(label_loading_progress, 0);
  
  QLabel *label_pitch = new QLabel("Pitch");
  label_pitch->setAlignment(Qt::AlignRight);
  QLabel *label_speed = new QLabel("Speed");
  label_speed->setAlignment(Qt::AlignRight);
  QSlider *slider_pitch = new QSlider;
  slider_pitch->setFocusPolicy(Qt::NoFocus);
  slider_pitch->setOrientation(Qt::Horizontal);
  slider_pitch->setTickPosition(QSlider::TicksAbove);
  slider_pitch->setRange(-12, 12);
  slider_pitch->setSingleStep(1);
  slider_pitch->setPageStep(1);
  slider_pitch->setTickInterval(12);
  QSlider *slider_speed = new QSlider;
  slider_speed->setFocusPolicy(Qt::NoFocus);
  slider_speed->setOrientation(Qt::Horizontal);
  slider_speed->setTickPosition(QSlider::TicksAbove);
  slider_speed->setRange(-24, 24);
  slider_speed->setSingleStep(1);
  slider_speed->setPageStep(1);
  slider_speed->setTickInterval(24);
  spinbox_pitch = new QSpinBox;
  spinbox_pitch->setFocusPolicy(Qt::NoFocus);
  spinbox_pitch->setRange(-12, 12);
  label_speed_value = new QLabel;
  label_speed_value->setAlignment(Qt::AlignCenter);
  QGridLayout *layout_sliders = new QGridLayout;
  layout_sliders->addWidget(label_pitch, 0, 0);
  layout_sliders->addWidget(label_speed, 1, 0);
  layout_sliders->addWidget(slider_pitch, 0, 1);
  layout_sliders->addWidget(slider_speed, 1, 1);
  layout_sliders->addWidget(spinbox_pitch, 0, 2);
  layout_sliders->addWidget(label_speed_value, 1, 2);
  QVBoxLayout *layout_settings = new QVBoxLayout;
  layout_settings->addLayout(layout_sliders);
  QGroupBox *groupbox_settings = new QGroupBox();
  groupbox_settings->setLayout(layout_settings);
  
  button_open = new QPushButton(open_icon, "Open file");
  button_open->setFocusPolicy(Qt::NoFocus);
  button_open->setToolTip(QStringLiteral("Ctrl+O"));
  button_cancel = new QPushButton(QIcon(QStringLiteral(":/cancel-32.png")), "Cancel");
  button_cancel->setFocusPolicy(Qt::NoFocus);
  button_play = new QPushButton(QIcon(QStringLiteral(":/play-32.png")), "Play");
  button_play->setFocusPolicy(Qt::NoFocus);
  button_pause = new QPushButton(QIcon(QStringLiteral(":/pause-32.png")), "Pause");
  button_pause->setFocusPolicy(Qt::NoFocus);
  button_stop = new QPushButton(QIcon(QStringLiteral(":/stop-32.png")), "Stop");
  button_stop->setFocusPolicy(Qt::NoFocus);
  QHBoxLayout *layout_buttons = new QHBoxLayout;
  layout_buttons->addWidget(button_open);
  layout_buttons->addWidget(button_cancel);
  layout_buttons->addWidget(button_play);
  layout_buttons->addWidget(button_pause);
  layout_buttons->addWidget(button_stop);

  button_bwd10 = new QPushButton(backward_icon, QStringLiteral("-10s"));
  button_bwd10->setFocusPolicy(Qt::NoFocus);
  button_bwd5 = new QPushButton(backward_icon, QStringLiteral("-5s"));
  button_bwd5->setFocusPolicy(Qt::NoFocus);
  button_fwd5 = new QPushButton(forward_icon, QStringLiteral("+5s"));
  button_fwd5->setFocusPolicy(Qt::NoFocus);
  button_fwd10 = new QPushButton(forward_icon, QStringLiteral("+10s"));
  button_fwd10->setFocusPolicy(Qt::NoFocus);
  QHBoxLayout *layout_buttons2 = new QHBoxLayout;
  layout_buttons2->addWidget(button_bwd10);
  layout_buttons2->addWidget(button_bwd5);
  layout_buttons2->addStretch();
  layout_buttons2->addWidget(button_fwd5);
  layout_buttons2->addWidget(button_fwd10);

  QVBoxLayout *layout_player = new QVBoxLayout;
  layout_player->addLayout(layout_buttons);
  layout_player->addLayout(layout_buttons2);
  //layout_player->addLayout(layout_progress);
  QGroupBox *groupbox_player = new QGroupBox("");
  groupbox_player->setLayout(layout_player);

  QVBoxLayout *layout_waveform = new QVBoxLayout;
  widget_waveform = new WaveformWidget;
  layout_waveform->addWidget(widget_waveform);
  QGroupBox *groupbox_waveform = new QGroupBox();
  groupbox_waveform->setLayout(layout_waveform);
  groupbox_waveform->setMinimumHeight(150);

  QVBoxLayout *layout_progress = new QVBoxLayout();
  label_progress = new QLabel;
  label_progress->setFont(fixed_font);
  layout_progress->addWidget(label_progress);
  QGroupBox *groupbox_progress = new QGroupBox();
  groupbox_progress->setLayout(layout_progress);
  groupbox_progress->setMaximumHeight(label_progress->fontInfo().weight());

  
  QVBoxLayout *layout_main = new QVBoxLayout;
  layout_main->addWidget(groupbox_settings);
  layout_main->addWidget(groupbox_player);
  layout_main->addWidget(groupbox_waveform);
  layout_main->addWidget(groupbox_progress);
  QWidget *widget_main = new QWidget;
  widget_main->setLayout(layout_main);
  setCentralWidget(widget_main);

  slider_pitch->setValue(0);
  updatePitch(0);
  slider_speed->setValue(0);
  updateSpeed(0);
  audio_player->updateOptionUseR3Engine(true);
  audio_player->updateOptionHighQuality(true);
  audio_player->updateOptionFormantPreserved(true);
  updateStatus(audio_player->getStatus());
  updateReadingPosition(-1);
  updateDuration(-1);
  
  adjustSize();
  setMaximumHeight(height());
  
  setAcceptDrops(true);

  settings = new AppSettings;
  settings_dialog = new SettingsDialog(settings);
  widget_waveform->setFfmpegPath(settings->getFfmpegPath());
  widget_waveform->setFfmpegConvertToMono(settings->getConvertMono());

  connect(action_open, &QAction::triggered, this, &PlayerWindow::openFileFromSelector);
  connect(action_quit, &QAction::triggered, this, &PlayerWindow::close);
  connect(action_about, &QAction::triggered, this, &PlayerWindow::showAbout);
  connect(action_about_qt, &QAction::triggered, [this](){ QMessageBox::aboutQt(this, "About Qt"); });
  connect(action_settings, &QAction::triggered, [this]() { PlayerWindow::showSettings(); });
  connect(button_open, &QPushButton::clicked, this, &PlayerWindow::openFileFromSelector);
  connect(button_cancel, &QPushButton::clicked, audio_player, &AudioPlayer::cancelDecoding);
  connect(button_play, &QPushButton::clicked, this, &PlayerWindow::playAudio);
  connect(button_pause, &QPushButton::clicked, audio_player, &AudioPlayer::pausePlaying);
  connect(button_stop, &QPushButton::clicked, audio_player, &AudioPlayer::stopPlaying);
  connect(button_bwd10, &QPushButton::clicked, [this](){ moveReadingPosition(-10000); });
  connect(button_bwd5, &QPushButton::clicked, [this](){ moveReadingPosition(-5000); });
  connect(button_fwd5, &QPushButton::clicked, [this](){ moveReadingPosition(5000); });
  connect(button_fwd10, &QPushButton::clicked, [this](){ moveReadingPosition(10000); });
  connect(spinbox_pitch, qOverload<int>(&QSpinBox::valueChanged), slider_pitch, &QAbstractSlider::setValue);
  connect(slider_pitch, &QAbstractSlider::valueChanged, this, &PlayerWindow::updatePitch);
  connect(slider_speed, &QAbstractSlider::valueChanged, this, &PlayerWindow::updateSpeed);
  connect(widget_waveform, &WaveformWidget::barClicked, this, &PlayerWindow::moveReadingPosition);
  connect(this, &PlayerWindow::playbackSpeedChanged, slider_speed, &QAbstractSlider::setValue);
  connect(this, &PlayerWindow::pitchValueChanged, slider_pitch, &QAbstractSlider::setValue);
  connect(settings_dialog, &SettingsDialog::indexOptionUseR3EngineChanged, audio_player, &AudioPlayer::updateOptionUseR3Engine );// [this](int index){ audio_player->updateOptionUseR3Engine(index == 1); });
  connect(settings_dialog, &SettingsDialog::checkUseHighQualityChanged, audio_player, &AudioPlayer::updateOptionHighQuality);
  connect(settings_dialog, &SettingsDialog::checkFormantPreservedChanged, audio_player, &AudioPlayer::updateOptionFormantPreserved);
  connect(audio_player, &AudioPlayer::statusChanged, this, &PlayerWindow::updateStatus);
  connect(audio_player, &AudioPlayer::loadingProgressChanged, [this](int progress){ label_loading_progress->setText(QStringLiteral("%1 \%").arg(progress)); });
  connect(audio_player, &AudioPlayer::durationChanged, this, &PlayerWindow::updateDuration);
  connect(audio_player, &AudioPlayer::readingPositionChanged, this, &PlayerWindow::updateReadingPosition);
  connect(audio_player, &AudioPlayer::audioDecodingError, this, &PlayerWindow::displayAudioDecodingError);
  connect(audio_player, &AudioPlayer::audioOutputError, this, &PlayerWindow::displayAudioDeviceError);
  connect(widget_waveform, &WaveformWidget::barClicked, audio_player, &AudioPlayer::moveReadingPosition);
  connect(settings_dialog, &SettingsDialog::ffmpegPathChanged, widget_waveform, &WaveformWidget::setFfmpegPath);
  connect(settings_dialog, &SettingsDialog::checkConvertMonoChanged, widget_waveform, &WaveformWidget::setFfmpegConvertToMono);



  const QStringList music_directories = QStandardPaths::standardLocations(QStandardPaths::MusicLocation);
  if (music_directories.isEmpty())
    music_directory = QDir::homePath();
  else
    music_directory = music_directories.first();

  if (!filename.isEmpty()) {
    const QFileInfo file_info(filename);
    
    if (file_info.exists() && file_info.isFile())
      openFile(file_info);
    else
      QMessageBox::critical(this, "File not found", QString("\"%1\" is not a valid file.").arg(file_info.filePath()), QMessageBox::Ok);
  }
}


// Destructor
PlayerWindow::~PlayerWindow()
{
  
}


// Prompt an error popup for an audio decoding error
void PlayerWindow::displayAudioDecodingError(QAudioDecoder::Error error)
{
  QString error_message;

  switch(error) {
  case QAudioDecoder::ResourceError :
    error_message = "File not found";
    break;
  case QAudioDecoder::FormatError :
    error_message = "Format not supported";
    break;
  case QAudioDecoder::AccessDeniedError :
    error_message = "Access denied";
    break;
  case QAudioDecoder::ServiceMissingError :
    error_message = "No audio decoding service available";
    break;
  default :
    error_message = "Unknown error";
  }

  QMessageBox::critical(this, "Audio decoding error", "Error while decoding audio file:\n" + error_message, QMessageBox::Ok);
}


// Prompt an error popup for an audio device error
void PlayerWindow::displayAudioDeviceError(QAudio::Error error)
{
  QString error_message;

  switch(error) {
  case QAudio::OpenError :
    error_message = "Unable to open audio device";
    break;
  case QAudio::IOError :
    error_message = "Unable to send data to audio device";
    break;
  case QAudio::FatalError :
    error_message = "Audio device not usable";
    break;
  default :
    error_message = "Unknown error";
  }

  QMessageBox::critical(this, "Audio device error", "Error while accessing audio device:\n" + error_message, QMessageBox::Ok);
}


// Open file given in parameter
void PlayerWindow::openFile(const QFileInfo &file_info)
{
  setWindowTitle(QStringLiteral("VPS Player [%1]").arg(file_info.fileName()));
  music_directory = file_info.canonicalPath();
  widget_waveform->setSource(new QFileInfo(file_info.canonicalFilePath()));
  audio_player->decodeFile(file_info.canonicalFilePath());
}


// Open a new file (chosen with a file selector)
void PlayerWindow::openFileFromSelector()
{
  QString audio_files_filter("Common audio files (*.aac *.flac *.m4a *.mp3 *.ogg *.wav *.wma)");
  const QString selected_file = QFileDialog::getOpenFileName(this, "Select audio file", music_directory, audio_files_filter + ";;All files (*)", &audio_files_filter);

  if (!selected_file.isEmpty())
    openFile(QFileInfo(selected_file));
}


// Start or resume audio playing
void PlayerWindow::playAudio()
{
  if (audio_player->getStatus() == AudioPlayer::Stopped)
    audio_player->startPlaying();
  else
    audio_player->resumePlaying();
}


// Moves reading position backward or forward. Parameter: position change in milliseconds
void PlayerWindow::moveReadingPosition(int delta)
{
  int new_position = widget_waveform->value();
  if (new_position >= widget_waveform->maximum())
    audio_player->stopPlaying();
  else
    audio_player->moveReadingPosition(qMax(0, new_position));
  qDebug() << qMax(0, new_position);
}


// Displays "About" dialog window
void PlayerWindow::showAbout()
{
  QMessageBox::about(this,
                     "About VPS Player",
                     QString("<h3>VPS Player</h3><p>High quality Variable Pitch and Speed audio player<br>Release <b>%1</b></p><p><a href=\"https://github.com/fcrollet/vpsplayer\">https://github.com/fcrollet/vpsplayer</a></p><p>Developed by François CROLLET</p><p>This program makes use of the Rubber Band Library<br><a href=\"https://www.breakfastquay.com/rubberband/\">https://www.breakfastquay.com/rubberband/</a></p>").arg(VERSION_STRING));
}

// Displays "Settings" dialog window
void PlayerWindow::showSettings()
{
    settings_dialog->open();
}


// Updates total file duration
void PlayerWindow::updateDuration(int duration)
{
  if (duration == -1)
    widget_waveform->setValue(0);
  else
    widget_waveform->setMaximum(duration);
  total_duration = Tools::convertMSecToText(duration);
  label_progress->setText(reading_progress + "/" + total_duration);
}


// Updates the pitch
void PlayerWindow::updatePitch(int pitch)
{
  spinbox_pitch->setValue(pitch);
  audio_player->updatePitch(pitch);
  pitch_value = pitch;
}


// Updates current reading position
void PlayerWindow::updateReadingPosition(int position)
{
  if (position == -1)
    widget_waveform->setValue(0);
  else{
    widget_waveform->setValue(position);
    //widget_waveform->setFormat(QString::number(position)); // to force progress bar refresh
  }
  reading_progress = Tools::convertMSecToText(position);
  label_progress->setText(reading_progress + "/" + total_duration);
}


// Updates the speed
void PlayerWindow::updateSpeed(int speed)
{
  qreal speed_ratio = qPow(qreal(2.0), speed / qreal(24.0));
  label_speed_value->setText(QStringLiteral("x %1").arg(speed_ratio, 0, 'f', 2));
  audio_player->updateSpeed(static_cast<double>(speed_ratio));
  playback_speed = speed;
}


// Updates the window based on the player status
void PlayerWindow::updateStatus(AudioPlayer::Status status)
{
  auto set_controls = [this](const QString &status_text, bool enable_open, bool decoding, bool playback_begun, bool enable_play, bool enable_pause) {
    label_status->setText(status_text);
    action_open->setEnabled(enable_open);
    button_open->setEnabled(enable_open);
    button_cancel->setEnabled(decoding);
    button_stop->setEnabled(playback_begun);
    button_bwd10->setEnabled(playback_begun);
    button_bwd5->setEnabled(playback_begun);
    button_fwd5->setEnabled(playback_begun);
    button_fwd10->setEnabled(playback_begun);
    button_play->setEnabled(enable_play);
    button_pause->setEnabled(enable_pause);
    widget_waveform->setClickable(playback_begun);
  };

  switch(status) {
  case AudioPlayer::NoFileLoaded :
    set_controls("No file loaded", true, false, false, false, false);
    setWindowTitle(QStringLiteral("VPS Player"));
    label_loading_progress->clear();
    break;
  case AudioPlayer::Loading :
    set_controls("Loading file", false, true, false, false, false);
    break;
  case AudioPlayer::Stopped :
    set_controls("Stopped", true, false, false, true, false);
    break;
  case AudioPlayer::Paused :
    set_controls("Paused", true, false, true, true, false);
    break;
  case AudioPlayer::Playing :
    set_controls("Playing", true, false, true, false, true);
    break;
  }
}


void PlayerWindow::dragEnterEvent(QDragEnterEvent *e){
    if (e->mimeData()->hasUrls())
    {
        QMimeDatabase mimeDb;
        QMimeType type = mimeDb.mimeTypeForUrl(e->mimeData()->urls()[0]);
        if (type.name().startsWith("audio"))
        {
            e->acceptProposedAction();
            return;
        }
    e->ignore();
    }
}

void PlayerWindow::dropEvent(QDropEvent *e)
{
    QFileInfo file(e->mimeData()->urls()[0].path());
    openFile(file);
}

void PlayerWindow::keyReleaseEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Shift && key_modifier)
        key_modifier = false;
}

void PlayerWindow::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Shift && !key_modifier)
    {
        key_modifier = true;
    }
    else if (e->key() == Qt::Key_Space && !key_modifier)
    {
        if (button_play->isEnabled())
            button_play->click();
        else if (button_pause->isEnabled())
            button_pause->click();
    }
    else if (e->key() == Qt::Key_Space && key_modifier)
    {
        if (button_stop->isEnabled())
            button_stop->click();
    }
    else if (e->key() == Qt::Key_Right || e->key() == Qt::Key_E)
    {
        if (!key_modifier && button_fwd5->isEnabled())
            button_fwd5->click();
        else if (key_modifier && button_fwd10->isEnabled())
            button_fwd10->click();
    }
    else if (e->key() == Qt::Key_Left || e->key() == Qt::Key_Q)
    {
        if (!key_modifier && button_bwd5->isEnabled())
            button_bwd5->click();
        else if (key_modifier && button_bwd10->isEnabled())
            button_bwd10->click();
    }
    else if (e->key() == Qt::Key_D)
    {
        if (playback_speed < 24)
            emit playbackSpeedChanged(playback_speed + (key_modifier ? 5 : 1));
    }
    else if (e->key() == Qt::Key_A)
    {
        if (playback_speed > -24)
            emit playbackSpeedChanged(playback_speed - (key_modifier ? 5 : 1));
    }
    else if (e->key() == Qt::Key_W)
    {
        if (pitch_value < 12)
            emit pitchValueChanged(pitch_value + (key_modifier ? 5 : 1));
    }
    else if (e->key() == Qt::Key_S)
    {
        if (pitch_value > -12)
            emit pitchValueChanged(pitch_value - (key_modifier ? 5 : 1));
    }

}


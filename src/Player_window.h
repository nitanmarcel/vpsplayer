// Copyright 2018-2022 François CROLLET

// This file is part of VPS Player.
// VPS Player is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 3 of the License, or (at your option) any later version.
// VPS Player is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with VPS Player. If not, see <http://www.gnu.org/licenses/>.

#ifndef PLAYER_WINDOW_H
#define PLAYER_WINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QFileInfo>
#include <QIcon>
#include <QLabel>
#include <QLCDNumber>
#include <QPushButton>
#include <QSpinBox>
#include <QString>
#include <QKeyEvent>

#include "waveformwidget.h"

#include "Audio_player.h"
#include "Playing_progress.h"
#include "settingsdialog.h"
#include "appsettings.h"


class PlayerWindow : public QMainWindow
{
  Q_OBJECT

private:
  AudioPlayer *audio_player;
  QAction *action_open;
  QPushButton *button_open;
  QPushButton *button_cancel;
  QPushButton *button_play;
  QPushButton *button_pause;
  QPushButton *button_stop;
  QPushButton *button_bwd10;
  QPushButton *button_bwd5;
  QPushButton *button_fwd5;
  QPushButton *button_fwd10;
  QSpinBox *spinbox_pitch;
  QLabel *label_speed_value;
  QString total_duration;
  QString reading_progress;
  PlayingProgress *progress_playing;
  QLabel *label_progress;
  QLabel *label_status;
  QLabel *label_loading_progress;
  QString music_directory;

  SettingsDialog *settings_dialog;
  WaveformWidget *widget_waveform;

  int playback_speed;
  int pitch_value;
  bool key_modifier;

  AppSettings *settings;
  
public:
  PlayerWindow(const QIcon &app_icon, const QString &filename = QString()); // Constructor
  ~PlayerWindow(); // Destructor
  void openFile(const QFileInfo &file_info); // Open file given in parameter

private:
  void displayAudioDecodingError(QAudioDecoder::Error error); // Prompt an error popup for an audio decoding error
  void displayAudioDeviceError(QAudio::Error error); // Prompt an error popup for an audio device error
  void openFileFromSelector(); // Open a new file (chosen with a file selector)
  void playAudio(); // Start or resume audio playing
  void playAudioBreakpoint(); // Starts or resumes audio playing from a set breakpoint
  void moveReadingPosToBreakpoint(); // Moves the reading position to the set breakpoint
  void pauseAudio(); // Pauses audio playing
  void pauseAudioFromBreakpoint(); // Pauses audio playing and resets to a set breakpoint
  void stopAudio(); // Stops the audio playing
  void moveReadingPosition(); // Moves reading position when the waveform bar is clicked. Parameter: position change in milliseconds
  void moveReadingPositionBar(int delta); // Moves reading position when the progress bar is clicked. Parameter: position change in milliseconds
  void bfReadingPosition(int miliseconds); // Moves reading position backward or forward. Parameter: position change in milliseconds
  void showAbout(); // Displays "About" dialog window
  void showSettings(); // Displays "Settings" dialog window
  void updateDuration(int duration); // Updates total file duration
  void updatePitch(int pitch); // Updates the pitch
  void updateReadingPosition(int position); // Updates current reading position
  void updateSpeed(int speed); // Updates the speed
  void updateStatus(AudioPlayer::Status status); // Updates the window based on the player status

  int pitchModifierValue;
  int speedModifierValue;
  int modifierKey;
  int pauseKey;
  int pauseKeyAlt;
  int pitchSliderKeyPlus;
  int pitchSliderKeyMinus;
  int speedSliderKeyPlus;
  int speedSliderKeyMinus;
  int playbackSliderKeyPlus;
  int playbackSliderKeyMinus;

protected:
  void dragEnterEvent(QDragEnterEvent *e);
  void dropEvent(QDropEvent *e);
  void keyPressEvent(QKeyEvent *e);
  void keyReleaseEvent(QKeyEvent *e);
  virtual void closeEvent(QCloseEvent *e);

signals:
  int playbackSpeedChanged(int speed);
  int pitchValueChanged(int value);
};

#endif

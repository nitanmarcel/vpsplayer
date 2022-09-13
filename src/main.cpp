// Copyright 2018-2020 François CROLLET

// This file is part of VPS Player.
// VPS Player is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 3 of the License, or (at your option) any later version.
// VPS Player is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with VPS Player. If not, see <http://www.gnu.org/licenses/>.

#include <QApplication>
#include <QCommandLineParser>
#include <QCoreApplication>
#include <QIcon>
#include <QString>
#include <QStringList>
#include <QFile>
#include <QDir>
#include <QStandardPaths>

#include "Player_window.h"


int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  QCoreApplication::setApplicationName(QStringLiteral("VPS Player"));
  QCoreApplication::setApplicationVersion(QStringLiteral(VERSION_STRING));
  const QIcon app_icon(QStringLiteral(":/vps-64.png"));
  app.setWindowIcon(app_icon);

  QString settingsPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QDir::separator() + "vpsplayer" + QDir::separator();
  if (!QDir(settingsPath).exists())
      QDir().mkdir(settingsPath);

  QFile stylesheet_file(settingsPath + "style.qss");
  if (!stylesheet_file.exists())
  {
      if (stylesheet_file.open(QFile::WriteOnly))
      {
          QFile qrc_stylesheet_file(":/default.qss");
          if (qrc_stylesheet_file.open(QFile::ReadOnly))
          {
              QTextStream stream(&stylesheet_file);
              stream << QLatin1String(qrc_stylesheet_file.readAll());
              qrc_stylesheet_file.close();
              stylesheet_file.close();
          }
      }
  }
      //QFile::copy(":/default.qss", settingsPath + "style.qss");
  stylesheet_file.open(QFile::ReadOnly);
  QString styleSheet = QLatin1String(stylesheet_file.readAll());
  app.setStyleSheet(styleSheet);

  QString filename;
  {
    QCommandLineParser parser;
    parser.setApplicationDescription("High quality Variable Pitch and Speed audio player");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file", "Audio file to open (optional)", "[file]");
    parser.process(app);
    const QStringList arguments = parser.positionalArguments();
    if (!arguments.isEmpty())
      filename = arguments.first();
  }
  
  PlayerWindow window(app_icon, filename);
  window.show();
  return app.exec();
}

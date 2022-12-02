include(thirdparty/SingleApplication/singleapplication.pri)
DEFINES += QAPPLICATION_CLASS=QApplication
CONFIG += qt warn_on release link_pkgconfig c++14 exceptions_off
QT += widgets multimedia concurrent
PKGCONFIG += rubberband
DEFINES += VERSION_STRING=\\\"2.0.0\\\"
MOC_DIR = build_tmp
OBJECTS_DIR = build_tmp
RCC_DIR = build_tmp
HEADERS += src/Audio_player.h \
          src/Player_window.h \
          src/Playing_progress.h \
          src/keyedit.h \
          src/menustyle.h \
          src/messagereceiver.h \
          src/settingsdialog.h \
          src/appsettings.h \
          src/tools.h \
          src/waveformwidget.h
SOURCES += src/main.cpp \
          src/Audio_player.cpp \
          src/Player_window.cpp \
          src/Playing_progress.cpp \
          src/keyedit.cpp \
          src/menustyle.cpp \
          src/messagereceiver.cpp \
          src/settingsdialog.cpp \
          src/appsettings.cpp \
          src/tools.cpp \
          src/waveformwidget.cpp
RESOURCES = icons.qrc styles.qrc
TARGET = vpsplayer

isEmpty(PREFIX) {
  PREFIX = /usr
}

documentation.path = $$PREFIX/share/doc/VPSPlayer
documentation.files = COPYING README.md

target.path = $$PREFIX/bin

icon64.path = $$PREFIX/share/icons/hicolor/64x64/apps
icon64.files = icons/64x64/com.github.fcrollet.vpsplayer.png

icon128.path = $$PREFIX/share/icons/hicolor/128x128/apps
icon128.files = icons/128x128/com.github.fcrollet.vpsplayer.png

desktop.path = $$PREFIX/share/applications
desktop.files = com.github.fcrollet.vpsplayer.desktop

appinfo.path = $$PREFIX/share/metainfo
appinfo.files = com.github.fcrollet.vpsplayer.appdata.xml

INSTALLS += target documentation icon64 icon128 desktop appinfo


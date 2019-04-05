TEMPLATE = app
CONFIG += qt warn_on release link_pkgconfig c++14
QT += widgets multimedia
PKGCONFIG += rubberband
DEFINES += VERSION_STRING=\\\"devel_1.0_pre\\\"
MOC_DIR = build_tmp
OBJECTS_DIR = build_tmp
RCC_DIR = build_tmp
HEADERS = src/Audio_player.h \
          src/Player_window.h \
          src/Playing_progress.h
SOURCES = src/main.cpp \
          src/Audio_player.cpp \
          src/Player_window.cpp \
          src/Playing_progress.cpp
RESOURCES = icons.qrc
TARGET = vpsplayer

documentation.path = $$PREFIX/share/doc/VPSPlayer
documentation.files = COPYING README.md

target.path = $$PREFIX/bin

icon64.path = $$PREFIX/share/icons/hicolor/64x64/apps
icon64.files = icons/64x64/org.fcrollet.VPSPlayer.png

icon128.path = $$PREFIX/share/icons/hicolor/128x128/apps
icon128.files = icons/128x128/org.fcrollet.VPSPlayer.png

desktop.path = $$PREFIX/share/applications
desktop.files = org.fcrollet.VPSPlayer.desktop

INSTALLS += target documentation icon64 icon128 desktop

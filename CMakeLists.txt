cmake_minimum_required(VERSION 3.16)
project(vpsplayer VERSION 1.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Set up AUTOMOC and some sensible defaults for runtime execution
# When using Qt 6.3, you can replace the code block below with
# qt_standard_project_setup()
set(CMAKE_AUTOMOC ON)
include(GNUInstallDirs)

find_package(QT NAMES Qt5 Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Concurrent Gui Multimedia Network Widgets)

qt5_add_resources(QRC_FILES styles.qrc icons.qrc)

add_executable(vpsplayer WIN32 MACOSX_BUNDLE
    ${QRC_FILES}
    
    src/Audio_player.cpp src/Audio_player.h
    src/Player_window.cpp src/Player_window.h
    src/Playing_progress.cpp src/Playing_progress.h
    src/appsettings.cpp src/appsettings.h
    src/keyedit.cpp src/keyedit.h
    src/main.cpp
    src/menustyle.cpp src/menustyle.h
    src/messagereceiver.cpp src/messagereceiver.h
    src/settingsdialog.cpp src/settingsdialog.h
    src/tools.cpp src/tools.h
    src/waveformbuilder.cpp src/waveformbuilder.h
    src/waveformwidget.cpp src/waveformwidget.h
    thirdparty/SingleApplication/SingleApplication
    thirdparty/SingleApplication/singleapplication.cpp thirdparty/SingleApplication/singleapplication.h thirdparty/SingleApplication/singleapplication_p.cpp thirdparty/SingleApplication/singleapplication_p.h
)
target_include_directories(vpsplayer PRIVATE
    thirdparty/SingleApplication
)

target_compile_definitions(vpsplayer PRIVATE
    QAPPLICATION_CLASS=QApplication
    VERSION_STRING="2.0.0"
)

target_link_libraries(vpsplayer PRIVATE
    Qt::Concurrent
    Qt::Core
    Qt::Gui
    Qt::Multimedia
    Qt::Network
    Qt::Widgets
    rubberband
)


# Resources:
set_source_files_properties("icons/64x64/com.github.fcrollet.vpsplayer.png"
    PROPERTIES QT_RESOURCE_ALIAS "vps-64.png"
)
set_source_files_properties("icons/backward-32.png"
    PROPERTIES QT_RESOURCE_ALIAS "backward-32.png"
)
set_source_files_properties("icons/cancel-32.png"
    PROPERTIES QT_RESOURCE_ALIAS "cancel-32.png"
)
set_source_files_properties("icons/forward-32.png"
    PROPERTIES QT_RESOURCE_ALIAS "forward-32.png"
)
set_source_files_properties("icons/open-32.png"
    PROPERTIES QT_RESOURCE_ALIAS "open-32.png"
)
set_source_files_properties("icons/pause-32.png"
    PROPERTIES QT_RESOURCE_ALIAS "pause-32.png"
)
set_source_files_properties("icons/play-32.png"
    PROPERTIES QT_RESOURCE_ALIAS "play-32.png"
)
set_source_files_properties("icons/qt-32.png"
    PROPERTIES QT_RESOURCE_ALIAS "qt-32.png"
)
set_source_files_properties("icons/quit-32.png"
    PROPERTIES QT_RESOURCE_ALIAS "quit-32.png"
)
set_source_files_properties("icons/stop-32.png"
    PROPERTIES QT_RESOURCE_ALIAS "stop-32.png"
)
set(icons_resource_files
    "icons/64x64/com.github.fcrollet.vpsplayer.png"
    "icons/backward-32.png"
    "icons/cancel-32.png"
    "icons/forward-32.png"
    "icons/open-32.png"
    "icons/pause-32.png"
    "icons/play-32.png"
    "icons/qt-32.png"
    "icons/quit-32.png"
    "icons/stop-32.png"
)
set_source_files_properties("styles/default.qss"
    PROPERTIES QT_RESOURCE_ALIAS "default.qss"
)
set(styles_resource_files
    "styles/default.qss"
)

if(MSVC AND WIN32)
    target_link_libraries(vpsplayer PRIVATE
        Advapi32.lib
    )
endif()

if(GCC AND WIN32)
    target_link_libraries(vpsplayer PRIVATE
        advapi32
    )
endif()

install(TARGETS vpsplayer
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Consider using qt_generate_deploy_app_script() for app deployment if
# the project can use Qt 6.3. In that case rerun qmake2cmake with
# --min-qt-version=6.3.

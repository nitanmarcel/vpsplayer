#!/bin/bast

set -e

if [[ -z "$GITLAB_CI" ]]; then
  echo "This script is ran on gitlab ci and might break your system if ran locally!"
  exit 1
fi

apt update -qq
apt install -y patchelf software-properties-common
add-apt-repository ppa:beineri/opt-qt-5.15.2-bionic -y
apt update -qq

export DEBIAN_FRONTEND=noninteractive
export TZ=Etc/UTC

apt install -y qtchooser qt515base qt515declarative qt515multimedia qt515svg qt515x11extras qt515wayland build-essential python3-pip ninja-build wget pkg-config libsamplerate0-dev libsndfile1-dev libfftw3-dev ladspa-sdk lv2-dev vamp-plugin-sdk libboost-test-dev meson ninja-build libssl-dev libgl1-mesa-dev fuse libfuse2 ibgstreamer1.0-dev libgstreamer-plugins-base1.0-dev libgstreamer-plugins-bad1.0-dev gstreamer1.0-plugins-base gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly gstreamer1.0-libav gstreamer1.0-doc gstreamer1.0-tools gstreamer1.0-x gstreamer1.0-alsa gstreamer1.0-gl gstreamer1.0-gtk3 gstreamer1.0-qt5 gstreamer1.0-pulseaudio git

pip3 install --user meson

export QTDIR="/opt/qt515"
export LD_LIBRARY_PATH="/opt/qt515/lib/x86_64-linux-gnu:/opt/qt515/lib:$LD_LIBRARY_PATH"
export PATH="/opt/qt515/bin:${HOME}/.local/bin:$PATH"
export PKG_CONFIG_PATH="/opt/qt515/lib/pkgconfig:$PKG_CONFIG_PATH"

echo "/opt/qt515/bin" >> /usr/lib/x86_64-linux-gnu/qtchooser/default.conf
echo "/opt/qt515/lib/x86_64-linux-gnu" >> /usr/lib/x86_64-linux-gnu/qtchooser/default.conf

mkdir cmake && cd cmake
wget https://github.com/Kitware/CMake/releases/download/v3.24.1/cmake-3.24.1.tar.gz
tar -xf cmake-3.24.1.tar.gz
cd cmake-3.24.1
./bootstrap
make -j$(nproc --all)
make install

mkdir rubberband && cd rubberband
wget https://breakfastquay.com/files/releases/rubberband-3.0.0.tar.bz2
tar -xf rubberband-3.0.0.tar.bz2
cd rubberband-3.0.0
meson build
ninja -C build
ninja -C build install

wget https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage
wget https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/continuous/linuxdeploy-plugin-qt-x86_64.AppImage
wget https://raw.githubusercontent.com/linuxdeploy/linuxdeploy-plugin-gstreamer/master/linuxdeploy-plugin-gstreamer.sh
wget https://github.com/AppImage/AppImageKit/releases/download/continuous/appimagetool-x86_64.AppImage
chmod a+x linuxdeploy-x86_64.AppImage
chmod a+x linuxdeploy-plugin-qt-x86_64.AppImage
chmod a+x linuxdeploy-plugin-gstreamer.sh
chmod a+x appimagetool-x86_64.AppImage

./linuxdeploy-x86_64.AppImage --appdir AppDir

qmake QMAKE_CXXFLAGS_RELEASE="-O2 -march=native -fvisibility-inlines-hidden -pipe -fno-plt -g0" vpsplayer.pro
make -j$(nproc --all)
strip --strip-unneeded vpsplayer
make install INSTALL_ROOT="$(pwd)/AppDir"

./linuxdeploy-x86_64.AppImage --plugin qt --appdir AppDir --desktop-file AppDir/usr/share/applications/com.github.fcrollet.vpsplayer.desktop
./linuxdeploy-x86_64.AppImage --plugin gstreamer --appdir AppDir --desktop-file AppDir/usr/share/applications/com.github.fcrollet.vpsplayer.desktop

rm AppDir/usr/lib/libgobject-2.0.so.0
rm AppDir/usr/lib/libgmodule-2.0.so.0
rm AppDir/usr/lib/libglib-2.0.so.0
rm AppDir/usr/lib/libgio-2.0.so.0

./linuxdeploy-x86_64.AppImage -p qt --appdir AppDir
chmod a+x VPS_Player-x86_64.AppImage

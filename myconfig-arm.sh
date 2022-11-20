#!/bin/sh
args=(
  --backend=sdl
  --prefix=/home/czak/projects/scummvm/prefix

  --host=arm-linux

  --disable-debug
  --enable-verbose-build

  --disable-all-engines
  --enable-engine=sky
  --disable-detection-full

  --opengl-mode=none

  --disable-mt32emu
  --disable-lua
  --disable-nuked-opl
  --disable-16bit
  --disable-highres
  --disable-savegame-timestamp
  --disable-scalers
  --disable-hq-scalers
  --disable-edge-scalers
  --disable-aspect
  --disable-translation
  --disable-taskbar
  --disable-cloud
  --disable-system-dialogs
  --disable-eventrecorder
  --disable-updates
  --disable-text-console
  --disable-tts
  --disable-bink
  --disable-alsa
  --disable-ogg
  --disable-vorbis
  --disable-tremor
  --disable-mad
  --disable-fribidi
  --disable-flac
  --disable-zlib
  --disable-mpeg2
  --disable-a52
  --disable-opengl-game
  --disable-opengl-game-classic
  --disable-opengl-game-shaders
  --disable-jpeg
  --disable-png
  --disable-gif
  --disable-theoradec
  --disable-faad
  --disable-fluidsynth
  --disable-fluidlite
  --disable-opl2lpt
  --disable-retrowave
  --disable-sparkle
  --disable-osx-dock-plugin
  --disable-freetype2
  --disable-nasm
  --disable-readline
  --disable-libunity
  --disable-gtk
  --disable-sndio
  --disable-sdlnet
  --disable-libcurl
  --disable-discord
  --disable-timidity
  --disable-seq-midi
)

_libraries="../libraries"

_cxxflags=(
  # -I "${_libraries}/sdl2/include"
  -DDISABLE_COMMAND_LINE=1
)

_ldflags=(
  # -target i386-unknown-linux-gnu
)

export CXX="arm-linux-gnueabi-g++"
export CXXFLAGS="${_cxxflags[@]}"
export LDFLAGS="${_ldflags[@]}"

./configure "${args[@]}"

#!/bin/sh
args=(
  --backend=sdl
  --prefix=/home/czak/projects/scummvm/prefix

  --disable-debug
  --enable-mold
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
  -target i386-unknown-linux-gnu
  -DDISABLE_COMMAND_LINE=1
  # -D_LIBCPP_HAS_NO_VENDOR_AVAILABILITY_ANNOTATIONS
  # -nostdinc
  # -nostdinc++
  #
  # -isystem "${_libraries}/std/include/libcxx/include"
  # -isystem "${_libraries}/std/include/libcxxabi/include"
  # -isystem "${_libraries}/std/include/libunwind/include"
  # -isystem "${_libraries}/std/include/include"
  # -isystem "${_libraries}/std/include/libc/include/i386-linux-gnu"
  # -isystem "${_libraries}/std/include/libc/include/generic-glibc"
  # -isystem "${_libraries}/std/include/libc/include/x86-linux-any"
  # -isystem "${_libraries}/std/include/libc/include/any-linux-any"
  #
  # -I "${_libraries}/sdl2/include"
)

_ldflags=(
  # -target i386-unknown-linux-gnu
)

export CXX="/usr/lib/llvm13/bin/clang++"
export CXXFLAGS="${_cxxflags[@]}"
export LDFLAGS="${_ldflags[@]}"

./configure "${args[@]}"

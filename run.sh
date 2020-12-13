#!/bin/sh

function is_int() { test "$@" -eq "$@" 2> /dev/null; }

FPS=0

if is_int "$FPS"; then
    FPS=$2
fi

if  [[ $1 = "--debug" || $1 = "-d" ]]; then
    echo -n "Debug mode enabled!"
    echo ""

    g++ -o ParticleEngine *.cpp -DDEBUG_APPLICATION -DTFPS=$FPS -lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++17
else
    echo -n "Release Mode enabled!"
    echo ""
    g++ -o ParticleEngine *.cpp -lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++17
fi

vblank_mode=0 ./ParticleEngine

rm -r ParticleEngine
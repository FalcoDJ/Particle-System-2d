#!/bin/sh
if  [[ $1 = "--debug" || $1 = "-d" ]]; then
    echo -n "Debug mode enabled!"
    echo ""
    g++ -o ParticleEngine *.cpp -DDEBUG_APPLICATION -lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++17
    vblank_mode=0 ./ParticleEngine
else
    echo -n "Release Mode enabled!"
    echo ""
    g++ -o ParticleEngine *.cpp -lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++17
    ./ParticleEngine
fi

rm -r ParticleEngine
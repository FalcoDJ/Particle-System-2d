#!/bin/sh
g++ -o ParticleEngine *.cpp -lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++17
vblank_mode=0 ./ParticleEngine
rm -r ParticleEngine
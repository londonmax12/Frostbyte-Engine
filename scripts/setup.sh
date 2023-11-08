#!/bin/bash

dependencies="xcb libxcb-xkb-dev x11-xkb-utils libx11-xcb-dev libxkbcommon-x11-dev"

sudo apt-get install $dependencies
cd ..
mkdir build
cd build
cmake ..
make
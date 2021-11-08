#!/bin/bash

set current_dir = $(pwd)
cd "$HOME/sources/dwm/"
make
sudo make install
cd "$current_dir"

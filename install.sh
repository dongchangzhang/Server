#!/bin/bash

# delete old icons
sudo rm -rf /usr/share/icons/combination.svg
sudo rm -rf /usr/share/icons/mars_mdpi_camera.svg

# copy icons
sudo cp ./data/icons/combination.svg /usr/share/icons/combination.svg
sudo cp ./data/icons/mars_mdpi_camera.svg /usr/share/icons/mars_mdpi_camera.svg
sudo cp ./data/icons/combination.jpg /usr/share/icons/combination.jpg
sudo cp ./data/icons/mars_mdpi_camera.jpg /usr/share/icons/mars_mdpi_camera.jpg

# delete old files
sudo rm -rf /usr/local/share/mars

# copy data
sudo cp -r ./data/mars_data  /usr/local/share/mars

# chmod for dataset
sudo chmod a+r /usr/local/share/mars/*

# delete old shutcuts
sudo rm -rf /usr/share/applications/combination.desktop
sudo rm -rf /usr/share/applications/mdpi_camera.desktop

# chmod
sudo chmod a+x ./data/shortcuts/combination.desktop
sudo chmod a+x ./data/shortcuts/mdpi_camera.desktop

# copy shutcuts
sudo cp ./data/shortcuts/combination.desktop /usr/share/applications/combination.desktop
sudo cp ./data/shortcuts/mdpi_camera.desktop /usr/share/applications/mdpi_camera.desktop

# chmod
sudo chmod a+x /usr/share/applications/combination.desktop
sudo chmod a+x /usr/share/applications/mdpi_camera.desktop

# compile
here=`pwd`

cd ./mdpi_camera
rm -rf ./build
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j4
sudo cp mdpi_camera /usr/local/bin/

cd $here

# compile
cd combination
rm -rf ./build
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j4
sudo cp Combination /usr/local/bin/

cd $here

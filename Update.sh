#!/bin/bash
matrixreleaseurl=$(curl -s https://api.github.com/repos/squarewavedot/matrixserver/releases | grep -m1 "browser_download_url.*deb" | cut -d : -f 2,3  | tr -d '"')
appsreleaseurl=$(curl -s https://api.github.com/repos/squarewavedot/exampleApplications/releases | grep -m1 "browser_download_url.*deb" | cut -d : -f 2,3  | tr -d '"')

matrixUpdateFilename=$(basename "$matrixreleaseurl")
wget $matrixreleaseurl && sudo apt install ./$matrixUpdateFilename && rm -f ./$matrixUpdateFilename

appsUpdateFilename=$(basename "$appsreleaseurl")
wget $appsreleaseurl && sudo dpkg -i ./$appsUpdateFilename && rm -f ./$appsUpdateFilename

sudo reboot
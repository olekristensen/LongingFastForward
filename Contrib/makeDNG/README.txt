--- Introduction ---

This folder contains the source code for makeDNG — a very simple command line tool for converting raw 16-bit bayer files to DNG format.

This is almost completely based on other peoples' work. In particular code and advice taken from the Elphel project here:

http://www3.elphel.com/importwiki?title=JP4

This folder contains the two dependancies that you'll need in order to build makeDNG — libjpeg and (a patched version of) liftiff.

First, build and install libjpeg and libtiff via the typical:

./configure ; make ; sudo make install

then inside the dng directory:

./build.sh

--- Usage ---

./makeDNG /var/tmp/firstBayer.bayer16 1280 960 /var/tmp/firstBayer.dng

This will take a 16 bit raw bayer file that's 1280x960 and turn it into a .dng file that's got just enough information for Adobe Camera Raw to open it. 

--- Assumptions ---

Specifically, firstBayer.bayer16 will be 1280x960x2 bytes long and contain a sensor data that has a RG/GB pattern — that is the sensor looks like:

RGRGRGRGRG…
GBGBGBGBGB…
RGRGRGRGRG…
GBGBGBGBGB…
…


The file contains no white balance information to speak off (this means ACR will initially preview the file as incredibly blue, just move the white balance slider over a bit). makeDNG assumes that only bottom 12-bits of the 16-bit bayer file are used.

The sensor bayer pattern and the 12-bit resolution should be easy to see and edit in the code — really, there's nothing to it.


 





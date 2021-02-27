#!/bin/bash
for f in *.pkg ; 
    do sudo installer -verbose -pkg "$f" -target /
done
pip install pyrebase
cp src/TeacherPlatform.bash TeacherPlatform.bash
./TeacherPlatform.bash
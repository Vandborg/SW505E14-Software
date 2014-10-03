#!/bin/sh
cd /nxtOSEK/SW505E14_Software/
echo "Cleaing"
echo "------------------"

rm -R build

echo "------------------"
echo "Initializing build"
echo "------------------"

make all

echo "------------------"
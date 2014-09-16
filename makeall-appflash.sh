#!/bin/sh
echo "Initializing build"
echo "------------------"

cd /nxtOSEK/SW505E14_Software/
make all

echo "------------------"
echo "Appflashing"
echo "------------------"

sh appflash.sh

echo "------------------"
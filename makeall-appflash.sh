#!/bin/sh
cd /nxtOSEK/SW505E14_Software/

echo "Generating Makefile"
echo "------------------"

var="TARGET = main_OSEK\n"
var="${var}TARGET_SOURCES = "
echo -e -n "$var" > Makefile
find=$(find -type f -iname "*.c" ! -iname "kernel_cfg.c" ! -iname "utility_movement.c")
echo $find | tr '\n' ' ' >> Makefile

var="TOPPERS_OSEK_OIL_SOURCE = ./main.oil"
echo -e "\n$var" >> Makefile

var="O_PATH ?= build"
echo -e "\n$var" >> Makefile
var="include ../ecrobot/ecrobot.mak"
echo -e "\n$var" >> Makefile
echo "------------------"

echo "Initializing build"
echo "------------------"

make all

echo "------------------"

echo "Appflashing"
echo "------------------"

sh appflash.sh

echo "------------------"
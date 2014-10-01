# Target specific macros
TARGET = main_OSEK
TARGET_SOURCES = \
	main.c \
	utility/utility_sound/utility_sound.c \
	utility/utility_lcd/utility_lcd.c \
	utility/utility_mode_handling/utility_mode_handling.c \
	utility/utility_movement/utility_braking.c \
	color_scanning/color_scanning.c \
	sonar_sensor/sonar_sensor.c \

TOPPERS_OSEK_OIL_SOURCE = ./main.oil

# Don't modify below part
O_PATH ?= build
include ../ecrobot/ecrobot.mak

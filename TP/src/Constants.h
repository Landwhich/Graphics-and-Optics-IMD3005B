#pragma once

#include "ofMain.h"

namespace Constants {
    static const int APP_WINDOW_WIDTH       = 1000;
    static const int APP_WINDOW_HEIGHT      = 800;
    static const int APP_DESIRED_FRAMERATE  = 60;

    //static const std::string ARDUINO_DEVICE_NAME = "COM3";  //if on windows this will be something like like COM3
    static const std::string ARDUINO_DEVICE_NAME = "tty.usbmodem101";  //if on windows this will be something like like COM3
    static const int ARDUINO_BAUD_RATE = 115200;
    static const int MAX_BRIGHTNESS = 230;      //foing full brightness (255) causes issues, so limiting here

    static const int PIN_FLEX_INPUT     = 0;
    static const int PIN_FORCE_INPUT    = 1;
    static const int PIN_STRETCH_INPUT  = 2;

    static const int PIN_RGB_PWM_RED_OUTPUT     = 6;
    static const int PIN_RGB_PWM_GREEN_OUTPUT   = 5;
    static const int PIN_RGB_PWM_BLUE_OUTPUT    = 3;
};

#pragma once

#include "ofMain.h"
#include "AudioAnalyser.h"

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);

    AudioAnalyser		m_audioAnalyser;

    float           m_circle1Brightness;
    float           m_circle2Brightness;
    float           m_circle3Brightness;
    float           m_circle4Brightness;
};

#pragma once

#include "ofMain.h"
#include "AudioAnalyser.h"
#include "Constants.h"
#include "ofxAssimpModelLoader.h"
// #include "ofTrueTypeFont"

class ofApp : public ofBaseApp {
public:

    ofEasyCam m_camera;
    ofLight m_light;

    ofxAssimpModelLoader m_violin;
    ofxAssimpModelLoader m_string1;
    ofxAssimpModelLoader m_string2;
    ofxAssimpModelLoader m_string3;
    ofxAssimpModelLoader m_string4;
    ofTexture m_violinTexture;

    float drums;
    float lowV;
    float highV;
    float violin;

    // check if ranges are on for game points and effects
    bool drumsPlaying;
    bool lowVPlaying;
    bool highVPlaying;
    bool violinPlaying;

    ofTrueTypeFont bearDays;
    float totalScore;
    float songTimeSeconds;

	void setup();
	void update();
	void draw();
	void keyPressed(int key);

    bool visualizerState; // 0 for analyser and 1 for visualizer
    AudioAnalyser		m_audioAnalyser;

    float           m_circle1Brightness;
    float           m_circle2Brightness;
    float           m_circle3Brightness;
    float           m_circle4Brightness;
};

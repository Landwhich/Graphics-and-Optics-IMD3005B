#pragma once

#include "ofMain.h"
#include "ofEvents.h"
// #include "ofxImGui.h"
#include "Constants.h"
#include "ofxAssimpModelLoader.h"

class ofApp : public ofBaseApp{
	public:
		ofEasyCam m_camera;
		ofLight m_light;
		ofMaterial m_material;

		ofxAssimpModelLoader m_planeHull;

		double m_planeRollSmooth = 0;
		double m_planePitchSmooth = 0;

		void setup();
		void update();
		void draw();
		
		//sensor inputs/values
		string buffer = "";

		//Arduino stuff
		ofSerial serial;
		float ax = 0, ay = 0, az = 0;
};

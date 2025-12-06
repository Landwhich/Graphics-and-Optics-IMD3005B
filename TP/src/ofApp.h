#pragma once

#include "ofMain.h"
#include "ofEvents.h"
// #include "ofxImGui.h"
#include "Constants.h"
#include "ofxAssimpModelLoader.h"

class ofApp : public ofBaseApp{
	public:
		// ofEasyCam m_camera;
		ofLight m_light;
		ofMaterial m_material;

		ofxAssimpModelLoader m_planeHull;

		double m_planeRollSmooth = 0;
		double m_planePitchSmooth = 0;

		ofVec3f planePos;
		ofVec3f planeVelocity;

		float playerScore = 0;

		float velocity = 0.0f;
		float baseSpeed = 0.4f;
		float boostSpeed = 1.2f;
		float acceleration = 0.03f;
		float decel = 0.015f;

		float worldOffsetZ = 0.0f;

		struct Ring {
			ofVec3f pos;
			float radius;
			bool collected = false;
		};

		vector<Ring> rings;

		void setup();
		void update();
		void draw();

		bool speedy = 0;
		
		//sensor inputs/values
		string buffer = "";

		//Arduino stuff
		ofSerial serial;
		float ax = 0, ay = 0, az = 0;
};

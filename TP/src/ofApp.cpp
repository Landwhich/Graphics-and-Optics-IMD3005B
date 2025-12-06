#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowShape(Constants::APP_WINDOW_WIDTH, Constants::APP_WINDOW_HEIGHT);
    ofSetFrameRate(Constants::APP_DESIRED_FRAMERATE);

    // --- 3D scene setup ---
    ofEnableDepthTest();
    m_light.setup();
    m_light.setPosition(300, 300, 600);
    m_material.setShininess(64);

    m_planeHull.load("cockpit.obj");

    serial.setup(Constants::ARDUINO_DEVICE_NAME, Constants::ARDUINO_BAUD_RATE);

    vector <ofSerialDeviceInfo> devices = serial.getDeviceList();


    // In case device wont work - saved lots of time in dev
    ofLog() << "Available serial devices:";
    for(auto & device : devices){
        ofLog() << "  " << device.getDeviceID() << " : " << device.getDeviceName();
    }
    if(!serial.setup(0, 115200)){ 
        ofLogError() << "Serial setup failed!";
    } else {
        ofLog() << "Serial setup success!";
    }


    for(int i = 0; i < 20; i++){
    Ring r;
    r.radius = ofRandom(50, ofRandom(50, 95));
    r.pos = ofVec3f(
        ofRandom(-500, 500),
        ofRandom(-300, 300),
        -600 - i * 500
    );
    rings.push_back(r);
    }

    planePos = ofVec3f(ofGetWidth()/2 + 150, ofGetHeight()/2 + 140, 0);
}

//--------------------------------------------------------------
void ofApp::update(){
    while (serial.available() > 0) {
        char c = serial.readByte();
        if (c == '\n') {
            vector<string> parts = ofSplitString(ofTrim(buffer), ",");
            if (parts.size() >= 3) {
                ax = ofToFloat(parts[0]);
                ay = ofToFloat(parts[1]);
                az = ofToFloat(parts[2]);
                //lerp for smoothness
                m_planePitchSmooth = ofLerp(m_planePitchSmooth, (ax * 0.005), 0.025);
                m_planeRollSmooth = ofLerp(m_planeRollSmooth, (ay * 0.00125), 0.025);
                // clamp so we dont exceed bounds
                m_planePitchSmooth = ofClamp(m_planePitchSmooth, -80, -60);
                m_planeRollSmooth = ofClamp(m_planeRollSmooth, -10, 10);
                speedy = ofToBool(parts[6]);
                ofLogNotice() << "speedy = " << (speedy ? "true" : "false");
            }
            buffer = "";
        } else {
            buffer += c;
        }
    }

    float targetSpeed = speedy ? boostSpeed : baseSpeed;

    if (planeVelocity.length() < targetSpeed)
        planeVelocity += (planeVelocity.normalized() * acceleration);
    else
        planeVelocity -= (planeVelocity.normalized() * decel);

    planeVelocity.limit(boostSpeed);

    // PLANE VECTOR 

    float pitchRad = ofDegToRad(m_planePitchSmooth);
    float rollRad  = ofDegToRad(m_planeRollSmooth);

    // Forward direction relative to pitch + roll
    ofVec3f forward;
    forward.x = sin(rollRad);
    forward.y = sin(ofDegToRad(ofMap(m_planePitchSmooth, -60, -80, 15, -25))); 
    forward.z = -cos(rollRad) * cos(pitchRad);

    forward.normalize();

    planeVelocity = ofVec3f(-forward.x * 2.5 * targetSpeed, forward.y * 0.5 * targetSpeed, forward.z * 1.75 * targetSpeed);

    // MOVE WORLD

    planePos += planeVelocity * 10.0f;

    // RINGS + COLLISIONS

    for (auto &r : rings) {

        ofVec3f relativePos = r.pos - planePos;  

        float dist = relativePos.length();  

        if (dist - 75 < r.radius) {
            // collected, respawn FAR away
            r.pos = ofVec3f(
                ofRandom(-1200, 1200),
                ofRandom(-600, 600),
                planePos.z - ofRandom(3000, 6000)
            );
            playerScore += r.radius * r.radius * 0.0025;
        }

        // if ring is behind the player, respawn
        if (r.pos.z > planePos.z + 200) {
            r.pos.z = planePos.z - ofRandom(3000, 6000);
            r.pos.x = ofRandom(-1200, 1200);
            r.pos.y = ofRandom(-600, 600);
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(20);

    ofSetColor(200);
    ofDrawBitmapString("MPU6500 Accelerometer:", 20, 20);

    ofDrawBitmapString("AX: " + ofToString(ax), 20, 50);
    ofDrawBitmapString("AY: " + ofToString(ay), 20, 70);
    ofDrawBitmapString("AZ: " + ofToString(az), 20, 90);

    ofDrawBitmapString("Pitch: " + ofToString(m_planePitchSmooth), 20, 120);
    ofDrawBitmapString("Roll: " + ofToString(m_planeRollSmooth), 20, 140);

    ofDrawBitmapString("Score: " + ofToString((int)playerScore), 20, 180);

    ofPushMatrix();
    
        ofEnableDepthTest();
        m_light.enable();

        ofTranslate(ofGetWidth()/2, ofGetHeight()/2, 0);
        // Move world opposite the plane
        ofTranslate(-planePos.x + 150, -planePos.y + 320, -planePos.z);

        for (auto &r : rings) {
            ofPushMatrix();
            ofTranslate(r.pos);
            ofSetColor(255, 180, 60);
            ofDrawCircle(0,0, r.radius);
            ofNoFill();
            ofDrawCircle(0,0, r.radius*0.9);
            ofPopMatrix();
        }

            // Draw plane cockpit
        ofPushMatrix();
            ofSetColor(100);
            ofTranslate(planePos);
            ofRotateXDeg(m_planePitchSmooth + 50);
            ofRotateYDeg(m_planeRollSmooth + 180);
            ofScale(3);
            m_planeHull.drawFaces();
        ofPopMatrix();

    ofPopMatrix();

    ofPushMatrix();
        ofSetColor(255,0,0);
        ofTranslate(planePos.x, planePos.y, planePos.z);
        ofDrawSphere(20);
    ofPopMatrix();

}
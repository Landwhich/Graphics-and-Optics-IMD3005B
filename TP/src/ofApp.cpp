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

    m_planeHull.load("plane_interior.fbx");

    serial.setup(Constants::ARDUINO_DEVICE_NAME, Constants::ARDUINO_BAUD_RATE);

    vector <ofSerialDeviceInfo> devices = serial.getDeviceList();

    ofLog() << "Available serial devices:";
    for(auto & device : devices){
        ofLog() << "  " << device.getDeviceID() << " : " << device.getDeviceName();
    }
    // Try connecting (replace index with the correct device ID)
    if(!serial.setup(0, 115200)){ 
        ofLogError() << "Serial setup failed!";
    } else {
        ofLog() << "Serial setup success!";
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    while (serial.available() > 0) {
        char c = serial.readByte();
        if (c == '\n') {
            // full line received, process it
            vector<string> parts = ofSplitString(ofTrim(buffer), ","); // split by space or comma
            if (parts.size() >= 3) {
                ax = ofToFloat(parts[0]);
                ay = ofToFloat(parts[1]);
                az = ofToFloat(parts[2]);
                m_planePitchSmooth = ofLerp(m_planePitchSmooth, (ax * 0.005), 0.025);
                m_planeRollSmooth = ofLerp(m_planeRollSmooth, (ay * 0.0025), 0.025);
            }
            buffer = ""; // clear buffer for next line
        } else {
            buffer += c; // add char to buffer
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(20);

    ofSetColor(255);
    ofDrawBitmapString("MPU6500 Accelerometer:", 20, 20);

    ofDrawBitmapString("AX: " + ofToString(ax), 20, 50);
    ofDrawBitmapString("AY: " + ofToString(ay), 20, 70);
    ofDrawBitmapString("AZ: " + ofToString(az), 20, 90);

    // Example: tilt cube
    ofPushMatrix();
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
    ofRotateXDeg(m_planePitchSmooth + 50);
    ofRotateYDeg(m_planeRollSmooth);
    m_planeHull.drawFaces();
    ofPopMatrix();
}
#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	//Shaders
	if(ofIsGLProgrammableRenderer()){
		shader.load("shadersGL3/shader");
	}else{
		shader.load("shadersGL2/shader");
	}

	int planeWidth = ofGetWidth();
	int planeHeight = ofGetHeight();
	int planeGridSize = 20;
	int planeColums = planeWidth / planeGridSize;
	int planeRows = planeHeight / planeGridSize;
	
	plane.set(planeWidth, planeHeight, planeColums, planeRows, OF_PRIMITIVE_TRIANGLES);

	camera.setFov(60);
	camera.setPosition(ofVec3f(ofGetWindowWidth() / 2, ofGetWindowHeight() * 1.2, 400));
	camera.lookAt(ofVec3f(ofGetWindowWidth() / 2, ofGetWindowHeight() / 2, 0));
	camera.setVFlip(true);

	ofDisableAlphaBlending();
	ofEnableDepthTest();
	light.enable();
	light.setPosition(ofVec3f(ofGetWindowWidth() / 2, ofGetWindowHeight() / 2, 500));
    light.lookAt(ofVec3f(ofGetWindowWidth() / 2, ofGetWindowHeight() / 2, 300));

	ofDisableArbTex();
	ofLoadImage(marsTexture, "Diffuse_2K.png");
	ofLoadImage(moonTexture, "Diffuse_2K_moon.png");
	ofLoadImage(solTexture, "2k_sun.jpg");

	bool loaded = rocketFont.load("nasa.otf", 128, true, true, false);
	// ofLogNotice() << "Font loaded: " << (loaded ? "YES" : "NO");

	rocketImage.load("rocket.png");
	rocketImage.setAnchorPercent(0.5,0.5);

	sol.loadModel("sol.obj");
	m_solPos = ofVec2f(ofGetWindowWidth() / 2.0, ofGetWindowHeight() / 2.0);
	// ofLogNotice() << "Model loaded: " << sol.getNumMeshes();

	mars.loadModel("Mars2K.obj");
	m_marsPos = ofVec2f(-600, 0);

	mars2.loadModel("Mars2K.obj");
	m_marsPos2 = ofVec2f(500, 0);

	mars3.loadModel("Mars2K.obj");
	m_marsPos3 = ofVec2f(0, 350);

	mars4.loadModel("Mars2K.obj");
	m_marsPos4 = ofVec2f(0, -400);

	phobos.loadModel("Moon2K.obj");
	m_phobosPos = ofVec2f(0, 500);

	deimos.loadModel("Moon2K.obj");
	m_deimosPos = ofVec2f(0, 500);

	m_rotFast = 0.0f;
	m_rotMed = 0.0f;
	m_rotSlow = 0.0f;

	m_mousePos = ofVec2f((float)0.0,(float)0.0);

}

//--------------------------------------------------------------
void ofApp::update(){

	m_rotFast += 0.15;
	m_rotMed += 0.075;
	m_rotSlow += 0.035;

	ofVec2f m_oldMousePos = m_mousePos;
	m_mousePos.set((float)ofGetMouseX(), (float)ofGetMouseY());

	m_rocketPos.set(m_mousePos);

	//smoke

	float dt = ofGetLastFrameTime();

    for (auto& p : smokeParticles) {
        p.update(dt);
    }

    smokeParticles.erase(
        std::remove_if(smokeParticles.begin(), smokeParticles.end(),
            [](const SmokeParticle& p) { return p.isDead(); }),
        smokeParticles.end()
    );
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	shader.begin();
	
	// center screen.
	float cx = ofGetWidth() / 2.0;
	float cy = ofGetHeight() / 2.0;
	
	// the plane is being position in the middle of the screen,
	// so we have to apply the same offset to the mouse coordinates before passing into the shader.
	float mx = mouseX - cx;
	float my = mouseY - cy;
	
	// we can pass in a single value into the shader by using the setUniform1 function.
	// if you want to pass in a float value, use setUniform1f.
	// if you want to pass in a integer value, use setUniform1i.
	shader.setUniform1f("mouseRange", 150);
	
	// we can pass in two values into the shader at the same time by using the setUniform2 function.
	// inside the shader these two values are set inside a vec2 object.
	shader.setUniform2f("mousePos", mx, my);
	
	// color changes from magenta to blue when moving the mouse from left to right.
	float percentX = mouseX / (float)ofGetWidth();
	percentX = ofClamp(percentX, 0, 1);
	ofFloatColor colorLeft = ofColor::magenta;
	ofFloatColor colorRight = ofColor::blue;
	ofFloatColor colorMix = colorLeft.getLerped(colorRight, percentX);
	
	// create a float array with the color values.
	float mouseColor[4] = {colorMix.r, colorMix.g, colorMix.b, colorMix.a};
	
	// we can pass in four values into the shader at the same time as a float array.
	// we do this by passing a pointer reference to the first element in the array.
	// inside the shader these four values are set inside a vec4 object.
	shader.setUniform4fv("mouseColor", mouseColor);

	camera.begin();

	ofPushMatrix();
		ofTranslate(cx, cy);
		plane.drawWireframe();
	ofPopMatrix();

	shader.end();

	rocketFont.drawString("click to create smoke", ofGetWindowWidth() / 2 - 850, ofGetWindowHeight() * -0.5);

	ofPushMatrix();
		ofDisableLighting();
		ofSetColor(255, 255, 255);  
		ofTranslate(m_solPos);
		ofRotateDeg(m_rotMed);
		ofScale(0.25, 0.25, 0.25);
		solTexture.bind();
		sol.drawFaces();
		solTexture.unbind();
		ofEnableLighting();
	ofPopMatrix();

	ofPushMatrix();

		ofTranslate(m_solPos);
		ofScale(0.5, 0.5, 0.5);

		ofPushMatrix();
			ofRotateDeg(m_rotMed);
			ofTranslate(m_marsPos);
			ofRotateDeg(m_rotMed);
			ofScale(0.25, 0.25, 0.25);
			marsTexture.bind();
			mars.drawFaces();
			marsTexture.unbind();

			ofPushMatrix();
				ofRotateDeg(m_rotFast);
				ofTranslate(m_phobosPos);
				ofRotateDeg(m_rotFast);
				ofScale(0.25, 0.25, 0.25);
				moonTexture.bind();
				phobos.drawFaces();
				moonTexture.unbind();
			ofPopMatrix();

		ofPopMatrix();

		ofPushMatrix();
			ofRotateDeg(m_rotSlow);
			ofTranslate(m_marsPos2);
			ofRotateDeg(m_rotSlow);
			ofScale(0.35, 0.35, 0.35);
			marsTexture.bind();
			mars2.drawFaces();
			marsTexture.unbind();

			ofPushMatrix();
				ofRotateDeg(m_rotFast * 2);
				ofTranslate(m_deimosPos);
				ofRotateDeg(m_rotFast);
				ofScale(0.25, 0.25, 0.25);
				moonTexture.bind();
				deimos.drawFaces();
				moonTexture.unbind();
			ofPopMatrix();

		ofPopMatrix();

		ofPushMatrix();
			ofRotateDeg(m_rotMed);
			ofTranslate(m_marsPos3);
			ofRotateDeg(m_rotMed);
			ofScale(0.2, 0.2, 0.2);
			marsTexture.bind();
			mars3.drawFaces();
		ofPopMatrix();

		ofPushMatrix();
			ofRotateDeg(m_rotFast);
			ofTranslate(m_marsPos4);
			ofRotateDeg(m_rotFast);
			ofScale(0.5, 0.5, 0.5);
			mars4.drawFaces();
			marsTexture.unbind();
		ofPopMatrix();

	ofPopMatrix();

	ofPushMatrix();
		ofTranslate(m_rocketPos);
		ofScale(0.05, 0.05);
		rocketImage.draw(0,0);
	ofPopMatrix();

	ofPushMatrix();
		// ofTranslate(m_mousePos);
		for (auto& p : smokeParticles) {
        	p.draw();
    	}
	ofPopMatrix();

	camera.end();
}

//--------------------------------------------------------------
// void ofApp::keyPressed(int key){
	
// }

// //--------------------------------------------------------------
// void ofApp::keyReleased(int key){

// }

// //--------------------------------------------------------------
// void ofApp::mouseMoved(int x, int y){
	
// }

// //--------------------------------------------------------------
// void ofApp::mouseDragged(int x, int y, int button){

// }

// //--------------------------------------------------------------

void ofApp::mousePressed(int x, int y, int button){
    for (int i = 0; i < 10; i++) {
        smokeParticles.push_back(SmokeParticle(ofVec2f(x, y)));
    }
}

// //--------------------------------------------------------------
// void ofApp::mouseReleased(int x, int y, int button){

// }

// //--------------------------------------------------------------
// void ofApp::windowResized(int w, int h){

// }

// //--------------------------------------------------------------
// void ofApp::gotMessage(ofMessage msg){

// }

// //--------------------------------------------------------------
// void ofApp::dragEvent(ofDragInfo dragInfo){ 

// }

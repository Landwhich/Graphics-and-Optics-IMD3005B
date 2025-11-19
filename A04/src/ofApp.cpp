#include "ofApp.h"

void ofApp::setup() 
{
	ofSetVerticalSync(true);
    ofSetWindowShape( Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT );

    visualizerState = false;

    drumsPlaying = false;
    lowVPlaying = false;
    highVPlaying = false;
    violinPlaying = false;

    totalScore = 0.0f;
    songTimeSeconds = 0.0f;

    bearDays.load("fonts/BearDays.ttf", 92);


    // jump to a specific time in our song (good for testing)
    // m_audioAnalyser.setPositionMS( 30 * 1000 ); // 30 seconds in (30000 ms)
	// note that this audioAnalyzer uses a modified version of ofFmodSOundPlayer so though it has the same+ functionality it should not be mixed with ofSound / ofFmodSoundPlayer

    // Set up the audio analyser:
    //     int _numBinsPerAverage: size in bins per average number from the full 513 bin spectrum
    //
    //     As an example, if _numBinsPerAverage is set to 10, every 10 bins from the full 513 bin spectrum
    //     will be averaged. You would have 513/10 averages in your set of linear averages.
    //     This would be 52 bins (51 + 1 bins as 10 does not go into 513 evenly).
    //     The reason for doing this is to get a smaller set of frequency ranges to work with. In the full 513
    //     bin spectrum, the each bin is 22,500hz/513 wide -> 43.8 Hz.
    //     If you average them into groups of 10, each bin is 438 Hz wide, which can be easier to work with

    // IMPORTANT TIP: Do not use the 20 that I have here, find a value that works well for your chosen song.
    // You want the lowest value that gives you the ability to clearly isolate the features of your song that
    // are interested in
    m_audioAnalyser.init("lacrimosa.mp3", 40 );
	m_audioAnalyser.setLoop(true);
	m_audioAnalyser.play();

    ofEnableDepthTest();
    m_light.setup();
    m_light.setPosition(300, 300, 600);

    ofLoadImage(m_violinTexture, "violin_col.png");
    m_violin.load("violinModelNoStrings.obj");
    m_string1.load("string1.obj");
    m_string2.load("string2.obj");
    m_string3.load("string3.obj");
    m_string4.load("string4.obj");

    m_string1.disableTextures();
    m_string2.disableTextures();
    m_string3.disableTextures();
    m_string4.disableTextures();
    m_string1.disableMaterials();
    m_string2.disableMaterials();
    m_string3.disableMaterials();
    m_string4.disableMaterials();

    // Lock cam in place
    // m_camera.setRotationSensitivity(0, 0, 0);
    // m_camera.setScale(1);
    // m_camera.setTranslationSensitivity(0, 0, 0);
}

void ofApp::update() 
{
    m_audioAnalyser.update();

    // Get the decibel levels for the frequency bins we are interested 
    drums = m_audioAnalyser.getLinearAverage( 0 ); 
    lowV = m_audioAnalyser.getLinearAverage( 3 );
    highV = m_audioAnalyser.getLinearAverage( 7 );
    violin = m_audioAnalyser.getLinearAverage( 2 );

    songTimeSeconds = m_audioAnalyser.getPositionMS() / 1000.0f;

    (drums > 11.0f) ? drumsPlaying = true : drumsPlaying = false;
    (lowV > 5.0f) ? lowVPlaying = true : lowVPlaying = false;
    (highV > 2.5f) ? highVPlaying = true : highVPlaying = false;
    (violin > 2.0f) ? violinPlaying = true : violinPlaying = false;

    if (totalScore < 0) totalScore = 0;
}


void ofApp::draw() 
{
    ofBackground(5);
    ofSetColor(255);

    if (!visualizerState){
        // Volume Level
        ofSetColor( ofColor::white );
        ofDrawBitmapString( "Volume Level", 140,  50 );

        ofDrawCircle( 100, 100, m_audioAnalyser.getLeftLevel()  * 100.0f );
        ofDrawCircle( 200, 100, m_audioAnalyser.getRightLevel() * 100.0f );
        ofDrawCircle( 300, 100, m_audioAnalyser.getMixLevel()   * 100.0f );

        ofDrawBitmapString( "Left",  80,  170 );
        ofDrawBitmapString( "Right", 180, 170 );
        ofDrawBitmapString( "Mix",   290, 170 );


        // Frequency / FFT information
        m_audioAnalyser.drawWaveform( 40, 300, 1200, 90 );  
        m_audioAnalyser.drawSpectrum( 40, 460, 1200, 128 );

        m_audioAnalyser.drawLinearAverages( 40, 650, 1200, 128 );

        // Advanced: can also get a custom average for a frequency range if you know the frequencies (can get them from mousing over the full spectrum)
        // float customAverage = m_audioAnalyser.getAvgForFreqRange( 128.0f, 300.0f );

        // Draw circles to indicate activity in the frequency ranges we are interested in
        // Must be remapped using the ranges of activity that we want
        m_circle1Brightness = ofMap( drums, 5.0f, 200.0f, 0.0f, 1.0f, true );
        m_circle2Brightness = ofMap( lowV, 0.0f, 100.0f, 0.0f, 1.0f, true );
        m_circle3Brightness = ofMap( highV, 0.0f, 100.0f, 0.0f, 1.0f, true );
        m_circle4Brightness = ofMap( violin, 5.0f, 50.0f, 0.0f, 1.0f, true );

        // Draw circles to indicate activity in the frequency ranges we are interested in
        ofSetColor( ofFloatColor( m_circle1Brightness, 0.0f, 0.0f ) );
        ofDrawCircle( 500, 100, 50 );

        ofSetColor( ofFloatColor( 0.0f, m_circle2Brightness, 0.0f ) );
        ofDrawCircle( 650, 100, 50 );

        ofSetColor( ofFloatColor( 0.0f, 0.0f, m_circle3Brightness ) );
        ofDrawCircle( 800, 100, 50 );

        ofSetColor( ofFloatColor( m_circle4Brightness, m_circle4Brightness, m_circle4Brightness ) );
        ofDrawCircle( 950, 100, 50 );

        ofSetColor( ofColor::white );
        ofDrawBitmapString( "Drum and Brass", 445, 200 );
        ofDrawBitmapString( "Low Vocals", 610, 200 );
        ofDrawBitmapString( "High Vocals", 760, 200 );
        ofDrawBitmapString( "Strings", 925, 200 );

        // song time in seconds. Can use m_soundPlayer.setPositionMS( time ) to jump to a specific time
        ofDrawBitmapString( "Song time: " + ofToString( songTimeSeconds ), 40, 250 );
    }

    else if (visualizerState){
        ofSetColor(0);
        ofEnableDepthTest();
        ofEnableLighting();
        m_camera.begin();
        m_light.enable();

        ofPushMatrix();
            ofRotateDeg(130, 1, 0, 0);
            ofScale(3);
            ofTranslate(0, 10 ,10);
            m_violinTexture.bind();
            m_violin.drawFaces();
            m_violinTexture.unbind();
            ofScale(.8);
            ofTranslate(-7, 10, 10);

            (drums > 11.0f) ? ofSetColor(255, 0, 0) : ofSetColor(0);
            m_string4.drawFaces();

            (lowV > 5.0f) ? ofSetColor(255, 255, 0) : ofSetColor(0);
            m_string3.drawFaces();

            (highV > 2.5f) ? ofSetColor(0, 255, 255) : ofSetColor(0);
            m_string2.drawFaces();

            (violin > 2.0f) ? ofSetColor(255, 0, 255) : ofSetColor(0);
            m_string1.drawFaces();
        ofPopMatrix();

        m_light.disable();
        ofDisableLighting();
        m_camera.end();
        ofDisableDepthTest();

        ofSetColor(255, ofMap(totalScore, 0.0, 100.0f, 255.0, 0.0, true), ofMap(totalScore, 0.0, 100.0f, 255.0, 0.0, true));
        bearDays.drawString("Score: " + ofToString((int)totalScore), Constants::WINDOW_WIDTH/2 - bearDays.stringWidth("Score: " + ofToString((int)totalScore))/2, Constants::WINDOW_HEIGHT*0.15);
    }
    
}

void ofApp::keyPressed(int key) {
	if (key == ' '){
        visualizerState = !visualizerState;
    }
    if (visualizerState){
        if (key == '1') (drumsPlaying) ? totalScore = (totalScore += 10.0f) * 1.25 : totalScore -= 0.0f;
        if (key == '2') (lowVPlaying) ? totalScore = (totalScore += 3.0f) * 1.0125 : totalScore -= 25.0f;
        if (key == '3') (highVPlaying) ? totalScore = (totalScore += 5.0f) * 1.05 : totalScore -= 15.0f;
        if (key == '4') (violinPlaying) ? totalScore = (totalScore += 3.0f) * 1.0125 : totalScore -= 35.0f;
    }
}

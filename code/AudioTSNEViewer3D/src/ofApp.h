#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxJSON.h"
#include "ofxOsc.h"
#include "SonicSphere.h"
//#include "ofxHoa.h"

#define HOST "localhost"
#define SENDPORT 1234
#define RECEIVEPORT 2345
#define NUM_MSG_STRINGS 20
#define XDIM 2500
#define YDIM 2500
#define ZDIM 2500

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void findCollisions(ofVec3f point);
    ofVec3f CarToPol (ofPoint thisPoint);
    void OscSendPointData (ofVec3f polCoords, string path);
	void updateActiveClasses(string cat, bool bDraw);
	void colorLerpChanged(float &f);
	void updateDuration(float &f);
//    vector<AudioClip> sounds;
    
    ofParameter<float> maxDuration;
    ofParameter<float> mouseRadius;
    ofParameter<float> pauseLength;
	ofParameter<float> colorSpaceLerp;
	ofParameter<bool> bRotateSystem;
    
    ofxPanel gui;
    ofEasyCam camera;
    ofPoint dimensions;
    ofPoint ambiData;
    string msg_strings[NUM_MSG_STRINGS];
    float timers[NUM_MSG_STRINGS];
	bool bOscSend;

	vector<SonicSphere> sonicSpheres;
	bool bShowMetallic = true;
	bool bShowWooden = true;
	bool bShowVoice = true;
	bool bShowLiquid = true;

//						HOA BINAURAL INIT 
//    Encoder<Hoa2d, float>::DC hoaEncoder = Encoder<Hoa2d, float>::DC(order);
//    Decoder<Hoa2d, float>::Binaural hoaDecoder = Decoder<Hoa2d, float>::Binaural(order);
//    Optim<Hoa2d, float>::Basic hoaOptim = Optim<Hoa2d, float>::Basic(order);
//    ofxHoaCoord<Hoa2d, float> hoaCoord = ofxHoaCoord<Hoa2d, float>(1);
    
};

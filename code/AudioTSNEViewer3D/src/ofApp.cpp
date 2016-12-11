#include "ofApp.h"

ofxOscSender sender;
ofxOscReceiver receiver;
ofVec3f currentNavPos;
ofParameter<string> receiveOscIP;
ofxJSONElement result;
//--------------------------------------------------------------
void ofApp::setup(){
    
	bOscSend = false;
    sender.setup(HOST, SENDPORT);
    receiver.setup (RECEIVEPORT);

	dimensions = ofPoint(XDIM, YDIM, ZDIM);

    gui.setup();
    gui.setName("Audio t-SNE 3D");
    gui.add(maxDuration.set("maxDuration", 0.5f, 0.01f, 2.0f));
    gui.add(mouseRadius.set("mouseRadius", 4.0f, 4.0f, 12.0f));
    //gui.add(pauseLength.set("pauseLength", 2.0f, 0.2f, 5.0f));
	gui.add(colorSpaceLerp.set("CMYK/RGB_lerp", 0.5f, 0.0f, 1.0f));
	gui.add(bRotateSystem.set("Rotate", false));
	
	//COLOR SPACE CHANGE EVENT LISTENER
	colorSpaceLerp.addListener(this, &ofApp::colorLerpChanged);
	maxDuration.addListener(this, &ofApp::updateDuration);

	string file = "points3D.json";
	bool parsingSuccessful = result.open(file);
    
	//SONIC SPHERES CONSTRUCTOR
	for (int i = 0; i < result.size(); i++)
	{
		string path = result[i]["path"].asString();
		
		float x = 2.0f * result[i]["point"][0].asFloat() - 1.0f;
		float y = 2.0f * result[i]["point"][1].asFloat() - 1.0f;
		float z = 2.0f * result[i]["point"][2].asFloat() - 1.0f;
		SonicSphere tSphere =  SonicSphere(path, ofPoint(x, y, z), 0.0f);
		tSphere.sphere.setUseVbo(true);
		sonicSpheres.push_back(tSphere);
	}

	//SONIC SPHERES SETUP
	for (int i = 0; i < sonicSpheres.size(); i++)
	{
		sonicSpheres[i].setup();
	}

	currentNavPos = ofVec3f(0, 0, 0)*dimensions;
	camera.setPosition(currentNavPos);
	camera.setFarClip(10000.0f);
}

//--------------------------------------------------------------
void ofApp::update()
{
	for (int i = 0; i < sonicSpheres.size(); i++)
	{
		if (bRotateSystem)
		{
			sonicSpheres[i].sphere.rotateAround(0.1f, ofVec3f(0.12, 0.2, 0.0), ofVec3f(0, 0, 0));
		}

		sonicSpheres[i].update();
	}

	ofSoundUpdate();

	////OSC SECTION//
 //   // hide old messages //
 //   for(int i = 0; i < NUM_MSG_STRINGS; i++)
	//{
 //       if(timers[i] < ofGetElapsedTimef())
	//	{
 //           msg_strings[i] = "";
 //       }
 //   }
 //   
 //    ////check for waiting messages
 //   while(receiver.hasWaitingMessages())
 //   {
 //       // get the next message
 //       ofxOscMessage m;
 //       receiver.getNextMessage(m);
 //       
 //       // check for mouse moved message
 //       if(m.getAddress() == "/NavigatorPosition"){
 //           // both the arguments are int32's
 //           currentNavPos.x = m.getArgAsFloat(0);
 //           currentNavPos.y = m.getArgAsFloat(1);
 //           currentNavPos.z = m.getArgAsFloat(2);
 //       }
 //       //findCollisions(currentNavPos);
 //   }
 //   
    
	//Debug Move Center Point
    //currentNavPos += 2.0f*(ofNoise(ofGetElapsedTimef()*0.1))-1.0f;
}

//--------------------------------------------------------------
void ofApp::draw()
{
	ofBackgroundGradient( ofColor(12, 23, 80), ofColor(0), OF_GRADIENT_CIRCULAR);

	//UPDATE CAMERA POSITION
    //camera.setPosition(currentNavPos);
    camera.begin();
    
	//SONIC SPHERES DRAW
	for (int i = 0; i < sonicSpheres.size(); i++)
	{
		sonicSpheres[i].draw();
	}

//    for (int i=0; i<sounds.size(); i++)
//    {
//        if (sounds[i].sound.isPlaying())
//        {
//            //glBegin(GL_LINES);
//            //glColor3f(sounds[i].point.x, sounds[i].point.y,sounds[i].point.z);
//            //glVertex3f(sounds[i].point.x*dimensions.x, sounds[i].point.y*dimensions.y,sounds[i].point.z*dimensions.z);
//			//glColor3f(255, 255, 255);
//			//glVertex3f(lastPlaying.point.x, lastPlaying.point.y, lastPlaying.point.z);  
//            //glEnd();
//
////            for (int j=0; j<sounds.size(); j++)
////            {
////                    glBegin(GL_LINES);
////                    glColor3f(sounds[i].point.x, sounds[i].point.y,sounds[i].point.z);
////                    glVertex3f(sounds[i].point.x*dimensions.x, sounds[i].point.y*dimensions.y,sounds[i].point.z*dimensions.z);
////                    glColor3f(sounds[j].point.x, sounds[j].point.y,sounds[j].point.z);
////                    glVertex3f(sounds[j].point.x*dimensions.x, sounds[j].point.y*dimensions.y,sounds[j].point.z*dimensions.z);
////                    glEnd();
////                    //ofSetColor(sphereCol);
////                    //ofDrawLine(sounds[i].point * dimensions, sounds[j].point * dimensions);
////            }
//            ofSetColor(255, 255, 255, 180);
//        }
//        else
//        {
//            ofVec3f colScal = 255 * ((sounds[i].point / 2.0f) +0.5f);
//            ofColor sphereCol = ofColor(colScal.x,colScal.y,colScal.z);
//            ofSetColor(sphereCol);
//        }
//        ofDrawSphere(sounds[i].point.x * dimensions.x, sounds[i].point.y*dimensions.y, sounds[i].point.z * dimensions.z,  10);

    //}
    //DEBUG DRAW MOVING AGENT
    //ofSetColor(255, 255, 255,120);
    //ofDrawSphere(currentNavPos,55);
    //ofPopMatrix();
	camera.end();

	gui.draw();

	//DEBUG STUFF DRAW 
	ofSetWindowTitle(ofToString(ofGetFrameRate()));
	ofSetColor(ofColor(255));
	string metString = "\n//Metalli: " + ofToString(bShowMetallic);
	string liqString = +"\n// Liquidi: " + ofToString(bShowLiquid);
	string woodString = +"\n// Legno: " + ofToString(bShowWooden);
	string voiceString = +"\n// Voci: " + ofToString(bShowVoice);
	ofDrawBitmapString("Keys [1-4] to toggle sound classes." + metString + liqString + woodString + voiceString, 50, ofGetHeight()-100);

}
//--------------------------------------------------------------
void ofApp::colorLerpChanged(float &f)
{
	for (int i = 0; i < sonicSpheres.size(); i++)
	{
		sonicSpheres[i].setColorSpaceLerp(f);
	}
}
//--------------------------------------------------------------
void ofApp::updateDuration(float &f)
{
	for (int i = 0; i < sonicSpheres.size(); i++)
	{
		sonicSpheres[i].maxDur = f;
	}
}
//--------------------------------------------------------------
ofVec3f ofApp::CarToPol(ofPoint thisPoint)
{
    float azimuth = PI+atan2(thisPoint.y,thisPoint.x);
    float range = sqrt(pow(thisPoint.x,2) + pow(thisPoint.y,2) + pow(thisPoint.z,2));
    float elevation = HALF_PI-acos(thisPoint.z/range);
    ofVec3f polCoord = ofVec3f(azimuth,elevation,range);
    return polCoord;
}
//--------------------------------------------------------------

void ofApp::OscSendPointData (ofVec3f polCoords, string path)
{
    ofxOscMessage m;
    
    m.setAddress("/pointData");
    m.addStringArg(path);
    m.addFloatArg(polCoords.x);
    m.addFloatArg(polCoords.y);
    m.addFloatArg(polCoords.z);
    sender.sendMessage(m, false);
    
    //debug OSC
    //cout<<ofToString(polCoords.x)+" "+ofToString(polCoords.y)+" "+ofToString(polCoords.z)<< endl;

}

void ofApp::updateActiveClasses(string cat, bool bDraw)
{
	for (int i = 0; i < sonicSpheres.size(); i++)
	{
		if (sonicSpheres[i].sClass == cat)
		{
			sonicSpheres[i].SetActive(bDraw);
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	if (key == '1') {
		bShowMetallic = !bShowMetallic;
		updateActiveClasses("metallici", bShowMetallic);
	}
	if (key == '2') {
		bShowLiquid = !bShowLiquid;
		updateActiveClasses("liquidi", bShowLiquid);
	}
	if (key == '3') {
		bShowWooden = !bShowWooden;
		updateActiveClasses("legno", bShowWooden);
	}
	if (key == '4') {
		bShowVoice = !bShowVoice;
		updateActiveClasses("voci", bShowVoice);
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
}

void ofApp::findCollisions(ofVec3f point)
{
    //for (int i=0; i<sounds.size(); i++)
    //{
    //    float distance = ofDistSquared(point.x,point.y,point.z,sounds[i].point.x,sounds[i].point.y,sounds[i].point.z);
    //    if (distance < 0.005f && !sounds[i].sound.isPlaying() && (ofGetElapsedTimef() - sounds[i].t > pauseLength))
    //    {
    //        sounds[i].t = ofGetElapsedTimef();
    //        //sounds[i].sound.play();
    //        OscSendPointData(sounds[i].polCoords,result[i]["path"].asString());
    //    }
    //}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	float mouseX = x;
	float mouseY = y;


	for (int i = 0; i < sonicSpheres.size(); i++)
	{
		//ofPoint soundPoint = sonicSpheres[i].point * sonicSpheres[i].dimensions;
		ofPoint projectedPoint = camera.worldToScreen(sonicSpheres[i].point * sonicSpheres[i].dimensions);
		float distanceToMouse = ofDist(mouseX, mouseY, projectedPoint.x, projectedPoint.y);
		if (distanceToMouse < mouseRadius)
		{
			sonicSpheres[i].SoundPlay();
			if (bOscSend)
			{
				OscSendPointData(sonicSpheres[i].polCoords, result[i]["path"].asString());
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    
}

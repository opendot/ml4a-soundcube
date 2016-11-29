#include "ofApp.h"

ofxOscSender sender;
ofxOscReceiver receiver;
ofVec3f currentNavPos;
ofParameter<string> receiveOscIP;
ofxJSONElement result;
//--------------------------------------------------------------
void ofApp::setup(){
    
    sender.setup(HOST, SENDPORT);
    receiver.setup (RECEIVEPORT);
    gui.setup();
    gui.setName("Audio t-SNE");
    gui.add(maxDuration.set("maxDuration", 1.0, 0.1, 2.0));
    gui.add(mouseRadius.set("mouseRadius", 250, 100, 500));
    gui.add(pauseLength.set("pauseLength", 2.0, 0.2, 5.0));
    gui.add(receiveOscIP.set("sendIP","127.0.0.1"));

    dimensions = ofPoint(ofGetWidth(), ofGetHeight(), 1200.0f);
    
    ofBackground(0);
    
    string file = "points3D.json";
    
    //cout<<result[0]["path"].asString()<<endl;
    bool parsingSuccessful = result.open(file);
    
    for (int i=0; i<result.size(); i++) {
        string path = result[i]["path"].asString();
        float x = 2.0f * result[i]["point"][0].asFloat() - 1.0f;
        float y = 2.0f * result[i]["point"][1].asFloat() - 1.0f;
        float z = 2.0f * result[i]["point"][2].asFloat() - 1.0f;
        AudioClip newSound;
        //snewSound.sound.load(path);
        newSound.point.set(x, y, z);
        newSound.polCoords = CarToPol(newSound.point);
        newSound.t = 0;
        //newSound.sound.setVolume(0.06);
        newSound.sound.setVolume(0);
        sounds.push_back(newSound);
    }
    
    currentNavPos = ofVec3f(0,0,0)*dimensions;
    camera.setPosition(currentNavPos);
    
}

//--------------------------------------------------------------
void ofApp::update()
{
    
    // hide old messages
    for(int i = 0; i < NUM_MSG_STRINGS; i++){
        if(timers[i] < ofGetElapsedTimef()){
            msg_strings[i] = "";
        }
    }
    
    // check for waiting messages
    while(receiver.hasWaitingMessages())
    {
        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(m);
        
        // check for mouse moved message
        if(m.getAddress() == "/NavigatorPosition"){
            // both the arguments are int32's
            currentNavPos.x = m.getArgAsFloat(0);
            currentNavPos.y = m.getArgAsFloat(1);
            currentNavPos.z = m.getArgAsFloat(2);
        }
        findCollisions(currentNavPos);
    }
    
    for (int i=0; i<sounds.size(); i++)
    {
        if (sounds[i].sound.isPlaying() && sounds[i].sound.getPositionMS() > maxDuration*1000)
        {
            sounds[i].sound.stop();
        }
    }
    ofSoundUpdate();
    currentNavPos += 2.0f*(ofNoise(ofGetElapsedTimef()*0.1))-1.0f;
}

//--------------------------------------------------------------
void ofApp::draw(){
    camera.setPosition(currentNavPos);
    camera.begin();
    
    ofPushMatrix();
    
    for (int i=0; i<sounds.size(); i++)
    {
        if (sounds[i].sound.isPlaying())
        {
            glBegin(GL_LINES);
            glColor3f(sounds[i].point.x, sounds[i].point.y,sounds[i].point.z);
            glVertex3f(sounds[i].point.x*dimensions.x, sounds[i].point.y*dimensions.y,sounds[i].point.z*dimensions.z);
            glColor3f(255,255,255);
            glVertex3f(currentNavPos.x, currentNavPos.y,currentNavPos.z);
            glEnd();

//            for (int j=0; j<sounds.size(); j++)
//            {
//                    glBegin(GL_LINES);
//                    glColor3f(sounds[i].point.x, sounds[i].point.y,sounds[i].point.z);
//                    glVertex3f(sounds[i].point.x*dimensions.x, sounds[i].point.y*dimensions.y,sounds[i].point.z*dimensions.z);
//                    glColor3f(sounds[j].point.x, sounds[j].point.y,sounds[j].point.z);
//                    glVertex3f(sounds[j].point.x*dimensions.x, sounds[j].point.y*dimensions.y,sounds[j].point.z*dimensions.z);
//                    glEnd();
//                    //ofSetColor(sphereCol);
//                    //ofDrawLine(sounds[i].point * dimensions, sounds[j].point * dimensions);
//            }
            //ofSetColor(255, 255, 255, 180);
        }
        else
        {
            ofVec3f colScal = 255 * ((sounds[i].point / 2.0f) +0.5f);
            ofColor sphereCol = ofColor(colScal.x,colScal.y,colScal.z);
            ofSetColor(sphereCol);
        }
        ofDrawSphere(sounds[i].point.x * ofGetWidth(), sounds[i].point.y*ofGetHeight(), sounds[i].point.z * dimensions.z,  10);

    }
    //debug draw moving agent
    //ofSetColor(255, 255, 255,120);
    //ofDrawSphere(currentNavPos,55);
    ofPopMatrix();
    camera.end();
    
    ofPushMatrix();
    gui.draw();
    ofPopMatrix();

}

ofVec3f ofApp::CarToPol(ofPoint thisPoint)
{
    float azimuth = PI+atan2(thisPoint.y,thisPoint.x);
    float range = sqrt(pow(thisPoint.x,2) + pow(thisPoint.y,2) + pow(thisPoint.z,2));
    float elevation = HALF_PI-acos(thisPoint.z/range);
    ofVec3f polCoord = ofVec3f(azimuth,elevation,range);
    return polCoord;
}

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

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
}

void ofApp::findCollisions(ofVec3f point)
{
    for (int i=0; i<sounds.size(); i++)
    {
        float distance = ofDistSquared(point.x,point.y,point.z,sounds[i].point.x,sounds[i].point.y,sounds[i].point.z);
        if (distance < 0.005f && !sounds[i].sound.isPlaying() && (ofGetElapsedTimef() - sounds[i].t > pauseLength))
        {
            sounds[i].t = ofGetElapsedTimef();
            //sounds[i].sound.play();
            OscSendPointData(sounds[i].polCoords,result[i]["path"].asString());
        }
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
    for (int i=0; i<sounds.size(); i++)
    {
        ofPoint tempPoint = ofPoint(sounds[i].point.x * ofGetWidth(), sounds[i].point.y * ofGetHeight(), sounds[i].point.z * dimensions.z);
        ofPoint projectedPoint = camera.worldToScreen(tempPoint);
        float distanceToMouse = ofDistSquared(x, y, projectedPoint.x, projectedPoint.y);
        if (distanceToMouse < mouseRadius && !sounds[i].sound.isPlaying() && (ofGetElapsedTimef() - sounds[i].t > pauseLength))
        {
            sounds[i].t = ofGetElapsedTimef();
            sounds[i].sound.play();
            OscSendPointData(sounds[i].polCoords,result[i]["path"].asString());
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

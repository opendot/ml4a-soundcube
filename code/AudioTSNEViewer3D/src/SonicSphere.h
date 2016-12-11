#ifndef _SONICSPHERE // if this class hasn't been defined, the program can define it
#define _SONICSPHERE // by using this if statement you prevent the class to be called more than once which would confuse the compiler
#include "ofMain.h" // we need to include this to have a reference to the openFrameworks framework
#define XDIM 2500
#define YDIM 2500
#define ZDIM 2500
class SonicSphere 
{
	public:
	void setup();	
	void update();  
	void draw();    
	ofVec3f CarToPol(ofPoint thisPoint);
	void SetActive(bool isActive);
	void setColorSpaceLerp(float lerping);
	void SoundPlay();
	//float getSoundDuration();

	ofVec3f dimensions = ofVec3f(XDIM, YDIM, ZDIM);
	ofSpherePrimitive sphere;
	ofSoundPlayer sound;
	ofPoint point;
	ofVec3f polCoords;
	string path;
	string sClass;
	float t;
	float maxDur;
	float size;        
	ofColor classColor; 
	ofColor drawColor;
	bool bActive;
	
	//CONSTRUCTORS
	SonicSphere();
	SonicSphere(string path, ofPoint point,float t);
	
	
	private: 
		
}; 
#endif

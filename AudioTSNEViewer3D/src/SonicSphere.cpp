#include "SonicSphere.h"
SonicSphere::SonicSphere()
{
}

SonicSphere::SonicSphere(string _path, ofPoint _point, float _t)
{
	bActive = true;
	path = _path;
	point = _point;
	t = _t;
}

void SonicSphere::setup()
{
	//GetClass
	vector<string> split = ofSplitString(path, "/");
	if (split.size() > 1)
	{
		sClass = (ofSplitString(path, "/")[split.size() - 2]);
	}
	//cout << sClass << endl;
	
	//SPHERE SETUP
	size = 10.0f;
	sphere.setRadius (size);
	sphere.setPosition (point*dimensions);
	sphere.setResolution(5);
	
	//SOUND SETUP
	t = 0;
	sound.setVolume(0.06);
	polCoords = CarToPol(point);
	
	//Set CMYK main color based on class/category
	if (sClass == "liquidi")
	{
		classColor = ofColor(0, 255, 255,90);
	}
	if (sClass == "metallici")
	{
		classColor = ofColor(255, 255, 0,90);
	}
	if (sClass == "legno")
	{
		classColor = ofColor(255, 0, 255,90);
	}
	if (sClass == "voci")
	{
		classColor = ofColor(0, 255, 0,90);
	}

	drawColor = classColor;
	maxDur = 1000.0f;
}

void SonicSphere::update()
{
	if (sound.getPositionMS() > maxDur * 1000)
	{
		sound.stop();
		//sound.unload();
		return;
	}
	if (!sound.isPlaying() && sound.isLoaded())
	{
		//cout << "Unloading Sound: " + path << endl;
		//sound.unload();
		return;
	}
}

void SonicSphere::draw()
{
	if (bActive)
	{
		if (!sound.isPlaying())
		{
			ofSetColor(drawColor);
		}
		else
		{
			ofSetColor(ofColor(255, 255, 255,255));
		}
		sphere.draw();
	}
}

void SonicSphere::SoundPlay()
{
	if (bActive)
	{
		if (!sound.isLoaded())
		{
			sound.load(path);
		}

		if (!sound.isPlaying() && (ofGetElapsedTimef() - t > 1.0f))
		{
			t = ofGetElapsedTimef();
			cout << "Play: " + path<< endl;
			sound.play();
		}
	}
}

void SonicSphere::setColorSpaceLerp(float lerping)
{
	ofVec3f normColorPoint = 255*((point / 2.0f) + 0.5f);
	ofColor tempCol = classColor;
	drawColor= tempCol.lerp(ofColor(normColorPoint.x, normColorPoint.y, normColorPoint.z), lerping);
}

void SonicSphere::SetActive(bool isActive)
{
	bActive = isActive;
}

ofVec3f SonicSphere::CarToPol(ofPoint thisPoint)
{
	return ofVec3f(0, 0, 0);
}

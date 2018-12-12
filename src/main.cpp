#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
//    ofSetupOpenGL(1920,1080,OF_WINDOW);
//    ofSetupOpenGL(1600,900,OF_WINDOW);
//    ofSetupOpenGL(1280,720,OF_WINDOW);
    ofSetupOpenGL(1024, 576,OF_WINDOW); // fov = 42.5

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}

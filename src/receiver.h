//
//  receiver.h
//  ar-facepalm
//
//  Created by Charles Holbrow on 12/7/18.
//

#ifndef receiver_h
#define receiver_h

#include "ofMain.h"
#include "ofxOsc.h"

struct CameraOrientation {
    ofVec3f pos = ofVec3f(1, 1, 1);
    ofQuaternion quat;
};

class Receiver : public ofThread {
public:
    void threadedFunction();
    CameraOrientation getState();
    float getFov();
    float getScale();
    
protected:
    ofxOscReceiver oscReceiver;
    CameraOrientation state;
    float fov = 31.333;
    float scale = 800;
};

#endif /* receiver_h */

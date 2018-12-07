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
    float roll = 1;
    float yaw = 1;
    float pitch = 1;
};

class Receiver : public ofThread {
public:
    void threadedFunction();
    CameraOrientation getState();
    
protected:
    ofxOscReceiver oscReceiver;
    CameraOrientation state;
};

#endif /* receiver_h */

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
#include "msgDelay.h"

struct Orientation7 {
    ofVec3f pos = ofVec3f(1, 1, 1);
    ofQuaternion quat;
};

class Receiver : public ofThread {
public:
    void threadedFunction();
    Orientation7 getCamera();
    float getFov();
    float getScale();
    float getDelay();
    Orientation7 getController();

protected:
    MsgDelay<Orientation7> cameraMessages;
    ofxOscReceiver oscReceiver;
    Orientation7 cameraState;
    Orientation7 controllerState;
    float fov = 39;
    float scale = 1000;
    float delay = 0.057;
};

#endif /* receiver_h */

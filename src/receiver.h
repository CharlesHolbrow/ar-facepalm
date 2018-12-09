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
    float getDelay();

protected:
    MsgDelay<CameraOrientation> cameraMessages;
    ofxOscReceiver oscReceiver;
    CameraOrientation cameraState;
    float fov = 31.3333;
    float scale = 800;
    float delay = 0.08;
};

#endif /* receiver_h */

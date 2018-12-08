#include "receiver.h"

void Receiver::threadedFunction() {
    oscReceiver.setup(6969);
    while(true) {
        while(oscReceiver.hasWaitingMessages()){
            ofxOscMessage m;
            oscReceiver.getNextMessage(m);
            if (m.getAddress() == "/pos") {
                ofVec3f pos = ofVec3f(m.getArgAsFloat(0),
                                      m.getArgAsFloat(1),
                                      m.getArgAsFloat(2));
                float w, x, y, z;
                w = m.getArgAsFloat(3);
                x = m.getArgAsFloat(4);
                y = m.getArgAsFloat(5);
                z = m.getArgAsFloat(6);
                ofQuaternion quat = ofQuaternion(x, y, z, w);
                // Adust for the mout angle of the vive tracker on the camera
                static const ofQuaternion r1 = ofQuaternion(90, ofVec3f(1, 0, 0));
                static const ofQuaternion r2 = ofQuaternion(180, ofVec3f(0, 1, 0));
                quat = r1 * r2 * quat;

                lock();
                state.pos = pos;
                state.quat = quat;
                unlock();
            }
        }
        ofSleepMillis(1);
    }
}

CameraOrientation Receiver::getState() {
    CameraOrientation result;
    lock();
    result = state;
    unlock();
    return result;
}

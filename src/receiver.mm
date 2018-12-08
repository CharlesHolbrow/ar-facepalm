#include "receiver.h"

void Receiver::threadedFunction() {
    oscReceiver.setup(6969);
    while(true) {
        while(oscReceiver.hasWaitingMessages()){
            ofxOscMessage m;
            oscReceiver.getNextMessage(m);
            if (m.getAddress() == "/pos") {
                lock();
                state.pos.x = m.getArgAsFloat(0);
                state.pos.y = m.getArgAsFloat(1);
                state.pos.z = m.getArgAsFloat(2);

                float a, b, c, d;
                a = m.getArgAsFloat(3);
                b = m.getArgAsFloat(4);
                c = m.getArgAsFloat(5);
                d = m.getArgAsFloat(6);

                state.quat = ofQuaternion(a, b, c, d);
                ofVec3f ang = state.quat.getEuler();
                // ang.x represents movement around the 1/4 inch thingy
                auto v1 = ofVec3f(-1, 0, 0);
                auto v2 = ofVec3f(0, 1, 0);
                auto v3 = ofVec3f(0, 0, -1);
                state.quat.makeRotate(ang.z, v1, ang.y, v2, ang.x, v3);

//                ofLog(OF_LOG_NOTICE, "(%5.3f, %5.3f, %5.3f)", state.pos.x, state.pos.y, state.pos.z);
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

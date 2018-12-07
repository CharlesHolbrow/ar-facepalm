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
                state.roll = m.getArgAsFloat(3);
                state.yaw = m.getArgAsFloat(4);
                state.pitch = m.getArgAsFloat(5);
                ofLog(OF_LOG_NOTICE, "(%5.3f, %5.3f, %5.3f)", state.pos.x, state.pos.y, state.pos.z);
                unlock();
            }
        }
        ofSleepMillis(100);
    }
}

CameraOrientation Receiver::getState() {
    CameraOrientation result;
    lock();
    result = state;
    unlock();
    return result;
}

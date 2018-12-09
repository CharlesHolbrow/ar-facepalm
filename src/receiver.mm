#include "receiver.h"

void Receiver::threadedFunction() {
    oscReceiver.setup(6969);
    while(true) {
        while(oscReceiver.hasWaitingMessages()){
            ofxOscMessage m;
            oscReceiver.getNextMessage(m);
            auto addr = m.getAddress();
            if (addr == "/pos") {
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
            } else if (addr == "/fov") {
                lock();
                fov = m.getArgAsFloat(0);
                unlock();
            } else if (addr == "/scale") {
                lock();
                scale = m.getArgAsFloat(0);
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

float Receiver::getFov() {
    float result;
    lock();
    result = fov;
    unlock();
    return result;
}

float Receiver::getScale() {
    float result;
    lock();
    result = scale;
    unlock();
    return result;
}

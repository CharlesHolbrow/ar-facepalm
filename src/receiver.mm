#include "receiver.h"

void Receiver::threadedFunction() {
    oscReceiver.setup(6969);
    while(true) {
        while(oscReceiver.hasWaitingMessages()){
            ofxOscMessage m;
            oscReceiver.getNextMessage(m);
            auto addr = m.getAddress();
            if (addr == "/pos") {
                lock();
                // Conver the message to 3d vector and a quaternion
                ofVec3f pos = ofVec3f(m.getArgAsFloat(0) * scale,
                                      m.getArgAsFloat(1) * scale,
                                      m.getArgAsFloat(2) * scale);
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

                Orientation7 result;
                result.pos = pos;
                result.quat = quat;

                double time = static_cast<double>(ofGetElapsedTimeMicros() * 0.000001);
                cameraMessages.add(time, result);
                unlock();
            } else if (addr == "/controller") {
                lock();
                ofVec3f pos = ofVec3f(m.getArgAsFloat(0) * scale,
                                      m.getArgAsFloat(1) * scale,
                                      m.getArgAsFloat(2) * scale);
                float w, x, y, z;
                w = m.getArgAsFloat(3);
                x = m.getArgAsFloat(4);
                y = m.getArgAsFloat(5);
                z = m.getArgAsFloat(6);
                ofQuaternion quat = ofQuaternion(x, y, z, w);
                controllerState.pos = pos;
                controllerState.quat = quat;
                unlock();
            } else if (addr == "/fov") {
                lock();
                fov = m.getArgAsFloat(0);
                unlock();
            } else if (addr == "/scale") {
                lock();
                scale = m.getArgAsFloat(0);
                unlock();
            } else if (addr == "/delay") {
                lock();
                delay = m.getArgAsFloat(0);
                unlock();
            }
        }
        ofSleepMillis(1);
    }
}

Orientation7 Receiver::getCamera() {
    double time = static_cast<double>(ofGetElapsedTimeMicros() * 0.000001);
    lock(); // --- LOCK ---

    time -= delay;
    // only update the state if we got a new message;
    if (cameraMessages.hasMessageAt(time)) cameraState = cameraMessages.getMessageAt(time);
    Orientation7 result = cameraState;

    unlock(); // --- UNLOCK ---
    return result;
}

Orientation7 Receiver::getController() {
    Orientation7 result;
    lock();
    result = controllerState;
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

float Receiver::getDelay() {
    float result;
    lock();
    result = delay;
    unlock();
    return delay;
}

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
//    ofEnableDepthTest();
    ofSetFrameRate(90);
    stepper.setStepSize(1. / 2000.);
    ofLog() << "Ticks per frame @ 60fps: " << 1. / 60. / stepper.getStepSize();

    // Threaded OSC Receive
    receiver.startThread();

    // Setup the 3d scene camera
    cam.disableMouseInput();
    cam.setPosition(110, 110, 665);
    cam.lookAt(ofVec3f(0), ofVec3f(0, 1, 0));
    cam.setFov(63);
    //cam.setAspectRatio(float) // By default the screen size is used

    // setup
    if (videoIn.setup(0)) {
//        videoIn.start(bmdModeHD1080i5994); // works
//        videoIn.start(bmdModeHD1080p30);   // works
//        videoIn.start(bmdModeHD720p60);    // works
//        videoIn.start(bmdModeHD1080p60);   // unknown
        videoIn.startAutoDisplayMode();
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    uint64_t microseconds = ofGetElapsedTimeMicros();
    uint64_t deltaMicroseconds = microseconds - previousMicroseconds;

    // What time did the frame start?
    double frameStart = static_cast<double>(previousMicroseconds * 0.000001);
    // time in seconds
    double frameEnd = static_cast<double>(microseconds * 0.000001);
    // delta time in seconds
    double frameDelta = static_cast<double>(deltaMicroseconds * 0.000001);

    // Setup the Stepper for this frame
    stepper.advanceFrame(frameDelta);
    
    // Process all input
    bool down = ofGetMousePressed();
    MouseEvent mouse;
    mouse.press = down && !previousMouse.isDown;
    mouse.release = !down && previousMouse.isDown;
    mouse.isDown = down;
    mouse.pos = ofVec2f(ofGetMouseX(), ofGetMouseY());
    mouse.previousPos = previousMouse.pos;
    mouse.vel = (mouse.pos - previousMouse.pos) / stepper.stepsDuration();

    // Get the position of the Tracker
    Orientation7 cor = receiver.getCamera();
    Orientation7 controller = receiver.getController();
    
    // calculate the world position of the mouse
    ofVec3f world = cam.screenToWorld(ofVec3f(mouse.pos.x, mouse.pos.y, 0));
    ofVec3f ray = (world - cam.getGlobalPosition()).normalize() * 900;
//    mouse.worldPos = cam.getGlobalPosition() + ray;
    mouse.worldPos = controller.pos;
    mouse.previousWorldPos = previousMouse.worldPos;
    mouse.worldVel = (mouse.worldPos - mouse.previousWorldPos) / stepper.stepsDuration();

    cam.setOrientation(cor.quat);
    cam.setPosition(cor.pos);
    cam.setFov(receiver.getFov());

    // tick our content
    content.update(stepper, mouse);
    
    // BlackMagic video
    videoIn.update();

    previousMicroseconds = microseconds;
    previousMouse = mouse;
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0, 0, 0);
    ofSetColor(255);
    ofClear(0);
    videoIn.draw(0, 0, ofGetWidth(), ofGetHeight());

    // DEBUG: info about our camera
    ofVec3f cPos = cam.getPosition();
    char str [100];
    std::sprintf(str, "Camera: %4.0f %4.0f %4.0f\tp:%4.0f y:%4.0f r:%4.0f",
                 cPos.x, cPos.y, cPos.z,
                 cam.getPitch(),
                 cam.getHeading(),
                 cam.getRoll());
    ofSetColor(255, 0, 0);
    ofDrawBitmapString(str, 2, 12);

    cam.begin();
        content.render();
        // Debug Info
        ofDrawAxis(100);
        for (ofNode n : nodes) {
            n.draw();
        }
    cam.end();

    if (state == RECORDING) {
        ofSetColor(155 + 100 * sin(ofGetElapsedTimef() * 4), 0, 0);
        ofDrawCircle(10, 10, 5);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    ofLog() << "Press: " << key;
    // Tab toggles recording state
    if (key == OF_KEY_TAB) {
        if (state == PLAYING) {
            state = RECORDING;
        }
        else {
            state = PLAYING;
            content.terminateMainGesture();
        }
        return;
    }
    ofNode n;
    switch(key){
        case OF_KEY_RETURN:
            content.replayMainGesture();
            break;
        case OF_KEY_BACKSPACE:
            content.terminateMainGesture();
            break;
        case OF_KEY_SHIFT:
            break;
        case OF_KEY_RIGHT_SHIFT:
            break;
        case ' ':
            n.setPosition(cam.getGlobalPosition());
            n.setOrientation(cam.getGlobalOrientation());
            nodes.push_back(n);
            break;
        default:
            if (state == PLAYING) {
                content.playSavedGesture(key);
            } else if (state == RECORDING) {
                content.mapMainGestureToKey(key);
                content.terminateMainGesture();
            }
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

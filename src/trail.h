//
//  trail.h
//  Follower
//
//  Created by Charles Holbrow on 11/26/18.
//

#ifndef trail_h
#define trail_h

#include "gesturePlayhead.h"

const static ofColor WHITE = ofColor(255, 255, 255);
const static ofColor BLACK = ofColor(0, 0, 0);

class Particle {
public:
    ofVec3f pos;
    ofVec3f velocity;
    double size;

    ofColor ofc = WHITE;

    void update(double deltaTime) {
        pos += velocity * deltaTime;
        size -= 0.6 * deltaTime;
        size = (size < 0) ? 0 : size;
    };

    ofColor color() {
        const float scale = ofMap(size, 0, 12, 0.1, 1, true);
        return BLACK.getLerped(ofc, scale);
    };

    bool isDead() {
        return (size < 1) ;
    };

    void render() {
        ofSetColor(color());
        ofDrawCircle(pos.x, pos.y, pos.z, size);
    };
};

class Trail {
public:
    GesturePlayhead playhead;
    float speed = 1;
    std::list <Particle> parts;
    void clear() {
        parts.clear();
    };

    // Update each particle
    void update(float deltaTime) {
        for (auto p = parts.begin(); p != parts.end(); p++) {
            p->update(deltaTime * speed);
        }
        // remove small particles from the "front"
        while (parts.size() > 0 && parts.front().isDead()) {
            parts.pop_front();
        }

        // see if there are new blips from the gesture
        double updateTime = 0;
        for (auto b : playhead.update(deltaTime)) {
            ofSetColor(127. + 127. * sin(b.gestureTime), 255, 255);
            add(b.pos, 16);
            updateLast(playhead.playbackTime - b.gestureTime);
        }
    };
    
    void updateLast(double deltaTime) {
        if (!parts.size()) return;
        parts.back().update(deltaTime * speed);
    };

    // Add a single particle, and update with deltaTime. Calls ofGetStyle for color.
    void add(ofVec3f pos, float size) {
        Particle p;
        p.size = size;
        p.pos = pos;
        p.velocity.x = 0;
        p.velocity.y = 0;
        p.ofc = ofGetStyle().color;
        parts.push_back(p);
    }

    // Draw the trail
    void render() {
        for (auto p = parts.begin(); p != parts.end(); p++) {
            p->render();
        }
    };
    
    bool isDead() {
        // For now, let's consider Trails with uninitialized gestures dead. I'm
        // not totally sure that this is the best practice, but it's probably
        // best to initialize the gesture as soon as the trail is created, until
        // we decide exactly how we want to handle constructors.
        if (playhead.gesture == NULL) return true;

        // verify playback is complete AND there are no remaining blips
        return playhead.playbackComplete() && parts.empty();
    };
};


#endif /* trail_h */

//
//  gesture.h
//  Hike
//
//  Created by Charles Holbrow on 11/10/18.
//
// Gesture is a touch input recorder. It keeps a sequential, timestamped
// list of touch events.

#ifndef gesture_h
#define gesture_h

#include <list>
#include <vector>

#include "ofMain.h"
#include "stepper.h"
#include "filter.h"

// A Blip on the radar. Timing and position of a point in the gesture.
struct Blip {
    // The x,y position of this blip
    ofVec3f pos;
    // How long into the gesture was this created?
    double gestureTime;
    // Is this the last blip in a gesture?
    bool terminal;
};

struct MouseEvent {
    ofVec2f pos;
    ofVec2f previousPos;
    ofVec2f vel;
    ofVec3f worldPos;
    ofVec3f worldVel;
    ofVec3f previousWorldPos;
    bool isDown;
    bool press;
    bool release;
};

class Gesture {
private:
    // Playhead position, so consecutive play() calls know where to pick up from
    double playbackTime;
    // The last added blip
    double recordingTime;
    // Smooth out input values
    Filter <ofVec3f, 60> filter;
public:
    std::vector<Blip> blipsVec;
    std::string id;

    // How many blips in the gesture? -1 means the gesture is not yet terminated
    int totalBlips = -1;

    // Clear all blips, and start a new gesture. (does not change stepper size)
    void reset();

    // Construct the gesture by adding a new blips
    void update(Stepper stepper, MouseEvent mouse);

    // terminate the gesture
    void terminate();

    // How many unplayed blips are in the gesture?
    int size();
};

#endif /* gesture_h */

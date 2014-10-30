#pragma once

#include "ofMain.h"
#include "ofxLeapMotion.h"
#include "ofxStrip.h"
#include "hParticle.h"

class testApp : public ofBaseApp{

  public:
    void setup();
    void update();
    void draw();
	
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void exit();
		
	ofxLeapMotion leap;
	vector <ofxLeapMotionSimpleHand> simpleHands;
    
	vector <int> fingersFound; 
	
	map <int, ofPolyline> fingerTrails;
    
    float averageX[2], averageY[2], averageZ[2];
    
    float handRadius[2];
    
    float tempRadius;
    
    //for h particles.
    ofImage bgImg;
    vector<hParticle> hhh;
    
    
    ofImage hurricane;

};

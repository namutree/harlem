//
//  hParticle.h
//  hParticle_repel_leap
//
//  Created by John Namu Choi on 7/8/14.
//
//

#ifndef __hParticle_repel_leap__hParticle__
#define __hParticle_repel_leap__hParticle__

#include <iostream>
#include "ofMain.h"


class hParticle {
public:
    hParticle();
    hParticle(float x, float y);
    
    void update();
    void display();
    void explosion();
    
    
    ofPoint location;
    ofPoint originalLocation;
    
    ofPoint vel;
    ofPoint acc;
    
    ofImage img;
    
    float timeInterval;
    
    ofPoint explodeValue;
};

#endif /* defined(__hParticle_repel_leap__hParticle__) */

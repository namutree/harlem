//
//  hParticle.cpp
//  hParticle_repel_leap
//
//  Created by John Namu Choi on 7/8/14.
//
//

#include "hParticle.h"

hParticle::hParticle(){
}

hParticle::hParticle(float x, float y){
    
    float rgby = ofRandom(10);
    if (rgby<2.5) img.loadImage("h0012.png");
    else if (rgby<5) img.loadImage("h001r2.png");
    else if (rgby<7.5) img.loadImage("h001g2.png");
    else img.loadImage("h001b2.png");
    
    timeInterval = ofRandom(10,20);
    location = ofPoint(x,y);
    originalLocation = ofPoint(x,y);
    
    vel = ofPoint(0,0);
    acc = ofPoint(0,0);
    
    explodeValue = ofPoint(0,0);
    
}



void hParticle::update(){
    
    if(location != originalLocation){
        ofPoint temp = originalLocation -location;
        
        location += temp/20;
        
        acc = acc*0.8;
        vel = vel*0.9;
        if(abs(temp.x) +abs(temp.y)<1) {
            location = originalLocation;
            acc = acc*0;
            vel = vel*0;
        }
    }
    
    vel += acc;
    location += vel;
 
}

void hParticle::display(){
    
    ofPushMatrix();
    ofTranslate(location);
    ofRotateZ(ofGetElapsedTimeMillis()/timeInterval);
    img.draw(-img.width/2,-img.height/2);
    //ofSetColor(ofRandom(100,255), ofRandom(100,255), ofRandom(100,255),ofRandom(100,255));
   // ofRect(-1, -1, 2, 2);
    ofPopMatrix();
    
    
//// lines between originalpoint and changed location
//    if(location != originalLocation){
//        ofPushStyle();
//        ofSetColor(255,80);
//        ofLine(location, originalLocation);
//        ofPopStyle();
//    }
    
}

void hParticle::explosion(){
    acc += explodeValue;
    
}



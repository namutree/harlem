#include "testApp.h"

/* Note on OS X, you must have this in the Run Script Build Phase of your project. 
where the first path ../../../addons/ofxLeapMotion/ is the path to the ofxLeapMotion addon. 

cp -f ../../../addons/ofxLeapMotion/libs/lib/osx/libLeap.dylib "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/MacOS/libLeap.dylib"; install_name_tool -change ./libLeap.dylib @executable_path/libLeap.dylib "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/MacOS/$PRODUCT_NAME";

   If you don't have this you'll see an error in the console: dyld: Library not loaded: @loader_path/libLeap.dylib
*/

//--------------------------------------------------------------
void testApp::setup(){

    //ofSetFrameRate(60);
    
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    // hide mouse
        CGDisplayHideCursor(NULL);

	leap.open();
    

	//glEnable(GL_DEPTH_TEST);
    //glEnable(GL_NORMALIZE);
    
    
    //////////////////////////////////////////////////////////////////////////
    //////////////////for h particle background setting//////////////////////
    /////////////////////////////////////////////////////////////////////////
    bgImg.loadImage("background.jpg");
    unsigned char *data = bgImg.getPixels();
    
    //Calculate number of pixel components
	int components = bgImg.bpp / 8;
    
	//Modify pixel array
	for (int y=0; y<bgImg.height; y++) {
		for (int x=0; x<bgImg.width; x++) {
            
			//Read pixel (x,y) color components
			int index = components * (x + bgImg.width * y);
			int red = data[ index ];
			int green = data[ index + 1 ];
			int blue = data[ index + 2 ];
            
			//Calculate periodical modulation
			float u = (red + green + blue)/3;
            
            // get the coordination of the whites (0.4%)
            if(u>200 && ofRandom(10)<0.2) {
                hhh.push_back(hParticle(x + (ofGetWidth()-bgImg.width)/2, y + (ofGetHeight()-bgImg.height)/2));
            }
		}
	}
	//Calling image.update() to apply changes
	bgImg.update();
    

    hurricane.loadImage("hurricane.png");
    
    
    handRadius[0] = 0;
    handRadius[1] = 0;
    
    tempRadius=0;
    cout << hhh.size() << endl;
}


//--------------------------------------------------------------
void testApp::update(){

	fingersFound.clear();
	

    //Option 1: Use the simple ofxLeapMotionSimpleHand - this gives you quick access to fingers and palms. 
    
    simpleHands = leap.getSimpleHands();
    
    if( leap.isFrameNew() && simpleHands.size() ){

        leap.setMappingX(-230, 230, 0, ofGetWidth());
		leap.setMappingY(90, 490, ofGetHeight(),0);
        leap.setMappingZ(-150, 150, -200, 200);
    
    
        for(int i = 0; i < simpleHands.size(); i++){
        
            for(int j = 0; j < simpleHands[i].fingers.size(); j++){
                int id = simpleHands[i].fingers[j].id;
                
                ofVec3f handMid(simpleHands[i].handPos.x, simpleHands[i].handPos.y,simpleHands[i].handPos.z);
                ofVec3f fingers(simpleHands[i].fingers[j].pos.x, simpleHands[i].fingers[j].pos.y, simpleHands[i].fingers[j].pos.z);
                handRadius[i] = handMid.distance(fingers);
                
                handRadius[i] += handRadius[i];

                
                
                //store fingers seen this frame for drawing
                fingersFound.push_back(id);
            }
        }
        
        // find the average location of 5 fingers of each hand
        for(int i=0; i<simpleHands.size();i++){
            handRadius[i] = handRadius[i]/5;
        }
    }
 
	//IMPORTANT! - tell ofxLeapMotion that the frame is no longer new.
	leap.markFrameAsOld();
    
    
//    for(int i =0; i<hhh.size();i++){
//        float accel[hhh.size()];
//        if(hhh[i].location != hhh[i].originalLocation){
//            accel[i]++;
//        }
//        if (!(leap.isFrameNew() && simpleHands.size())) {
//            hhh[i].acc += ofRandom(-accel[i], accel[i]);
//        }
//    }
    
    
}

//--------------------------------------------------------------
void testApp::draw(){
	
    ofBackground(0,0,0);
	
	ofSetColor(200);
	//ofDrawBitmapString("ofxLeapMotion - Example App\nLeap Connected? " + ofToString(leap.isConnected()) +"\nframerate: " + ofToString(ofGetFrameRate()), 20, 20);


    for (int i=0 ; i<hhh.size();i++){
        hhh[i].update();
        hhh[i].display();
    }


    
    for(int i = 0; i < simpleHands.size(); i++){
        
        for(int j = 0; j < simpleHands[i].fingers.size(); j++){
            
            float fingersX = simpleHands[i].fingers[j].pos.x;
            float fingersY = simpleHands[i].fingers[j].pos.y;

        
            ofPushStyle();
            ofSetColor(255, 0, 100,70);

            float radius = ofMap(handRadius[i], 60,90,0,ofGetWidth()/4);//40,80,0,ofGetWidth()/2.6);
            if (radius <0)  radius =0;
            
            
            
            /// hand/////////////////////////////////////
            
            tempRadius += (radius-tempRadius)*0.2;
            ofPushMatrix();
            ofSetColor(200);
            ofTranslate(simpleHands[i].handPos.x , simpleHands[i].handPos.y);
            ofRotateZ(ofGetElapsedTimeMillis()/5);
            hurricane.draw(-tempRadius/2,-tempRadius/2, tempRadius, tempRadius);
            ofPopMatrix();
            
            
            
            ofSetColor(0,0,255);
            //ofEllipse(simpleHands[i].handPos.x, simpleHands[i].handPos.y, 10, 10);
            ofPopStyle();
            
           // cout << handRadius[i] <<" radius: " << radius <<" tempra: "<< tempRadius<< endl;
            
            ////////////////////////////////////////////////////////////////////////////////////
            ////////////////// Hs in the distance will be affected by force..../////////////////
            ////////////////////////////////////////////////////////////////////////////////////
            
            for(int a=0; a<hhh.size() ; a++){
                float distance = ofDist(hhh[a].location.x, hhh[a].location.y, simpleHands[i].handPos.x, simpleHands[i].handPos.y);
                if (distance < tempRadius){
                    
                    float theta = atan2(hhh[a].location.y-simpleHands[i].handPos.y, hhh[a].location.x-simpleHands[i].handPos.x);
                    
                    hhh[a].acc = ofPoint(cos(theta + PI/8 +PI), sin(theta + PI/8+PI))*7;//*5;
                    //hhh[a].vel += ofPoint(ofRandom(-1,1), ofRandom(-1,1));
                    
                    //cout << "theta: "<<theta<<endl;
                    
                    
//                    if(a%4==0) hhh[a].explodeValue += ofPoint(ofRandom(-1,0.8),ofRandom(-1,0.8),0);
//                    else if(a%4==1) hhh[a].explodeValue += ofPoint(ofRandom(-0.8,1),ofRandom(-0.8,1),0);
//                    else if(a%4==2) hhh[a].explodeValue += ofPoint(ofRandom(-1,0.8),ofRandom(-0.8,1),0);
//                    else if(a%4==3) hhh[a].explodeValue += ofPoint(ofRandom(-0.8,1),ofRandom(-1,0.8),0);
                    
                    //hhh[a].explodeValue += ofPoint(ofRandom(-1,1),ofRandom(-1,1),0);
                    
                    if(a%2==0) hhh[a].explodeValue += ofPoint(ofRandom(-1,0.8),0);
                    else if(a%2==1) hhh[a].explodeValue += ofPoint(ofRandom(-0.8,1),0);
                    
                    if(abs(hhh[a].explodeValue.x) + abs(hhh[a].explodeValue.y) > 70) hhh[a].explodeValue = hhh[a].explodeValue/1.5;
                    
                }
            }
        }
    }
    
    
    /// when making the fist, it is going to explode!!!!//////////////
    for(int i=0; i<hhh.size() ; i++){
        if(tempRadius<3){
            hhh[i].explosion();
            //if(hhh[i].explodeValue != ofPoint(0,0,0)) cout << "yah " << hhh[i].explodeValue <<endl;
            hhh[i].explodeValue = ofPoint(0,0);
        }
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
  
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
    
}

//--------------------------------------------------------------
void testApp::exit(){
    // let's close down Leap and kill the controller
    leap.close();
}

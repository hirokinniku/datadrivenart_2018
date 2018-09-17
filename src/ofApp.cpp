#include "ofApp.h"
#include "ofMain.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(233,255);
    
    brush.load("brush.png");
    
    gui.setup();
    gui.add(strokeWidth.setup("width",40,10,100));
    gui.add(weight.setup("weight",1,1,8));
    gui.add(sumMax.setup("sumMax",6,1,40));
    gui.add(ratioBias.setup("ratio Bias",0.0,-5.0,2.0));
    gui.add(sumBias.setup("sum Bias",0.15,-3.0,3.0));
    guiDraw = false;
    
    state = STATE_WATER;
    currentPigment = 0;
    ofSetFrameRate(25);
    
    pressed = true;
    
    //circle settings
    index = 0;//draw pts index
    ratio = 0.1;
    float radius = 420;
    float angle = PI/2;
    
    line.addVertex(ofGetWidth()/2 + radius*cos(angle), ofGetHeight()/2 + radius*sin(angle));
    while (angle <= TWO_PI+PI/2 ) {
        line.curveTo( ofPoint(ofGetWidth()/2 + radius*cos(angle), ofGetHeight()/2 + radius*sin(angle)));
        angle += TWO_PI / 100;//sampling num
    }
    line.addVertex(ofGetWidth()/2 + radius*cos(angle), ofGetHeight()/2 + radius*sin(angle));
    ptsNum = line.getVertices().size();
    
    drawline = line;
    drawline.resize(int(ptsNum * ratio));
    drawPts = drawline.getVertices();
    drawPtsNum = drawPts.size();
    
    //openbci settings
    receiver.setReceivePort(PORT);
    
}



//--------------------------------------------------------------
void ofApp::update(){
    pos = drawPts[index];
    
    canvas.update();
    
    receiver.update();
    static float sum = 0;
    //get average of all value in all channel
    if(receiver.isReceiving()){
        receiver.addToOneChannel();
        sum = 0;
        for(auto&& v : receiver.values[0]){
            sum += v;
        }
        sum /= receiver.values[0].size();
        //        ratio = ofClamp(ratioBias + (sumMax - pow(MAX(sum - sumBias,0),weight))/(sumMax + 0),0,1.);
        ratio = ofClamp(ofMap(sum,1,0.15,0,1),0,1);
        resetDrawLine();
    }else{
        //        ratio = ofClamp(ratioBias,0,1);
        //        resetDrawLine();
    }
    if (pressed) {strokeDraw();}
    
    
    stringstream ss;
    ss << "fps: " << int(ofGetFrameRate());
    ss << " index: " << index << "/" << (int(ptsNum * ratio)) << " ratio: " << ofToString(ratio,3) <<" sum: " <<  ofToString(sum,3);
    ofSetWindowTitle(ss.str());
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    canvas.draw();
    if(guiDraw) gui.draw();
    
}

//--------------------------------------------------------------
void ofApp::strokeDraw() {
    int counter = 0;
    ofPoint target = drawPts[index + 10]; //10 index ahead
    if(index + 10 >= drawPtsNum) target = drawPts[index];
    float len = pos.distance(target);
    
    static float cnt = 0;
    
    while (counter < 1000) {
        counter ++;
        currentWidth = strokeWidth + 5 * ofSignedNoise(cnt);
        
        vec += (target - pos).normalize() * 0.03;
        vec *= 0.97;
        if (vec.length() > 2) {
            vec = vec.normalize() * 2;
        }
        
        pos += vec;
        
        if (state == STATE_WATER || state == STATE_MIX) {
            canvas.beginWaterDraw();
            ofPushStyle();
            ofSetColor(255, 255);
            brush.draw(pos.x - currentWidth * 1.05 / 2 * 2, pos.y - currentWidth * 1.05/ 2 * 2, currentWidth * 1.05 * 2, currentWidth * 1.05 * 2);
            ofPopStyle();
            canvas.endWaterDraw();
        }
        
        if (pos.distance(target) < 10) {break;}
    }
    index += 5;
    if(index >= drawPtsNum) index = drawPtsNum * 0.9;
    
    cnt += 0.1;
    if(cnt > 5) cnt = 0;
}



//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    switch (key) {
        case OF_KEY_LEFT:
            ratioBias = ratioBias - 0.05;
            break;
        case OF_KEY_RIGHT:
            ratioBias = ratioBias + 0.05;
            break;
        case OF_KEY_DOWN:
            ratioBias = ratioBias - 0.5;
            break;
        case 'g' :
            guiDraw = !guiDraw;
            break;
        case 'f' :
            ofToggleFullscreen();
            break;
        case 'r' :
            ratioBias = 0;
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}
void ofApp::resetDrawLine(){ //have to be fixed
    drawline = line;
    drawline.resize(int(ptsNum * ratio));
    drawPts = drawline.getVertices();
    drawPtsNum = drawPts.size();
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
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}


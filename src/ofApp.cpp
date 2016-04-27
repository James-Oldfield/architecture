#include "ofApp.h"
#include "ofxDatGui.h"

using namespace ofxCv;
using namespace cv;

void ofApp::setup() {
  // Shard recursive
  Architecture img1("shard.jpg", 210, 0);
  Architecture img2("shard.jpg", 210, 1);

  // Stacked
  Architecture img3("plain.jpg", 185, 0);
  Architecture img4("gold.jpg", 237, 0);

  // BK
  Architecture img5("brut2.jpg", 210, 0);
  Architecture img6("bk2.jpg", 232, 0);
  
  // Camden
  Architecture img7("straight2.jpg", 140, 0);
  Architecture img8("camden.jpg", 320, 0);

  images.push_back(img1);
  images.push_back(img2);
  images.push_back(img3);
  images.push_back(img4);
  images.push_back(img5);
  images.push_back(img6);
  images.push_back(img7);
  images.push_back(img8);
  
  ofSetLineWidth(1);

  /**
   * DatGui setup
   */
  gui = new ofxDatGui(100, 100);
  ofxDatGuiLog::quiet();

  gui->addHeader(":: Drag Me To Reposition ::");

  gui->addLabel("Show static segmentation?");
  showSegStatic = gui->addToggle("Show Static Seg", false);
  showSegStatic->setLabelColor( ofColor(255, 99, 71) );
  gui->addBreak()->setHeight(10.0f);

  gui->addLabel("Show input info?");
  showInputInfo = gui->addToggle("Show input info", false);
  showInputInfo->setLabelColor( ofColor(255, 99, 71) );
  gui->addBreak()->setHeight(10.0f);

  gui->addLabel("Loop through image modes");
  loopArchitecture = gui->addButton("Loop Architecture");
  loopArchitecture->setLabelColor( ofColor(255, 99, 71) );
  loopArchitecture->onButtonEvent(this, &ofApp::onButtonEvent);
  gui->addBreak()->setHeight(10.0f);

  gui->addLabel("Show rebuild process?");
  showRebuildProcess = gui->addToggle("Show Rebuild", true);
  showRebuildProcess->setLabelColor( ofColor(255, 99, 71) );
  showRebuildProcess->onButtonEvent(this, &ofApp::onButtonEvent);
  gui->addBreak()->setHeight(20.0f);

  gui->addLabel("Show new creation?");
  showNewArc = gui->addToggle("Show creation", true);
  showNewArc->setLabelColor( ofColor(255, 99, 71) );
  showNewArc->onButtonEvent(this, &ofApp::onButtonEvent);
  gui->addBreak()->setHeight(20.0f);

  gui->addLabel("Start sound");
  startSound = gui->addButton("Start Sound");
  startSound->setLabelColor( ofColor(255, 99, 71) );
  startSound->onButtonEvent(this, &ofApp::onButtonEvent);
  gui->addBreak()->setHeight(10.0f);

  gui->addLabel("Next building");
  startSound = gui->addButton("Next building");
  startSound->setLabelColor( ofColor(255, 99, 71) );
  startSound->onButtonEvent(this, &ofApp::onButtonEvent);
  gui->addBreak()->setHeight(10.0f);

  gui->addLabel("Start image rebuild");
  rebuildImage = gui->addButton("Rebuild Image");
  rebuildImage->setLabelColor( ofColor(58, 125, 255) );
  rebuildImage->onButtonEvent(this, &ofApp::onButtonEvent);
  gui->addBreak()->setHeight(10.0f);
}

void ofApp::update() {};

void ofApp::draw() {
  ofBackground(255, 255, 255);
  ofSetColor(255, 255, 255);
  if(arguments.size() > 0) {
    if ( toDisplay == images.size() );
    else {
      // draw hough or not?
      if ( showSegStatic->getEnabled() )
        images.at(building).drawImage();
      else
        images.at(building).drawImageOriginal();
    }
    
    if ( !compThread.isThreadRunning() && hasBeenPlayed ) {
      Architecture::ambient.stop();
      compThread.stopThread();
      hasBeenPlayed = false;
      
      // Reset the segments
      for ( auto & seg : images.at(building).segments ) {
        seg.bestSegMatch = nullptr;
        seg.bestMatch = 100;
        seg.hasBeenUsed = false;
        seg.beingUsed = false;
      }
      
      for ( auto & seg : images.at(building+1).segments ) {
        seg.bestSegMatch = nullptr;
        seg.bestMatch = 100;
        seg.hasBeenUsed = false;
        seg.beingUsed = false;
      }
      
      Architecture::playSound = false;
      Architecture::ambient.unload();
      
      if ( building < 6 )
        building += 2;
      else
        building = 0;
      
      hasBeenPlayed = true;
      Architecture::ambient.load(images.at(building+1).imageName + ".wav");
      Architecture::playSound = true;
      Architecture::ambient.setMultiPlay(true);
      
      compThread.setup(images.at(building), images.at(building+1), 0);
      compThread.startThread();
    }
    
// Draw all the best segment replacements in place of the old ones.
  for ( auto const & seg : images.at(building).segments )
    if ( seg.bestSegMatch != nullptr ) {

      // Resize the image to boundaries of original seg, keeping ratio.
      auto rat = fmin( seg.imgFinal.getWidth() / seg.bestSegMatch->imgFinal.getWidth(), seg.imgFinal.getHeight() / seg.bestSegMatch->imgFinal.getHeight() );

      ofSetColor(255, 255, 255);
      seg.bestSegMatch->imgFinal.draw(seg.topLeft, rat * seg.bestSegMatch->imgFinal.getWidth(), rat * seg.bestSegMatch->imgFinal.getHeight());

      // if the new architecture is desired to be shown on the side.
      if ( showNewArc->getEnabled() ) {
        ofPushMatrix();
          ofTranslate(images.at(building).image.getWidth(), 0);
          seg.bestSegMatch->imgFinal.draw(seg.topLeft, rat * seg.bestSegMatch->imgFinal.getWidth(), rat * seg.bestSegMatch->imgFinal.getHeight());
        
          if (seg.beingUsed)
            seg.imgFinal.draw(seg.topLeft, seg.imgSpace.getWidth(), seg.imgSpace.getHeight());
          ofPopMatrix();
      }
    }
  } else {
    exit();
    cout << "No images supplied, or incorrectly... exiting." << endl;
  }

  // Draws the current segment's lines, if enabled.
  if ( showRebuildProcess->getEnabled() )
    for ( auto & arc : images )
      for ( auto & seg : arc.segments )
        if ( seg.beingUsed ) seg.drawSegmentHoughLines();
}

void ofApp::onButtonEvent(ofxDatGuiButtonEvent e) {
  // Handle the loop architecture
  if ( e.target->getLabel() == "LOOP ARCHITECTURE" )
    (toDisplay < images.size()) ? toDisplay ++ : toDisplay = 0;

  if ( e.target->getLabel() == "REBUILD IMAGE" ) {
    hasBeenPlayed = true;
    Architecture::ambient.load(images.at(building+1).imageName + ".wav");
    Architecture::playSound = true;
    Architecture::ambient.setMultiPlay(true);
    
    compThread.setup(images.at(building), images.at(building+1), 0);
    compThread.startThread();
  }

  // Play the sound if globally activated.
  if ( e.target->getLabel() == "START SOUND" )
    if ( Architecture::playSound )
      Architecture::ambient.play();

  if ( e.target->getLabel() == "NEXT BUILDING" ) {
      Architecture::ambient.stop();
      compThread.stopThread();
      hasBeenPlayed = false;
      
      // Reset the segments
      for ( auto & seg : images.at(building).segments ) {
        seg.bestSegMatch = nullptr;
        seg.bestMatch = 100;
        seg.hasBeenUsed = false;
        seg.beingUsed = false;
      }
      
      for ( auto & seg : images.at(building+1).segments ) {
        seg.bestSegMatch = nullptr;
        seg.bestMatch = 100;
        seg.hasBeenUsed = false;
        seg.beingUsed = false;
      }
      
      Architecture::playSound = false;
      Architecture::ambient.unload();
      
      if ( building < 6 )
        building += 2;
      else
        building = 0;
      
      hasBeenPlayed = true;
      Architecture::ambient.load(images.at(building+1).imageName + ".wav");
      Architecture::playSound = true;
      Architecture::ambient.setMultiPlay(true);
      
      compThread.setup(images.at(building), images.at(building+1), 0);
      compThread.startThread();
  }
}

void ofApp::keyPressed(int key){
  if ( key == 'h' ) {
    GUIVis =! GUIVis;
    gui->setVisible(GUIVis);
  }
}


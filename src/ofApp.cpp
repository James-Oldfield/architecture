#include "ofApp.h"
#include "ofxDatGui.h"

using namespace ofxCv;
using namespace cv;

void ofApp::setup() {
  Architecture img1(arguments.at(3), stoi(arguments.at(1)), stoi(arguments.at(6)));
  Architecture img2(arguments.at(4), stoi(arguments.at(2)), 0);

  images.push_back(img1);
  images.push_back(img2);

  ofSetLineWidth(2);

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
  showInputInfo = gui->addToggle("Show input info", true);
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
        images.at(toDisplay).drawImage();
      else
        images.at(toDisplay).drawImageOriginal();
    }

  // Draw all the best segment replacements in place of the old ones.
  for ( auto const & seg : images.at(0).segments )
    if ( seg.bestSegMatch != nullptr ) {

      // Resize the image to boundaries of original seg, keeping ratio.
      auto rat = fmin( seg.imgFinal.getWidth() / seg.bestSegMatch->imgFinal.getWidth(), seg.imgFinal.getHeight() / seg.bestSegMatch->imgFinal.getHeight() );

      ofSetColor(255, 255, 255);
      seg.bestSegMatch->imgFinal.draw(seg.topLeft, rat * seg.bestSegMatch->imgFinal.getWidth(), rat * seg.bestSegMatch->imgFinal.getHeight());
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

  if ( showInputInfo->getEnabled() ) {
    // draw make info
    ofDrawBitmapStringHighlight( arguments.at(3) + " + " + arguments.at(4), 10, ofGetHeight()-35 );
    ofDrawBitmapStringHighlight( arguments.at(1) + " " + arguments.at(2), 10, ofGetHeight()-10 );
  }
}

void ofApp::onButtonEvent(ofxDatGuiButtonEvent e) {
  // Handle the loop architecture
  if ( e.target->getLabel() == "LOOP ARCHITECTURE" )
    (toDisplay < images.size()) ? toDisplay ++ : toDisplay = 0;

  if ( e.target->getLabel() == "REBUILD IMAGE" ) {
    compThread.setup(images.at(0), images.at(1), stoi(arguments.at(5)));
    compThread.startThread();

    if ( Architecture::playSound )
      Architecture::ambient.play();
  }

}

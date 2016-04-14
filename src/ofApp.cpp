#include "ofApp.h"
#include "ofxDatGui.h"

using namespace ofxCv;
using namespace cv;

void ofApp::setup() {
  Architecture img1(arguments.at(3), stoi(arguments.at(1)));
  Architecture img2(arguments.at(4), stoi(arguments.at(2)));

  images.push_back(img1);
  images.push_back(img2);

  /**
   * DatGui setup
   */
  gui = new ofxDatGui(100, 100);
  ofxDatGuiLog::quiet();

  gui->addHeader(":: Drag Me To Reposition ::");

  gui->addLabel("Show static segmentation?");
  showSegStatic = gui->addToggle("Show Static Seg", false);
  gui->addBreak()->setHeight(10.0f);

  gui->addLabel("Loop through image modes");
  loopArchitecture = gui->addButton("Loop Architecture");
  loopArchitecture->onButtonEvent(this, &ofApp::onButtonEvent);
  gui->addBreak()->setHeight(10.0f);

  gui->addLabel("Start image rebuild");
  rebuildImage = gui->addButton("Rebuild Image");
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

  // Draws the current segment's lines.
  for ( auto & arc : images )
    for ( auto & seg : arc.segments )
      if ( seg.beingUsed ) seg.drawSegmentHoughLines();

  // draw make info
  ofDrawBitmapStringHighlight( arguments.at(3) + " + " + arguments.at(4), 10, ofGetHeight()-35 );
  ofDrawBitmapStringHighlight( arguments.at(1) + " " + arguments.at(2), 10, ofGetHeight()-10 );
}

void ofApp::onButtonEvent(ofxDatGuiButtonEvent e) {
  // Handle the loop architecture
  if ( e.target->getLabel() == "LOOP ARCHITECTURE" )
    (toDisplay < images.size()) ? toDisplay ++ : toDisplay = 0;

  if ( e.target->getLabel() == "REBUILD IMAGE" ) {
    compThread.setup(images.at(0), images.at(1), stoi(arguments.at(5)));
    compThread.startThread();
  }
}

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
  gui = new ofxDatGui(ofxDatGuiAnchor::TOP_RIGHT);
  ofxDatGuiLog::quiet();


  ofxDatGuiButton* myButton = new ofxDatGuiButton(string label);
  gui->addBu

  gui->addLabel("Choose the seed colour");
  gui->addColorPicker("Seed colour")->setColor(ofColor(255, 98, 63));
  gui->addBreak()->setHeight(10.0f);

  gui->addLabel("Or load in a palette from a Dribbble project");
  gui->addTextInput("URL", "https://dribbble.com/shots/2585138-Sunset-Badge");
  gui->addBreak()->setHeight(10.0f);
  gui->setWidth(500);

  gui->addLabel("Choose the type of colour scheme");
  gui->addDropdown("Palette type", options);
  gui->addBreak()->setHeight(10.0f);

  gui->addButton("Generate Palette");
  gui->onButtonEvent(this, &ofApp::onButtonEvent);
  gui->addBreak()->setHeight(30.0f);

}

void ofApp::update() {};

void ofApp::draw() {
  ofBackground(255, 255, 255);
  ofSetColor(255, 255, 255);
  if(arguments.size() > 0) {
    if ( toDisplay == images.size() );
    else {
      // draw hough or not?
      if ( drawHough )
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

void ofApp::mousePressed(int x, int y, int button) {}

void ofApp::keyPressed(int key) {
  if (key == OF_KEY_RETURN) {
    // Loop through the pictures
    (toDisplay < images.size()) ? toDisplay ++ : toDisplay = 0;
  } else if (key == OF_KEY_RIGHT) {
    // Kicks off replacement.
    compThread.setup(images.at(0), images.at(1), stoi(arguments.at(5)));
    compThread.startThread();
  } else if (key == OF_KEY_UP) {
    drawHough =! drawHough;
  }
}

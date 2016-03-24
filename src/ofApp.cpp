#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

void ofApp::setup() {
  Architecture img1(arguments.at(3), stoi(arguments.at(1)));
  Architecture img2(arguments.at(4), stoi(arguments.at(2)));

  images.push_back(img1);
  images.push_back(img2);
}

void ofApp::update() {};

void ofApp::draw() {
  ofBackground(255, 255, 255);
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

      if ( drawStretched )
        seg.bestSegMatch->imgFinal.draw(seg.topLeft, seg.imgSeg.getWidth(), seg.imgSeg.getHeight());
      else
        seg.bestSegMatch->imgFinal.draw(seg.topLeft);
    }
  } else {
    exit();
    cout << "No images supplied, or incorrectly... exiting." << endl;
  }

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
    // Kicks of replacement.
    Architecture::findBestMatches(images.at(0), images.at(1), stoi(arguments.at(5)));
  } else if (key == OF_KEY_LEFT) {
    drawStretched =! drawStretched;
  } else if (key == OF_KEY_UP) {
    drawHough =! drawHough;
  }
}

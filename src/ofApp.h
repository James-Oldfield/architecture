#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "architecture.h"
#include "comparisonThread.h"
#include "ofxDatGui.h"

class ofApp : public ofBaseApp {
private:
  ComparisonThread compThread;
public:
  void setup();
  void update();
  void draw();
  void mousePressed(int x, int y, int button);
  void keyPressed(int key);

  vector<string> arguments;
  vector<Architecture> images;

  bool drawHough = true; // draw hough or original version?
  int toDisplay = 0;
  ofVec2f toCompare = { 0, 0 };

  ofxDatGui * gui;
};

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

  vector<string> arguments;
  vector<Architecture> images;

  int toDisplay = 0;
  ofVec2f toCompare = { 0, 0 };

  ofxDatGui * gui;

  ofxDatGuiToggle * showSegStatic;
  ofxDatGuiToggle * showRebuildProcess;

  ofxDatGuiButton * loopArchitecture;
  ofxDatGuiButton * rebuildImage;

  void onButtonEvent(ofxDatGuiButtonEvent e);
};

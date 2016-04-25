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
  void keyPressed(int key);

  vector<string> arguments;
  vector<Architecture> images;

  int toDisplay = 0;
  ofVec2f toCompare = { 0, 0 };

  int building = 0;
  int noPairs = 3;

  ofxDatGui * gui;

  ofxDatGuiToggle * showSegStatic;
  ofxDatGuiToggle * showInputInfo;
  ofxDatGuiToggle * showNewArc;
  ofxDatGuiToggle * showRebuildProcess;

  ofxDatGuiButton * loopArchitecture;
  ofxDatGuiButton * startSound;
  ofxDatGuiButton * rebuildImage;

  bool GUIVis = true;

  void onButtonEvent(ofxDatGuiButtonEvent e);
};

/**
 * segment.h
 *
 * Describes a subsection of an image and provides
 * an interface to export them.
 *
 * @author James Oldfield
 * @version 1.0 25/01/16
 */

#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "lineIntersection.h"

using VecPoints = vector<cv::Point>;

using namespace ofxCv;
using namespace cv;

class Segment {
  private:
    static int segCount; // Global count of objects created

    const int cCount; // local cache of global segment count

    vector<ofPoint> edges; // stores the location of segment edges
    int h, w; // image width and height
    int imageNo; // Image index, used for folder number.

    void removeBackground();
    bool exportSegments();

  public:
    Segment(ofImage _imgSegH, ofImage _imgSeg, ofPoint _topLeft, ofPoint _bottomRight, LineIntersection _intersectionTL, LineIntersection _intersectionBR, int _imageNo);
    void exportSegment();
    void addVertex(int i, int row); // Declare a pixel a vertex of the shape

    static constexpr float matchUpper = 0.01; // Upper bounds of 'worthy' match.
    static constexpr float matchLower = 0.75; // Lower bounds of 'worth' match.

    bool hasBeenUsed = false; // Checks whether this segment has been used before or not.
    bool beingUsed = false; // Communicates with main thread about whether this segment is being compared currently or not.

    const ofPoint topLeft; // location of the segment in the original image
    const ofPoint bottomRight; // Bottom right of the segment in original image.

    const ofRectangle imgSpace; // Bounding box of the segment.

    const LineIntersection intersectionTL; // Stores the top left point intersection and its corresponding lines.
    const LineIntersection intersectionBR; // Stores the bottom right intersection and its corresponding lines.

    double bestMatch = 100; // Stores the best match from matchShapes.
    Segment * bestSegMatch = nullptr; // Stores the best segment match.

    ofImage imgSeg; // the image data in ofImage format
    ofImage imgSegH; // ditto ^ but for hough lines
    ofImage imgFinal; // Contains the image post-background subtraction.
    Mat alphaImage; // Mat version of the image post-bg subtraction.
    Mat mask; // Stores the masked image for background removal.
    
    string name; // Contains the name of the file for easy reference.

    static double compareSegs(Segment & seg1, Segment & seg2);

    void drawSegmentHoughLines(); // Draws the Hough Lines associated with the segment.

    vector<VecPoints> segContours;
    int biggestContour;
};

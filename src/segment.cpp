/**
 * segment.cpp
 *
 * Describes a subsection of an image and provides
 * an interface to export them.
 *
 * @author James Oldfield
 * @version 1.0 25/01/16
 */

#include "segment.h"

using namespace ofxCv;
using namespace cv;

Segment::Segment(ofImage _imgSegH, ofImage _imgSeg, ofPoint _topLeft, int _imageNo): imgSegH(_imgSegH), imgSeg(_imgSeg), topLeft(_topLeft), imageNo(_imageNo), cCount(segCount++) {}

int Segment::segCount = 0;

/**
 * Static function to compare two images.
 * @param seg1 A reference to the first segment.
 * @param seg2 Ditto above the second segment.
 * @return double The shape's contour's similarity.
 */
double Segment::compareSegs(Segment & seg1, Segment & seg2) {

  /**
   * cache member variables for ease of re-use
   */
  auto mask1 = seg1.mask;
  auto sc1 = seg1.segContours;
  auto bc1 = seg1.biggestContour;

  auto mask2 = seg2.mask;
  auto sc2 = seg2.segContours;
  auto bc2 = seg2.biggestContour;

  drawContours(mask1, sc1, bc1, Scalar(255), CV_FILLED);
  drawContours(mask2, sc2, bc2, Scalar(255), CV_FILLED);

  imshow("seg 1", mask1);
  imshow("seg 2", mask2);

  // Compare the largest contours (at bcX index) of each segment.
  double result = matchShapes( sc1.at(bc1), sc2.at(bc2), CV_CONTOURS_MATCH_I1, 0.0 );

  string data = "Similarity of segments " + seg1.name + " + " + seg2.name + " = " + to_string(result) + "\n";

  // Write to file if the comparison is under matchShapesThresh
  if (result != 0 && result < Segment::matchShapesThresh) {
  }

    cout << data << endl;


  return result;
}

void Segment::exportSegment() {

  removeBackground();

  // string houghName = "seg" + to_string(imageNo) + "/segment" + to_string(cCount) + "-hough.png";
  
  // imgSegH.save(houghName, OF_IMAGE_QUALITY_BEST);
}

void Segment::removeBackground() {
  ofImage temp = imgSeg;
  Mat img1 = toCv(temp);
  Mat imgGS;

  cvtColor(img1, imgGS, CV_BGR2GRAY);
  threshold(imgGS, imgGS, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

  findContours( imgGS, segContours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE );
  mask = Mat::zeros( imgGS.rows, imgGS.cols, CV_8UC1 );

  vector<double> areas(segContours.size());

  // Find the most prominent contour.
  for(int i = 0; i < segContours.size(); i++)
    areas[i] = contourArea(Mat(segContours[i]));

  double max;
  cv::Point maxPosition;
  minMaxLoc(Mat(areas),0,&max,0,&maxPosition);

  // Cache the value of the largest contour for comparison later with other shapes
  biggestContour = maxPosition.y;
   
  drawContours(mask, segContours, maxPosition.y, Scalar(255), CV_FILLED);

  Mat rgb[3];
  split(img1,rgb);

  Mat rgba[4]={rgb[0],rgb[1],rgb[2],mask};
  merge(rgba,4,alphaImage);

  normalize(mask.clone(), mask, 0.0, 255.0, CV_MINMAX, CV_8UC1);

  toOf( alphaImage, imgFinal );
  name = "seg" + to_string(imageNo) + "/segment" + to_string(cCount) + ".png";
  imgFinal.save( name );
}

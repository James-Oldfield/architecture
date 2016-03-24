/**
 * architecture.cpp
 *
 * Describes a representation of an input image
 *
 * @author James Oldfield
 * @version 1.0 22/02/16
 */

#include "architecture.h"

using namespace ofxCv;
using namespace cv;

int Architecture::arcCount = 0;

/**
 *  Creates an instance of 'Architecture' and kicks off the segmentation.
 *  
 *  @param string _image - The string to load into the ofImage.
 */
Architecture::Architecture(string _image, int _threshold): cCount(arcCount++), threshold(_threshold) {
  image.load(_image);
  imgCopy = image;

  thresh.allocate(ofGetWidth(), ofGetHeight(), OF_IMAGE_GRAYSCALE);
  convertColor(image, thresh, CV_RGB2GRAY);
  
  // Apply the canny edge detector for better segmentation
  Canny(image, thresh, 50, 200, 3);
  thresh.update();

  // Map the OF image data to a cv-style binary data
  threshBin = toCv(thresh);

  img = toCv(image); // Convert OF image to CV bin representation

  // Apply the Hough Lines transform
  HoughLines(threshBin, lines, 1, CV_PI/180, threshold, 0, 0);

  cout << "------" << endl;
  cout << "Performing Hough lines segmentation for image " << arcCount << "..." << endl;

  for( int i = 0; i < lines.size(); i++ ) {
     float rho = lines[i][0], theta = lines[i][1];
     cv::Point pt1, pt2;
     double a = cos(theta), b = sin(theta);
     double x0 = a*rho, y0 = b*rho;
     pt1.x = cvRound(x0 + 1000*(-b));
     pt1.y = cvRound(y0 + 1000*(a));
     pt2.x = cvRound(x0 - 1000*(-b));
     pt2.y = cvRound(y0 - 1000*(a));
     line( img, pt1, pt2, Scalar(255,0,0), 0.1, CV_AA);

     // Cache the cartesian representations in vector storage
     ofPolyline tmp;

     tmp.addVertex( ofPoint(pt1.x, pt1.y, 0) );
     tmp.addVertex( ofPoint(pt2.x, pt2.y, 0) );
     tmp.close();

     cartCoord.push_back(tmp);
  }

  cout << "Hough lines found. Now checking for intersection points..." << endl;

  // Check for line intersection via brute force
  for(auto l1 : cartCoord) {
    for(auto l2 : cartCoord) {
      doSegsIntersect(l1, l2);
    }
  }

  cout << "No. of intersection points found = " << iPts.size() << endl;

  for(auto const &pt : iPts) {
    for(auto const &pt2 : iPts) {
      if(abs(pt2.x - pt.x) > 75 &&
         abs(pt2.x - pt.x) < image.getWidth() / 1.5 &&
         abs(pt2.y - pt.y) > 75 &&
         abs(pt2.y - pt.y) < image.getWidth() / 1.5
        ) {
          ofImage tmp, tmpH;
          ofRectangle imgSpace = ofRectangle(pt, pt2);

          // create object from the two version of the image
          tmpH.cropFrom(image, imgSpace.getTopLeft().x, imgSpace.getTopLeft().y, imgSpace.width, imgSpace.height);
          tmp.cropFrom(imgCopy, imgSpace.getTopLeft().x, imgSpace.getTopLeft().y, imgSpace.width, imgSpace.height);

          Segment seg(tmpH, tmp, imgSpace.getTopLeft(), cCount );
          segments.push_back(seg);
      }
    }
  }

 //  cout << "Now exporting the segments..." << endl;

   for_each( segments.begin(), segments.end(), [] ( Segment &seg ) {
     seg.exportSegment();
   } );

}

/**
 * @brief Performs comparison of two image's segments to find the best replacement segment.
 *
 * @param arc1 The first architecture to compare.
 * @param arc2 The second architecture to compare.
 */
void Architecture::findBestMatches(Architecture & arc1, Architecture & arc2, int recursive) {
  
  cout << "-----" << endl;
  cout << "Finding best matching segments between the two images with specificity of between " << Segment::matchUpper << " and " << Segment::matchLower << endl;

  for ( auto & arc : arc1.segments )
    for ( auto & arc2 : arc2.segments )
      Segment::compareSegs( arc, arc2 );

  // provides an option to use own architecture to recreate. passed through command line, 'arguments.at(5)'
  if ( recursive )
    for ( auto & arc : arc1.segments )
      for ( auto & arc2 : arc1.segments )
        Segment::compareSegs( arc, arc2 );
}//

/**
 * Checks whether or not there is an intersection been two non-infinite line segments.

 *  @param ofPolyline a - the first line to compare
 *  @param ofPolyline b - the second line to compare
 *
 *  @return bool - whether or not there was any points of intersection.
 */
bool Architecture::doSegsIntersect(ofPolyline a, ofPolyline b) {
  /**
   * Function loosely based on answers from: http://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
   */
  vector<ofPoint> aLine = a.getVertices();
  vector<ofPoint> bLine = b.getVertices();

  float a0x = aLine[0].x;
  float a0y = aLine[0].y;
  float a1x = aLine[1].x;
  float a1y = aLine[1].y;

  float b0x = bLine[0].x;
  float b0y = bLine[0].y;
  float b1x = bLine[1].x;
  float b1y = bLine[1].y;

  float s1x = a1x - a0x;
  float s1y = a1y - a0y;
  float s2x = b1x - b0x;
  float s2y = b1y - b0y;

  float s, t;
  s = (-s1y * (a0x - b0x) + s1x * (a0y - b0y)) / (-s2x * s1y + s1x * s2y);
  t = ( s2x * (a0y - b0y) - s2y * (a0x - b0x)) / (-s2x * s1y + s1x * s2y);

  // If a collision has been detected
  if (s >= 0 && s <= 1 && t >= 0 && t <= 1) {
    float x = a0x + (t * s1x);
    float y = a0y + (t * s1y);

    // ensure the point is contained within the boundaries of the image
    // and also not off the screen
    if (x <= image.getWidth() &&
        y <= image.getHeight() &&
        x > 0 &&
        y > 0) {

      bool unique = true;

      /**
       * Check to see if there's already an intersection point close by.
       * If there are points within a 7px difference
       */
      for( auto pt : iPts ) {
        if( abs(pt.x - x) < 15 && abs(pt.y - y) < 15 ) {
          unique = false;
          break;
        }
      }

      // only push the point if there aren't multiples
      if (unique)
        iPts.push_back( ofPoint(x, y) );
    }
    
    return 1;
  }

  return 0; // No collision

}

void Architecture::drawImageOriginal() {
  imgCopy.draw(0, 0);
}

/**
 *  Mainly an interface to drawing the ofImage
 */
void Architecture::drawImage() {
  image.draw(0, 0);

  // Draw the points of intersection
  // given a reasonable threshold
  if( drawIntersections )
    for(auto const &pt : iPts) {
      ofDrawCircle(pt.x, pt.y, 2);

      for(auto const &pt2 : iPts) {
        if(abs(pt2.x - pt.x) > 75 &&
           abs(pt2.x - pt.x) < image.getWidth() / 1.5 &&
           abs(pt2.y - pt.y) > 75 &&
           abs(pt2.y - pt.y) < image.getWidth() / 1.5
          ) {
            ofImage tmp;
            ofRectangle imgSpace = ofRectangle(pt, pt2);

            ofNoFill();
            ofDrawRectangle(imgSpace);
        }
      }
    }
}

/**
 * LineIntersection.h
 *
 * Holds two hough lines and their intersection point.
 * Used for a corner of a segment.
 *
 * @author James Oldfield
 * @version 1.0 15/04/16
 */

#pragma once

#include "ofMain.h"

class LineIntersection {
  public:
    LineIntersection(const ofPoint & _intersection, const ofPolyline & _a, const ofPolyline & _b );

    ofPoint intersection; // stores the point of intersection.
    ofPolyline a, b; // Stores the two hough lines composing this intersection.
};

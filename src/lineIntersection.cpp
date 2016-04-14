/**
 * LineIntersection.cpp
 *
 * Holds two hough lines and their intersection point.
 * Used for a corner of a segment.
 *
 * @author James Oldfield
 * @version 1.0 15/04/16
 */

#include "LineIntersection.h"

LineIntersection::LineIntersection(const ofPoint & _intersection, const ofPolyline & _a, const ofPolyline & _b ): intersection(_intersection), a(_a), b(_b) {};

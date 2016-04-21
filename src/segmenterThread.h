#include "ofMain.h"
#include "ofxCv.h"
#include "segment.h"
#include "architecture.h"

/**
 * @class SegmenterThread
 * @brief Offloads the hough line calculation from the main thread.
 */
class SegmenterThread: public ofThread {
  private:
    Architecture * arc;
  public:
    void setup(Architecture & _arc) {
      arc = &_arc;
    }

    void threadedFunction() {
      arc->segment();
    }
};
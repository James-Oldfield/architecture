#include "ofMain.h"
#include "ofxCv.h"
#include "segment.h"
#include "architecture.h"

/**
 * @class ComparisonThread
 * @brief Offloads the computer vision comparison tasks into a separate thread to visualise real time in main GL one.
 */
class ComparisonThread: public ofThread {
  private:
    int recursive;
    Architecture * arc1;
    Architecture * arc2;
  public:
    void setup(Architecture & _a1, Architecture & _a2, int _r) {
      arc1 = &_a1;
      arc2 = &_a2;
      recursive = _r;
    }

    void threadedFunction() {
      Architecture::findBestMatches(*arc1, *arc2, recursive);
    }
};
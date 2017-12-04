#include "kinecthandler.h"
#include <cstdlib>

KinectHandler::KinectHandler(SandboxWindow* theBox) : QEventLoop(0) { // MM: 0 = parent
  box = theBox;
  startTimer(4000);
  boxMaxDepth = theBox->maxDepth;
  startlate = 0;  // MM: starts changing text on second timer event instead of first
  // so that there's time to move the window to sandbox and see what it looks like
}

void KinectHandler::timerEvent(QTimerEvent *event) {
  size_t depthsToDisplay[MAXROWS][MAXCOLS]; // initialize new array
  calcDepthsToDisplay(depthsToDisplay);     // calculate depth levels to display
  box->updateTextDisplay(depthsToDisplay);  // send Sandbox Window depth levels
}

void KinectHandler::calcDepthsToDisplay(size_t depthsToDisplay[MAXROWS][MAXCOLS]) {
  
  // don't change depth first time
  if (startlate == 0) {
    for (size_t row = 0; row < MAXROWS; row++) {
      for (size_t col = 0; col < MAXCOLS; col++)
	depthsToDisplay[row][col] = 0;
    }	
    startlate++;  // change depth next time
    return;
  }
  
  // change depth randomly
  size_t num = 0;
  for (size_t row = 0; row < MAXROWS; row++) {
    for (size_t col = 0; col < MAXCOLS; col++) {
      if (col % 3)
	num = rand() % boxMaxDepth;    // get random depth for every 3rd col
      depthsToDisplay[row][col] = num; // so demo display is somewhat readable
    }
  }
}

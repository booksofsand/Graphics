#include "kinecthandler.h"
#include <cstdlib>

KinectHandler::KinectHandler(SandboxWindow* theBox) : QEventLoop(0) { // MM: 0 = parent
  box = theBox;
  startTimer(4000);
  boxMaxDepth = theBox->maxDepth;
}

void KinectHandler::timerEvent(QTimerEvent *event) {
  size_t depthsToDisplay[MAXROWS][MAXCOLS]; // initialize new array
  calcDepthsToDisplay(depthsToDisplay);     // calculate depth levels to display
  box->updateTextDisplay(depthsToDisplay);  // send Sandbox Window depth levels
}

void KinectHandler::calcDepthsToDisplay(size_t depthsToDisplay[MAXROWS][MAXCOLS]) {
  size_t num = 0;
  for (size_t row = 0; row < MAXROWS; row++) {
    for (size_t col = 0; col < MAXCOLS; col++) {
      if (col % 2)
	num = rand() % boxMaxDepth;    // get random depth for every 2nd col
      depthsToDisplay[row][col] = num;
    }
  }
}

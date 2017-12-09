#ifndef SANDBOXWINDOW_H
#define SANDBOXWINDOW_H

#include <string>
#include <vector>
#include <QGridLayout>
#include "config.h"
#include "helpers.h"
#include "kinecthandler.h"

class KinectHandler; // forward declaration
class SandboxWindow
{
public:
    SandboxWindow(std::vector<std::string> filenames);
    void switchToDepth(size_t depth);               // for testing only
    void updateTextDisplay(size_t depthsToDisplay[MAXROWS][MAXCOLS]);
    
private:
    QGridLayout* grid;                              // grid object holding displayed labels
    QString sourceText[MAXDEPTH][MAXROWS][MAXCOLS]; // array of chars at different depths,rows,cols
    size_t depthsDisplayed[MAXROWS][MAXCOLS];       // array of depth numbers for each row,col
    KinectHandler* kinectHandler;                   // object that interfaces with the Kinect

    void readLines(std::string filename, size_t depth = 0);
};

#endif // SANDBOXWINDOW_H

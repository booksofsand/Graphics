#ifndef SANDBOXWINDOW_H
#define SANDBOXWINDOW_H

#include "kinecthandler.h"
#include <string>
#include <vector>
#include <QGridLayout>

#define MAXDEPTH 10
#define MAXROWS 30
#define MAXCOLS 60

class SandboxWindow
{
public:
    SandboxWindow(std::vector<std::string> filenames);
    void switchToDepth(size_t depth);
    void updateDepthDisplay(size_t** depthsToDisplay);
    
private:
    QGridLayout* grid;                    // grid object holding displayed labels
    //QString lines[MAXDEPTH][MAXROWS];     // array of lines at different depths
    QString lines[MAXDEPTH][MAXROWS][MAXCOLS]; // array of chars at different depths,rows,cols
    size_t depthsDisplayed[MAXROWS][MAXCOLS];  // array of depth numbers for each row,col
    KinectHandler* kinectHandler;              // object that interfaces with the Kinect

    void readLines(std::string filename, size_t depth = 0);
    void setText(QString text, size_t row, size_t col);
};

#endif // SANDBOXWINDOW_H

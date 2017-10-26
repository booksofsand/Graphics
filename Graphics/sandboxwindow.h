#ifndef SANDBOXWINDOW_H
#define SANDBOXWINDOW_H

#include "kinecthandler.h"
#include <string>
#include <vector>
#include <QGridLayout>

#define MAXDEPTH 10
#define MAXROWS 28
#define MAXCOLS 60

class SandboxWindow
{
public:
    SandboxWindow(std::vector<std::string> filenames);
    void addWord(QString word, int row, int col, int depth);
    void removeWord(int row, int col, int depth);
    void switchToDepth(int depth);

private:
    QGridLayout* grid;                    // grid that actually displays
    QString lines[MAXDEPTH][MAXROWS];     // array of lines at different depths
    //QString lines[MAXDEPTH][MAXROWS][MAXCOLS];     // array of chars at different depths,rows,cols
    KinectHandler* kinectHandler;

    void readLines(std::string filename, size_t depth = 0);
};

#endif // SANDBOXWINDOW_H

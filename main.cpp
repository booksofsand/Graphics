#include <QApplication>
#include <iostream>
#include <vector>
#include <QtWidgets>
#include "sandboxwindow.h"

using namespace std;

int main(int argc, char *argv[])
{
    // make an application
    QApplication a(argc, argv);

    // initialize window with optional filename(s)
    std::vector<std::string> filenames;
    for (int i = 1; i < argc; i++) {
        char* arg = argv[i];
        filenames.push_back(arg);
    }
    
    SandboxWindow* box = new SandboxWindow(filenames);
 
    // return the executable
    return a.exec();
}

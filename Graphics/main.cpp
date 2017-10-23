#include <QApplication>
#include "sandboxwindow.h"

int main(int argc, char *argv[])
{
    // make an application
    QApplication a(argc, argv);

    //SandboxWindow* box = new SandboxWindow();
    new SandboxWindow();
    //box->removeWord(10,10,0);

    // MM: main() should probably do the interacting with Kinect
    //     but how to call these updates? Is there some sort of
    //     mainloop we can define for this QApplication?
    //     Is _main()_ the mainloop?

    // return the executable
    return a.exec();
}

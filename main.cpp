#include "sandboxwindow.h"
#include <QApplication>
#include <iostream>
#include <vector>

int main(int argc, char *argv[])
{
    // make an application
    QApplication a(argc, argv);

    // initialize window with optional filename
    std::vector<std::string> filenames;
    for (int i = 1; i < argc; i++) {
        char* arg = argv[i];
        //std::cout << "arg: " << arg << std::endl; // MM: testing

        // if arg isn't in dict of flag options, assume it's a filename
        filenames.push_back(arg);
    }


    SandboxWindow* box = new SandboxWindow(filenames);

    /*
    // test switchToDepth
    if (argc > 3) {
        std::string op = argv[3];
        if (op == "switchDepth")
            box->switchToDepth(1);
        else
            std::cout << "argv[3]: " << argv[3] << std::endl;
    }
    */

    // MM: main() should probably do the interacting with Kinect
    //     but how to call these updates? Is there some sort of
    //     mainloop we can define for this QApplication?
    //     Is _main()_ the mainloop?

    // return the executable
    return a.exec();
}

#include <QApplication>
#include <iostream>
#include "sandboxwindow.h"

int main(int argc, char *argv[])
{
    // make an application
    QApplication a(argc, argv);

    std::cout << "Making a SandboxWindow." << std::endl;
    //SandboxWindow* box = new SandboxWindow();
    SandboxWindow* box = new SandboxWindow(argv[1]);

    // test switchToDepth
    if (argc > 2) {
        std::string op = argv[2];
        if (op == "switchDepth")
            box->switchToDepth(1);
        else
            std::cout << "argv[2]: " << argv[2] << std::endl;
    }

    // MM: main() should probably do the interacting with Kinect
    //     but how to call these updates? Is there some sort of
    //     mainloop we can define for this QApplication?
    //     Is _main()_ the mainloop?

    // return the executable
    return a.exec();
}

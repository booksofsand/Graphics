#ifndef SANDBOXWINDOW_H
#define SANDBOXWINDOW_H

#include <string>
#include <QGridLayout>

#define MAXDEPTH 10
#define MAXROWS 28

class SandboxWindow
{
public:
    SandboxWindow(std::string filename);
    void addWord(QString word, int row, int col, int depth);
    void removeWord(int row, int col, int depth);
    void switchToDepth(int depth);

private:
    QGridLayout* grid;                    // grid that actually displays
    QString lines[MAXDEPTH][MAXROWS];     // array of lines at different depths
    void readLines(std::string filename);
};

#endif // SANDBOXWINDOW_H

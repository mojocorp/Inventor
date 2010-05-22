#include <QApplication>
#include "MainWindow.h"
#include <Inventor/Qt/SoQt.h>

int main (int argc, char* argv[])
{
    QApplication app(argc, argv);
    QApplication::setQuitOnLastWindowClosed (true);
    SoQt::init ("SceneViewer");
    MainWindow mainWin;
    mainWin.show();
    return app.exec();;
}

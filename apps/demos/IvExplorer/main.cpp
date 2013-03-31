#include <QApplication>
#include "MainWindow.h"

int main(int argc, char **argv)
{
    QApplication app( argc, argv );

    QCoreApplication::setOrganizationName("Mojocorp");
    QCoreApplication::setOrganizationDomain("https://github.com/mojocorp");
    QCoreApplication::setApplicationName("IvExplorer");

    MainWindow mainWin;
    mainWin.show();

    return app.exec();
}

#include <QtGui>

#include "MainWindow.h"
#include "QRecentFilesMenu.h"

#include <Inventor/SoDB.h>
#include <Inventor/SoInput.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>

MainWindow::MainWindow()
{
    QCoreApplication::setOrganizationName("sgi");
    QCoreApplication::setOrganizationDomain("sgi.com");
    QCoreApplication::setApplicationName("SceneViewer");

    viewer = new SoQtExaminerViewer(this, NULL, TRUE, SoQtFullViewer::BUILD_ALL, SoQtViewer::BROWSER);
    
    setCentralWidget(viewer->getWidget());

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();

    readSettings();

    setCurrentFile("");
    setUnifiedTitleAndToolBarOnMac(true);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    writeSettings();
    event->accept();
}

void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty())
        loadFile(fileName);
}

bool MainWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this);
    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Application"),
                       tr("The <b>Application</b> example demonstrates how to "
                          "write modern GUI applications using Qt, with a menu bar, "
                          "toolbars, and a status bar."));
}

void MainWindow::createActions()
{
    openAct = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAsAct = new QAction(QIcon(":/images/save.png"), tr("Save &As..."), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save the document under a new name"));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    menuBar()->addSeparator();

    recentFilesMenu = new QRecentFilesMenu(tr("Recent Files"), fileMenu);

    connect(recentFilesMenu, SIGNAL(recentFileTriggered(const QString &)), this, SLOT(loadFile(const QString &)));

    fileMenu->addMenu(recentFilesMenu);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
}

void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(openAct);
    fileToolBar->addAction(saveAsAct);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::readSettings()
{
    QSettings settings;
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(400, 400)).toSize();
    resize(size);
    move(pos);
    recentFilesMenu->restoreState(settings.value("recentFiles").toByteArray());
}

void MainWindow::writeSettings()
{
    QSettings settings;
    settings.setValue("pos", pos());
    settings.setValue("size", size());
    settings.setValue("recentFiles", recentFilesMenu->saveState());
}

void MainWindow::loadFile(const QString &fileName)
{
    SoInput in;
    if (!in.openFile(qPrintable(fileName))) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read file %1.")
                             .arg(fileName));
        return;
    }

    SoSeparator * root = SoDB::readAll(&in);
    if (!root) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read file %1.")
                             .arg(fileName));
        return;
    }

    viewer->setSceneGraph (root);
    viewer->viewAll();

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);

    recentFilesMenu->addRecentFile(fileName);
}

bool MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    SoOutput out;
    if (!out.openFile(qPrintable(fileName))) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot write file %1.")
                             .arg(fileName));
        return false;
    }
    QApplication::setOverrideCursor(Qt::WaitCursor);
    SoWriteAction wa(&out);
    wa.apply(viewer->getSceneGraph());
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;

    setWindowModified(false);

    QString shownName = curFile;
    if (curFile.isEmpty())
        shownName = "untitled.txt";
    setWindowFilePath(shownName);
}


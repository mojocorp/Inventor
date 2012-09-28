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
    delete viewer;

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
    QFileDialog dialog(this, tr("Save File"));
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilter(tr("Inventor Ascii (*.iv);;Inventor Binary (*.iv)"));

    if (dialog.exec()) {
         QString fileName = dialog.selectedFiles().first();
         bool binary = (dialog.selectedNameFilter() == tr("Inventor Binary (*.iv)"));

         return saveFile(fileName, binary);
    }
    return false;
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

    screenDoorAct = new QAction(tr("Screen Door"), this);
    screenDoorAct->setCheckable(true);

    addAct = new QAction(tr("Add"), this);
    addAct->setCheckable(true);

    delayedAddAct = new QAction(tr("Delayed Add"), this);
    delayedAddAct->setCheckable(true);

    sortedObjectAddAct = new QAction(tr("Sorted Object Add"), this);
    sortedObjectAddAct->setCheckable(true);

    blendAct = new QAction(tr("Blend"), this);
    blendAct->setCheckable(true);

    delayedBlendAct = new QAction(tr("Delayed Blend"), this);
    delayedBlendAct->setCheckable(true);

    sortedObjectBlendAct = new QAction(tr("Sorted Object Blend"), this);
    sortedObjectBlendAct->setCheckable(true);

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

    viewMenu = menuBar()->addMenu(tr("&View"));

    transpMenu = viewMenu->addMenu(tr("&Transparency"));
    transpMenu->addAction(screenDoorAct);
    transpMenu->addAction(addAct);
    transpMenu->addAction(delayedAddAct);
    transpMenu->addAction(sortedObjectAddAct);
    transpMenu->addAction(blendAct);
    transpMenu->addAction(delayedBlendAct);
    transpMenu->addAction(sortedObjectBlendAct);

    connect(transpMenu, SIGNAL(triggered(QAction*)), SLOT(transpMenuTriggered(QAction*)));
    connect(transpMenu, SIGNAL(aboutToShow()), SLOT(updateTranspActions()));

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
    if (!in.openFile(SbString::fromUtf8(fileName.toUtf8().data()))) {
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

bool MainWindow::saveFile(const QString &fileName, bool binary)
{
    SoOutput out;
    out.setBinary(binary);
    if (!out.openFile(SbString::fromUtf8(fileName.toUtf8().data()))) {
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

    recentFilesMenu->addRecentFile(fileName);

    return true;
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;

    setWindowModified(false);

    QString shownName = curFile;
    if (curFile.isEmpty())
        shownName = "untitled";
    setWindowFilePath(shownName);
}

void MainWindow::transpMenuTriggered(QAction* action)
{
    if (action == screenDoorAct)
        viewer->setTransparencyType(SoGLRenderAction::SCREEN_DOOR);
    else if (action == addAct)
        viewer->setTransparencyType(SoGLRenderAction::ADD);
    else if (action == delayedAddAct)
        viewer->setTransparencyType(SoGLRenderAction::DELAYED_ADD);
    else if (action == sortedObjectAddAct)
        viewer->setTransparencyType(SoGLRenderAction::SORTED_OBJECT_ADD);
    else if (action == blendAct)
        viewer->setTransparencyType(SoGLRenderAction::BLEND);
    else if (action == delayedBlendAct)
        viewer->setTransparencyType(SoGLRenderAction::DELAYED_BLEND);
    else if (action == sortedObjectBlendAct)
        viewer->setTransparencyType(SoGLRenderAction::SORTED_OBJECT_BLEND);
}

void MainWindow::updateTranspActions()
{
    screenDoorAct->setChecked(viewer->getTransparencyType() == SoGLRenderAction::SCREEN_DOOR);
    addAct->setChecked(viewer->getTransparencyType() == SoGLRenderAction::ADD);
    delayedAddAct->setChecked(viewer->getTransparencyType() == SoGLRenderAction::DELAYED_ADD);
    sortedObjectAddAct->setChecked(viewer->getTransparencyType() == SoGLRenderAction::SORTED_OBJECT_ADD);
    blendAct->setChecked(viewer->getTransparencyType() == SoGLRenderAction::BLEND);
    delayedBlendAct->setChecked(viewer->getTransparencyType() == SoGLRenderAction::DELAYED_BLEND);
    sortedObjectBlendAct->setChecked(viewer->getTransparencyType() == SoGLRenderAction::SORTED_OBJECT_BLEND);
}

#include "MainWindow.h"

#include <Inventor/SbColor.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/actions/SoWriteAction.h> 
#include <Inventor/nodes/SoSelection.h> 
#include <Inventor/Qt/SoQt.h>

#include <QtGui>

#include "QxTreeModel.h"
#include "QxTableModel.h"
#include "QxTreeModelItem.h"
#include "QxFieldItemDelegate.h"
#include "QRecentFilesMenu.h"
#include "QxViewerLayout.h"

void MainWindow::made_selection( MainWindow * mw, SoPath * path )
{
    mw->m_p_root->touch(); // to redraw
}

MainWindow::MainWindow(QWidget * parent, Qt::WindowFlags flags)
        : QMainWindow(parent, flags)
{
    setWindowTitle( QCoreApplication::applicationName() );

    SoQt::init("IvExplorer");

    m_p_treeModel = new QxTreeModel(this);
    m_p_listModel = new QxTableModel(this);

    m_p_root = new SoSelection;
    m_p_root->addSelectionCallback( (SoSelectionPathCB*)MainWindow::made_selection, this );
    m_p_root->addDeselectionCallback( (SoSelectionPathCB*)MainWindow::made_selection, this );
    m_p_root->ref();

    m_p_layout = new QxViewerLayout(this);
    m_p_layout->setSceneGraph(m_p_root);

    setCentralWidget(m_p_layout);

    createActions();
    createMenus();
    createDockWindows();
    createToolBars();
    createStatusBar();

    readSettings();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    writeSettings();
    event->accept();
}

MainWindow::~MainWindow()
{
    delete m_p_layout;

    SoQt::finish();
}

void MainWindow::createActions()
{
    openAct = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
    openAct->setShortcut(tr("Ctrl+O"));
    openAct->setStatusTip(tr("Open an existing scene graph"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAct = new QAction(QIcon(":/images/save.png"), tr("&Save"), this);
    saveAct->setShortcut(tr("Ctrl+S"));
    saveAct->setStatusTip(tr("Save the scene graph to disk"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAct = new QAction(tr("Save &As..."), this);
    saveAsAct->setStatusTip(tr("Save the scene graph under a new name"));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

    removeAct = new QAction(tr("Remove"), this);
    removeAct->setStatusTip(tr("Remove"));
    connect(removeAct, SIGNAL(triggered()), this, SLOT(removeRow()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    oneViewer = new QAction(QIcon(":/images/1viewer.png"), tr("&1 Viewer Layout"), this);
    connect(oneViewer, SIGNAL(triggered()), this, SLOT(changeLayout()));
    twoViewer = new QAction(QIcon(":/images/2viewers.png"), tr("&2 Viewer Layout"), this);
    connect(twoViewer, SIGNAL(triggered()), this, SLOT(changeLayout()));
    fourViewer = new QAction(QIcon(":/images/4viewers.png"), tr("&4 Viewer Layout"), this);
    connect(fourViewer, SIGNAL(triggered()), this, SLOT(changeLayout()));
    fullscreen = new QAction(QIcon(":/images/fullscreen.png"), tr("&View Fullscreen"), this);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);

    recentFilesMenu = new QRecentFilesMenu(tr("Recent Files"), fileMenu);

    connect(recentFilesMenu, SIGNAL(recentFileTriggered(const QString &)), this, SLOT(loadFile(const QString &)));

    fileMenu->addMenu(recentFilesMenu);

    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(removeAct);

    viewMenu = menuBar()->addMenu(tr("&View"));

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
}

void MainWindow::createDockWindows()
{
    nodeInspectorDockWidget = new QDockWidget(tr("Node Inspector"), this);
    nodeInspectorDockWidget->setObjectName("nodeInspector");
    nodeInspectorDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    
    inspectorView = new QTreeView();
    inspectorView->setObjectName("inspectorView");
    inspectorView->setDragDropMode(QAbstractItemView::InternalMove);
    inspectorView->setAlternatingRowColors(true);
    inspectorView->setModel(m_p_treeModel);
    
    nodeInspectorDockWidget->setWidget(inspectorView);

    connect(inspectorView->selectionModel(), SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), SLOT(currentSelectionChanged(const QModelIndex&, const QModelIndex &)));

    propertyEditorDockWidget = new QDockWidget(tr("Property Editor"), this);
    propertyEditorDockWidget->setObjectName("propertyEditor");
    propertyEditorDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    propertyView = new QTableView();
    propertyView->setItemDelegate(new QxFieldItemDelegate(this));
    propertyView->setObjectName("propertyView");
    propertyView->setSelectionBehavior(QAbstractItemView::SelectRows);
    propertyView->setAlternatingRowColors(true);
    propertyView->setModel(m_p_listModel);
    propertyView->horizontalHeader()->setStretchLastSection(true);
    propertyView->verticalHeader()->hide();
    propertyEditorDockWidget->setWidget(propertyView);

    addDockWidget(Qt::LeftDockWidgetArea, nodeInspectorDockWidget);
    addDockWidget(Qt::LeftDockWidgetArea, propertyEditorDockWidget);

    viewMenu->addAction(nodeInspectorDockWidget->toggleViewAction());
    viewMenu->addAction(propertyEditorDockWidget->toggleViewAction());
}

void MainWindow::createToolBars()
{
    setUnifiedTitleAndToolBarOnMac(true);

    toolBar = addToolBar(tr("File"));

    toolBar->addAction(m_p_layout->getPickAction());
    toolBar->addAction(m_p_layout->getViewAction());
    toolBar->addAction(m_p_layout->getHomeAction());
    toolBar->addAction(m_p_layout->getSetHomeAction());
    toolBar->addAction(m_p_layout->getViewAllAction());
    toolBar->addAction(m_p_layout->getSeekAction());
    toolBar->addAction(m_p_layout->getPerspAction());

    QWidget *spacerWidget = new QWidget(this);
    spacerWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    toolBar->addWidget(spacerWidget);

    toolBar->addAction(oneViewer);
    toolBar->addAction(twoViewer);
    toolBar->addAction(fourViewer);
    toolBar->addAction(fullscreen);
}

void MainWindow::createStatusBar()
{
    statusBar();
}

void MainWindow::readSettings()
{
    QSettings settings;
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(800, 600)).toSize();
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

void MainWindow::open()
{
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open Inventor Files"), m_rescentDir, tr("Inventor Files (*.iv *.wrl *.vrml)"));
        if (!fileName.isEmpty()) {
            loadFile(fileName);
        }
    }
}

void MainWindow::openRecentFile()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
        loadFile(action->data().toString());
}

bool MainWindow::save()
{
    if (m_currFile.isEmpty()) {
        return saveAs();
    }
    return saveFile(m_currFile);
}

bool MainWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Inventor Files"), m_rescentDir);
    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);
}

bool MainWindow::maybeSave()
{
    
    return true;
}

void MainWindow::removeRow()
{
    QModelIndex index = inspectorView->selectionModel()->currentIndex();
    QAbstractItemModel *model = inspectorView->model();
    model->removeRow(index.row(), index.parent());
}

bool MainWindow::loadFile(const QString &fileName)
{
    SoInput in;
    if (!in.openFile(fileName.toAscii().data()))
        return false;

    QApplication::setOverrideCursor(Qt::WaitCursor);
    SoSeparator* root = SoDB::readAll(&in);

    if (root) {
        //m_p_treeModel->clear();
        
        m_p_root->deselectAll();
        m_p_root->removeAllChildren();
        m_p_root->addChild(root);
        
        m_p_layout->viewAll();

        m_p_treeModel->setupModelData(root);

        //ML m_p_treeModel->setupModelData(m_p_root);

        setCurrentFile(fileName);
    }
    QApplication::restoreOverrideCursor();

    return true;
}

bool MainWindow::saveFile(const QString &fileName)
{
    SoOutput out;
    if (out.openFile(fileName.toAscii().data())) {
        QApplication::setOverrideCursor(Qt::WaitCursor);
        SoWriteAction wa(&out);
        wa.apply(m_p_root->getChild(0));
        QApplication::restoreOverrideCursor();

        setCurrentFile(fileName);
    }
    return false;
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    m_currFile = fileName;
    m_rescentDir = QDir(fileName).absolutePath();

    recentFilesMenu->addRecentFile(fileName);
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About IvExplorer"),
                       tr("The <b>IvExplorer</b> example demonstrates how to "
                          "write modern GUI applications using Qt, with a menu bar, "
                          "toolbars, and a status bar."));
}

void MainWindow::currentSelectionChanged(const QModelIndex& current, const QModelIndex &)
{
    m_p_listModel->setupModelData(m_p_treeModel->getNode(current));
}

void MainWindow::changeLayout()
{
    QAction *action = (QAction*)sender();
    if (action == oneViewer) {
        m_p_layout->setLayout(QxViewerLayout::ONE);
    } else if (action == twoViewer) {
        m_p_layout->setLayout(QxViewerLayout::TWO);
    } else if (action == fourViewer) {
        m_p_layout->setLayout(QxViewerLayout::FOUR);
    }
}

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>

class QAction;
class QMenu;
class QxTreeModel;
class QxTableModel;
class SoQtExaminerViewer;
class SoSeparator;
class SoSelection;
class SoPath;
class QRecentFilesMenu;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget * parent = 0, Qt::WindowFlags flags = 0);
    virtual ~MainWindow();
protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void open();
    bool save();
    bool saveAs();
    void about();
    void removeRow();

    bool loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);

    void openRecentFile();
    void currentSelectionChanged(const QModelIndex&, const QModelIndex &);
private:
    void createActions();
    void createMenus();
    void createDockWindows();
    void createToolBars();
    void createStatusBar();

    void readSettings();
    void writeSettings();

    bool maybeSave();
    void setCurrentFile(const QString &fileName);
    void updateRecentFileActions();
    
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *separatorAct;
    QAction *exitAct;
    QAction *removeAct;
    QAction *aboutAct;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;
    
    QToolBar *fileToolBar;

    QRecentFilesMenu *recentFilesMenu;

    QDockWidget* nodeInspectorDockWidget;
    QDockWidget* propertyEditorDockWidget;

    QTreeView*  inspectorView;
    QTableView* propertyView;

    QString m_rescentDir;
    QString m_currFile;

    QxTreeModel * m_p_treeModel;
    QxTableModel * m_p_listModel;
    SoQtExaminerViewer* m_p_viewer;
    SoSelection* m_p_root;
    
    static void made_selection( MainWindow * mw, SoPath * path );
};

#endif

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QAction;
class QMenu;
class SoQtExaminerViewer;
class QRecentFilesMenu;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void open();
    bool saveAs();
    void about();
    void loadFile(const QString &fileName);
    void transpMenuTriggered(QAction*);
    void updateTranspActions();

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    bool saveFile(const QString &fileName, bool binary);
    void setCurrentFile(const QString &fileName);

    SoQtExaminerViewer *viewer;
    QString curFile;

    QMenu *fileMenu;
    QRecentFilesMenu *recentFilesMenu;
    QMenu *viewMenu;
    QMenu *transpMenu;
    QMenu *helpMenu;
    QToolBar *fileToolBar;

    QAction *openAct;
    QAction *saveAsAct;
    QAction *exitAct;

    QAction *screenDoorAct;
    QAction *addAct;
    QAction *delayedAddAct;
    QAction *sortedObjectAddAct;
    QAction *blendAct;
    QAction *delayedBlendAct;
    QAction *sortedObjectBlendAct;

    QAction *aboutAct;
    QAction *aboutQtAct;
};

#endif

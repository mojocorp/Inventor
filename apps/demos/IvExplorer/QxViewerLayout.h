#ifndef QXVIEWERLAYOUT_H
#define QXVIEWERLAYOUT_H

#include <QWidget>
#include <QVector>

class QAction;
class SoQtExaminerViewer;
class SoNode;

class QxViewerLayout : public QWidget
{
    Q_OBJECT
public:
    QxViewerLayout(QWidget *parent = 0);
    ~QxViewerLayout();

    enum Layout {
        ONE,
        TWO,
        FOUR
    };

    void setLayout(Layout l);
    void setSceneGraph(SoNode * node);
    void viewAll();

    QAction *getPickAction() const { return pickAct; }
    QAction *getViewAction() const { return viewAct; }
    QAction *getHomeAction() const { return homeAct; }
    QAction *getSetHomeAction() const { return setHomeAct; }
    QAction *getViewAllAction() const { return viewAllAct; }
    QAction *getSeekAction() const { return seekAct; }
    QAction *getPerspAction() const { return perspAct; }
private:
    QAction *pickAct;
    QAction *viewAct;
    QAction *homeAct;
    QAction *setHomeAct;
    QAction *viewAllAct;
    QAction *seekAct;
    QAction *perspAct;

    Layout m_layout;
    QVector<SoQtExaminerViewer*> m_p_viewers;
    SoNode* m_p_root;
};

#endif

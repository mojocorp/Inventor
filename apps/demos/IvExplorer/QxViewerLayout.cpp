#include <QGridLayout>
#include <QIcon>
#include <QAction>

#include "QxViewerLayout.h"

#include <Inventor/actions/SoLineHighlightRenderAction.h>
#include <Inventor/Qt/SoQt.h>
#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>

QxViewerLayout::QxViewerLayout(QWidget *parent)
    : QWidget(parent),
      m_p_root(NULL)
{
    pickAct = new QAction(QIcon(":/images/pickMode.png"), tr("&Pick"), this);
    viewAct = new QAction(QIcon(":/images/viewMode.png"), tr("&View"), this);
    homeAct = new QAction(QIcon(":/images/home.png"), tr("&Home"), this);
    setHomeAct = new QAction(QIcon(":/images/sethome.png"), tr("&Set Home"), this);
    viewAllAct = new QAction(QIcon(":/images/viewAll.png"), tr("&View All"), this);
    seekAct = new QAction(QIcon(":/images/seekMode.png"), tr("&Seek"), this);
    perspAct = new QAction(QIcon(":/images/persp.png"), tr("&Perspective"), this);

    QGridLayout *vbl = new QGridLayout(this);
    vbl->setMargin(0);

    setLayout(ONE);
}

QxViewerLayout::~QxViewerLayout()
{
    qDeleteAll(m_p_viewers);
}

void QxViewerLayout::setLayout(Layout l)
{
    m_layout = l;
    QGridLayout *grid = (QGridLayout*)layout();

    int numViewers = 0;
    switch(m_layout)
    {
    case ONE:
        numViewers = 1;
        break;
    case TWO:
        numViewers = 2;
        break;
    case FOUR:
        numViewers = 4;
        break;
    }

    for (int i=numViewers; i<m_p_viewers.size(); i++) {
        delete m_p_viewers[i];
    }
    m_p_viewers.resize(numViewers);
    for (int i=0; i<m_p_viewers.size(); i++) {
        if (m_p_viewers[i]==0) {
            m_p_viewers[i] = new SoQtExaminerViewer();
            m_p_viewers[i]->setGLRenderAction(new SoLineHighlightRenderAction());
            m_p_viewers[i]->setSceneGraph(m_p_root);
        }
    }

    switch(m_layout)
    {
    case ONE:
        grid->addWidget(m_p_viewers[0]->getWidget(), 0, 0);
        break;
    case TWO:
        grid->addWidget(m_p_viewers[0]->getWidget(), 0, 0);
        grid->addWidget(m_p_viewers[1]->getWidget(), 0, 1);
        break;
    case FOUR:
        grid->addWidget(m_p_viewers[0]->getWidget(), 0, 0);
        grid->addWidget(m_p_viewers[1]->getWidget(), 0, 1);
        grid->addWidget(m_p_viewers[2]->getWidget(), 1, 0);
        grid->addWidget(m_p_viewers[3]->getWidget(), 1, 1);
        break;
    }
}

void QxViewerLayout::setSceneGraph(SoNode * node)
{
    m_p_root = node;

    for (int i=0; i<m_p_viewers.size(); i++) {
        m_p_viewers[i]->setSceneGraph(m_p_root);
    }
}

void QxViewerLayout::viewAll()
{
    for (int i=0; i<m_p_viewers.size(); i++) {
        m_p_viewers[i]->viewAll();
    }
}

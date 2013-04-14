#ifndef QXTREEMODELITEM_H
#define QXTREEMODELITEM_H

#include <QList>
#include <QVariant>
#include <QVector>

class SoNode;

class QxTreeModelItem
{
public:
    QxTreeModelItem();
    QxTreeModelItem(QxTreeModelItem * parent, SoNode* node);
    virtual ~QxTreeModelItem();

    void removeAllChildren();

    QxTreeModelItem *child(int number);
    int childCount() const;
    Qt::ItemFlags flags(int column) const;
    QVariant data(int column, int role) const;
    QxTreeModelItem *parent();
    bool removeChildren(int position, int count);
    int childNumber() const;
    bool setData(int column, const QVariant &value, int role);

    SoNode* getNode() const {  return m_p_node; }

    void addChild(QxTreeModelItem*);

private:
    QList<QxTreeModelItem*> childItems;
    SoNode* m_p_node;
    QxTreeModelItem *parentItem;
};

#endif

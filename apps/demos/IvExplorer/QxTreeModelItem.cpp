#include "QxTreeModelItem.h"
#include "QxTreeModel.h"

#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodes/SoGroup.h>

QxTreeModelItem::QxTreeModelItem() :
    m_p_node(NULL),
    parentItem(NULL)
{

}

QxTreeModelItem::QxTreeModelItem(QxTreeModelItem * parent, SoNode* node) :
    m_p_node(NULL),
    parentItem(parent)
{
    m_p_node = node;

    SoGroup *group = dynamic_cast<SoGroup*>(m_p_node);
    if (group) {
        for(int i=0; i<group->getNumChildren(); i++) {
            SoNode* node = group->getChild(i);

            addChild(new QxTreeModelItem(this, node));
        }
    }
}

QxTreeModelItem::~QxTreeModelItem()
{
    removeAllChildren();
}

void QxTreeModelItem::removeAllChildren()
{
    qDeleteAll(childItems);
    childItems.clear();
}

QxTreeModelItem *QxTreeModelItem::child(int number)
{
    return childItems.value(number);
}

void QxTreeModelItem::addChild(QxTreeModelItem* item)
{
    childItems.append(item);
}

int QxTreeModelItem::childCount() const
{
    return childItems.count();
}

Qt::ItemFlags QxTreeModelItem::flags(int column) const
{
    Qt::ItemFlags defaultFlags = Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled;
    
    if (column == 2)
        defaultFlags |= Qt::ItemIsEditable;
    if (dynamic_cast<SoGroup*>(m_p_node))
        defaultFlags |= Qt::ItemIsDropEnabled;

    return defaultFlags;
}

int QxTreeModelItem::childNumber() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<QxTreeModelItem*>(this));

    return 0;
}

QVariant QxTreeModelItem::data(int column, int role) const
{
    switch(role)
    {
    case Qt::EditRole:
    case Qt::DisplayRole:
        switch(column)
        {
        case 0: return m_p_node->getTypeId().getName().getString();
        case 1: return m_p_node->getRefCount();
        case 2: return m_p_node->getName().getString();
        }
        break;
    }
    return QVariant();
}

QxTreeModelItem *QxTreeModelItem::parent()
{
    return parentItem;
}

bool QxTreeModelItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > childItems.size())
        return false;

    for (int row = 0; row < count; ++row) {
        delete childItems.takeAt(position);
        ((SoGroup*)m_p_node)->removeChild(row);
    }
    return true;
}

bool QxTreeModelItem::setData(int column, const QVariant &value, int role)
{
    if (role == Qt::EditRole)
    {
        if (column==2)
        {
            m_p_node->setName(qPrintable(value.toString()));

            return true;
        }
    }
    return false;
}


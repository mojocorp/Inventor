#include <QtGui>

#include "QxTreeModel.h"
#include "QxTreeModelItem.h"

#include <Inventor/SbName.h> 
#include <Inventor/misc/SoChildList.h> 
#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/sensors/SoNodeSensor.h> 
#include <Inventor/actions/SoSearchAction.h> 

QxTreeModel::QxTreeModel(QObject *parent)
    : QAbstractItemModel(parent),
    m_p_root(0)
{
    rootItem = new QxTreeModelItem();

    nodeSensor = new SoNodeSensor((SoSensorCB*)QxTreeModel::sensorCallback, this);
    nodeSensor->setPriority(0);//Needed in order to do getTriggerNode()    
}

QxTreeModel::~QxTreeModel()
{
    delete rootItem;
}

int QxTreeModel::columnCount(const QModelIndex &parent) const
{
    return 3;
}

bool QxTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    QxTreeModelItem* item = getItem(index);

    bool result = item->setData(index.column(), value, role);

    if (result) {
        emit dataChanged(index, index);
    }

    return result;
}

QVariant QxTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    
    return getItem(index)->data(index.column(), role);
}

Qt::ItemFlags QxTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return getItem(index)->flags(index.column());
}

QVariant QxTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch(section)
        {
        case 0: return "Class";
        case 1: return "Ref";
        case 2: return "Name";
        }
    }

    return QVariant();
}

QModelIndex QxTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    QxTreeModelItem *parentItem = getItem(parent);

    QxTreeModelItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex QxTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    QxTreeModelItem *childItem = getItem(index);
    QxTreeModelItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

int QxTreeModel::rowCount(const QModelIndex &parent) const
{
    QxTreeModelItem *parentItem = getItem(parent);

    return parentItem->childCount();
}

bool QxTreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    QxTreeModelItem *parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}

QStringList QxTreeModel::mimeTypes() const
{
   QStringList types = QAbstractItemModel::mimeTypes();
   types << "application/x-pointer";
   return types;
}

QMimeData *QxTreeModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData * mimeData = new QMimeData();
    QByteArray encodedData;

    QDataStream stream(&encodedData, QIODevice::WriteOnly);

    foreach (QModelIndex index, indexes) {
        if (index.isValid() && index.column() == 0) {
            SoNode *node = getNode(index);
            SoGroup *parent = (SoGroup*)getItem(index)->parent()->getNode();
            
            node->ref();
            parent->removeChild(node);

            stream << reinterpret_cast<quintptr>(node);
        }
    }

    mimeData->setData("application/x-pointer", encodedData);
    return mimeData;
}

bool QxTreeModel::dropMimeData(QMimeData const * data, Qt::DropAction action, int row, int column, QModelIndex const & parent)
{
   if (action == Qt::IgnoreAction)
       return true;
   if (!data->hasFormat("application/x-pointer"))
       return false;
   if (column > 0)
       return false;

   int beginRow;
   if (row != -1) {
      beginRow = row;
   } else if (parent.isValid()) {
      beginRow = parent.row();
   } else {
      beginRow = rowCount(QModelIndex());
   }

   QByteArray encodedData = data->data("application/x-pointer");
   QDataStream stream(&encodedData, QIODevice::ReadOnly);

   QList<quintptr> ptrs;
   while (!stream.atEnd())
   {
       quintptr ptr;
       stream >> ptr;
       ptrs << ptr;
   }

   // No pointers, nothing to do
   if (ptrs.empty())
       return false;

   // Get the group we have to drop into
   SoGroup *dropToGroup = dynamic_cast<SoGroup*>(getNode(parent));

   // Do the moves
   foreach(quintptr ptr, ptrs)
   {
       SoNode* node = reinterpret_cast<SoNode*>(ptr);
       dropToGroup->addChild(node);

       printf("add %s to %s\n", node->getTypeId().getName().getString(), dropToGroup->getTypeId().getName().getString());
       node->unref();
   }

   beginInsertRows(
           createIndex(0, 0, dropToGroup),
           row,
           row+ptrs.size());
   endInsertRows();
//reset();
   return true;
 }

Qt::DropActions QxTreeModel::supportedDropActions() const
{
    return Qt::MoveAction;
}

void QxTreeModel::setupModelData(SoNode * root)
{
    m_p_root = root;
    
    rootItem->removeAllChildren();
    rootItem->addChild(new QxTreeModelItem(rootItem, m_p_root));

    nodeSensor->detach();
    nodeSensor->attach(m_p_root);

    reset();
}

// Helper
SoNode* QxTreeModel::getNode(const QModelIndex &index) const
{
    QxTreeModelItem * item = getItem(index);

    return item->getNode();
}

QxTreeModelItem *QxTreeModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        QxTreeModelItem *item = static_cast<QxTreeModelItem*>(index.internalPointer());
        if (item) return item;
    }
    return rootItem;
}

void QxTreeModel::sensorCallback(QxTreeModel* tm, SoNodeSensor* sensor)
{
    //tm->rootItem->populate();
    printf("update\n");
}


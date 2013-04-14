#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

class SoNode;
class SoNodeSensor;
class QxTreeModelItem;

class QxTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    QxTreeModel(QObject *parent = 0);
    virtual ~QxTreeModel();

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    bool removeRows(int position, int rows, const QModelIndex &parent = QModelIndex());

    QStringList mimeTypes() const;
    QMimeData *mimeData(const QModelIndexList &indexes) const;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
    Qt::DropActions supportedDropActions() const;

    void setupModelData(SoNode * root);

    SoNode* getNode(const QModelIndex &index) const;
private:
    static void sensorCallback(QxTreeModel* tv, SoNodeSensor* sensor);

    SoNodeSensor* nodeSensor;

    QxTreeModelItem *getItem(const QModelIndex &index) const;

    SoNode* m_p_root;
    QxTreeModelItem* rootItem;

    friend class QxTreeModelItem;
};

#endif

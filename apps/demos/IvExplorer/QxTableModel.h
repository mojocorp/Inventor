#ifndef QX_TABLE_MODEL_H
#define QX_TABLE_MODEL_H

#include <QAbstractTableModel>
#include <QModelIndex>
#include <QVariant>
#include <QVector>

class SoNode;
class SoSensor;
class SoNodeSensor;

class QxTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    QxTableModel(QObject *parent = 0);
    virtual ~QxTableModel();

    void setupModelData(SoNode * root);

    Qt::ItemFlags flags( const QModelIndex & index ) const;

    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
private:
    void updateSensor(SoNodeSensor*);
    static void sensorCallback(QxTableModel*, SoSensor*);

    SoNode* m_p_root;
    SoNodeSensor* sensor;
};

#endif

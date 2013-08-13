#include <QtGui>

#include "QxTableModel.h"
#include "QxFieldItemDelegate.h"

#include <Inventor/SbString.h> 
#include <Inventor/misc/SoChildList.h> 
#include <Inventor/nodes/SoNode.h>
#include <Inventor/fields/SoField.h> 
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/sensors/SoNodeSensor.h>
#include <Inventor/lists/SoFieldList.h>

QxTableModel::QxTableModel(QObject *parent)
    : QAbstractTableModel(parent),
      m_p_root(0)
{

}

QxTableModel::~QxTableModel()
{

}

void QxTableModel::setupModelData(SoNode * root)
{
    m_p_root = root;

    sensor = new SoNodeSensor((SoSensorCB*)QxTableModel::sensorCallback, this);
    sensor->setPriority(0);
    sensor->attach(m_p_root);

    reset();
}

Qt::ItemFlags QxTableModel::flags( const QModelIndex & index ) const
{
    return Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsEnabled;

}

bool QxTableModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if (role != Qt::EditRole)
        return false;

    if (index.column() == 2)
    {
        SoFieldList fields;
        m_p_root->getFields(fields);
        fields[index.row()]->set(value.toString().toUtf8().data());
        fields[index.row()]->getContainer()->touch();
    }

    emit dataChanged(index, index);
}

QVariant QxTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    SoFieldList fields;
    m_p_root->getFields(fields);

    if (role == Qt::DisplayRole)
    {
        switch(index.column())
        {
        case 0: //Class
            return fields[index.row()]->getTypeId().getName().getString();
        case 1: //Name
        {
            SbName name;
            m_p_root->getFieldName(fields[index.row()], name);
            return name.getString();
        }
            break;
        case 2: //Value
        {
            SbString value;
            fields[index.row()]->get(value);
            return QString::fromUtf8(value.getString());
        }
        }
    }

    if (role == Qt::EditRole)
    {
        switch(index.column())
        {
        case 2: //Value
            return QVariant::fromValue(fields[index.row()]);
        default: break;
        }

    }
    return QVariant();
}

QVariant QxTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch(section)
        {
        case 0: return "Class";
        case 1: return "Name";
        case 2: return "Value";
        }
    }

    return QVariant();
}

QModelIndex QxTableModel::index(int row, int column, const QModelIndex &parent) const
{
    return hasIndex(row, column, parent) ? createIndex(row, column, 0) : QModelIndex();
}

int QxTableModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0 || !m_p_root)
        return 0;

    SoFieldList fields;

    return m_p_root->getFields(fields);
}

int QxTableModel::columnCount(const QModelIndex &parent) const
{
    return 3;
}

void QxTableModel::updateSensor(SoNodeSensor* sensor)
{
    SoFieldList fields;
    m_p_root->getFields(fields);

    int row = fields.find(sensor->getTriggerField());

    emit dataChanged(index(row,0), index(row, columnCount()));
}

void QxTableModel::sensorCallback(QxTableModel* model, SoSensor* sensor)
{
    model->updateSensor((SoNodeSensor*)sensor);
}


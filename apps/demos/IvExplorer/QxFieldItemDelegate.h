#ifndef QXFIELDITEMDELEGATE_H
#define QXFIELDITEMDELEGATE_H

#include <QStyledItemDelegate>

#include <Inventor/fields/SoField.h>

Q_DECLARE_METATYPE(SoField*)

class QxFieldItemDelegate : public QStyledItemDelegate
{
Q_OBJECT
public:
    QxFieldItemDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
signals:

public slots:

};

#endif // QXFIELDITEMDELEGATE_H

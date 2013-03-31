#include "QxFieldItemDelegate.h"

#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>

#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFBool.h>
//#include <Inventor/fields/SoSFDouble.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoMFString.h>

QxFieldItemDelegate::QxFieldItemDelegate(QObject *parent) :
        QStyledItemDelegate(parent)
{
}

QWidget *QxFieldItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    SoField * field = index.data(Qt::EditRole).value<SoField*>();
    if (field)
    {
        if (dynamic_cast<SoSFInt32*>(field))
        {
            QSpinBox *editor = new QSpinBox(parent);
            return editor;
        }

        if (dynamic_cast<SoSFFloat*>(field) /*|| dynamic_cast<SoSFDouble*>(field)*/)
        {
            QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
            return editor;
        }

        if (dynamic_cast<SoSFString*>(field))
        {
            QLineEdit *editor = new QLineEdit(parent);
            return editor;
        }

        if (dynamic_cast<SoMFString*>(field))
        {
            QTextEdit *editor = new QTextEdit(parent);
            return editor;
        }

        if (dynamic_cast<SoSFEnum*>(field))
        {
            SoSFEnum * enumField = dynamic_cast<SoSFEnum*>(field);

            QComboBox *editor = new QComboBox(parent);

            for (int i=0; i<enumField->getNumEnums(); i++)
            {
                SbName name;
                enumField->getEnum (i, name);
                editor->addItem(name.getString());
            }
            return editor;
        }

        if (dynamic_cast<SoSFBool*>(field))
        {
            SoSFBool * boolField = dynamic_cast<SoSFBool*>(field);

            QComboBox *editor = new QComboBox(parent);
            editor->addItem("TRUE");
            editor->addItem("FALSE");
            return editor;
        }
    }
    return QStyledItemDelegate::createEditor(parent, option, index);
}

void QxFieldItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    SoField * field = index.data(Qt::EditRole).value<SoField*>();
    if (field)
    {
        SbString value;
        field->get(value);

        if (dynamic_cast<SoSFInt32*>(field))
        {
            QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
            spinBox->setValue(QString(value.getString()).toInt());
        }

        if (dynamic_cast<SoSFFloat*>(field) /*|| dynamic_cast<SoSFDouble*>(field)*/)
        {
            QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
            spinBox->setValue(QString(value.getString()).toDouble());
        }

        if (dynamic_cast<SoSFString*>(field))
        {
            QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
            lineEdit->setText(QString::fromUtf8(value.getString()));
        }

        if (dynamic_cast<SoMFString*>(field))
        {
            QTextEdit *textEdit = static_cast<QTextEdit*>(editor);
            textEdit->setText(QString::fromUtf8(value.getString()));
        }

        if (dynamic_cast<SoSFEnum*>(field))
        {
            QComboBox * comboBox = static_cast<QComboBox*>(editor);
            comboBox->setCurrentIndex(comboBox->findText(value.getString()));
        }

        if (dynamic_cast<SoSFBool*>(field))
        {
            QComboBox * comboBox = static_cast<QComboBox*>(editor);
            comboBox->setCurrentIndex(comboBox->findText(value.getString()));
        }
    }
    else
    {
        QStyledItemDelegate::setEditorData(editor, index);
    }
}

void QxFieldItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    SoField * field = index.data(Qt::EditRole).value<SoField*>();
    if (field)
    {
        QSpinBox * spinBox = dynamic_cast<QSpinBox*>(editor);
        if (spinBox)
        {
            model->setData(index, QString::number(spinBox->value()), Qt::EditRole);
        }

        QDoubleSpinBox * dblSpinBox = dynamic_cast<QDoubleSpinBox*>(editor);
        if (dblSpinBox)
        {
            model->setData(index, QString::number(dblSpinBox->value()), Qt::EditRole);
        }

        QComboBox * comboBox = dynamic_cast<QComboBox*>(editor);
        if (comboBox)
        {
            model->setData(index, comboBox->currentText(), Qt::EditRole);
        }

        QLineEdit * lineEdit = dynamic_cast<QLineEdit*>(editor);
        if (lineEdit)
        {
            model->setData(index, lineEdit->text(), Qt::EditRole);
        }

        QTextEdit * textEdit = dynamic_cast<QTextEdit*>(editor);
        if (textEdit)
        {
            model->setData(index, textEdit->toPlainText(), Qt::EditRole);
        }
    }
    else
    {
        QStyledItemDelegate::setModelData(editor, model, index);

    }
}

void QxFieldItemDelegate::updateEditorGeometry(QWidget *editor,  const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}

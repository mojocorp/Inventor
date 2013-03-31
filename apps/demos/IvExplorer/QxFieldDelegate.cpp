#include "QxFieldDelegate.h"

SoSFEnumEditor::SoSFEnumEditor(QWidget *widget) : QComboBox(widget)
{
    populateList();
}

SoField* SoSFEnumEditor::value() const
{
    return NULL; //qVariantValue<QColor>(itemData(currentIndex(), Qt::DecorationRole));
}

void SoSFEnumEditor::setValue(const SoField* value)
{
    //setCurrentIndex(findData(color, int(Qt::DecorationRole)));
}

void SoSFEnumEditor::populateList()
{
    /*QStringList colorNames = QColor::colorNames();

    for (int i = 0; i < colorNames.size(); ++i) {
        QColor color(colorNames[i]);

        insertItem(i, colorNames[i]);
        setItemData(i, color, Qt::DecorationRole);
    }*/
}

#ifndef QXFIELDDELEGATE_H
#define QXFIELDDELEGATE_H

#include <QMetaType>
#include <QComboBox>

#include <Inventor/fields/SoSFEnum.h>

Q_DECLARE_METATYPE(SoField*)
Q_DECLARE_METATYPE(SoSFEnum*)

class SoSFEnumEditor : public QComboBox
{
    Q_OBJECT
    Q_PROPERTY(SoField* value READ value WRITE setValue USER true)

public:
    SoSFEnumEditor(QWidget *widget = 0);

public:
    SoField* value() const;
    void setValue(const SoField* value);

private:
    void populateList();
};

#endif // QXFIELDDELEGATE_H

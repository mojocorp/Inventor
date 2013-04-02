#ifndef QMORF_H
#define QMORF_H

#include <QWidget>

class SoSeparator;
class SoPerspectiveCamera;

class QSlider;
class QCheckBox;

class QMorfWidget : public QWidget
{
    Q_OBJECT
public:
    QMorfWidget(SoSeparator *root, SoPerspectiveCamera *c);

protected slots:
    void showAboutDialog();
    void toggleShapeAnimation();
    void toggleColorAnimation();
    void dragColorSlider(int value);
    void dragShapeSlider(int value);
public:
    QSlider *shapeSlider;
    QSlider *colorSlider;
    QCheckBox *shapeAnimateCheckBox;
    QCheckBox *colorAnimateCheckBox;
};

#endif

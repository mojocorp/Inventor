#ifndef SOQTPREFERENCEDIALOG_H
#define SOQTPREFERENCEDIALOG_H

#include <QDialog>

#include <Inventor/SbVec2f.h>

namespace Ui {
    class SoQtPreferenceDialog;
}

class SoCamera;
class SoQtFullViewer;
class QTabWidget;

class SoQtPreferenceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SoQtPreferenceDialog(SoQtFullViewer *viewer, QWidget *parent = 0);
    ~SoQtPreferenceDialog();

    QTabWidget *tabWidget() const;

    void setCamera(SoCamera * camera);

public slots:
    void setSeekTime(double seconds);
    void setZoom(double value);

    void setAutoClipping(bool state);
    void setClipNearValue(double value);
    void setClipFarValue(double value);

    void setStereoViewing(bool state);

    void setAnimationEnabled(bool state);
    void setFeedbackVisibility(bool state);
    void setFeebackSize(int size);
protected slots:
    void setDetailSeek();
    void setSeekDistMode();

    void seekDistValueChanged(double);
    void seekDistSliderChanged(int);

    void zoomSliderChanged(int);

    void zoomMinFieldChanged(double value);
    void zoomMaxFieldChanged (double value);

    void stereoTypeChanged(int index);
    void eyeSeparationSliderChanged(int value);
    void parallaxBalanceSliderChanged(int value);
    void eyeSeparationInputChanged(double value);
    void parallaxBalanceInputChanged(double value);

private:
    Ui::SoQtPreferenceDialog *ui;

    SoQtFullViewer *viewer;

    // zoom pref sheet vars and callbacks
    SbVec2f         zoomSldRange;
};

#endif // SOQTPREFERENCEDIALOG_H

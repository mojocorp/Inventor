#include <QMessageBox>

#include <Inventor/Qt/viewers/SoQtPreferenceDialog.h>
#include <Inventor/Qt/viewers/SoQtFullViewer.h>
#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoOrthographicCamera.h>

#include "ui_SoQtPreferenceDialog.h"

SoQtPreferenceDialog::SoQtPreferenceDialog(SoQtFullViewer *_viewer, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SoQtPreferenceDialog),
    viewer(_viewer)
{
    ui->setupUi(this);

    // Seek prefs
    ui->seekTimeInput->setValue(viewer->getSeekTime());
    connect(ui->seekTimeInput, SIGNAL (valueChanged(double)), this, SLOT (setSeekTime(double)));

    ui->seekPointsButton->setChecked(viewer->isDetailSeek());
    ui->seekObjectsButton->setChecked(!viewer->isDetailSeek());
    connect(ui->seekPointsButton, SIGNAL(clicked()), SLOT(setDetailSeek()));
    connect(ui->seekObjectsButton, SIGNAL(clicked()), SLOT(setDetailSeek()));

    ui->seekDistSlider->setValue(viewer->seekDistance);
    ui->seekDistInput->setValue(viewer->seekDistance);
    connect(ui->seekDistSlider, SIGNAL(valueChanged(int)), SLOT(seekDistSliderChanged(int)));
    connect(ui->seekDistInput, SIGNAL(valueChanged(double)), SLOT(seekDistValueChanged(double)));

    ui->seekDistPercButton->setChecked(viewer->seekDistAsPercentage);
    ui->seekDistAbsButton->setChecked(!viewer->seekDistAsPercentage);
    connect(ui->seekDistPercButton, SIGNAL(clicked()), SLOT(setSeekDistMode()));
    connect(ui->seekDistAbsButton, SIGNAL(clicked()), SLOT(setSeekDistMode()));

    // Zoom prefs
    zoomSldRange.setValue(1, 140);

    bool zoomEnabled = (viewer->camera != NULL && viewer->camera->isOfType(SoPerspectiveCamera::getClassTypeId()));
    ui->zoomTab->setEnabled(zoomEnabled);

    ui->zoomSlider->setValue(viewer->getCameraZoom());
    ui->zoomInput->setValue(viewer->getCameraZoom());
    connect(ui->zoomInput, SIGNAL(valueChanged(double)), SLOT(setZoom(double)));
    connect(ui->zoomSlider, SIGNAL(valueChanged(int)), SLOT(zoomSliderChanged(int)));

    ui->zoomFromInput->setValue(zoomSldRange[0]);
    ui->zoomToInput->setValue(zoomSldRange[1]);
    connect(ui->zoomFromInput, SIGNAL(valueChanged(double)), SLOT(zoomMinFieldChanged(double)));
    connect(ui->zoomToInput, SIGNAL(valueChanged(double)), SLOT(zoomMinFieldChanged(double)));

    // Clip prefs
    ui->clippingTab->setEnabled (viewer->camera != NULL);
    float nearDistance = (viewer->camera != NULL) ? viewer->camera->nearDistance.getValue() : 0.0;
    float farDistance = (viewer->camera != NULL) ? viewer->camera->farDistance.getValue() : 0.0;

    connect(ui->autoClippingToggle, SIGNAL(clicked(bool)), SLOT(setAutoClipping(bool)));

    ui->clipNearInput->setValue(nearDistance);
    ui->clipFarInput->setValue(farDistance);
    connect(ui->clipNearInput, SIGNAL(valueChanged(double)), SLOT(setClipNearValue(double)));
    connect(ui->clipFarInput, SIGNAL(valueChanged(double)), SLOT(setClipFarValue(double)));

    setAutoClipping(viewer->isAutoClipping());

    // Stereo prefs
    ui->stereoViewingToggle->setCheckState (viewer->isStereoViewing() ? Qt::Checked : Qt::Unchecked);
    ui->stereoDistInput->setValue (viewer->getStereoOffset());

    // Exam prefs
    SoQtExaminerViewer *ew = dynamic_cast<SoQtExaminerViewer*>(viewer);
    if (ew) {
        ui->enableSpinToggle->setCheckState (ew->isAnimationEnabled() ? Qt::Checked : Qt::Unchecked);
        connect(ui->enableSpinToggle, SIGNAL(clicked(bool)), SLOT(setAnimationEnabled(bool)));

        ui->showRotPointToggle->setEnabled (viewer->camera != NULL);
        ui->showRotPointToggle->setCheckState (ew->isFeedbackVisible() ? Qt::Checked : Qt::Unchecked);
        connect(ui->showRotPointToggle, SIGNAL(clicked(bool)), SLOT(setFeedbackVisibility(bool)));

        ui->feedbackInput->setEnabled (viewer->camera != NULL);
        ui->feedbackInput->setValue(ew->getFeedbackSize());
        connect(ui->feedbackInput, SIGNAL(valueChanged(int)), SLOT(setFeebackSize(int)));
    } else {
        ui->tabWidget->removeTab(ui->tabWidget->indexOf(ui->examTab));
    }
}

SoQtPreferenceDialog::~SoQtPreferenceDialog()
{
    delete ui;
}

void
SoQtPreferenceDialog::setCamera(SoCamera * camera)
{
    SbBool enable = camera != NULL && camera->isOfType (SoPerspectiveCamera::getClassTypeId());

    ui->zoomTab->setEnabled(enable);
    ui->clippingTab->setEnabled (camera != NULL);
    ui->clippingTab->setEnabled (viewer->camera != NULL);
    float nearDistance = (viewer->camera != NULL) ? viewer->camera->nearDistance.getValue() : 0.0;
    float farDistance = (viewer->camera != NULL) ? viewer->camera->farDistance.getValue() : 0.0;
    ui->clipNearInput->setValue(nearDistance);
    ui->clipFarInput->setValue(farDistance);

    ui->showRotPointToggle->setEnabled (viewer->camera != NULL);
    ui->feedbackInput->setEnabled (viewer->camera != NULL);

    // update the UI if enabled
    if (enable) {
        double zoom = viewer->getCameraZoom();
        setZoom(zoom);
    }
}

void
SoQtPreferenceDialog::setSeekTime(double seconds)
{
    viewer->setSeekTime(seconds);
}

void
SoQtPreferenceDialog::setDetailSeek()
{
    viewer->setDetailSeek(ui->seekPointsButton->isChecked());
}

void
SoQtPreferenceDialog::setSeekDistMode()
{
    viewer->seekDistAsPercentage = ui->seekDistPercButton->isChecked();
}

void
SoQtPreferenceDialog::seekDistSliderChanged(int value)
{
    ui->seekDistInput->setValue(value);
}

void
SoQtPreferenceDialog::seekDistValueChanged(double value)
{
    viewer->seekDistance = value;

    ui->seekDistSlider->setValue(value);
}

void
SoQtPreferenceDialog::setZoom(double zoom)
{
    if (zoom > 0.0) {
        // check for valid perspective camera range
        if ( viewer->camera != NULL &&
            viewer->camera->isOfType(SoPerspectiveCamera::getClassTypeId()) ) {
            zoom = (zoom < 0.01) ? 0.01 : ((zoom > 179.99) ? 179.99 : zoom);
        }

        // check if the new value changed the slider range
        if (zoom < zoomSldRange[0]) {
            zoomSldRange[0] = zoom;
        } else if (zoom > zoomSldRange[1]) {
            zoomSldRange[1] = zoom;
        }
        viewer->setCameraZoom (zoom);
    } else {
        zoom = viewer->getCameraZoom();
    }
    ui->zoomInput->setValue(zoom);
    ui->zoomSlider->blockSignals(true);
    ui->zoomSlider->setValue(zoom);
    ui->zoomSlider->blockSignals(false);
}

void
SoQtPreferenceDialog::zoomSliderChanged(int value)
{
    setZoom(value);
}

void
SoQtPreferenceDialog::zoomMinFieldChanged (double value)
{
    // check for valid perspective camera range
    if (viewer->camera != NULL &&
        viewer->camera->isOfType(SoPerspectiveCamera::getClassTypeId())) {
        value = (value < 0.01) ? 0.01 : ((value > 178.99) ? 178.99 : value);

        // finally update the slider to reflect the changes
        zoomSldRange[0] = value;
        ui->zoomInput->setValue(viewer->getCameraZoom());
    }
}

void
SoQtPreferenceDialog::zoomMaxFieldChanged (double value)
{
    // check for valid perspective camera range
    if (viewer->camera != NULL &&
        viewer->camera->isOfType(SoPerspectiveCamera::getClassTypeId())) {
        value = (value < 0.01) ? 0.01 : ((value > 178.99) ? 178.99 : value);

        // finally update the slider to reflect the changes
        zoomSldRange[1] = value;
        ui->zoomInput->setValue(viewer->getCameraZoom());
    }
}

void
SoQtPreferenceDialog::setAutoClipping(bool state)
{
    ui->autoClippingToggle->setCheckState (state ? Qt::Checked : Qt::Unchecked);
    ui->clipNearInput->setEnabled(!state);
    ui->clipFarInput->setEnabled(!state);

    viewer->setAutoClipping(state);
}

void
SoQtPreferenceDialog::setClipNearValue(double value)
{
    if (viewer->camera == NULL)
        return;

    if (value > 0.0 || viewer->camera->isOfType(SoOrthographicCamera::getClassTypeId())) {
        viewer->camera->nearDistance = value;
    }

    ui->clipNearInput->setValue (viewer->camera->nearDistance.getValue());
    ui->clipFarInput->setValue (viewer->camera->farDistance.getValue());
}

void
SoQtPreferenceDialog::setClipFarValue (double value)
{
    if (viewer->camera == NULL)
        return;

    if (value > 0.0 || viewer->camera->isOfType(SoOrthographicCamera::getClassTypeId())) {
        viewer->camera->farDistance = value;
    }

    ui->clipNearInput->setValue (viewer->camera->nearDistance.getValue());
    ui->clipFarInput->setValue (viewer->camera->farDistance.getValue());
}

void
SoQtPreferenceDialog::setStereoViewing(bool state)
{
    // checks to make sure stereo viewing can be set, else
    // grey the UI and bring and error message.
    if (state != viewer->isStereoViewing()) {
        viewer->setStereoViewing (state);
    }
    if (state && !viewer->isStereoViewing()) {
        // we couldn't activate stereo viewing:
        ui->stereoViewingToggle->setCheckState (Qt::Unchecked);
        ui->stereoViewingToggle->setEnabled (false);
        QMessageBox::critical (this, tr("Stereo Error"),
                               tr("Stereo Viewing can't be set on this machine."));
        return;
    }

    // show/hide the eye spacing thumbwheel
    //TODO ui->stereoDistWidgets->setEnabled (isStereoViewing());
}

void
SoQtPreferenceDialog::setStereoDistance (double value)
{
    // get text value from the label and update camera
    viewer->setStereoOffset(value);
    viewer->scheduleRedraw();

    ui->stereoDistInput->setValue (value);
}

void
SoQtPreferenceDialog::stereoSliderChanged (int value)
{
    // shorter/grow the stereo camera offset
    //TODO setStereoDistance (getStereoOffset() * pow (80.0, value - stereoWheelVal) / 360.0);
    //TODO stereoWheelVal = value;
}

void
SoQtPreferenceDialog::setAnimationEnabled(bool state)
{
    SoQtExaminerViewer *ew = dynamic_cast<SoQtExaminerViewer*>(viewer);
    if (ew) {
        ew->setAnimationEnabled(state);

        ui->enableSpinToggle->setCheckState (state ? Qt::Checked : Qt::Unchecked);
    }
}

void
SoQtPreferenceDialog::setFeedbackVisibility(bool state)
{
    SoQtExaminerViewer *ew = dynamic_cast<SoQtExaminerViewer*>(viewer);
    if (ew) {
        ew->setFeedbackVisibility(state);

        ui->showRotPointToggle->setCheckState (state ? Qt::Checked : Qt::Unchecked);
        ui->feedbackInput->setEnabled(state);
    }
}

void
SoQtPreferenceDialog::setFeebackSize(int size)
{
    SoQtExaminerViewer *ew = dynamic_cast<SoQtExaminerViewer*>(viewer);
    if (ew) {
        ew->setFeedbackSize(size);
    }
}

QTabWidget *SoQtPreferenceDialog::tabWidget() const
{
    return ui->tabWidget;
}

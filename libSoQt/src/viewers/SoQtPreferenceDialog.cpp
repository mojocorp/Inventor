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
    ui->tabWidget->setCurrentIndex(0);

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
    connect(ui->stereoViewingToggle, SIGNAL(toggled(bool)), SLOT(setStereoViewing(bool)));

    ui->stereoViewingToggle->setChecked(viewer->isStereoViewing());
    ui->stereoAdjustmentsGroupBox->setEnabled(ui->stereoViewingToggle->isChecked());
    ui->stereoTypeComboBox->setCurrentIndex(viewer->isStereoViewing() ? (int)viewer->getStereoType() - 1 : 0);

    connect(ui->stereoTypeComboBox, SIGNAL(activated(int)), SLOT(stereoTypeChanged(int)));
    connect(ui->eyeSeparationSlider,   SIGNAL(valueChanged(int)), SLOT(eyeSeparationSliderChanged(int)));
    connect(ui->parallaxBalanceSlider, SIGNAL(valueChanged(int)), SLOT(parallaxBalanceSliderChanged(int)));
    connect(ui->eyeSeparationInput,   SIGNAL(valueChanged(double)), SLOT(eyeSeparationInputChanged(double)));
    connect(ui->parallaxBalanceInput, SIGNAL(valueChanged(double)), SLOT(parallaxBalanceInputChanged(double)));

    ui->eyeSeparationInput->setValue (viewer->getStereoOffset());
    ui->parallaxBalanceInput->setValue(viewer->getStereoBalance());

    // Exam prefs
    SoQtExaminerViewer *ew = dynamic_cast<SoQtExaminerViewer*>(viewer);
    if (ew) {
        ui->enableSpinToggle->setChecked (ew->isAnimationEnabled());
        connect(ui->enableSpinToggle, SIGNAL(clicked(bool)), SLOT(setAnimationEnabled(bool)));

        ui->showRotPointToggle->setEnabled(viewer->camera != NULL);
        ui->showRotPointToggle->setChecked(ew->isFeedbackVisible());
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
    ui->autoClippingToggle->setChecked(state);
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
    if (state) {
        viewer->setStereoType((SoQtViewer::StereoType)(ui->stereoTypeComboBox->currentIndex() + 1));
    } else {
        viewer->setStereoType(SoQtViewer::MONOSCOPIC);
    }
    ui->stereoViewingToggle->setChecked(viewer->isStereoViewing());
    ui->stereoAdjustmentsGroupBox->setEnabled(viewer->isStereoViewing());
}

void
SoQtPreferenceDialog::stereoTypeChanged(int index)
{
    if (!ui->stereoViewingToggle->isChecked())
        return;

    switch(index)
    {
    case 0: viewer->setStereoType(SoQtViewer::QUADBUFFER); break;
    case 1: viewer->setStereoType(SoQtViewer::ANAGLYPH_RED_CYAN); break;
    case 2: viewer->setStereoType(SoQtViewer::ANAGLYPH_BLUE_YELLOW); break;
    case 3: viewer->setStereoType(SoQtViewer::ANAGLYPH_GREEN_MAGENTA); break;
    default: break;
    }
    ui->stereoTypeComboBox->setCurrentIndex(viewer->isStereoViewing() ? (int)viewer->getStereoType() - 1 : 0);
    ui->stereoViewingToggle->setChecked(viewer->isStereoViewing());
    ui->stereoAdjustmentsGroupBox->setEnabled(viewer->isStereoViewing());
}

void
SoQtPreferenceDialog::eyeSeparationSliderChanged(int value)
{
    ui->eyeSeparationInput->setValue(value / 100.0);
}

void
SoQtPreferenceDialog::parallaxBalanceSliderChanged(int value)
{
    ui->parallaxBalanceInput->setValue(value / 100.0);
}

void
SoQtPreferenceDialog::eyeSeparationInputChanged(double value)
{
    viewer->setStereoOffset(value);
    viewer->scheduleRedraw();

    ui->eyeSeparationSlider->blockSignals(true);
    ui->eyeSeparationSlider->setValue (value * 100);
    ui->eyeSeparationSlider->blockSignals(false);
}

void
SoQtPreferenceDialog::parallaxBalanceInputChanged(double value)
{
    viewer->setStereoBalance(value);
    viewer->scheduleRedraw();

    ui->parallaxBalanceSlider->blockSignals(true);
    ui->parallaxBalanceSlider->setValue(value * 100);
    ui->parallaxBalanceSlider->blockSignals(false);
}

void
SoQtPreferenceDialog::setAnimationEnabled(bool state)
{
    SoQtExaminerViewer *ew = dynamic_cast<SoQtExaminerViewer*>(viewer);
    if (ew) {
        ew->setAnimationEnabled(state);

        ui->enableSpinToggle->setChecked(state);
    }
}

void
SoQtPreferenceDialog::setFeedbackVisibility(bool state)
{
    SoQtExaminerViewer *ew = dynamic_cast<SoQtExaminerViewer*>(viewer);
    if (ew) {
        ew->setFeedbackVisibility(state);

        ui->showRotPointToggle->setChecked(state);
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

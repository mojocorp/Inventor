/*
 *
 *  Copyright (C) 2000 Silicon Graphics, Inc.  All Rights Reserved. 
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  Further, this software is distributed without any warranty that it is
 *  free of the rightful claim of any third person regarding infringement
 *  or the like.  Any license provided herein, whether implied or
 *  otherwise, applies only to this software file.  Patent licenses, if
 *  any, provided herein do not apply to combinations of this program with
 *  other software, or any other product whatsoever.
 * 
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  Contact information: Silicon Graphics, Inc., 1600 Amphitheatre Pkwy,
 *  Mountain View, CA  94043, or:
 * 
 *  http://www.sgi.com 
 * 
 *  For further information regarding this notice, see: 
 * 
 *  http://oss.sgi.com/projects/GenInfo/NoticeExplan/
 *
 */

//
// Program to transform one quad mesh into another.  Linear
// interpolation of the vertices is done.  Transparent blending is
// also done to interpolate colors or textures on the objects (again,
// linearly).
//
// Written by Gavin Bell for Silicon Graphics
//
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <QApplication>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include <QCheckBox>

#include <Inventor/SoDB.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/nodes/SoCallback.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoMaterialBinding.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/nodes/SoTextureCoordinate2.h>
#include <Inventor/nodes/SoTextureCoordinateBinding.h>
#include <Inventor/nodes/SoTextureCoordinatePlane.h>
#include <Inventor/sensors/SoIdleSensor.h>
#include <Inventor/Qt/SoQt.h>
#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>
#include <Inventor/Qt/SoQtRenderArea.h>

#include "../../samples/common/InventorLogo.h"
#include "QuadThing.h"
#include "Background.h"
#include "qmorf.h"

//
// The list of things we'll morph between
//

static QuadThingList objectList;

//
// The interpolated shape we'll constantly modify
//
static QuadThing *TheShape;

//
// Scale for slider bars
//
static const int SLIDER_MAX = 1000;

//
// Sensor to do automatic animation
//
static SoIdleSensor *animationSensor;

FlashBackground *background;
static int nobackground = 0;
static int notextures = 0;

//
// Create a scene graph that looks something like this:
//
//                    Separator root
//                   /              \ 
//                  /                \ 
//      Separator ToStuff         Separator FromStuff
//     /              |            |           |     |
// ToTexSwitch        |     FromTexSwitch   TransparentMaterial
//  /        \        |     /          \            |
// Texture Material   |  Texture    Material        /
//                    \                            /
//                     \                          /
//                      ...Separator for quadmesh shape...
//
// ... So the quadMesh gets drawn twice, to blend the two sets of
// colors.  The transparency field in the TransparentMaterial is used
// to control how much of the 'From' object is blended into the 'To'
// object.
//

static SoSeparator *ToStuff, *FromStuff;
static SoGroup *placeHolder;
static SoGroup *ToTexGroup, *FromTexGroup;
static SoGroup *ToMatGroup, *FromMatGroup;
static SoMaterial *TransparentMaterial;
static SoSwitch *ToTexSwitch, *FromTexSwitch;
static SoTexture2 *noopTexture;
static SoTextureCoordinatePlane *noopTextureCoord;

//
// Setup the scene graph when we change the shapes we are
// interpolating.  The topology of the scene graph is always the same,
// but we may replace nodes like materials or textures on the fly (if
// an object has no material or texture a placeholder node that does
// not affect anything is used).
//
void
setToFromStuff(int fromObject)
{
    int toObject = (fromObject+1) % objectList.getLength();

    SoNode *t;

    // Texture image:
    if ((t = objectList[fromObject]->getTexture2()) != NULL) {
	FromTexGroup->replaceChild(0, t);
	if (FromTexSwitch->whichChild.getValue() != 1)
	    FromTexSwitch->whichChild = 0;
    } else {
	FromTexGroup->replaceChild(0, noopTexture);
	if (FromTexSwitch->whichChild.getValue() != 1)
	    FromTexSwitch->whichChild = SO_SWITCH_ALL;
    }

    // Texture coordinates:
    if ((t = objectList[fromObject]->getTexCoord()) != NULL)
	FromTexGroup->replaceChild(1, t);
    else
	FromTexGroup->replaceChild(1, noopTextureCoord);

    // Texture coordinate binding:
    if ((t = objectList[fromObject]->getTexBinding()) != NULL)
	FromTexGroup->replaceChild(2, t);
    else
	FromTexGroup->replaceChild(2, placeHolder);

    // No texture, has materials (maybe)
    if ((t = objectList[fromObject]->getMaterial()) != NULL)
	FromMatGroup->replaceChild(0, t);
    else
	FromMatGroup->replaceChild(0, placeHolder);
    if ((t = objectList[fromObject]->getMatBinding()) != NULL)
	FromMatGroup->replaceChild(1, t);
    else
	FromMatGroup->replaceChild(1, placeHolder);

    if ((t = objectList[toObject]->getTexture2()) != NULL) {
	ToTexGroup->replaceChild(0, t);
	if (ToTexSwitch->whichChild.getValue() != 1)
	    ToTexSwitch->whichChild = 0;
    } else {
	ToTexGroup->replaceChild(0, noopTexture);
	if (ToTexSwitch->whichChild.getValue() != 1)
	    ToTexSwitch->whichChild = SO_SWITCH_ALL;
    }
    if ((t = objectList[toObject]->getTexCoord()) != NULL)
	ToTexGroup->replaceChild(1, t);
    else
	ToTexGroup->replaceChild(1, noopTextureCoord);
    if ((t = objectList[toObject]->getTexBinding()) != NULL)
	ToTexGroup->replaceChild(2, t);
    else
	ToTexGroup->replaceChild(2, placeHolder);
    if ((t = objectList[toObject]->getMaterial()) != NULL)
	ToMatGroup->replaceChild(0, t);
    else
	ToMatGroup->replaceChild(0, placeHolder);
    if ((t = objectList[toObject]->getMatBinding()) != NULL)
	ToMatGroup->replaceChild(1, t);
    else
	ToMatGroup->replaceChild(1, placeHolder);
}

//
// A bunch of stuff used to control the animation
//
static const int COLOR = 0;
static const int SHAPE = 1;
static int lastColor = (-1);
static double animationTime[2] = { 0.0, 0.0 };
static int whatIsAnimating[2] = { 1, 1 };
static SbTime lastTime;

//
// Stop animation.  what is either COLOR or SHAPE.  Called by the UI
// widgets (the animation buttons or to stop the animation when a
// slider is moved).
//
void
stopAnimating(int what)
{
    whatIsAnimating[what] = 0;
    if (!(whatIsAnimating[0] || whatIsAnimating[1]))
	animationSensor->unschedule();
}
void
startAnimating(int what)
{
    whatIsAnimating[what] = 1;
    animationSensor->schedule();
    lastTime = SbTime::getTimeOfDay();
}

//
// Change the COLOR or SHAPE to the given value.  Called by the slider
// widgets.
//
void
setValue(int what, double value)
{
    animationTime[what] = fmod(value, objectList.getLength());

    double t = fmod(animationTime[what], 1.0);

    if (what == SHAPE)
    {
	int object1 = int(animationTime[SHAPE]);
	int object2 = (object1+1) % objectList.getLength();
	TheShape->interp(objectList[object1], objectList[object2], t);
    }
    else
    {
	// If we'll be using the colors of another shape, must change
	// scene graph:
	if (int(animationTime[COLOR]) != lastColor)
	{
	    lastColor = int(animationTime[COLOR]);
	    setToFromStuff(lastColor);
	}
	TransparentMaterial->transparency.setValue(t);
    }
}

//
// Called by a sensor to automatically change the objects into one
// another.  This also updates the slider widgets.
//
void
morfCallback(void *userdata, SoSensor *sensor)
{
    QMorfWidget *widget = (QMorfWidget*)userdata;

    static const double transition_time = 4.0;	// Seconds

    SbTime now = SbTime::getTimeOfDay();
    SbTime dt = (now - lastTime) / transition_time;
    static SbTime bgShape, bgColor;

    if (whatIsAnimating[COLOR])
    {
	setValue(COLOR, animationTime[COLOR] + dt.getValue());
	double t = animationTime[COLOR] / objectList.getLength();
    widget->colorSlider->setValue(t * SLIDER_MAX);
	bgColor += dt;
	if (!nobackground)
	    background->animateColor(bgColor.getValue());
    }
    if (whatIsAnimating[SHAPE])
    {
	setValue(SHAPE, animationTime[SHAPE] + dt.getValue());
	double t = animationTime[SHAPE] / objectList.getLength();
    widget->shapeSlider->setValue(t * SLIDER_MAX);
	bgShape += dt / 3.0;
	if (!nobackground)
	    background->animateShape(bgShape.getValue());
    }
    lastTime = now;
    sensor->schedule();
}

void
parse_args(int argc, char **argv)
{
    int err = 0;	// Flag: error in options?
    int c;

    // Note: optind and optarg are declared in getopt.h

    while ((c = getopt(argc, argv, "bth")) != -1)
    {
	switch(c)
	{
	  case 'b':
	    nobackground = 1;
	    break;
	  case 't':
	    notextures = 1;
	    break;
	  case 'h':	// Help
	  default:
	    err = 1;
	    break;
	}
    }
// Handle optional filenames
    for (; optind < argc; optind++)
    {
	QuadThing *q = new QuadThing(argv[optind]);
	if (q->getSceneGraph() != NULL)
	    objectList.append(q);
	else delete q;
    }
    if (objectList.getLength() < 2)
    {
	err = 1;
    }

    if (err)
    {
	fprintf(stderr, "Usage: %s [-bth] file file [file ...]\n", argv[0]);
	fprintf(stderr, "-b : No background\n");
	fprintf(stderr, "-t : No textures\n");
	fprintf(stderr, "-h : This message (help)\n");
	fprintf(stderr, "At least two files must be given, and "
		"they must contain QuadMeshes (qmorf can\n");
	fprintf(stderr, "only morph QuadMesh nodes).\n");
	fprintf(stderr, "The directory "
        "OIVHOME/data/models/CyberHeads "
		"contains good data to morph.\n");

	exit(7);
    }
}

//
// Callback for 'About...' button
//
void
QMorfWidget::showAboutDialog()
{
#if TODO
    if (access("/demos/Inventor/qmorf.about", R_OK) != 0)
    {
	system("xmessage 'Sorry, could not find "
           "/demos/Inventor/qmorf.about' > /dev/null");
	return;
    }
    char command[100];
    sprintf(command, "which acroread > /dev/null");

    int err = system(command);
    if (err)
    {
	system("xmessage 'You must install acroread"
	       " for this function to work' > /dev/null");
	return;
    }

    sprintf(command, "acroread /demos/Inventor/qmorf.about &");
    system(command);
#endif
}	

//
// Callback for the animation buttons
//
void
QMorfWidget::toggleShapeAnimation()
{
    if (whatIsAnimating[SHAPE])
        stopAnimating(SHAPE);
    else startAnimating(SHAPE);
}

void
QMorfWidget::toggleColorAnimation()
{
    if (whatIsAnimating[COLOR])
        stopAnimating(COLOR);
    else startAnimating(COLOR);
}

//
// Callbacks for color and shape sliders
//
void
QMorfWidget::dragColorSlider(int value)
{
    if (whatIsAnimating[COLOR])
    {
    stopAnimating(COLOR);
    }
    double fv = value * objectList.getLength() / (double)SLIDER_MAX;
    setValue(COLOR, fv);
}

void
QMorfWidget::dragShapeSlider(int value)
{
    if (whatIsAnimating[SHAPE])
    {
    stopAnimating(SHAPE);
    }
    double fv = value * objectList.getLength() / (double)SLIDER_MAX;
    setValue(SHAPE, fv);
}

////////////////////////////////////////////////////////////////////////
//
//  Draw the Inventor logo in the overlays.
//
////////////////////////////////////////////////////////////////////////

static void
logoCB(void *, SoAction *action)
{
    if (action->isOfType(SoGLRenderAction::getClassTypeId())) {
        static int pushedViewport = 0;
        if (!pushedViewport) {
            pushedViewport = 1;
            glPushAttrib(GL_VIEWPORT_BIT);
            glViewport(0, 0, 80, 80);
        }
        else {
            pushedViewport = 0;
            glPopAttrib();
        }
    }
}

static void
setOverlayLogo(SoSeparator *sep)
{
    static SoSeparator *logo = NULL;

    if (logo == NULL) {
        SoInput in;
        in.setBuffer((void *)ivLogo, ivLogoSize); // see common directory
        logo = SoDB::readAll(&in);
        logo->ref();
        SoCallback *cb = new SoCallback;    // sets GL viewport
        cb->setCallback(logoCB);
        logo->insertChild(cb, 0);
        logo->addChild(cb); // Will pop viewport
    }

    sep->addChild(logo);
}


//////////////////////////////////////////////////////////////////////
//
// Create and layout all of the UI stuff (I'm really not very fond of
// Motif).
//
QMorfWidget::QMorfWidget(SoSeparator *root, SoPerspectiveCamera *c)
//////////////////////////////////////////////////////////////////////
{
    QVBoxLayout *vlayout = new QVBoxLayout(this);
    vlayout->setMargin(0);

    QGridLayout *grid = new QGridLayout();

    QPushButton *aboutPushButton = new QPushButton("About...", this);
    connect(aboutPushButton, SIGNAL(clicked()), SLOT(showAboutDialog()));

    QPushButton *quitPushButton = new QPushButton("Quit...", this);
    connect(quitPushButton, SIGNAL(clicked()), SLOT(close()));

    shapeSlider = new QSlider(Qt::Horizontal, this);
    shapeSlider->setRange(0, SLIDER_MAX + SLIDER_MAX/20);
    shapeSlider->setSingleStep(SLIDER_MAX/40);
    shapeSlider->setPageStep(SLIDER_MAX/20);
    connect(shapeSlider, SIGNAL(sliderMoved(int)), SLOT(dragShapeSlider(int)));

    colorSlider = new QSlider(Qt::Horizontal, this);
    colorSlider->setRange(0, SLIDER_MAX + SLIDER_MAX/20);
    colorSlider->setSingleStep(SLIDER_MAX/40);
    colorSlider->setPageStep(SLIDER_MAX/20);
    connect(colorSlider, SIGNAL(sliderMoved(int)), SLOT(dragColorSlider(int)));

    shapeAnimateCheckBox = new QCheckBox("Animate", this);
    shapeAnimateCheckBox->setChecked(whatIsAnimating[COLOR]);
    connect(shapeAnimateCheckBox, SIGNAL(clicked()), SLOT(toggleShapeAnimation()));

    colorAnimateCheckBox = new QCheckBox("Animate", this);
    colorAnimateCheckBox->setChecked(whatIsAnimating[SHAPE]);
    connect(colorAnimateCheckBox, SIGNAL(clicked()), SLOT(toggleColorAnimation()));

    grid->addWidget(aboutPushButton, 0, 0);
    grid->addWidget(quitPushButton, 1, 0);
    grid->addWidget(new QLabel("Shape", this), 0, 1);
    grid->addWidget(new QLabel("Color", this), 1, 1);

    grid->addWidget(shapeSlider, 0, 2);
    grid->addWidget(colorSlider, 1, 2);

    grid->addWidget(shapeAnimateCheckBox, 0, 3);
    grid->addWidget(colorAnimateCheckBox, 1, 3);
    //
    // Create viewer:
    //
    SoQtExaminerViewer *viewer = new SoQtExaminerViewer();
    viewer->setSceneGraph(root);
    viewer->setCamera(c);
    viewer->setTransparencyType(SoGLRenderAction::BLEND);
    viewer->setDecoration(FALSE);
    viewer->setPopupMenuEnabled(FALSE);
    viewer->setBorder(TRUE);
    viewer->setFeedbackVisibility(FALSE);
    viewer->setAutoClipping(FALSE);
    // Set the draw style to as is, otherwise we'll never see
    // any texture mapping on systems without graphics hardware
    viewer->setDrawStyle(SoQtViewer::STILL, SoQtViewer::VIEW_AS_IS);

    vlayout->addWidget(viewer->getWidget());
    vlayout->addLayout(grid);

    // Add Inventor logo to viewer
    setOverlayLogo( root );
}

int
main(int argc, char **argv)
{
    QApplication app(argc, argv);

    SoQt::init("qmorf");

    parse_args(argc, argv);

    TheShape = new QuadThing(objectList[0]);
    TheShape->interp(objectList[0], objectList[1], 0.0);

    SoSeparator *root = new SoSeparator;
    root->ref();

    // Add a shapeHints node
    SoShapeHints *hints = new SoShapeHints;
    hints->vertexOrdering = SoShapeHints::COUNTERCLOCKWISE;
    hints->faceType = SoShapeHints::CONVEX;
    root->insertChild(hints, 0);

    SoPerspectiveCamera *camera = new SoPerspectiveCamera;
    root->addChild(camera);

    // And set up a no-op texture node.  The GL doesn't guarantee
    // pixel exactness if textured and non-textured polygons are
    // blended together, so we'll make sure all shapes are textured by
    // default (with a 1 by 1 all-white modulated texture, which won't
    // affect the shape's color at all!).
    noopTexture = new SoTexture2;
    noopTexture->ref();
    unsigned char whitePix[1]; whitePix[0] = 0xff;
    noopTexture->image.setValue(SbVec2s(1,1), 1, whitePix);
    // And an aribrary function for coords:
    noopTextureCoord = new SoTextureCoordinatePlane;
    noopTextureCoord->ref();

    ToStuff = new SoSeparator;
    root->addChild(ToStuff);
    FromStuff = new SoSeparator;
    root->addChild(FromStuff);
    placeHolder = new SoGroup;
    placeHolder->ref();
    ToTexSwitch = new SoSwitch;
    if (notextures)
	ToTexSwitch->whichChild = 1;
    else
	ToTexSwitch->whichChild = 0;
    ToStuff->addChild(ToTexSwitch);
    ToTexGroup = new SoGroup;
    ToTexSwitch->addChild(ToTexGroup);
    ToTexGroup->addChild(noopTexture);
    ToTexGroup->addChild(noopTextureCoord);
    ToTexGroup->addChild(placeHolder);  // Room for texCoordBinding
    ToMatGroup = new SoGroup;
    ToTexSwitch->addChild(ToMatGroup);
    ToMatGroup->addChild(placeHolder);  // Room for material
    ToMatGroup->addChild(placeHolder);  // Room for MaterialBinding
    
    ToStuff->addChild(TheShape->getSceneGraph());

    FromTexSwitch = new SoSwitch;
    FromStuff->addChild(FromTexSwitch);
    FromTexGroup = new SoGroup;
    FromTexSwitch->addChild(FromTexGroup);
    if (notextures)
	FromTexSwitch->whichChild = 1;
    else
	FromTexSwitch->whichChild = 0;
    FromTexGroup->addChild(noopTexture);
    FromTexGroup->addChild(noopTextureCoord);
    FromTexGroup->addChild(placeHolder); // Room for texCoordBinding
    FromMatGroup = new SoGroup;
    FromTexSwitch->addChild(FromMatGroup);
    FromMatGroup->addChild(placeHolder); // Room for material
    FromMatGroup->addChild(placeHolder); // Room for materialBinding

    TransparentMaterial = new SoMaterial;
    TransparentMaterial->diffuseColor.setIgnored(TRUE);
    TransparentMaterial->ambientColor.setIgnored(TRUE);
    TransparentMaterial->specularColor.setIgnored(TRUE);
    TransparentMaterial->emissiveColor.setIgnored(TRUE);
    TransparentMaterial->shininess.setIgnored(TRUE);
    TransparentMaterial->transparency.setValue(0.0);
    FromStuff->addChild(TransparentMaterial);
    FromStuff->addChild(TheShape->getSceneGraph());

    setToFromStuff(0);

    camera->viewAll(root, SbVec2s(1,1));
    camera->nearDistance = 0.1;
    camera->farDistance = 500.0;

    if (!nobackground)
    {
	background = new FlashBackground(20);
	root->addChild(background->getSceneGraph());
    }

    QMorfWidget widget(root, camera);

    animationSensor = new SoIdleSensor(morfCallback, &widget);
    animationSensor->schedule();
    // The animation sensor is scheduled the first time the scene is
    // rendered.
    widget.resize(600, 600);
    widget.show();

    return SoQt::mainLoop();
}

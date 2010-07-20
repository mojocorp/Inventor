#include <Inventor/SoCompat.h>
#include <Inventor/SoDB.h>
#include <Inventor/nodekits/SoNodeKit.h>
#include <Inventor/SoInteraction.h>

#include <Inventor/actions/SoDistributeAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/actions/SoNurbsBREPAction.h>
#include <Inventor/actions/SoToVRMLAction.h>
#include <Inventor/actions/SoToVRML2Action.h>

#include <Inventor/elements/SoModifyViewVolumeElement.h>
#include <Inventor/elements/SoLogicalViewportElement.h>
#include <Inventor/elements/SoProjectionElement.h>

#include <Inventor/nodes/SoBackground.h>
#include <Inventor/nodes/SoGradientBackground.h>
#include <Inventor/nodes/SoImageBackground.h>
#include <Inventor/nodes/SoDepthBuffer.h>
#include <Inventor/nodes/SoProjection.h>
#include <Inventor/nodes/SoTextureUnit.h>

SbBool SoCompat::initialized = FALSE;

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Initialize all interaction class.
//
// Use: public

void
SoCompat::init()
//
////////////////////////////////////////////////////////////////////////
{
    // Only initialize once
    if (! initialized) {

    SoDB::init(); // no problem if called multiple times
    SoNodeKit::init(); // no problem if called multiple times
    SoInteraction::init(); // no problem if called multiple times

    // Actions
    SoDistributeAction::initClass();
    SoGetPrimitiveCountAction::initClass();
    SoNurbsBREPAction::initClass();
    SoToVRMLAction::initClass();
    SoToVRML2Action::initClass();

    // Elements
    SoModifyViewVolumeElement::initClass();
    SoLogicalViewportElement::initClass();
    SoProjectionElement::initClass();

    // Nodes
    SoBackground::initClass();
    SoGradientBackground::initClass();
    SoImageBackground::initClass();
    SoProjection::initClass();
    SoTextureUnit::initClass();

    initialized = TRUE;
    }
}

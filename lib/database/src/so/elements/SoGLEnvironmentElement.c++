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

/*
 * Copyright (C) 1990,91   Silicon Graphics, Inc.
 *
 _______________________________________________________________________
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 |
 |   $Revision: 1.3 $
 |
 |   Classes:
 |	SoGLEnvironmentElement
 |
 |   Author(s)		: Morgan Leborgne
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */
#include <Inventor/elements/SoGLEnvironmentElement.h>
#include <Inventor/elements/SoViewVolumeElement.h>
#include <glad/gl.h>
#include <Inventor/SbVec.h>

SO_ELEMENT_SOURCE(SoGLEnvironmentElement);

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Destructor.
//
// Use: private

SoGLEnvironmentElement::~SoGLEnvironmentElement()
//
////////////////////////////////////////////////////////////////////////
{}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Initializes SoGLLinePatternElement class.
//
// Use: internal

void
SoGLEnvironmentElement::initClass()
//
////////////////////////////////////////////////////////////////////////
{
    SO_ELEMENT_INIT_CLASS(SoGLEnvironmentElement, SoEnvironmentElement);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Initializes element.
//
// Use: public

void
SoGLEnvironmentElement::init(SoState *state)
//
////////////////////////////////////////////////////////////////////////
{
    // Initialize base class stuff
    SoEnvironmentElement::init(state);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Overrides push() method to copy polygon offset
//
// Use: public

void
SoGLEnvironmentElement::push(SoState *)
//
////////////////////////////////////////////////////////////////////////
{
    const SoGLEnvironmentElement *prevElt =
        (const SoGLEnvironmentElement *)getNextInStack();

    ambientIntensity = prevElt->ambientIntensity;
    ambientColor = prevElt->ambientColor;
    attenuation = prevElt->attenuation;
    fogType = prevElt->fogType;
    fogColor = prevElt->fogColor;
    fogVisibility = prevElt->fogVisibility;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Pops element, causing side effects in GL.
//
// Use: public

void
SoGLEnvironmentElement::pop(SoState *state, const SoElement *childElt)
//
////////////////////////////////////////////////////////////////////////
{
    // Since popping this element has GL side effects, make sure any
    // open caches capture it.  We may not send any GL commands, but
    // the cache dependency must exist even if we don't send any GL
    // commands, because if the element changes, the _lack_ of GL
    // commands here is a bug (remember, GL commands issued here are
    // put inside the cache).
    capture(state);

    // If the previous element didn't have the same value...
    const SoGLEnvironmentElement *child =
        (const SoGLEnvironmentElement *)childElt;
    if (ambientIntensity != child->ambientIntensity ||
        ambientColor != child->ambientColor ||
        attenuation != child->attenuation || fogType != child->fogType ||
        fogColor != child->fogColor || fogVisibility != child->fogVisibility)
        send();
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Sets environment in element.
//
// Use: protected, virtual

void
SoGLEnvironmentElement::setElt(float          _ambientIntensity,
                               const SbColor &_ambientColor,
                               const SbVec3f &_attenuation,
                               const int32_t _fogType, const SbColor &_fogColor,
                               float _fogVisibility)
//
////////////////////////////////////////////////////////////////////////
{
    SoEnvironmentElement::setElt(_ambientIntensity, _ambientColor, _attenuation,
                                 _fogType, _fogColor, _fogVisibility);
    send();
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Sends environment in element to GL.
//
// Use: private

void
SoGLEnvironmentElement::send()
//
////////////////////////////////////////////////////////////////////////
{
    //////////////////////
    //
    // Set up ambient lighting.
    //

    // RGBA ambient intensity is the product of the color and
    // intensity, with 1.0 alpha
    SbVec3f v3 = ambientIntensity * ambientColor;
    SbVec4f v4(v3[0], v3[1], v3[2], 1.0);

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, v4.getValue());

    //////////////////////
    //
    // Set up fog.
    //
    FogType type = (FogType)fogType;

    if (type == NONE) {
        glDisable(GL_FOG);
    } else {
        glEnable(GL_FOG);
        glFogfv(GL_FOG_COLOR, fogColor.getValue());

        switch (type) {

        case NONE:
            // Can't get here!
            break;

        case HAZE:
            // Set up linear ramp based on visibility
            glFogf(GL_FOG_MODE, GL_LINEAR);
            glFogf(GL_FOG_START, 0.0);
            glFogf(GL_FOG_END, fogVisibility);
            break;

        case FOG:
            glEnable(GL_FOG);
            glFogf(GL_FOG_MODE, GL_EXP);
            glFogf(GL_FOG_DENSITY, computeDensity(fogVisibility, FALSE));
            break;

        case SMOKE:
            glFogf(GL_FOG_MODE, GL_EXP2);
            glFogf(GL_FOG_DENSITY, computeDensity(fogVisibility, TRUE));
            break;
        }
    }
}

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

//  -*- C++ -*-

/*
 * Copyright (C) 1990,91   Silicon Graphics, Inc.
 *
 _______________________________________________________________________
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 |
 |   $Revision: 1.1 $
 |
 |   Description:
 |	This file defines the SoEnvironmentElement class.
 |
 |   Author(s)		: Morgan Leborgne
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */
#ifndef _SO_ENVIRONMENT_ELEMENT_
#define _SO_ENVIRONMENT_ELEMENT_

#include <Inventor/elements/SoReplacedElement.h>
#include <Inventor/SbColor.h>
#include <Inventor/SbVec.h>

//////////////////////////////////////////////////////////////////////////////
//
//  Class: SoEnvironmentElement
//
//  Element that stores the current environment.
//
//  This class allows read-only access to the top element in the state
//  to make accessing several values in it more efficient. Individual
//  values must be accessed through this instance.
//
//////////////////////////////////////////////////////////////////////////////

SoEXTENDER
class SoEnvironmentElement : public SoReplacedElement {
    SO_ELEMENT_HEADER(SoEnvironmentElement);

  public:
    /// Type of fog:
    enum FogType { NONE, HAZE, FOG, SMOKE };

    /// Initializes element
    virtual void init(SoState *state);

    /// Sets the current environment
    static void set(SoState *state, SoNode *node, float ambientIntensity,
                    const SbColor &ambientColor, const SbVec3f &attenuation,
                    int32_t fogType, const SbColor &fogColor,
                    float fogVisibility);

    /// Returns the environment from an element
    static void get(SoState *state, float &ambientIntensity,
                    SbColor &ambientColor, SbVec3f &attenuation,
                    int32_t &fogType, SbColor &fogColor, float &fogVisibility);

    /// Returns the default environment.
    static void getDefault(float &ambientIntensity, SbColor &ambientColor,
                           SbVec3f &attenuation, int32_t &fogType,
                           SbColor &fogColor, float &fogVisibility);

  protected:
    float   ambientIntensity;
    SbColor ambientColor;
    SbVec3f attenuation;
    int32_t fogType;
    SbColor fogColor;
    float   fogVisibility;

    // Destructor.
    virtual ~SoEnvironmentElement();

    virtual void setElt(float ambientIntensity, const SbColor &ambientColor,
                        const SbVec3f &attenuation, const int32_t fogType,
                        const SbColor &fogColor, float fogVisibility);

    // Computes fog density based on visibility
    static float computeDensity(float visibility, SbBool squared);

    SoINTERNAL
  public:
    // Initializes the SoEnvironmentElement class.
    static void initClass();
};

#endif // _SO_ENVIRONMENT_ELEMENT_

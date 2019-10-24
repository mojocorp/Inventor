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
 * Copyright (C) 1990,91,92   Silicon Graphics, Inc.
 *
 _______________________________________________________________________
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 |
 |   $Revision: 1.1 $
 |
 |   Class:
 |	Initializes all element classes
 |
 |   Author(s)		: Ronen Barzel, Paul S. Strauss
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#include <Inventor/elements/SoElements.h>

////////////////////////////////////////////////////////////////////////
//
// Description:
//    This initializes ALL Inventor element classes.
//
// Use: internal

void
SoElement::initElements()
//
////////////////////////////////////////////////////////////////////////
{
    // Initialize base classes first
    SoElement::initClass();
    SoAccumulatedElement::initClass();
    SoReplacedElement::initClass();
    SoInt32Element::initClass();
    SoFloatElement::initClass();

    // Initialize derived classes
    SoCacheElement::initClass();
    SoClipPlaneElement::initClass();
    SoComplexityElement::initClass();
    SoComplexityTypeElement::initClass();
    SoCoordinateElement::initClass();
    SoCreaseAngleElement::initClass();
    SoDepthBufferElement::initClass();
    SoDrawStyleElement::initClass();
    SoFocalDistanceElement::initClass();
    SoFontNameElement::initClass();
    SoFontSizeElement::initClass();
    // We must put this before Lazy Element:
    SoShapeStyleElement::initClass();
    SoLazyElement::initClass();
    SoLightAttenuationElement::initClass();
    SoLinePatternElement::initClass();
    SoLineWidthElement::initClass();
    SoMaterialBindingElement::initClass();
    SoModelMatrixElement::initClass();
    SoNormalBindingElement::initClass();
    SoNormalElement::initClass();
    SoOverrideElement::initClass();
    SoPickRayElement::initClass();
    SoPickStyleElement::initClass();
    SoPointSizeElement::initClass();
    SoProfileCoordinateElement::initClass();
    SoProfileElement::initClass();
    SoProjectionMatrixElement::initClass();
    SoShapeHintsElement::initClass();
    SoSwitchElement::initClass();
    SoTextureCoordinateBindingElement::initClass();
    SoTextureCoordinateElement::initClass();
    SoTextureImageElement::initClass();
    SoTextureMatrixElement::initClass();
    SoTextureQualityElement::initClass();
    SoTextureOverrideElement::initClass();
    SoUnitsElement::initClass();
    SoViewVolumeElement::initClass();
    SoViewingMatrixElement::initClass();
    SoViewportRegionElement::initClass();

    // GL specific elements must be initialized after their more
    // generic counterparts

    SoGLCacheContextElement::initClass();
    SoGLClipPlaneElement::initClass();
    SoGLCoordinateElement::initClass();
    SoGLDepthBufferElement::initClass();
    SoGLDrawStyleElement::initClass();
    SoGLLazyElement::initClass();
    SoGLLightIdElement::initClass();
    SoGLLinePatternElement::initClass();
    SoGLLineWidthElement::initClass();
    SoGLModelMatrixElement::initClass();
    SoGLNormalElement::initClass();
    SoGLPointSizeElement::initClass();
    SoGLProjectionMatrixElement::initClass();
    SoGLRenderPassElement::initClass();
    SoGLShapeHintsElement::initClass();
    SoGLTextureCoordinateElement::initClass();
    SoGLTextureEnabledElement::initClass();
    SoGLTextureImageElement::initClass();
    SoGLTextureMatrixElement::initClass();
    SoGLUpdateAreaElement::initClass();
    SoGLViewingMatrixElement::initClass();
    SoGLViewportRegionElement::initClass();

    // Other derived classes 
    SoBBoxModelMatrixElement::initClass();
    SoLocalBBoxMatrixElement::initClass();
}

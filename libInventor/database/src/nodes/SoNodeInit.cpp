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
 |	call initClasses for all node classes
 |
 |   Author(s)		: Ronen Barzel, Gavin Bell
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#include <Inventor/nodes/SoNodes.h>
#include <SoUnknownNode.h>

////////////////////////////////////////////////////////////////////////
//
// Description:
//    This initializes ALL Inventor node classes.
//
// Use: internal

void
SoNode::initClasses()
//
////////////////////////////////////////////////////////////////////////
{
    // Base class must be initialized first
    SoNode::initClass();

    // Parent classes must always be initialized before their subclasses
    SoCamera::initClass();
    SoGroup::initClass();
    SoLight::initClass();
    SoProfile::initClass();
    SoSeparator::initClass();
    SoShape::initClass();
    SoTransformation::initClass();
    SoVertexShape::initClass();
    SoIndexedShape::initClass();
    SoNonIndexedShape::initClass();

    SoAnnotation::initClass();
    SoArray::initClass();
    SoAsciiText::initClass();
    SoBaseColor::initClass();
    SoCallback::initClass();
    SoClipPlane::initClass();
    SoColorIndex::initClass();
    SoComplexity::initClass();
    SoCone::initClass();
    SoCoordinate3::initClass();
    SoCoordinate4::initClass();
    SoCube::initClass();
    SoCylinder::initClass();
    SoDepthBuffer::initClass();
    SoDirectionalLight::initClass();
    SoDrawStyle::initClass();
    SoEnvironment::initClass();
    SoEventCallback::initClass();
    SoFaceSet::initClass();
    SoFile::initClass();
    SoFont::initClass();
    SoFontStyle::initClass();
    SoIndexedFaceSet::initClass();
    SoIndexedLineSet::initClass();
    SoIndexedPointSet::initClass();
    SoIndexedNurbsCurve::initClass();
    SoIndexedNurbsSurface::initClass();
    SoIndexedTriangleStripSet::initClass();
    SoInfo::initClass();
    SoLabel::initClass();
    SoLevelOfDetail::initClass();
    SoLOD::initClass();
    SoLocateHighlight::initClass();
    SoLightModel::initClass();
    SoLineSet::initClass();
    SoLinearProfile::initClass();
    SoMaterial::initClass();
    SoMaterialBinding::initClass();
    SoMatrixTransform::initClass();
    SoMultipleCopy::initClass();
    SoNormal::initClass();
    SoNormalBinding::initClass();
    SoNurbsCurve::initClass();
    SoNurbsProfile::initClass();
    SoNurbsSurface::initClass();
    SoOrthographicCamera::initClass();
    SoPackedColor::initClass();
    SoPathSwitch::initClass();
    SoPerspectiveCamera::initClass();
    SoPickStyle::initClass();
    SoPointLight::initClass();
    SoPointSet::initClass();
    SoPolygonOffset::initClass();
    SoProfileCoordinate2::initClass();
    SoProfileCoordinate3::initClass();
    SoQuadMesh::initClass();
    SoResetTransform::initClass();
    SoRotation::initClass();
    SoRotationXYZ::initClass();
    SoScale::initClass();
    SoShapeHints::initClass();
    SoSphere::initClass();
    SoSpotLight::initClass();
    SoSwitch::initClass();
    SoText2::initClass();
    SoText3::initClass();
    SoTextureCoordinate2::initClass();
    SoTextureCoordinateBinding::initClass();
    SoTextureCoordinateFunction::initClass();
    SoTextureCoordinateDefault::initClass();
    SoTextureCoordinateEnvironment::initClass();
    SoTextureCoordinatePlane::initClass();
    SoTexture2::initClass();
    SoTexture2Transform::initClass();
    SoTransform::initClass();
    SoTransformSeparator::initClass();
    SoTranslation::initClass();
    SoTransparencyType::initClass();
    SoTriangleStripSet::initClass();
    SoUnits::initClass();
    SoUnknownNode::initClass();
    SoVertexProperty::initClass();
    SoWWWAnchor::initClass();
    SoWWWInline::initClass();

    // Four self-animating nodes. These are subclassed from 
    // SoSwitch, SoRotation, and SoTranslation
    SoBlinker::initClass();
    SoPendulum::initClass();
    SoRotor::initClass();
    SoShuttle::initClass();
}

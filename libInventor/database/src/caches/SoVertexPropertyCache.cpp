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
 |   $Revision: 1.1 $
 |
 |   Classes:
 |	SoVertexPropertyCache
 |
 |   Author(s)		: Paul S. Strauss
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#include <Inventor/caches/SoVertexPropertyCache.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/elements/SoGLLazyElement.h>
#include <Inventor/elements/SoGLCoordinateElement.h>
#include <Inventor/elements/SoOverrideElement.h>
#include <Inventor/elements/SoGLNormalElement.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>
#include <Inventor/misc/SoGL.h>

void vp_glColor4ubv(const GLubyte *v)
{
    GLubyte _v[4];
    DGL_HTON_INT32(*((int32_t*)_v), *((int32_t*)v));

    glColor4ubv(_v);
}

void
SoVertexPropertyCache::fillInCache(const SoVertexProperty *vp,
                 SoState *state)
{
    renderCase = 0;

    // Coordinates:
    if (vp && (numVerts = vp->vertex.getNum()) != 0) {
        vertexFunc = (SoVPCacheFunc *)glVertex3fv;
        vertexStride = sizeof(SbVec3f);
        vertexPtr = (const char *)vp->vertex.getValues(0);
        needFromState &= ~COORD_FROM_STATE_BIT;
    } else {
        const SoCoordinateElement *ce =
            SoCoordinateElement::getInstance(state);
        numVerts = ce->getNum();
        if (ce->is3D()) {
            vertexFunc = (SoVPCacheFunc *)glVertex3fv;
            vertexStride = sizeof(SbVec3f);
            vertexPtr = (const char *) &ce->get3(0);
        } else {
            vertexFunc = (SoVPCacheFunc *)glVertex4fv;
            vertexStride = sizeof(SbVec4f);
            vertexPtr = (const char *) &ce->get4(0);
        }
        needFromState |= COORD_FROM_STATE_BIT;
    }

    // Diffuse colors:
    transpIsInVP = FALSE;
    SbBool colorOverride = ((SoOverrideElement::getDiffuseColorOverride(state))||
                    (SoOverrideElement::getTransparencyOverride(state)));
    if (SoGLLazyElement::isColorIndex(state)) {
        const SoLazyElement *le = SoLazyElement::getInstance(state);
        numColors = le->getNumDiffuse();
        colorFunc = (SoVPCacheFunc *)glIndexiv;
        colorStride = sizeof(int32_t);
        colorPtr = (const char *)le->getColorIndexPointer();
        needFromState |= COLOR_FROM_STATE_BITS;
    } else {
        if ((!colorOverride) &&
            (vp && (numColors = vp->orderedRGBA.getNum()) != 0)) {
            colorPtr = (const char *)vp->orderedRGBA.getValues(0);
            colorFunc = (SoVPCacheFunc *)vp_glColor4ubv;
            colorStride = sizeof(uint32_t);
            needFromState &= ~COLOR_FROM_STATE_BITS;
            colorIsInVP = TRUE;
            //Check to see if there really is a non-opaque transparency:
            for(int i = 0; i< vp->orderedRGBA.getNum(); i++){
                if ((vp->orderedRGBA[i] & 0xff) != 0xff){
                    transpIsInVP = TRUE;
                    break;
                }
            }
        } else {  //Get color from state
            const SoLazyElement *le = SoLazyElement::getInstance(state);
            colorIsInVP = FALSE;
            numColors = le->getNumDiffuse();
            needFromState |= COLOR_FROM_STATE_BITS;
            colorPtr = (const char *) le->getPackedPointer();
            colorFunc = (SoVPCacheFunc *)vp_glColor4ubv;
            colorStride = sizeof(uint32_t);
        }

    }

    //setup materialBinding
    if (needFromState & COLOR_FROM_STATE_BITS ||
            SoOverrideElement::getMaterialBindingOverride(state)) {
        materialBinding = SoMaterialBindingElement::get(state);
    }
    else {
        materialBinding = (SoMaterialBindingElement::Binding)
            vp->materialBinding.getValue();
    }

    switch (materialBinding) {
      case SoMaterialBindingElement::OVERALL:
        // 00... bits, nothing to do.  But if color is overridden, still
        // needFromState should remain on, to guarantee that we will
        // continue to fill in the cache.
        if(!colorOverride) needFromState &= ~COLOR_FROM_STATE_BITS;
        numColors = 1;
        break;
      case SoMaterialBindingElement::PER_PART:
        renderCase |= PART_COLOR;
        break;
      case SoMaterialBindingElement::PER_PART_INDEXED:
        renderCase |= PART_COLOR;
        break;
      case SoMaterialBindingElement::PER_FACE:
        renderCase |= FACE_COLOR;
        break;
      case SoMaterialBindingElement::PER_FACE_INDEXED:
        renderCase |= FACE_COLOR;
        break;
      case SoMaterialBindingElement::PER_VERTEX:
        renderCase |= VERTEX_COLOR;
        break;
      case SoMaterialBindingElement::PER_VERTEX_INDEXED:
        renderCase |= VERTEX_COLOR;
        break;
    }

    // Normals:
    // Setup as if normals are needed, even if they aren't

    normalFunc = (SoVPCacheFunc *)glNormal3fv;
    normalStride = sizeof(SbVec3f);

    if (vp && (numNorms = vp->normal.getNum()) != 0) {
        normalPtr = (const char *)vp->normal.getValues(0);
        needFromState &= ~NORMAL_FROM_STATE_BITS;
        generateNormals = FALSE;
    } else {
        needFromState |= NORMAL_FROM_STATE_BITS;

        const SoNormalElement *ne =
            SoNormalElement::getInstance(state);
        if ((numNorms = ne->getNum()) > 0) {
            normalPtr = (const char *) &ne->get(0);
            generateNormals = FALSE;
        } else {
            generateNormals = TRUE;
        }
    }

    // set up normalBinding
    if (generateNormals) {
        normalBinding = SoNormalBindingElement::PER_VERTEX;
    }
    else if (needFromState & NORMAL_FROM_STATE_BITS) {
        normalBinding = SoNormalBindingElement::get(state);
    }
    else {
        normalBinding = (SoNormalBindingElement::Binding)
             vp->normalBinding.getValue();
    }

    switch (normalBinding) {
        case SoNormalBindingElement::OVERALL:
            // 00... bits, nothing to do
            break;
        case SoNormalBindingElement::PER_PART:
            renderCase |= PART_NORMAL;
            break;
        case SoNormalBindingElement::PER_PART_INDEXED:
            renderCase |= PART_NORMAL;
            break;
        case SoNormalBindingElement::PER_FACE:
            renderCase |= FACE_NORMAL;
            break;
        case SoNormalBindingElement::PER_FACE_INDEXED:
            renderCase |= FACE_NORMAL;
            break;
        case SoNormalBindingElement::PER_VERTEX:
            renderCase |= VERTEX_NORMAL;
            break;
        case SoNormalBindingElement::PER_VERTEX_INDEXED:
            renderCase |= VERTEX_NORMAL;
            break;
    }



    //Setup as if texture coords are needed, even if they are not:
    if (vp && (numTexCoords = vp->texCoord.getNum()) != 0) {
        texCoordFunc = (SoVPCacheFunc *)glTexCoord2fv;
        texCoordStride = sizeof(SbVec2f);
        texCoordPtr = (const char *)vp->texCoord.getValues(0);
        needFromState &= ~TEXCOORD_FROM_STATE_BIT;
        needFromState |= TEXTURE_FUNCTION_BIT;
        texCoordBinding =
                SoTextureCoordinateBindingElement::PER_VERTEX_INDEXED;
        renderCase |= TEXCOORD_BIT;
        generateTexCoords = FALSE;
    } else {
        const SoTextureCoordinateElement *tce =
                SoTextureCoordinateElement::getInstance(state);
        if ((numTexCoords = tce->getNum()) > 0) {
            if (tce->is2D()) {
                texCoordFunc = (SoVPCacheFunc *)glTexCoord2fv;
                texCoordStride = sizeof(SbVec2f);
                texCoordPtr = (const char *) &tce->get2(0);
            } else {
                texCoordFunc = (SoVPCacheFunc *)glTexCoord4fv;
                texCoordStride = sizeof(SbVec4f);
                texCoordPtr = (const char *) &tce->get4(0);
            }
                texCoordBinding =
                    SoTextureCoordinateBindingElement::get(state);
                renderCase |= TEXCOORD_BIT;
                generateTexCoords = FALSE;
            } else {
                texCoordFunc = NULL;
                generateTexCoords = TRUE;
                renderCase &= ~TEXCOORD_BIT;  // No normals, use glTexGen
            }
        needFromState |= TEXCOORD_FROM_STATE_BIT;
        needFromState &= ~TEXTURE_FUNCTION_BIT;
    }
}

SoVertexPropertyCache::SoVertexPropertyCache()
{
    vertexFunc = normalFunc = colorFunc = texCoordFunc = NULL;
    vertexPtr = normalPtr = colorPtr = texCoordPtr = NULL;
    vertexStride = normalStride = colorStride = texCoordStride = 0;
    numVerts = numNorms = numColors = numTexCoords = 0;
    colorIsInVP = transpIsInVP = FALSE;
    generateNormals = generateTexCoords = FALSE;
    needFromState = ALL_FROM_STATE_BITS;
    renderCase = 0;
}

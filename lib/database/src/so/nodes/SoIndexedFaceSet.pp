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
 |      SoIndexedFaceSet
 |
@@ This next line is true for the output file only - ignore it here:
 |   Note: This file was preprocessed from another file. Do not edit it.
 |
 |   Author(s)          : Paul S. Strauss, Gavin Bell
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#include <Inventor/misc/SoGL.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/bundles/SoNormalBundle.h>
#include <Inventor/bundles/SoTextureCoordinateBundle.h>
#include <Inventor/caches/SoNormalCache.h>
#include <Inventor/details/SoFaceDetail.h>
#include <Inventor/elements/SoGLCoordinateElement.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/elements/SoGLLazyElement.h>
#include <Inventor/elements/SoNormalBindingElement.h>
#include <Inventor/elements/SoShapeHintsElement.h>
#include <Inventor/elements/SoTextureCoordinateBindingElement.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoMaterialBinding.h>
#include <Inventor/nodes/SoNormalBinding.h>

//////////////////////////////////////////////////////////////////////////
// Following preprocessor-generated routines handle all combinations of
// Normal binding (per vertex, per face, per part, overall/none)
// Color Binding (per vertex, per face, per part, overall)
// Textures (on or off)
//////////////////////////////////////////////////////////////////////////

@@ ----------------------------------------------------------------------
@@ Triangle rendering:
@@
@={ TriRenderFunction
void
SoIndexedFaceSet::
@ FuncName 
    (SoGLRenderAction* ) {
    const int32_t *const vertexIndex = coordIndex.getValues(0);
@?{ On
    // Send one normal, if there are any normals in vpCache:
    if (vpCache.getNumNormals() > 0)
	vpCache.sendNormal(vpCache.getNormals(0));
@?}
    const char *vertexPtr = vpCache.getVertices(0);
    const unsigned int vertexStride = vpCache.getVertexStride();
    SoVPCacheFunc *const vertexFunc = vpCache.vertexFunc;
@?{ Fm | Vm
    const char *colorPtr = vpCache.getColors(0);
    const unsigned int colorStride = vpCache.getColorStride();
    SoVPCacheFunc *const colorFunc = vpCache.colorFunc;
    const int32_t *const colorIndx = getColorIndices();
@?}
@?{ Fn | Vn
    const char *normalPtr = vpCache.getNormals(0);
    const unsigned int normalStride = vpCache.getNormalStride();
    SoVPCacheFunc *const normalFunc = vpCache.normalFunc;
    const int32_t *const normalIndx = getNormalIndices();
@?}
@?{ T
    const char *texCoordPtr = vpCache.getTexCoords(0);
    const unsigned int texCoordStride = vpCache.getTexCoordStride();
    SoVPCacheFunc *const texCoordFunc = vpCache.texCoordFunc;
    const int32_t *const tCoordIndx = getTexCoordIndices();
@?}

    glBegin(GL_TRIANGLES);
    int vtxCtr = 0;
    for (int tri = 0; tri < numTris; tri++) {
@?{ Fm
	(*colorFunc)(colorPtr+colorStride*colorIndx[tri]);
@?}
@?{ Fn
	(*normalFunc)(normalPtr+normalStride*normalIndx[tri]);
@?}

@?{ Vm
	(*colorFunc)(colorPtr+colorStride*colorIndx[vtxCtr]);
@?}
@?{ Vn
	(*normalFunc)(normalPtr+normalStride*normalIndx[vtxCtr]);
@?}
@?{ T
	(*texCoordFunc)(texCoordPtr+texCoordStride*tCoordIndx[vtxCtr]);
@?}
	(*vertexFunc)(vertexPtr+vertexStride*vertexIndex[vtxCtr]);

@?{ Vm
	(*colorFunc)(colorPtr+colorStride*colorIndx[vtxCtr+1]);
@?}
@?{ Vn
	(*normalFunc)(normalPtr+normalStride*normalIndx[vtxCtr+1]);
@?}
@?{ T
	(*texCoordFunc)(texCoordPtr+texCoordStride*tCoordIndx[vtxCtr+1]);
@?}
	(*vertexFunc)(vertexPtr+vertexStride*vertexIndex[vtxCtr+1]);

@?{ Vm
	(*colorFunc)(colorPtr+colorStride*colorIndx[vtxCtr+2]);
@?}
@?{ Vn
	(*normalFunc)(normalPtr+normalStride*normalIndx[vtxCtr+2]);
@?}
@?{ T
	(*texCoordFunc)(texCoordPtr+texCoordStride*tCoordIndx[vtxCtr+2]);
@?}
	(*vertexFunc)(vertexPtr+vertexStride*vertexIndex[vtxCtr+2]);
	vtxCtr += 4; // Skip past END_OF_FACE_INDEX
    }
    glEnd();
}

@=}

@@ ----------------------------------------------------------------------
@@ Quad rendering:
@@
@={ QuadRenderFunction
void
SoIndexedFaceSet::
@ FuncName 
    (SoGLRenderAction* ) {
    const int32_t *const vertexIndex = coordIndex.getValues(0);
@?{ On
    // Send one normal, if there are any normals in vpCache:
    if (vpCache.getNumNormals() > 0)
	vpCache.sendNormal(vpCache.getNormals(0));
@?}
    const char *vertexPtr = vpCache.getVertices(0);
    const unsigned int vertexStride = vpCache.getVertexStride();
    SoVPCacheFunc *const vertexFunc = vpCache.vertexFunc;
@?{ Fm | Vm
    const char *colorPtr = vpCache.getColors(0);
    const unsigned int colorStride = vpCache.getColorStride();
    SoVPCacheFunc *const colorFunc = vpCache.colorFunc;
    const int32_t *const colorIndx = getColorIndices();
@?}
@?{ Fn | Vn
    const char *normalPtr = vpCache.getNormals(0);
    const unsigned int normalStride = vpCache.getNormalStride();
    SoVPCacheFunc *const normalFunc = vpCache.normalFunc;
    const int32_t *const normalIndx = getNormalIndices();
@?}
@?{ T
    const char *texCoordPtr = vpCache.getTexCoords(0);
    const unsigned int texCoordStride = vpCache.getTexCoordStride();
    SoVPCacheFunc *const texCoordFunc = vpCache.texCoordFunc;
    const int32_t *const tCoordIndx = getTexCoordIndices();
@?}

    glBegin(GL_QUADS);
    int vtxCtr = numTris*4;
@?{ Fn | Fm
    int faceCtr = numTris;
@?}	
    for (int quad = 0; quad < numQuads; quad++) {
@?{ Fm
	(*colorFunc)(colorPtr+colorStride*colorIndx[faceCtr]);
@?}
@?{ Fn
	(*normalFunc)(normalPtr+normalStride*normalIndx[faceCtr]);
@?}
@?{ Fn | Fm
	++faceCtr;
@?}	

@?{ Vm
	(*colorFunc)(colorPtr+colorStride*colorIndx[vtxCtr]);
@?}
@?{ Vn
	(*normalFunc)(normalPtr+normalStride*normalIndx[vtxCtr]);
@?}
@?{ T
	(*texCoordFunc)(texCoordPtr+texCoordStride*tCoordIndx[vtxCtr]);
@?}
	(*vertexFunc)(vertexPtr+vertexStride*vertexIndex[vtxCtr]);

@?{ Vm
	(*colorFunc)(colorPtr+colorStride*colorIndx[vtxCtr+1]);
@?}
@?{ Vn
	(*normalFunc)(normalPtr+normalStride*normalIndx[vtxCtr+1]);
@?}
@?{ T
	(*texCoordFunc)(texCoordPtr+texCoordStride*tCoordIndx[vtxCtr+1]);
@?}
	(*vertexFunc)(vertexPtr+vertexStride*vertexIndex[vtxCtr+1]);

@?{ Vm
	(*colorFunc)(colorPtr+colorStride*colorIndx[vtxCtr+2]);
@?}
@?{ Vn
	(*normalFunc)(normalPtr+normalStride*normalIndx[vtxCtr+2]);
@?}
@?{ T
	(*texCoordFunc)(texCoordPtr+texCoordStride*tCoordIndx[vtxCtr+2]);
@?}
	(*vertexFunc)(vertexPtr+vertexStride*vertexIndex[vtxCtr+2]);

@?{ Vm
	(*colorFunc)(colorPtr+colorStride*colorIndx[vtxCtr+3]);
@?}
@?{ Vn
	(*normalFunc)(normalPtr+normalStride*normalIndx[vtxCtr+3]);
@?}
@?{ T
	(*texCoordFunc)(texCoordPtr+texCoordStride*tCoordIndx[vtxCtr+3]);
@?}
	(*vertexFunc)(vertexPtr+vertexStride*vertexIndex[vtxCtr+3]);
	vtxCtr += 5; // Skip past END_OF_FACE_INDEX
    }
    glEnd();
}

@=}

@@ ----------------------------------------------------------------------
@@ General polygon rendering:
@@
@={ GenRenderFunction
void
SoIndexedFaceSet::
@ FuncName 
    (SoGLRenderAction* )
{
    const int32_t *const vertexIndex = coordIndex.getValues(0);
    const int numVI = coordIndex.getNum();
@?{ On
    // Send one normal, if there are any normals in vpCache:
    if (vpCache.getNumNormals() > 0)
	vpCache.sendNormal(vpCache.getNormals(0));
@?}
    const char *vertexPtr = vpCache.getVertices(0);
    const unsigned int vertexStride = vpCache.getVertexStride();
    SoVPCacheFunc *const vertexFunc = vpCache.vertexFunc;
@?{ Fm | Vm
    const char *colorPtr = vpCache.getColors(0);
    const unsigned int colorStride = vpCache.getColorStride();
    SoVPCacheFunc *const colorFunc = vpCache.colorFunc;
    const int32_t *const colorIndx = getColorIndices();
@?}
@?{ Fn | Vn
    const char *normalPtr = vpCache.getNormals(0);
    const unsigned int normalStride = vpCache.getNormalStride();
    SoVPCacheFunc *const normalFunc = vpCache.normalFunc;
    const int32_t *const normalIndx = getNormalIndices();
@?}
@?{ T
    const char *texCoordPtr = vpCache.getTexCoords(0);
    const unsigned int texCoordStride = vpCache.getTexCoordStride();
    SoVPCacheFunc *const texCoordFunc = vpCache.texCoordFunc;
    const int32_t *const tCoordIndx = getTexCoordIndices();
@?}
    int vtxCtr = numQuads*5 + numTris*4;
@?{ Fn | Fm
    int faceCtr = numQuads + numTris;
@?}
    while (vtxCtr < numVI) {
@?{ Fm
	(*colorFunc)(colorPtr+colorStride*colorIndx[faceCtr]);
@?}
@?{ Fn
	(*normalFunc)(normalPtr+normalStride*normalIndx[faceCtr]);
@?}
@?{ Fn | Fm
	++faceCtr;
@?}	
	glBegin(GL_POLYGON);
	while (vtxCtr < numVI &&
	       (vertexIndex[vtxCtr] != SO_END_FACE_INDEX)) {
@?{ Vm
	    (*colorFunc)(colorPtr+colorStride*colorIndx[vtxCtr]);
@?}
@?{ Vn
	    (*normalFunc)(normalPtr+normalStride*normalIndx[vtxCtr]);
@?}
@?{ T
	    (*texCoordFunc)(texCoordPtr+texCoordStride*tCoordIndx[vtxCtr]);
@?}
	    (*vertexFunc)(vertexPtr+vertexStride*vertexIndex[vtxCtr]);
	    vtxCtr++;
	}
	vtxCtr++; // Skip over END_FACE_INDEX
	glEnd();
    }
}

@=}

// 32 different rendering loops; the 5 bits used to determine the
// rendering case are:
// 43210  BITS            Routine suffix
// -----  ----            --------------
// 00...  Overall mtl     (Om)
// 01...  Part mtl        (Pm)  NOT GENERATED, Fm is same!
// 10...  Face mtl        (Fm)
// 11...  Vtx mtl         (Vm)
// ..00.  Overall/No norm (On)
// ..01.  Part norm       (Pn)  NOT GENERATED, Fn is same!
// ..10.  Face norm       (Fn)
// ..11.  Vtx norm        (Vn)
// ....0  No texcoord     -none-
// ....1  Vtx texcoord    (T)
//
SoIndexedFaceSet::PMFS SoIndexedFaceSet::TriRenderFunc[32] = {
    &SoIndexedFaceSet::TriOmOn, &SoIndexedFaceSet::TriOmOnT,
    &SoIndexedFaceSet::TriOmFn, &SoIndexedFaceSet::TriOmFnT,
    &SoIndexedFaceSet::TriOmFn, &SoIndexedFaceSet::TriOmFnT,
    &SoIndexedFaceSet::TriOmVn, &SoIndexedFaceSet::TriOmVnT,
    &SoIndexedFaceSet::TriFmOn, &SoIndexedFaceSet::TriFmOnT,
    &SoIndexedFaceSet::TriFmFn, &SoIndexedFaceSet::TriFmFnT,
    &SoIndexedFaceSet::TriFmFn, &SoIndexedFaceSet::TriFmFnT,
    &SoIndexedFaceSet::TriFmVn, &SoIndexedFaceSet::TriFmVnT,
    &SoIndexedFaceSet::TriFmOn, &SoIndexedFaceSet::TriFmOnT,
    &SoIndexedFaceSet::TriFmFn, &SoIndexedFaceSet::TriFmFnT,
    &SoIndexedFaceSet::TriFmFn, &SoIndexedFaceSet::TriFmFnT,
    &SoIndexedFaceSet::TriFmVn, &SoIndexedFaceSet::TriFmVnT,
    &SoIndexedFaceSet::TriVmOn, &SoIndexedFaceSet::TriVmOnT,
    &SoIndexedFaceSet::TriVmFn, &SoIndexedFaceSet::TriVmFnT,
    &SoIndexedFaceSet::TriVmFn, &SoIndexedFaceSet::TriVmFnT,
    &SoIndexedFaceSet::TriVmVn, &SoIndexedFaceSet::TriVmVnT,
    };
SoIndexedFaceSet::PMFS SoIndexedFaceSet::QuadRenderFunc[32] = {
    &SoIndexedFaceSet::QuadOmOn, &SoIndexedFaceSet::QuadOmOnT,
    &SoIndexedFaceSet::QuadOmFn, &SoIndexedFaceSet::QuadOmFnT,
    &SoIndexedFaceSet::QuadOmFn, &SoIndexedFaceSet::QuadOmFnT,
    &SoIndexedFaceSet::QuadOmVn, &SoIndexedFaceSet::QuadOmVnT,
    &SoIndexedFaceSet::QuadFmOn, &SoIndexedFaceSet::QuadFmOnT,
    &SoIndexedFaceSet::QuadFmFn, &SoIndexedFaceSet::QuadFmFnT,
    &SoIndexedFaceSet::QuadFmFn, &SoIndexedFaceSet::QuadFmFnT,
    &SoIndexedFaceSet::QuadFmVn, &SoIndexedFaceSet::QuadFmVnT,
    &SoIndexedFaceSet::QuadFmOn, &SoIndexedFaceSet::QuadFmOnT,
    &SoIndexedFaceSet::QuadFmFn, &SoIndexedFaceSet::QuadFmFnT,
    &SoIndexedFaceSet::QuadFmFn, &SoIndexedFaceSet::QuadFmFnT,
    &SoIndexedFaceSet::QuadFmVn, &SoIndexedFaceSet::QuadFmVnT,
    &SoIndexedFaceSet::QuadVmOn, &SoIndexedFaceSet::QuadVmOnT,
    &SoIndexedFaceSet::QuadVmFn, &SoIndexedFaceSet::QuadVmFnT,
    &SoIndexedFaceSet::QuadVmFn, &SoIndexedFaceSet::QuadVmFnT,
    &SoIndexedFaceSet::QuadVmVn, &SoIndexedFaceSet::QuadVmVnT,
    };
SoIndexedFaceSet::PMFS SoIndexedFaceSet::GenRenderFunc[32] = {
    &SoIndexedFaceSet::GenOmOn, &SoIndexedFaceSet::GenOmOnT,
    &SoIndexedFaceSet::GenOmFn, &SoIndexedFaceSet::GenOmFnT,
    &SoIndexedFaceSet::GenOmFn, &SoIndexedFaceSet::GenOmFnT,
    &SoIndexedFaceSet::GenOmVn, &SoIndexedFaceSet::GenOmVnT,
    &SoIndexedFaceSet::GenFmOn, &SoIndexedFaceSet::GenFmOnT,
    &SoIndexedFaceSet::GenFmFn, &SoIndexedFaceSet::GenFmFnT,
    &SoIndexedFaceSet::GenFmFn, &SoIndexedFaceSet::GenFmFnT,
    &SoIndexedFaceSet::GenFmVn, &SoIndexedFaceSet::GenFmVnT,
    &SoIndexedFaceSet::GenFmOn, &SoIndexedFaceSet::GenFmOnT,
    &SoIndexedFaceSet::GenFmFn, &SoIndexedFaceSet::GenFmFnT,
    &SoIndexedFaceSet::GenFmFn, &SoIndexedFaceSet::GenFmFnT,
    &SoIndexedFaceSet::GenFmVn, &SoIndexedFaceSet::GenFmVnT,
    &SoIndexedFaceSet::GenVmOn, &SoIndexedFaceSet::GenVmOnT,
    &SoIndexedFaceSet::GenVmFn, &SoIndexedFaceSet::GenVmFnT,
    &SoIndexedFaceSet::GenVmFn, &SoIndexedFaceSet::GenVmFnT,
    &SoIndexedFaceSet::GenVmVn, &SoIndexedFaceSet::GenVmVnT,
    };

@= Om ON
@= On ON
@= Fm
@= Vm
@= Fn
@= Vn
@= T
@= FuncName TriOmOn
@ TriRenderFunction
@= FuncName QuadOmOn
@ QuadRenderFunction
@= FuncName GenOmOn
@ GenRenderFunction

@= Om ON
@= On ON
@= Fm
@= Vm
@= Fn
@= Vn
@= T ON
@= FuncName TriOmOnT
@ TriRenderFunction
@= FuncName QuadOmOnT
@ QuadRenderFunction
@= FuncName GenOmOnT
@ GenRenderFunction

@= Om ON
@= On
@= Fm
@= Vm
@= Fn ON
@= Vn
@= T
@= FuncName TriOmFn
@ TriRenderFunction
@= FuncName QuadOmFn
@ QuadRenderFunction
@= FuncName GenOmFn
@ GenRenderFunction

@= Om ON
@= On
@= Fm
@= Vm
@= Fn ON
@= Vn
@= T ON
@= FuncName TriOmFnT
@ TriRenderFunction
@= FuncName QuadOmFnT
@ QuadRenderFunction
@= FuncName GenOmFnT
@ GenRenderFunction

@= Om ON
@= On
@= Fm
@= Vm
@= Fn
@= Vn ON
@= T
@= FuncName TriOmVn
@ TriRenderFunction
@= FuncName QuadOmVn
@ QuadRenderFunction
@= FuncName GenOmVn
@ GenRenderFunction

@= Om ON
@= On
@= Fm
@= Vm
@= Fn
@= Vn ON
@= T ON
@= FuncName TriOmVnT
@ TriRenderFunction
@= FuncName QuadOmVnT
@ QuadRenderFunction
@= FuncName GenOmVnT
@ GenRenderFunction


@= Om
@= On ON
@= Fm ON
@= Vm
@= Fn
@= Vn
@= T
@= FuncName TriFmOn
@ TriRenderFunction
@= FuncName QuadFmOn
@ QuadRenderFunction
@= FuncName GenFmOn
@ GenRenderFunction

@= Om
@= On ON
@= Fm ON
@= Vm
@= Fn
@= Vn
@= T ON
@= FuncName TriFmOnT
@ TriRenderFunction
@= FuncName QuadFmOnT
@ QuadRenderFunction
@= FuncName GenFmOnT
@ GenRenderFunction

@= Om
@= On
@= Fm ON
@= Vm
@= Fn ON
@= Vn
@= T
@= FuncName TriFmFn
@ TriRenderFunction
@= FuncName QuadFmFn
@ QuadRenderFunction
@= FuncName GenFmFn
@ GenRenderFunction

@= Om
@= On
@= Fm ON
@= Vm
@= Fn ON
@= Vn
@= T ON
@= FuncName TriFmFnT
@ TriRenderFunction
@= FuncName QuadFmFnT
@ QuadRenderFunction
@= FuncName GenFmFnT
@ GenRenderFunction

@= Om
@= On
@= Fm ON
@= Vm
@= Fn
@= Vn ON
@= T
@= FuncName TriFmVn
@ TriRenderFunction
@= FuncName QuadFmVn
@ QuadRenderFunction
@= FuncName GenFmVn
@ GenRenderFunction

@= Om
@= On
@= Fm ON
@= Vm
@= Fn
@= Vn ON
@= T ON
@= FuncName TriFmVnT
@ TriRenderFunction
@= FuncName QuadFmVnT
@ QuadRenderFunction
@= FuncName GenFmVnT
@ GenRenderFunction


@= Om
@= On ON
@= Fm
@= Vm ON
@= Fn
@= Vn
@= T
@= FuncName TriVmOn
@ TriRenderFunction
@= FuncName QuadVmOn
@ QuadRenderFunction
@= FuncName GenVmOn
@ GenRenderFunction

@= Om
@= On ON
@= Fm
@= Vm ON
@= Fn
@= Vn
@= T ON
@= FuncName TriVmOnT
@ TriRenderFunction
@= FuncName QuadVmOnT
@ QuadRenderFunction
@= FuncName GenVmOnT
@ GenRenderFunction

@= Om
@= On
@= Fm
@= Vm ON
@= Fn ON
@= Vn
@= T
@= FuncName TriVmFn
@ TriRenderFunction
@= FuncName QuadVmFn
@ QuadRenderFunction
@= FuncName GenVmFn
@ GenRenderFunction

@= Om
@= On
@= Fm
@= Vm ON
@= Fn ON
@= Vn
@= T ON
@= FuncName TriVmFnT
@ TriRenderFunction
@= FuncName QuadVmFnT
@ QuadRenderFunction
@= FuncName GenVmFnT
@ GenRenderFunction

@= Om
@= On
@= Fm
@= Vm ON
@= Fn
@= Vn ON
@= T
@= FuncName TriVmVn
@ TriRenderFunction
@= FuncName QuadVmVn
@ QuadRenderFunction
@= FuncName GenVmVn
@ GenRenderFunction

@= Om
@= On
@= Fm
@= Vm ON
@= Fn
@= Vn ON
@= T ON
@= FuncName TriVmVnT
@ TriRenderFunction
@= FuncName QuadVmVnT
@ QuadRenderFunction
@= FuncName GenVmVnT
@ GenRenderFunction


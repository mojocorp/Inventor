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
 |      SoFaceSet
 |
@@ This next line is true for the output file only - ignore it here:
 |   Note: This file was preprocessed from another file. Do not edit it.
 |
 |   Author(s)          : Gavin Bell
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */


///////////////////////////////////////////////////////////////////////////
#include <glad/gl.h>
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
#include <Inventor/elements/SoNormalBindingElement.h>
#include <Inventor/elements/SoShapeHintsElement.h>
#include <Inventor/elements/SoGLLazyElement.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/nodes/SoFaceSet.h>
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
SoFaceSet::
@ FuncName 
    (SoGLRenderAction* ) {
@?{ On
    // Send one normal, if there are any normals in vpCache:
    if (vpCache.getNumNormals() > 0)
	vpCache.sendNormal(vpCache.getNormals(0));
@?}
    const char *vertexPtr = vpCache.getVertices(startIndex.getValue());
    const unsigned int vertexStride = vpCache.getVertexStride();
    SoVPCacheFunc *const vertexFunc = vpCache.vertexFunc;
@?{ Vm
    const char *colorPtr = vpCache.getColors(startIndex.getValue());
    const unsigned int colorStride = vpCache.getColorStride();
    SoVPCacheFunc *const colorFunc = vpCache.colorFunc;
@?}
@?{ Fm 
    const char *colorPtr = vpCache.getColors(0);
    const unsigned int colorStride = vpCache.getColorStride();
    SoVPCacheFunc *const colorFunc = vpCache.colorFunc;
@?}
@?{ Vn
    const char *normalPtr = vpCache.getNormals(startIndex.getValue());
    const unsigned int normalStride = vpCache.getNormalStride();
    SoVPCacheFunc *const normalFunc = vpCache.normalFunc;
@?}
@?{ Fn 
    const char *normalPtr = vpCache.getNormals(0);
    const unsigned int normalStride = vpCache.getNormalStride();
    SoVPCacheFunc *const normalFunc = vpCache.normalFunc;
@?}
@?{ T
    const char *texCoordPtr = vpCache.getTexCoords(startIndex.getValue());
    const unsigned int texCoordStride = vpCache.getTexCoordStride();
    SoVPCacheFunc *const texCoordFunc = vpCache.texCoordFunc;
@?}

    glBegin(GL_TRIANGLES);
 
    for (int tri = 0; tri < numTris; tri++) {
@?{ Fm
	(*colorFunc)(colorPtr); colorPtr += colorStride;
@?}
@?{ Fn
	(*normalFunc)(normalPtr); normalPtr += normalStride;
@?}

@?{ Vm
	(*colorFunc)(colorPtr); colorPtr += colorStride;
@?}
@?{ Vn
	(*normalFunc)(normalPtr); normalPtr += normalStride;
@?}
@?{ T
	(*texCoordFunc)(texCoordPtr); texCoordPtr += texCoordStride;
@?}
	(*vertexFunc)(vertexPtr); vertexPtr += vertexStride;
	
@?{ Vm
	(*colorFunc)(colorPtr); colorPtr += colorStride;
@?}
@?{ Vn
	(*normalFunc)(normalPtr); normalPtr += normalStride;
@?}
@?{ T
	(*texCoordFunc)(texCoordPtr); texCoordPtr += texCoordStride;
@?}
	(*vertexFunc)(vertexPtr); vertexPtr += vertexStride;
	
@?{ Vm
	(*colorFunc)(colorPtr); colorPtr += colorStride;
@?}
@?{ Vn
	(*normalFunc)(normalPtr); normalPtr += normalStride;
@?}
@?{ T
	(*texCoordFunc)(texCoordPtr); texCoordPtr += texCoordStride;
@?}
	(*vertexFunc)(vertexPtr); vertexPtr += vertexStride;

    }
    glEnd();
}

@=}

@@ ----------------------------------------------------------------------
@@ Quad rendering:
@@
@={ QuadRenderFunction
void
SoFaceSet::
@ FuncName 
    (SoGLRenderAction* ) {
 
@?{ On
    // Send one normal, if there are any normals in vpCache:
    if (vpCache.getNumNormals() > 0)
	vpCache.sendNormal(vpCache.getNormals(0));
@?}
    const char *vertexPtr = vpCache.getVertices(startIndex.getValue()+3*numTris);
    const unsigned int vertexStride = vpCache.getVertexStride();
    SoVPCacheFunc *const vertexFunc = vpCache.vertexFunc;
@?{ Fm  
    const char *colorPtr = vpCache.getColors(numTris);
    const unsigned int colorStride = vpCache.getColorStride();
    SoVPCacheFunc *const colorFunc = vpCache.colorFunc;
@?}
@?{ Vm
    const char *colorPtr = vpCache.getColors(startIndex.getValue()+3*numTris);
    const unsigned int colorStride = vpCache.getColorStride();
    SoVPCacheFunc *const colorFunc = vpCache.colorFunc;
@?}
@?{ Fn 
    const char *normalPtr = vpCache.getNormals(numTris);
    const unsigned int normalStride = vpCache.getNormalStride();
    SoVPCacheFunc *const normalFunc = vpCache.normalFunc;
@?}
@?{ Vn
    const char *normalPtr = vpCache.getNormals(startIndex.getValue()+3*numTris);
    const unsigned int normalStride = vpCache.getNormalStride();
    SoVPCacheFunc *const normalFunc = vpCache.normalFunc;
@?}
@?{ T
    const char *texCoordPtr = vpCache.getTexCoords(startIndex.getValue()+3*numTris);
    const unsigned int texCoordStride = vpCache.getTexCoordStride();
    SoVPCacheFunc *const texCoordFunc = vpCache.texCoordFunc;
@?}

    glBegin(GL_QUADS);

    for (int quad = 0; quad < numQuads; quad++) {
@?{ Fm
	(*colorFunc)(colorPtr); colorPtr += colorStride;
@?}
@?{ Fn
	(*normalFunc)(normalPtr); normalPtr += normalStride;
@?}
@?{ Vm
	(*colorFunc)(colorPtr); colorPtr += colorStride;
@?}
@?{ Vn
	(*normalFunc)(normalPtr); normalPtr += normalStride;
@?}
@?{ T
	(*texCoordFunc)(texCoordPtr); texCoordPtr += texCoordStride;
@?}
	(*vertexFunc)(vertexPtr); vertexPtr += vertexStride;
	
@?{ Vm
	(*colorFunc)(colorPtr); colorPtr += colorStride;
@?}
@?{ Vn
	(*normalFunc)(normalPtr); normalPtr += normalStride;
@?}
@?{ T
	(*texCoordFunc)(texCoordPtr); texCoordPtr += texCoordStride;
@?}
	(*vertexFunc)(vertexPtr); vertexPtr += vertexStride;
	
@?{ Vm
	(*colorFunc)(colorPtr); colorPtr += colorStride;
@?}
@?{ Vn
	(*normalFunc)(normalPtr); normalPtr += normalStride;
@?}
@?{ T
	(*texCoordFunc)(texCoordPtr); texCoordPtr += texCoordStride;
@?}
	(*vertexFunc)(vertexPtr); vertexPtr += vertexStride;

@?{ Vm
	(*colorFunc)(colorPtr); colorPtr += colorStride;
@?}
@?{ Vn
	(*normalFunc)(normalPtr); normalPtr += normalStride;
@?}
@?{ T
	(*texCoordFunc)(texCoordPtr); texCoordPtr += texCoordStride;
@?}
	(*vertexFunc)(vertexPtr); vertexPtr += vertexStride;

    }
    glEnd();
}

@=}

@@ ----------------------------------------------------------------------
@@ General polygon rendering:
@@
@={ GenRenderFunction
void
SoFaceSet::
@ FuncName 
    (SoGLRenderAction* )
{

@?{ On
    // Send one normal, if there are any normals in vpCache:
    if (vpCache.getNumNormals() > 0)
	vpCache.sendNormal(vpCache.getNormals(0));
@?}
    const char *vertexPtr = 
	vpCache.getVertices(startIndex.getValue()+3*numTris+4*numQuads);
    const unsigned int vertexStride = vpCache.getVertexStride();
    SoVPCacheFunc *const vertexFunc = vpCache.vertexFunc;
@?{ Fm 
    const char *colorPtr = vpCache.getColors(numTris+numQuads);
    const unsigned int colorStride = vpCache.getColorStride();
    SoVPCacheFunc *const colorFunc = vpCache.colorFunc;
@?}
@?{ Vm
    const char *colorPtr = 
	vpCache.getColors(startIndex.getValue()+ 3*numTris + 4*numQuads);
    const unsigned int colorStride = vpCache.getColorStride();
    SoVPCacheFunc *const colorFunc = vpCache.colorFunc;
@?}
@?{ Fn 
    const char *normalPtr = vpCache.getNormals(numTris+numQuads);
    const unsigned int normalStride = vpCache.getNormalStride();
    SoVPCacheFunc *const normalFunc = vpCache.normalFunc;
@?}
@?{ Vn
    const char *normalPtr = 
	vpCache.getNormals(startIndex.getValue() + 3*numTris + 4*numQuads);
    const unsigned int normalStride = vpCache.getNormalStride();
    SoVPCacheFunc *const normalFunc = vpCache.normalFunc;
@?}
@?{ T
    const char *texCoordPtr = 
	vpCache.getTexCoords(startIndex.getValue() + 3*numTris + 4*numQuads);
    const unsigned int texCoordStride = vpCache.getTexCoordStride();
    SoVPCacheFunc *const texCoordFunc = vpCache.texCoordFunc;
@?}
    const int nf = numVertices.getNum();
    for(int faceCtr = numQuads + numTris; faceCtr < nf; faceCtr++) {
@?{ Fm
	(*colorFunc)(colorPtr); colorPtr+= colorStride;
@?}
@?{ Fn
	(*normalFunc)(normalPtr); normalPtr += normalStride;
@?}
	glBegin(GL_POLYGON);
	
	const int nv = numVertices[faceCtr];
	for(int i=0; i<nv; i++) {
@?{ Vm
	    (*colorFunc)(colorPtr); colorPtr += colorStride;
@?}
@?{ Vn
	    (*normalFunc)(normalPtr); normalPtr += normalStride;
@?}
@?{ T
	    (*texCoordFunc)(texCoordPtr); texCoordPtr += texCoordStride;
@?}
	    (*vertexFunc)(vertexPtr); vertexPtr += vertexStride;
	}

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
SoFaceSet::PMFS SoFaceSet::TriRenderFunc[32] = {
    &SoFaceSet::TriOmOn, &SoFaceSet::TriOmOnT,
    &SoFaceSet::TriOmFn, &SoFaceSet::TriOmFnT,
    &SoFaceSet::TriOmFn, &SoFaceSet::TriOmFnT,
    &SoFaceSet::TriOmVn, &SoFaceSet::TriOmVnT,
    &SoFaceSet::TriFmOn, &SoFaceSet::TriFmOnT,
    &SoFaceSet::TriFmFn, &SoFaceSet::TriFmFnT,
    &SoFaceSet::TriFmFn, &SoFaceSet::TriFmFnT,
    &SoFaceSet::TriFmVn, &SoFaceSet::TriFmVnT,
    &SoFaceSet::TriFmOn, &SoFaceSet::TriFmOnT,
    &SoFaceSet::TriFmFn, &SoFaceSet::TriFmFnT,
    &SoFaceSet::TriFmFn, &SoFaceSet::TriFmFnT,
    &SoFaceSet::TriFmVn, &SoFaceSet::TriFmVnT,
    &SoFaceSet::TriVmOn, &SoFaceSet::TriVmOnT,
    &SoFaceSet::TriVmFn, &SoFaceSet::TriVmFnT,
    &SoFaceSet::TriVmFn, &SoFaceSet::TriVmFnT,
    &SoFaceSet::TriVmVn, &SoFaceSet::TriVmVnT,
    };
SoFaceSet::PMFS SoFaceSet::QuadRenderFunc[32] = {
    &SoFaceSet::QuadOmOn, &SoFaceSet::QuadOmOnT,
    &SoFaceSet::QuadOmFn, &SoFaceSet::QuadOmFnT,
    &SoFaceSet::QuadOmFn, &SoFaceSet::QuadOmFnT,
    &SoFaceSet::QuadOmVn, &SoFaceSet::QuadOmVnT,
    &SoFaceSet::QuadFmOn, &SoFaceSet::QuadFmOnT,
    &SoFaceSet::QuadFmFn, &SoFaceSet::QuadFmFnT,
    &SoFaceSet::QuadFmFn, &SoFaceSet::QuadFmFnT,
    &SoFaceSet::QuadFmVn, &SoFaceSet::QuadFmVnT,
    &SoFaceSet::QuadFmOn, &SoFaceSet::QuadFmOnT,
    &SoFaceSet::QuadFmFn, &SoFaceSet::QuadFmFnT,
    &SoFaceSet::QuadFmFn, &SoFaceSet::QuadFmFnT,
    &SoFaceSet::QuadFmVn, &SoFaceSet::QuadFmVnT,
    &SoFaceSet::QuadVmOn, &SoFaceSet::QuadVmOnT,
    &SoFaceSet::QuadVmFn, &SoFaceSet::QuadVmFnT,
    &SoFaceSet::QuadVmFn, &SoFaceSet::QuadVmFnT,
    &SoFaceSet::QuadVmVn, &SoFaceSet::QuadVmVnT,
    };
SoFaceSet::PMFS SoFaceSet::GenRenderFunc[32] = {
    &SoFaceSet::GenOmOn, &SoFaceSet::GenOmOnT,
    &SoFaceSet::GenOmFn, &SoFaceSet::GenOmFnT,
    &SoFaceSet::GenOmFn, &SoFaceSet::GenOmFnT,
    &SoFaceSet::GenOmVn, &SoFaceSet::GenOmVnT,
    &SoFaceSet::GenFmOn, &SoFaceSet::GenFmOnT,
    &SoFaceSet::GenFmFn, &SoFaceSet::GenFmFnT,
    &SoFaceSet::GenFmFn, &SoFaceSet::GenFmFnT,
    &SoFaceSet::GenFmVn, &SoFaceSet::GenFmVnT,
    &SoFaceSet::GenFmOn, &SoFaceSet::GenFmOnT,
    &SoFaceSet::GenFmFn, &SoFaceSet::GenFmFnT,
    &SoFaceSet::GenFmFn, &SoFaceSet::GenFmFnT,
    &SoFaceSet::GenFmVn, &SoFaceSet::GenFmVnT,
    &SoFaceSet::GenVmOn, &SoFaceSet::GenVmOnT,
    &SoFaceSet::GenVmFn, &SoFaceSet::GenVmFnT,
    &SoFaceSet::GenVmFn, &SoFaceSet::GenVmFnT,
    &SoFaceSet::GenVmVn, &SoFaceSet::GenVmVnT,
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


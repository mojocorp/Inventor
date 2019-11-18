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
 |      SoQuadMesh
 |
@@ This next line is true for the output file only - ignore it here:
 |   Note: This file was preprocessed from another file. Do not edit it.
 |
 |   Author(s)          : Paul S. Strauss
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#include <Inventor/misc/SoGL.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoRayPickAction.h>
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
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/nodes/SoMaterialBinding.h>
#include <Inventor/nodes/SoNormalBinding.h>
#include <Inventor/nodes/SoQuadMesh.h>

//////////////////////////////////////////////////////////////////////////
// Following preprocessor-generated routines handle all combinations of
// Normal binding (per vertex, per face, per part, overall/none)
// Color Binding (per vertex, per face, per part, overall)
// Textures (on or off)
//////////////////////////////////////////////////////////////////////////
@@ ----------------------------------------------------------------------
@@ General prototype of renderfunction is as follows:
@@
@={ RenderFunction
void
SoQuadMesh::
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
    const unsigned int vertexRowStride = vertexStride*
	verticesPerRow.getValue();
@?{ Pm | Fm
    const char *colorPtr = vpCache.getColors(0);
    const unsigned int colorStride = vpCache.getColorStride();
    SoVPCacheFunc *const colorFunc = vpCache.colorFunc;
@?}
@?{ Vm
    const char *colorPtr = vpCache.getColors(startIndex.getValue());
    const unsigned int colorStride = vpCache.getColorStride();
    const unsigned int colorRowStride = colorStride*
	verticesPerRow.getValue();
    SoVPCacheFunc *const colorFunc = vpCache.colorFunc;
@?}
@?{ Pn | Fn
    const char *normalPtr = vpCache.getNormals(0);
    const unsigned int normalStride = vpCache.getNormalStride();
    SoVPCacheFunc *const normalFunc = vpCache.normalFunc;
@?}
@?{ Vn
    const char *normalPtr = vpCache.getNormals(startIndex.getValue());
    const unsigned int normalStride = vpCache.getNormalStride();
    const unsigned int normalRowStride = normalStride*verticesPerRow.getValue();
    SoVPCacheFunc *const normalFunc = vpCache.normalFunc;
@?}
@?{ T
    const char *texCoordPtr = vpCache.getTexCoords(0);
    const unsigned int texCoordStride = vpCache.getTexCoordStride();
    SoVPCacheFunc *const texCoordFunc = vpCache.texCoordFunc;
    const unsigned int texCoordRowStride = texCoordStride*
	verticesPerRow.getValue();
@?}

    const int numRows = verticesPerColumn.getValue() - 1;
@?{ Fm | Fn
    const int nv = verticesPerRow.getValue() - 1;
@?}
@?{ Vm | Om | Pm
@?{ Vn | On | Pn
     const int nv = verticesPerRow.getValue();
@?}
@?}

    int v;
    for (int row = 0; row < numRows; row++) {
@?{ Pm
	(*colorFunc)(colorPtr); colorPtr += colorStride;
@?}
@?{ Pn
	(*normalFunc)(normalPtr); normalPtr += normalStride;
@?}
@?{ Fn | Fm
	glBegin(GL_QUADS);
	for(v = 0; v < nv; v++) {
@?{ Fm
	    (*colorFunc)(colorPtr); colorPtr += colorStride;
@?}
@?{ Fn
	    (*normalFunc)(normalPtr); normalPtr += normalStride;
@?}
@?{ Vm
	    (*colorFunc)(colorPtr); 
@?}
@?{ Vn
	    (*normalFunc)(normalPtr); 
@?}
@?{ T
	    (*texCoordFunc)(texCoordPtr);
@?}
	    (*vertexFunc)(vertexPtr);
@?{ Vm
	    (*colorFunc)(colorPtr+colorRowStride); 
@?}
@?{ Vn
	    (*normalFunc)(normalPtr+normalRowStride); 
@?}	    
@?{ T
	    (*texCoordFunc)(texCoordPtr+texCoordRowStride);
@?}	    	    
	    (*vertexFunc)(vertexPtr+vertexRowStride);
@?{ Vm
	    (*colorFunc)(colorPtr+colorStride+colorRowStride); 
@?}
@?{ Vn
	    (*normalFunc)(normalPtr+normalStride+normalRowStride); 
@?}	    	    
@?{ T
	    (*texCoordFunc)(texCoordPtr+texCoordStride+texCoordRowStride);
@?}	    	    
	    (*vertexFunc)(vertexPtr+vertexStride+vertexRowStride);
@?{ Vm
	    (*colorFunc)(colorPtr+colorStride); 
	    colorPtr += colorStride;
@?}
@?{ Vn
	    (*normalFunc)(normalPtr+normalStride);
	    normalPtr += normalStride; 
@?}	    	 	    
@?{ T
	    (*texCoordFunc)(texCoordPtr+texCoordStride);
	    texCoordPtr += texCoordStride;
@?}	    	    
	    (*vertexFunc)(vertexPtr+vertexStride); 
	    vertexPtr += vertexStride;
	}
	glEnd(); //GL_QUADS
	//At the end of a row skip over last vertex:
	vertexPtr += vertexStride;
@?{ T
	texCoordPtr += texCoordStride;
@?}
@?{ Vm
	colorPtr += colorStride;
@?}
@?{ Vn
	normalPtr += normalStride; 
@?}	   

@?} // end Fn | Fm (Quads)

//Do Strip rendering if both ~Fn and ~Fm
@?{ Vm | Om | Pm
@?{ Vn | On | Pn
	glBegin(GL_TRIANGLE_STRIP);
	for (v = 0; v < nv; v++) {

@?{ Vm
	    (*colorFunc)(colorPtr);
@?}
@?{ Vn
	    (*normalFunc)(normalPtr);
@?}
@?{ T
	    (*texCoordFunc)(texCoordPtr);
@?}
	    (*vertexFunc)(vertexPtr);

@?{ Vm
	    (*colorFunc)(colorPtr+colorRowStride);
	    colorPtr += colorStride;
@?}
@?{ Vn
	    (*normalFunc)(normalPtr+normalRowStride);
	    normalPtr += normalStride;
@?}
@?{ T
	    (*texCoordFunc)(texCoordPtr+texCoordRowStride);
	    texCoordPtr += texCoordStride;
@?}
	    (*vertexFunc)(vertexPtr+vertexRowStride);
	    vertexPtr += vertexStride;
	}

	glEnd();
@?}
@?}// end Strip rendering 

    }
}

@=}

// 32 different rendering loops; the 5 bits used to determine the
// rendering case are:
// 43210  BITS            Routine suffix
// -----  ----            --------------
// 00...  Overall mtl     (Om)
// 01...  Part mtl        (Pm)
// 10...  Face mtl        (Fm)
// 11...  Vtx mtl         (Vm)
// ..00.  Overall/No norm (On)
// ..01.  Part norm       (Pn)
// ..10.  Face norm       (Fn)
// ..11.  Vtx norm        (Vn)
// ....0  No texcoord     -none-
// ....1  Vtx texcoord    (T)
//
SoQuadMesh::PMQM SoQuadMesh::renderFunc[32] = {
    &SoQuadMesh::OmOn, &SoQuadMesh::OmOnT,
    &SoQuadMesh::OmPn, &SoQuadMesh::OmPnT,
    &SoQuadMesh::OmFn, &SoQuadMesh::OmFnT,
    &SoQuadMesh::OmVn, &SoQuadMesh::OmVnT,
    &SoQuadMesh::PmOn, &SoQuadMesh::PmOnT,
    &SoQuadMesh::PmPn, &SoQuadMesh::PmPnT,
    &SoQuadMesh::PmFn, &SoQuadMesh::PmFnT,
    &SoQuadMesh::PmVn, &SoQuadMesh::PmVnT,
    &SoQuadMesh::FmOn, &SoQuadMesh::FmOnT,
    &SoQuadMesh::FmPn, &SoQuadMesh::FmPnT,
    &SoQuadMesh::FmFn, &SoQuadMesh::FmFnT,
    &SoQuadMesh::FmVn, &SoQuadMesh::FmVnT,
    &SoQuadMesh::VmOn, &SoQuadMesh::VmOnT,
    &SoQuadMesh::VmPn, &SoQuadMesh::VmPnT,
    &SoQuadMesh::VmFn, &SoQuadMesh::VmFnT,
    &SoQuadMesh::VmVn, &SoQuadMesh::VmVnT,
    };



@= FuncName OmOn
@= Om ON
@= On ON
@= Pm
@= Fm
@= Vm
@= Pn
@= Fn
@= Vn
@= T
@ RenderFunction

@= FuncName OmOnT
@= Om ON
@= On ON
@= Pm
@= Fm
@= Vm
@= Pn
@= Fn
@= Vn
@= T  ON
@ RenderFunction

@= FuncName OmPn
@= Om ON
@= On
@= Pm
@= Fm
@= Vm
@= Pn ON
@= Fn
@= Vn
@= T
@ RenderFunction

@= FuncName OmPnT
@= Om ON
@= On
@= Pm
@= Fm
@= Vm
@= Pn ON
@= Fn
@= Vn
@= T  ON
@ RenderFunction

@= FuncName OmFn
@= Om ON
@= On
@= Pm
@= Fm
@= Vm
@= Pn
@= Fn ON
@= Vn    
@= T
@ RenderFunction

@= FuncName OmFnT
@= Om ON
@= On
@= Pm
@= Fm
@= Vm
@= Pn
@= Fn ON
@= Vn
@= T  ON
@ RenderFunction

@= FuncName OmVn
@= Om ON
@= On
@= Pm
@= Fm
@= Vm
@= Pn
@= Fn
@= Vn ON
@= T
@ RenderFunction

@= FuncName OmVnT
@= Om ON
@= On
@= Pm
@= Fm
@= Vm
@= Pn
@= Fn
@= Vn ON
@= T  ON
@ RenderFunction

@= FuncName PmOn
@= Om
@= On ON
@= Pm ON
@= Fm
@= Vm
@= Pn
@= Fn
@= Vn
@= T
@ RenderFunction

@= FuncName PmOnT
@= Om
@= On ON
@= Pm ON
@= Fm
@= Vm
@= Pn
@= Fn
@= Vn
@= T  ON
@ RenderFunction

@= FuncName PmPn
@= Om
@= On
@= Pm ON
@= Fm
@= Vm
@= Pn ON
@= Fn
@= Vn
@= T
@ RenderFunction

@= FuncName PmPnT
@= Om
@= On
@= Pm ON
@= Fm
@= Vm
@= Pn ON
@= Fn
@= Vn
@= T  ON
@ RenderFunction

@= FuncName PmFn
@= Om
@= On
@= Pm ON
@= Fm
@= Vm
@= Pn
@= Fn ON
@= Vn
@= T
@ RenderFunction

@= FuncName PmFnT
@= Om
@= On
@= Pm ON
@= Fm
@= Vm
@= Pn
@= Fn ON
@= Vn
@= T  ON
@ RenderFunction

@= FuncName PmVn
@= Om
@= On
@= Pm ON
@= Fm
@= Vm
@= Pn
@= Fn
@= Vn ON
@= T
@ RenderFunction

@= FuncName PmVnT
@= Om
@= On
@= Pm ON
@= Fm
@= Vm
@= Pn
@= Fn
@= Vn ON
@= T  ON
@ RenderFunction

@= FuncName FmOn
@= Om
@= On ON
@= Pm
@= Fm ON
@= Vm
@= Pn
@= Fn
@= Vn
@= T
@ RenderFunction

@= FuncName FmOnT
@= Om
@= On ON
@= Pm
@= Fm ON
@= Vm
@= Pn
@= Fn
@= Vn
@= T  ON
@ RenderFunction

@= FuncName FmPn
@= Om
@= On
@= Pm
@= Fm ON
@= Vm
@= Pn ON
@= Fn
@= Vn
@= T
@ RenderFunction

@= FuncName FmPnT
@= Om
@= On
@= Pm
@= Fm ON
@= Vm
@= Pn ON
@= Fn
@= Vn
@= T  ON
@ RenderFunction

@= FuncName FmFn
@= Om
@= On
@= Pm
@= Fm ON
@= Vm
@= Pn
@= Fn ON
@= Vn
@= T
@ RenderFunction

@= FuncName FmFnT
@= Om
@= On
@= Pm
@= Fm ON
@= Vm
@= Pn
@= Fn ON
@= Vn
@= T  ON
@ RenderFunction

@= FuncName FmVn
@= Om
@= On
@= Pm
@= Fm ON
@= Vm
@= Pn
@= Fn  
@= Vn ON
@= T   
@ RenderFunction
 
@= FuncName FmVnT
@= Om
@= On
@= Pm
@= Fm ON
@= Vm
@= Pn
@= Fn  
@= Vn ON
@= T  ON   
@ RenderFunction


@= FuncName VmOn
@= Om
@= On ON
@= Pm
@= Fm
@= Vm ON
@= Pn
@= Fn
@= Vn
@= T
@ RenderFunction

@= FuncName VmOnT
@= Om
@= On ON
@= Pm
@= Fm
@= Vm ON
@= Pn
@= Fn
@= Vn
@= T  ON
@ RenderFunction

@= FuncName VmPn
@= Om
@= On
@= Pm
@= Fm
@= Vm ON
@= Pn ON
@= Fn
@= Vn
@= T
@ RenderFunction

@= FuncName VmPnT
@= Om
@= On
@= Pm
@= Fm
@= Vm ON
@= Pn ON
@= Fn
@= Vn
@= T  ON
@ RenderFunction

@= FuncName VmFn
@= Om
@= On
@= Pm
@= Fm  
@= Vm ON
@= Pn  
@= Fn ON
@= Vn
@= T   
@ RenderFunction
  
@= FuncName VmFnT
@= Om
@= On
@= Pm
@= Fm  
@= Vm ON
@= Pn  
@= Fn ON
@= Vn
@= T  ON   
@ RenderFunction 

@= FuncName VmVn
@= Om
@= On
@= Pm
@= Fm
@= Vm ON
@= Pn
@= Fn
@= Vn ON
@= T
@ RenderFunction

@= FuncName VmVnT
@= Om
@= On
@= Pm
@= Fm
@= Vm ON
@= Pn
@= Fn
@= Vn ON
@= T  ON
@ RenderFunction


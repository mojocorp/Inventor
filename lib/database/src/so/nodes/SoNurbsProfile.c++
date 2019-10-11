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
 |   $Revision $
 |
 |   Classes:
 |	SoNurbsProfile
 |
 |   Author(s)          : Thad Beier, Dave Immel
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#include <GL/glu.h>
#include <Inventor/elements/SoComplexityElement.h>
#include <Inventor/elements/SoComplexityTypeElement.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/elements/SoProfileCoordinateElement.h>
#include <Inventor/elements/SoProjectionMatrixElement.h>
#include <Inventor/elements/SoViewingMatrixElement.h>
#include <Inventor/elements/SoViewportRegionElement.h>
#include <Inventor/nodes/SoNurbsProfile.h>

#include <math.h>
#include <vector>

SO_NODE_SOURCE(SoNurbsProfile);

////////////////////////////////////////////////////////////////////////
//
// Description:
//    This initializes the SoNurbsProfile class.
//
// Use: internal

void
SoNurbsProfile::initClass()
//
////////////////////////////////////////////////////////////////////////
{
    SO__NODE_INIT_CLASS(SoNurbsProfile, "NurbsProfile", SoProfile);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Constructor
//
// Use: public

SoNurbsProfile::SoNurbsProfile()
//
////////////////////////////////////////////////////////////////////////
{
    SO_NODE_CONSTRUCTOR(SoNurbsProfile);
    SO_NODE_ADD_FIELD(knotVector, (0));
    isBuiltIn = TRUE;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Destructor (necessary since inline destructor is too complex)
//
// Use: private

SoNurbsProfile::~SoNurbsProfile()
//
////////////////////////////////////////////////////////////////////////
{
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Returns the list of control points and the knot vector. Space
//    will be allocated to store this data.  The calling routine is
//    responsible for freeing this space.
//
// Use: elements

void
SoNurbsProfile::getTrimCurve(SoState *state, int32_t &numPoints, float *&points,
			     int &floatsPerVec,
			     int32_t &numKnots, float *&knots)
//
////////////////////////////////////////////////////////////////////////
{
    const float				*tknots;

    const SoProfileCoordinateElement *pce = SoProfileCoordinateElement::getInstance(state);

    numPoints = index.getNum();

    if (pce->is2D()) {
	floatsPerVec = 2;
	points = new float[numPoints * 2];

        for (int i = 0; i < numPoints; i++) {
	    const SbVec2f &t = pce->get2((int) index[i]);
	    points[i*2]   = t[0];
	    points[i*2+1] = t[1];
	}
    }
    else {
	floatsPerVec = 3;
	points = new float[numPoints * 3];

        for (int i = 0; i < numPoints; i++) {
	    const SbVec3f &t = pce->get3((int) index[i]);
	    points[i*3]   = t[0];
	    points[i*3+1] = t[1];
	    points[i*3+2] = t[2];
	}
    }

    numKnots = (int32_t) (knotVector.getNum());
    knots    = new float[numKnots];
    tknots   = knotVector.getValues(0);
    memcpy((void *) knots, (const void *) tknots,
	  (int) numKnots * sizeof(float));
}

namespace {
//
// Description:
//    Gets details back from the NURBS library and sends them to the
//    generate primitive callbacks.
//
struct glu_cb_data{
    size_t numVertices;
    size_t numAllocVertices;
    SbVec2f *vertices;
};

void
vertexCallback(float *vertex, glu_cb_data * userData)
{
    //
    // Add the vertex to the list of vertices.  The 3rd component of the
    // vertex is 0.0 and is ignored.  Allocate more vertices if there is
    // not enough space.
    //
    if (userData->numVertices == userData->numAllocVertices)
    {
        userData->numAllocVertices += 20;
        SbVec2f *tmpBlock = new SbVec2f[userData->numAllocVertices];
        memcpy ((void *)tmpBlock, (void *)userData->vertices,
                (int) userData->numVertices*sizeof(SbVec2f));
        delete[] userData->vertices;
        userData->vertices = tmpBlock;
    }

    userData->vertices[userData->numVertices][0] = vertex[0];
    userData->vertices[userData->numVertices++][1] = vertex[1];
}
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Returns vertices approximating the profile.  Space will be
//    allocated to store the vertices.  The calling routine is
//    responsible for freeing this space.
//
// Use: extender

void
SoNurbsProfile::getVertices(SoState *state,
			    int32_t &nVertices, SbVec2f *&vertices)
//
////////////////////////////////////////////////////////////////////////
{
    const SoProfileCoordinateElement *pce = SoProfileCoordinateElement::getInstance(state);

    const int32_t numPoints = index.getNum();

    // Check for no points
    if (numPoints == 0) {
        nVertices = 0;
        vertices = NULL;
        return;
    }

    glu_cb_data data = {0, 0, NULL};

    GLUnurbs *theNurb = gluNewNurbsRenderer();
    gluNurbsProperty(theNurb, GLU_NURBS_MODE, GLU_NURBS_TESSELLATOR);
    gluNurbsCallbackData(theNurb, &data);
    gluNurbsCallback(theNurb, GLU_NURBS_VERTEX_DATA,    (void (SB_CALLBACK*)())vertexCallback);
    float complexity = SoComplexityElement::get(state);

    if (complexity < 0.0)
        complexity = 0.0;
    if (complexity > 1.0)
        complexity = 1.0;

    if (SoComplexityTypeElement::get(state) == SoComplexityTypeElement::OBJECT_SPACE) {
        int steps;
        if (complexity < 0.10)
            steps = 2;
        else if (complexity < 0.25)
            steps = 3;
        else if (complexity < 0.40)
            steps = 4;
        else if (complexity < 0.55)
            steps = 5;
        else
            steps = (int) (powf(complexity, 3.32f) * 28) + 2;

        gluNurbsProperty(theNurb, GLU_SAMPLING_METHOD, GLU_DOMAIN_DISTANCE);
        gluNurbsProperty(theNurb, GLU_U_STEP, (GLfloat)steps);
        gluNurbsProperty(theNurb, GLU_V_STEP, (GLfloat)steps);
    } else {
        const float pixTolerance = 104.0f * complexity * complexity - 252.0f * complexity + 150;

        gluNurbsProperty(theNurb, GLU_SAMPLING_METHOD, GLU_OBJECT_PATH_LENGTH);
        gluNurbsProperty(theNurb, GLU_SAMPLING_TOLERANCE, pixTolerance);

        const SbMatrix matModelView = SoModelMatrixElement::get(state) * SoViewingMatrixElement::get(state);
        const SbMatrix & matProjection = SoProjectionMatrixElement::get(state);

        const SbViewportRegion & vpRegion = SoViewportRegionElement::get(state);
        const SbVec2s & vpOrig = vpRegion.getViewportOriginPixels();
        const SbVec2s & vpSize = vpRegion.getViewportSizePixels();

        GLint viewport[4] = {vpOrig[0],
                             vpOrig[1],
                             vpSize[0],
                             vpSize[1]};

        gluNurbsProperty(theNurb, GLU_AUTO_LOAD_MATRIX, GL_FALSE);
        gluLoadSamplingMatrices(theNurb,
                                (float*)matModelView.getValue(),
                                (float*)matProjection.getValue(),
                                viewport);
    }

    //
    // Draw the NURBS curve
    //
    std::vector<float> points;
    if (pce->is2D()) {
        points.resize(numPoints * 3);

        for (int i = 0; i < numPoints; i++) {
            const SbVec2f &t = pce->get2((int)(index[i]));
            points[i*3+0] = t[0];
            points[i*3+1] = t[1];
            points[i*3+2] = 0.0;
        }
    }
    else {
        points.resize(numPoints * 4);

        for (int i = 0; i < numPoints; i++) {
            const SbVec3f &t = pce->get3((int)(index[i]));
            points[i*4+0] = t[0];
            points[i*4+1] = t[1];
            points[i*4+2] = t[2];
            points[i*4+3] = 0.0;
        }
    }

    gluBeginCurve(theNurb);
    gluNurbsCurve(theNurb,
            knotVector.getNum(), (float*)knotVector.getValues(0),
            pce->is2D() ? 3 : 4, points.data(), knotVector.getNum() - numPoints,
            pce->is2D() ? GL_MAP1_VERTEX_3 : GL_MAP1_VERTEX_4);
    gluEndCurve(theNurb);

    gluDeleteNurbsRenderer( theNurb );

    //
    // The render now contains the list of vertices.  Return them to
    // the caller.
    //
    nVertices = data.numVertices;
    vertices = data.vertices;
}


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
 |      SoIndexedNurbsSurface
 |
 |   Author(s)          : Dave Immel, Thad Beier
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#include <glad/gl.h>
#include <GL/glu.h>
#include <Inventor/SbBox.h>
#include <Inventor/SoDB.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/bundles/SoTextureCoordinateBundle.h>
#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/elements/SoComplexityElement.h>
#include <Inventor/elements/SoComplexityTypeElement.h>
#include <Inventor/elements/SoCoordinateElement.h>
#include <Inventor/elements/SoDrawStyleElement.h>
#include <Inventor/elements/SoGLTextureEnabledElement.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/elements/SoProfileElement.h>
#include <Inventor/elements/SoProjectionMatrixElement.h>
#include <Inventor/elements/SoShapeHintsElement.h>
#include <Inventor/elements/SoTextureCoordinateBindingElement.h>
#include <Inventor/elements/SoTextureCoordinateElement.h>
#include <Inventor/elements/SoViewingMatrixElement.h>
#include <Inventor/elements/SoViewportRegionElement.h>
#include <Inventor/nodes/SoIndexedNurbsSurface.h>
#include <Inventor/nodes/SoProfile.h>

#include <vector>

SO_NODE_SOURCE(SoIndexedNurbsSurface);

////////////////////////////////////////////////////////////////////////
//
// Description:
//    This initializes the SoIndexedNurbsSurface class.
//
// Use: internal

void
SoIndexedNurbsSurface::initClass()
//
////////////////////////////////////////////////////////////////////////
{
    SO__NODE_INIT_CLASS(SoIndexedNurbsSurface, "IndexedNurbsSurface", SoShape);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Constructor
//
// Use: public

SoIndexedNurbsSurface::SoIndexedNurbsSurface()
//
////////////////////////////////////////////////////////////////////////
{
    SO_NODE_CONSTRUCTOR(SoNurbsSurface);
    SO_NODE_ADD_FIELD(numUControlPoints, (0));
    SO_NODE_ADD_FIELD(numVControlPoints, (0));
    SO_NODE_ADD_FIELD(numSControlPoints, (0));
    SO_NODE_ADD_FIELD(numTControlPoints, (0));
    SO_NODE_ADD_FIELD(coordIndex, (0));
    SO_NODE_ADD_FIELD(uKnotVector, (0.0));
    SO_NODE_ADD_FIELD(vKnotVector, (0.0));
    SO_NODE_ADD_FIELD(sKnotVector, (0.0));
    SO_NODE_ADD_FIELD(tKnotVector, (0.0));
    SO_NODE_ADD_FIELD(textureCoordIndex, (-1));

    isBuiltIn = TRUE;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Destructor
//
// Use: private

SoIndexedNurbsSurface::~SoIndexedNurbsSurface()
//
////////////////////////////////////////////////////////////////////////
{}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Performs GL rendering of the NURBS surface.
//
// Use: private

void
SoIndexedNurbsSurface::GLRender(SoGLRenderAction *action)
//
////////////////////////////////////////////////////////////////////////
{
    // First see if the object is visible and should be rendered now
    if (!shouldGLRender(action))
        return;

    const SoCoordinateElement *ce =
        SoCoordinateElement::getInstance(action->getState());

    // Check for 0 control points
    int32_t nCoords = ce->getNum();
    if (nCoords == 0)
        return;

    // Make sure the first current material is sent to GL
    SoMaterialBundle mb(action);
    mb.sendFirst();

    if (SoDrawStyleElement::get(action->getState()) ==
        SoDrawStyleElement::POINTS) {
        //
        // Render the control points of the surface.  Rendering the points
        // of the surface would be very slow, as the Software NURBS library
        // would have to be used, and because of the view dependent
        // tessellation, points would not necessarily remain visible.
        //
        const int32_t *coordIndices = coordIndex.getValues(0);
        glBegin(GL_POINTS);

        // Check to see whether the current coordinates are rational or
        // not and get the correct set.
        if (ce->is3D()) {
            for (int i = 0; i < nCoords; i++) {
                const SbVec3f &coords3 = ce->get3((int)coordIndices[i]);
                glVertex3f(coords3[0], coords3[1], coords3[2]);
            }
        } else {
            for (int i = 0; i < nCoords; i++) {
                const SbVec4f &coords4 = ce->get4((int)coordIndices[i]);
                glVertex4f(coords4[0], coords4[1], coords4[2], coords4[3]);
            }
        }
        glEnd();
        return;
    }

    //
    // Render the NURBS surface using the GLU.
    //
    GLUnurbsObj *nurbsObj = gluNewNurbsRenderer();

    switch (SoDrawStyleElement::get(action->getState())) {
    case SoDrawStyleElement::FILLED:
        gluNurbsProperty(nurbsObj, (GLenum)GLU_DISPLAY_MODE, GLU_FILL);
        break;
    case SoDrawStyleElement::LINES:
        gluNurbsProperty(nurbsObj, (GLenum)GLU_DISPLAY_MODE,
                         GLU_OUTLINE_POLYGON);
        break;
    default:
        break;
    }

    const SbBool doTextures =
        SoGLTextureEnabledElement::get(action->getState());

    gluNurbsProperty(nurbsObj, GLU_NURBS_MODE, GLU_NURBS_TESSELLATOR);
    gluNurbsCallback(nurbsObj, GLU_NURBS_BEGIN, (void (*)())glBegin);
    gluNurbsCallback(nurbsObj, GLU_NURBS_END, (void (*)())glEnd);
    gluNurbsCallback(nurbsObj, GLU_NURBS_VERTEX, (void (*)())glVertex3fv);
    gluNurbsCallback(nurbsObj, GLU_NURBS_NORMAL, (void (*)())glNormal3fv);
    gluNurbsCallback(nurbsObj, GLU_NURBS_TEXTURE_COORD,
                     (void (*)())glTexCoord2fv);

    drawNURBS(nurbsObj, action, doTextures);

    gluDeleteNurbsRenderer(nurbsObj);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Generates triangles representing the NURBS surface.
//
// Use: protected

void
SoIndexedNurbsSurface::generatePrimitives(SoAction *action)
//
////////////////////////////////////////////////////////////////////////
{
    GLUnurbs *nurbsObj = gluNewNurbsRenderer();

    //
    // Draw the NURBS surface.  The SoPrimRender class will receive primitive
    // drawn by the NURBS library, triangulate them, and send them to the
    // callback.
    struct glu_cb_data {
        std::vector<SbVec3f> vertices;
        std::vector<SbVec3f> normals;
        std::vector<SbVec4f> texCoord;

        static void vertexCallback(float *vertex, glu_cb_data *userData) {
            userData->vertices.push_back(
                SbVec3f(vertex[0], vertex[1], vertex[2]));
        }
        static void normalCallback(float *normal, glu_cb_data *userData) {
            userData->normals.push_back(
                SbVec3f(normal[0], normal[1], normal[2]));
        }
        static void texCoordCallback(float *texCoord, glu_cb_data *userData) {
            userData->texCoord.push_back(
                SbVec4f(texCoord[0], texCoord[1], texCoord[2], texCoord[3]));
        }
    } data;

    gluNurbsProperty(nurbsObj, GLU_NURBS_MODE, GLU_NURBS_TESSELLATOR);
    gluNurbsCallbackData(nurbsObj, &data);
    gluNurbsCallback(nurbsObj, GLU_NURBS_VERTEX_DATA,
                     (void(GLAPIENTRY *)())glu_cb_data::vertexCallback);
    gluNurbsCallback(nurbsObj, GLU_NURBS_NORMAL_DATA,
                     (void(GLAPIENTRY *)())glu_cb_data::normalCallback);
    gluNurbsCallback(nurbsObj, GLU_NURBS_TEXTURE_COORD_DATA,
                     (void(GLAPIENTRY *)())glu_cb_data::texCoordCallback);

    drawNURBS(nurbsObj, action, true);

    gluDeleteNurbsRenderer(nurbsObj);

    for (size_t i = 0; i < data.vertices.size() - 2; i += 3) {
        SoPrimitiveVertex pv[3];
        for (int j = 0; j < 3; j++) {
            pv[j].setPoint(data.vertices[i + j]);
            pv[j].setNormal(data.normals[i + j]);
            pv[j].setTextureCoords(data.texCoord[i + j]);
        }
        invokeTriangleCallbacks(action, &pv[0], &pv[1], &pv[2]);
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Computes bounding box for the NURBS surface. The center of the
//    surface is defined as the average of all control point
//    coordinates used for the surface.
//
// Use: extender

void
SoIndexedNurbsSurface::computeBBox(SoAction *action, SbBox3f &box,
                                   SbVec3f &center)

//
////////////////////////////////////////////////////////////////////////
{
    const SoCoordinateElement *ce =
        SoCoordinateElement::getInstance(action->getState());

    const int32_t numCoordIndices = coordIndex.getNum();

    // Check for a degenerate surface
    if (numCoordIndices == 0)
        return;

    //
    // Loop through coordinates, keeping max bounding box and sum of coords
    // If the coordinates are rational, divide the first three values by
    // the fourth value before extending the bounding box.
    //
    const int32_t *coordIndices = coordIndex.getValues(0);
    center.setValue(0.0, 0.0, 0.0);
    if (ce->is3D()) {
        for (int i = 0; i < numCoordIndices; i++) {
            const SbVec3f &coord = ce->get3((int)coordIndices[i]);
            box.extendBy(coord);
            center += coord;
        }
    } else {
        for (int i = 0; i < numCoordIndices; i++) {
            const SbVec4f &coord = ce->get4((int)coordIndices[i]);
            SbVec3f        tmpCoord;
            coord.getReal(tmpCoord);
            box.extendBy(tmpCoord);
            center += tmpCoord;
        }
    }

    center /= (float)numCoordIndices;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Draws the NURBS surface.
//
// Use: protected

void
SoIndexedNurbsSurface::drawNURBS(GLUnurbs *nurbsObj, SoAction *action,
                                 bool doTextures)

//
////////////////////////////////////////////////////////////////////////
{
    //
    // Find the number of steps required for object space tessellation and
    // the pixel tolerance used for screen space tessellation.
    //
    float val = SoComplexityElement::get(action->getState());
    if (val < 0.0)
        val = 0.0;
    if (val > 1.0)
        val = 1.0;

    if (SoComplexityTypeElement::get(action->getState()) ==
        SoComplexityTypeElement::OBJECT_SPACE) {
        gluNurbsProperty(nurbsObj, GLU_SAMPLING_METHOD, GLU_DOMAIN_DISTANCE);
        gluNurbsProperty(nurbsObj, GLU_U_STEP, 400 * val);
        gluNurbsProperty(nurbsObj, GLU_V_STEP, 400 * val);
    } else {
        float pixTolerance = 104.0f * val * val - 252.0f * val + 150;

        gluNurbsProperty(nurbsObj, GLU_SAMPLING_METHOD, GLU_OBJECT_PATH_LENGTH);
        gluNurbsProperty(nurbsObj, (GLenum)GLU_SAMPLING_TOLERANCE,
                         (GLfloat)pixTolerance);
    }

    //
    // Collect the control points and knot vectors into an array suitable
    // for sending to the GL.  The control points and knot vectors must be
    // converted to double precision so that they can be passed to the
    // GL NURBS routines.
    //
    const SoCoordinateElement *ce =
        SoCoordinateElement::getInstance(action->getState());
    const int32_t *coordIndices = coordIndex.getValues(0);
    const int32_t  nCoords = ce->getNum();

    std::vector<GLfloat> dCoords;
    if (ce->is3D()) {
        dCoords.resize(3 * nCoords);
        for (int i = 0; i < nCoords; i++) {
            const SbVec3f &c3 = ce->get3(coordIndices[i]);
            dCoords[3 * i + 0] = c3[0];
            dCoords[3 * i + 1] = c3[1];
            dCoords[3 * i + 2] = c3[2];
        }
    } else {
        dCoords.resize(4 * nCoords);
        for (int i = 0; i < nCoords; i++) {
            const SbVec4f &c4 = ce->get4(coordIndices[i]);
            dCoords[4 * i + 0] = c4[0];
            dCoords[4 * i + 1] = c4[1];
            dCoords[4 * i + 2] = c4[2];
            dCoords[4 * i + 3] = c4[3];
        }
    }
    const int32_t uOffset = ce->is3D() ? 3 : 4;
    const int32_t vOffset = uOffset * numUControlPoints.getValue();

    const float *duKnots = uKnotVector.getValues(0);
    const float *dvKnots = vKnotVector.getValues(0);

    // Texture mapping.  If doTextures == TRUE
    // we are drawing textures.  If the textureCoordinateBinding is
    // DEFAULT, we have to build a default NURBS surface for the texture
    // coordinates, otherwise we use the texture coordinates in the texture
    // element.
    // If there is a software texture function defined, then we have to
    // create a texture nurb surface with the same number of points and
    // knots as the original surface, and call the texture coordinate function
    // at each vertex.
    int32_t              sOffset, tOffset;
    int32_t              nsKnots, ntKnots, nsCoords, ntCoords;
    GLfloat *            sKnots, *tKnots;
    std::vector<GLfloat> dstCoords;

    if (doTextures) {

        switch (SoTextureCoordinateElement::getType(action->getState())) {

        // software texture functions
        case SoTextureCoordinateElement::FUNCTION: {
            // generate S and T coords from U and V coords
            SbVec3f                   coord;
            SbVec2f                   stCoord;
            SoTextureCoordinateBundle tb(action, TRUE);

            nsCoords = numUControlPoints.getValue();
            ntCoords = numVControlPoints.getValue();

            sKnots = (float *)duKnots;
            tKnots = (float *)dvKnots;
            nsKnots = uKnotVector.getNum();
            ntKnots = vKnotVector.getNum();

            const size_t nDstCoords = nsCoords * ntCoords;
            dstCoords.resize(nDstCoords * 2);

            for (int v = 0; v < ntCoords; v++) {
                for (int u = 0; u < nsCoords; u++) {
                    if (ce->is3D()) {
                        int offset = 3 * (v * (int)nsCoords + u);
                        coord[0] = dCoords[offset + 0];
                        coord[1] = dCoords[offset + 1];
                        coord[2] = dCoords[offset + 2];
                    } else {
                        int offset = 4 * (v * (int)nsCoords + u);
                        coord[0] = dCoords[offset + 0] / dCoords[offset + 3];
                        coord[1] = dCoords[offset + 1] / dCoords[offset + 3];
                        coord[2] = dCoords[offset + 2] / dCoords[offset + 3];
                    }
                    const SbVec4f &tc = tb.get(coord, SbVec3f(0.0, 1.0, 0.0));
                    dstCoords[(v * (int)nsCoords + u) * 2 + 0] = tc[0];
                    dstCoords[(v * (int)nsCoords + u) * 2 + 1] = tc[1];
                }
            }
            break;
        }

            // texture coordinates defined from texture node
        case SoTextureCoordinateElement::EXPLICIT:
            // get texture coordinates from texture node
            // get knot vectors from this node
            const SoTextureCoordinateElement *te =
                SoTextureCoordinateElement::getInstance(action->getState());
            int32_t nstCoords = te->getNum();
            if (nstCoords < 1) {
                // Default texture coordinates are computed by defining
                // a bezier surface that is defined in the same valid
                // parameter space as the geometric surface.  The valid
                // parameter space is defined based on the order and knot
                // vector.  The coordinates go from 0 to one and the knot
                // vectors span the valid range of the geometric surface.
                // The knot vectors default to 0 and 1 in the event of bogus
                // input data.
                GLfloat sKnotVal1, sKnotVal2, tKnotVal1, tKnotVal2;

                int uOrder =
                    uKnotVector.getNum() - numUControlPoints.getValue();
                int vOrder =
                    vKnotVector.getNum() - numVControlPoints.getValue();
                if ((uOrder > 0) && (uOrder < uKnotVector.getNum()))
                    sKnotVal1 = duKnots[uOrder - 1];
                else
                    sKnotVal1 = 0;
                if ((uOrder > 0) && (uOrder < uKnotVector.getNum()))
                    sKnotVal2 = duKnots[uKnotVector.getNum() - uOrder];
                else
                    sKnotVal2 = 1;
                if ((vOrder > 0) && (vOrder < vKnotVector.getNum()))
                    tKnotVal1 = dvKnots[vOrder - 1];
                else
                    tKnotVal1 = 0;
                if ((vOrder > 0) && (vOrder < vKnotVector.getNum()))
                    tKnotVal2 = dvKnots[vKnotVector.getNum() - vOrder];
                else
                    tKnotVal2 = 1;

                // do a linear 2x2 array
                nsKnots = 4;
                ntKnots = 4;

                sKnots = (GLfloat *)new GLfloat[4];
                tKnots = (GLfloat *)new GLfloat[4];

                sKnots[0] = sKnots[1] = sKnotVal1;
                tKnots[0] = tKnots[1] = tKnotVal1;
                sKnots[2] = sKnots[3] = sKnotVal2;
                tKnots[2] = tKnots[3] = tKnotVal2;

                // allocate a 2 x 2 array of GLfloat[2]'s
                nsCoords = 2;
                ntCoords = 2;
                const size_t nDstCoords = nsCoords * ntCoords * 2;
                dstCoords.resize(nDstCoords);
                for (int i = 0; i < 2; i++) {
                    for (int j = 0; j < 2; j++) {
                        dstCoords[(i * 2 + j) * 2 + 0] = (GLfloat)j;
                        dstCoords[(i * 2 + j) * 2 + 1] = (GLfloat)i;
                    }
                }
            } else {
                // use coord indices if textureCoordIndices undefined
                const int32_t *texCoordIndices = textureCoordIndex.getValues(0);
                if (textureCoordIndex.getNum() == 1 && texCoordIndices[0] < 0)
                    texCoordIndices = coordIndices;

                nsKnots = sKnotVector.getNum();
                sKnots = (GLfloat *)sKnotVector.getValues(0);

                ntKnots = tKnotVector.getNum();
                tKnots = (GLfloat *)tKnotVector.getValues(0);

                nsCoords = numSControlPoints.getValue();
                ntCoords = numTControlPoints.getValue();

                const size_t nDstCoords = 2 * nstCoords;
                dstCoords.resize(nDstCoords);

                if (SoTextureCoordinateBindingElement::get(
                        action->getState()) ==
                    SoTextureCoordinateBindingElement::PER_VERTEX)
                    for (int i = 0; i < nstCoords; i++) {
                        const SbVec2f &tc = te->get2(i);
                        dstCoords[i * 2 + 0] = tc[0];
                        dstCoords[i * 2 + 1] = tc[1];
                    }
                else
                    for (int i = 0; i < nstCoords; i++) {
                        const SbVec2f &tc = te->get2(texCoordIndices[i]);
                        dstCoords[i * 2 + 0] = tc[0];
                        dstCoords[i * 2 + 1] = tc[1];
                    }
            }
            break;
        }
        sOffset = 2;
        tOffset = sOffset * nsCoords;
    }

    //
    // Draw the NURBS surface.  Begin the surface.  Then load the texture
    // map as a nurbs surface.  Then, draw the geometric surface followed
    // by all of its trim curves.  Then, end the surface.
    //

    // Get one camera based element so that this node will be registered
    // with the cache.  If the camera changes, this element will cause
    // the cache to be blown for this node and the nurbs surface will be
    // regenerated.
    SoViewingMatrixElement::get(action->getState());
    SoModelMatrixElement::get(action->getState());

    // Begin the surface.
    gluBeginSurface(nurbsObj);

    // Draw the texture surface
    if (doTextures) {
        // send down nurbs surface, then free memory
        gluNurbsSurface(nurbsObj, (GLint)nsKnots, sKnots, (GLint)ntKnots,
                        tKnots, (GLint)sOffset, (GLint)tOffset,
                        dstCoords.data(), (GLint)(nsKnots - nsCoords),
                        (GLint)(ntKnots - ntCoords), GL_MAP2_TEXTURE_COORD_2);

        // delete knots if not sharing them with the surface description
        // (in the case of software texture coordinates only)
        if (sKnots != duKnots) {
            delete[] sKnots;
            delete[] tKnots;
        }
    }

    // Draw the geometric surface
    gluNurbsSurface(
        nurbsObj, (GLint)(uKnotVector.getNum()), (GLfloat *)duKnots,
        (GLint)(vKnotVector.getNum()), (GLfloat *)dvKnots, (GLint)uOffset,
        (GLint)vOffset, dCoords.data(),
        (GLint)(uKnotVector.getNum() - numUControlPoints.getValue()),
        (GLint)(vKnotVector.getNum() - numVControlPoints.getValue()),
        ce->is3D() ? GL_MAP2_VERTEX_3 : GL_MAP2_VERTEX_4);

    //
    // Get all of the trim curves and use them to trim the surface.
    //
    const SoNodeList &trimNodes = SoProfileElement::get(action->getState());
    SbBool            haveTrim = FALSE;
    int               numTrims = trimNodes.getLength();

    //
    // For each trim curve, check its linkage to find out if it should be
    // continued on to the previous trim curve or if it should begin a
    // new trim curve.  Then, send the trim to the NURBS library.
    //
    for (int i = 0; i < numTrims; i++) {
        int     floatsPerVec;
        float * trimCoords, *trimKnots;
        int32_t numTrimCoords, numKnots;

        // Get the trim curve.
        SoProfile *profile = (SoProfile *)trimNodes[i];
        profile->getTrimCurve(action->getState(), numTrimCoords, trimCoords,
                              floatsPerVec, numKnots, trimKnots);

        // Check for degenerate trim curves
        if (numTrimCoords == 0)
            continue;

        // Check the linkage.
        if ((profile->linkage.getValue() == SoProfileElement::START_FIRST) ||
            (profile->linkage.getValue() == SoProfileElement::START_NEW)) {
            if (haveTrim)
                gluEndTrim(nurbsObj);
            gluBeginTrim(nurbsObj);
            haveTrim = TRUE;
        }

        // Set the data type of the control points to non-rational or rational
        GLenum type = (floatsPerVec == 2) ? GLU_MAP1_TRIM_2 : GLU_MAP1_TRIM_3;
        if (numKnots == 0) {
            // Send down a Piecewise Linear Trim Curve
            gluPwlCurve(nurbsObj, (GLint)numTrimCoords, trimCoords,
                        (GLint)floatsPerVec, type);
        } else {
            // Send down a NURBS Trim Curve
            gluNurbsCurve(nurbsObj, (GLint)numKnots, trimKnots,
                          (GLint)floatsPerVec, trimCoords,
                          (GLint)(numKnots - numTrimCoords), type);
            delete[] trimKnots;
        }
        delete[] trimCoords;
    }
    if (haveTrim)
        gluEndTrim(nurbsObj);

    gluEndSurface(nurbsObj);
}

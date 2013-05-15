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
 |	SoGLTextureImageElement
 |
 |   Author(s)		: Gavin Bell
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#include <Inventor/misc/SoGL.h>
#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/caches/SoGLDisplayList.h>
#include <Inventor/elements/SoGLTextureImageElement.h>
#include <Inventor/elements/SoGLTextureEnabledElement.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/misc/SoState.h>

#include <vector>

SO_ELEMENT_SOURCE(SoGLTextureImageElement);

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Destructor.
//
// Use: private

SoGLTextureImageElement::~SoGLTextureImageElement()
//
////////////////////////////////////////////////////////////////////////
{
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Initializes SoGLTextureImageElement class.
//
// Use: internal

void
SoGLTextureImageElement::initClass()
{
    SO_ELEMENT_INIT_CLASS(SoGLTextureImageElement, SoTextureImageElement);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Initializes element.
//
// Use: public

void
SoGLTextureImageElement::init(SoState *state)
//
////////////////////////////////////////////////////////////////////////
{
    // Initialize base class stuff
    SoTextureImageElement::init(state);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Pops element, causing side effects in GL.
//
// Use: public

void
SoGLTextureImageElement::pop(SoState *state, const SoElement *)
//
////////////////////////////////////////////////////////////////////////
{
    // Empty texture, don't need to do anything-- the
    // GLTextureEnabledElement will turn off texturing.
    if (image.isNull()) {
        return;
    }
    // Since popping this element has GL side effects, make sure any
    // open caches capture it
    capture(state);

    // Restore previous texture image (if any)
    sendTexEnv(state);
    sendTex(state);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Sets image.
//
// Use: protected, virtual

void
SoGLTextureImageElement::setElt(const SbImage &,
                                int, int, int,
                                int, int,
                                const SbColor &)
//
////////////////////////////////////////////////////////////////////////
{
#ifdef DEBUG
    SoDebugError::post("SoGLTextureImageElement::setElt",
                       "Nodes must call SoGLTextureImageElement::set"
                       " for GLRender, not SoTextureImageElement::set");
#endif
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Does the right GL stuff.  This takes a GL display list that can
//    be used to render the texture; if -1 is passed in as the display
//    list, this will try to build a display list (if there are none
//    already open) and returns the display list, which must be freed
//    by the node that sets this element.
//
// Use: public, static

SoGLDisplayList *
SoGLTextureImageElement::set(SoState *state, SoNode *node,
                             const SbImage &img,
                             int _wrapS, int _wrapT, int _model,
                             int _magFilter, int _minFilter,
                             const SbColor &_blendColor,
                             SoGLDisplayList *_list)
//
////////////////////////////////////////////////////////////////////////
{
    SoGLTextureImageElement	*elt;

    // Get an instance we can change (pushing if necessary)
    elt = (SoGLTextureImageElement *) getElement(state, classStackIndex, node);

    if (elt != NULL) {
        elt->SoTextureImageElement::setElt(img,
                                           _wrapS, _wrapT,
                                           _model,
                                           _magFilter, _minFilter,
                                           _blendColor);

        elt->list = _list;
        elt->sendTexEnv(state);
        elt->sendTex(state);
        return elt->list;
    }
    return NULL;
}

//
// Helper routine stolen from the gluBuild2DMipMaps code:
//
// Compute the nearest power of 2 number.  This algorithm is a little 
// strange, but it works quite well.
//
static int
nearestPowerOf2(GLuint value)
{
    int i = 0;

#ifdef DEBUG
    if (value <= 0) {
        SoDebugError::post("SoGLTextureImageElement::nextPowerOf2",
                           "size <= 0");
        return 0;
    }
#endif

    for (;;) {
        if (value == 1) {
            return i;
        } else if (value == 3) {
            return i+2;
        }
        value = value >> 1;
        i++;
    }
}

static GLint getGLModel(int model)
{
    switch(model)
    {
    case SoTextureImageElement::MODULATE: return GL_MODULATE;
    case SoTextureImageElement::DECAL:    return GL_DECAL;
    case SoTextureImageElement::BLEND:    return GL_BLEND;
    case SoTextureImageElement::REPLACE:  return GL_REPLACE;
    default: break;
    }
#ifdef DEBUG
    SoDebugError::post("SoGLTextureImageElement::getGLModel",
                       "Invalid Texture Model");
#endif
    return GL_INVALID_VALUE;
}

static GLint getGLWrap(int wrap)
{
    switch(wrap)
    {
    case SoTextureImageElement::REPEAT:          return GL_REPEAT;
    case SoTextureImageElement::CLAMP:           return GL_CLAMP;
    case SoTextureImageElement::CLAMP_TO_BORDER: return GL_CLAMP_TO_BORDER;
    case SoTextureImageElement::CLAMP_TO_EDGE:   return GL_CLAMP_TO_EDGE;
    case SoTextureImageElement::MIRRORED_REPEAT: return GL_MIRRORED_REPEAT;
    default: break;
    }
#ifdef DEBUG
    SoDebugError::post("SoGLTextureImageElement::getGLWrap",
                       "Invalid Texture Wrap");
#endif
    return GL_INVALID_VALUE;
}

static GLint getGLFilter(int filter)
{
    switch(filter)
    {
    case SoTextureImageElement::NEAREST:                return GL_NEAREST;
    case SoTextureImageElement::LINEAR:                 return GL_LINEAR;
    case SoTextureImageElement::NEAREST_MIPMAP_NEAREST: return GL_NEAREST_MIPMAP_NEAREST;
    case SoTextureImageElement::NEAREST_MIPMAP_LINEAR:  return GL_NEAREST_MIPMAP_LINEAR;
    case SoTextureImageElement::LINEAR_MIPMAP_NEAREST:  return GL_LINEAR_MIPMAP_NEAREST;
    case SoTextureImageElement::LINEAR_MIPMAP_LINEAR:   return GL_LINEAR_MIPMAP_LINEAR;
    default: break;
    }
#ifdef DEBUG
    SoDebugError::post("SoGLTextureImageElement::getGLFilter",
                       "Invalid Texture Filter");
#endif
    return GL_INVALID_VALUE;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Sends texture environment.  This is separated from send()
//    because texture objects do NOT store the texture environment
//    state.
//
// Use: private

void
SoGLTextureImageElement::sendTexEnv(SoState *)
//
////////////////////////////////////////////////////////////////////////
{
    // This state isn't stored in a texture object:
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, getGLModel(model));
    if (getGLModel(model) == GL_BLEND) {
        glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, blendColor.getValue());
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Sends down a 2D texture.  Builds or uses a display list, if it
//    can.
//
// Use: private

void
SoGLTextureImageElement::sendTex(SoState *state)
//
////////////////////////////////////////////////////////////////////////
{
    if (list) {
        // use display list
        list->call(state);
        return;
    }

    const SbVec3s & size = image.getSize();
    int numComponents = image.getNumComponents();

    SbVec3s newSize = size;
    if ( !GLEW_ARB_texture_non_power_of_two ) {
        // Scale the image to closest power of 2 smaller than maximum
        // texture size:
        GLint maxsize = 0;
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxsize);

        // Use nearest power of 2:
        newSize[0] = size[0] > maxsize ? maxsize : 1 << nearestPowerOf2(size[0]);
        newSize[1] = size[1] > maxsize ? maxsize : 1 << nearestPowerOf2(size[1]);
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);  // Not default
    
    // Format in memory
    int format = GL_INVALID_VALUE;
    switch(image.getFormat())
    {
    case SbImage::Format_Luminance:
        format = GL_LUMINANCE;
        break;
    case SbImage::Format_Luminance_Alpha:
        format = GL_LUMINANCE_ALPHA;
        break;
    case SbImage::Format_RGB24:
        format = GL_RGB;
        break;
    case SbImage::Format_RGBA32:
        format = GL_RGBA;
        break;
    default:
        SoDebugError::post("SoGLTextureImageElement::sendTex",
                           "Invalid image format");
        break;
    }

    int internalFormat = format;

    SbBool buildList = !SoCacheElement::anyOpen(state);
    if (buildList) {
        list = new SoGLDisplayList(state, SoGLDisplayList::TEXTURE_OBJECT);
        list->open(state);
    }

    // If we aren't creating a texture object, then we need to
    // unbind the current texture object so we don't overwrite it's state.
    if (!buildList)
        glBindTexture(GL_TEXTURE_2D, 0);

    // These need to go inside the display list or texture object
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, getGLFilter(magFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, getGLFilter(minFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, getGLWrap(wrapS));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, getGLWrap(wrapT));
    
    bool needMipMaps = (getGLFilter(minFilter) >= GL_NEAREST_MIPMAP_NEAREST) && (getGLFilter(minFilter) <= GL_LINEAR_MIPMAP_LINEAR);

    std::vector<GLubyte> level0;
    if (newSize != size) {
        level0.resize(newSize[0]*newSize[1]*numComponents*sizeof(GLubyte));

        // Use gluScaleImage (which does linear interpolation or box
        // filtering) if using a linear interpolation magnification
        // filter:
        gluScaleImage(
                    (GLenum)format, size[0], size[1], GL_UNSIGNED_BYTE, image.getConstBytes(),
                    newSize[0], newSize[1], GL_UNSIGNED_BYTE, &level0[0]);
    }
    
    if(needMipMaps && GLEW_VERSION_1_4 && !GLEW_VERSION_3_0) {
        glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    }

    // Send level-0 mipmap:
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, newSize[0], newSize[1],
                 0, (GLenum)format, GL_UNSIGNED_BYTE,
                 level0.empty() ? image.getConstBytes() : &level0[0]);
    
    // If necessary, send other mipmaps:
    if (needMipMaps && GLEW_VERSION_3_0) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    if (buildList) {
        list->close(state);
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);  // Reset to default
}

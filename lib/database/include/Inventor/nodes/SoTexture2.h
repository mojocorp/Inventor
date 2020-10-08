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
 |	This file defines the SoTexture node class.
 |
 |   Author(s)		: John Rohlf, Thad Beier, Gavin Bell
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#pragma once

#include <Inventor/elements/SoTextureImageElement.h>
#include <Inventor/fields/SoSFColor.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFImage.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/nodes/SoSubNode.h>

class SoSensor;
class SoFieldSensor;
class SoGLDisplayList;

/// Texture mapping node.
/// \ingroup Nodes
/// This property node defines a texture map and parameters for that map. This
/// map is used to apply texture to subsequent shapes as they are rendered.
///
/// The texture can be read from the file specified by the #filename
/// field. Once the texture has been read, the #image field contains
/// the texture data. However, this field is marked so the image is not
/// written out when the texture node is written to a file.  To turn off
/// texturing, set the #filename field to an empty string ("").
///
/// Textures can also be specified in memory by setting the #image
/// field to contain the texture data. Doing so resets the #filename to
/// the empty string.
///
/// If the texture image's width or height is not a power of 2, or the
/// image's width or height is greater than the maximum supported by
/// OpenGL, then the image will be automatically scaled up or down to the
/// next power of 2 or the maximum texture size.  For maximum speed,
/// point-sampling is used to do the scale; if you want more accurate
/// resampling, pre-filter images to a power of 2 smaller than the maximum
/// texture size (use the OpenGL glGetIntegerv(GL_MAX_TEXTURE_SIZE...)
/// call to determine maximum texture for a specific OpenGL
/// implementation).
///
/// The quality of the texturing is affected by the <b>textureQuality</b>
/// field of the <tt>SoComplexity</tt> node.  The <b>textureQuality</b>
/// field affects what kind of filtering is done to the texture when it must be
/// minified or magnified.  The mapping of a particular texture quality
/// value to a particular OpenGL filtering technique is implementation
/// dependent, and varies based on the texturing performance.
/// If mipmap filtering is required, mipmaps are automatically created
/// using the simple box filter.
///
/// \par Action behavior:
/// <b>SoGLRenderAction, SoCallbackAction</b>
/// Sets current texture in state.
///
/// \par File format/defaults:
/// \code
/// SoTexture2 {
///    filename     ""
///    image        0 0 0
///    wrapS        REPEAT
///    wrapT        REPEAT
///    model        MODULATE
///    blendColor   0 0 0
/// }
/// \endcode
/// \sa
/// SoComplexity,SoMaterial,SoTexture2Transform,SoTextureCoordinate2,SoTextureCoordinateBinding,SoTextureCoordinateFunction
class SoTexture2 : public SoNode {

    SO_NODE_HEADER(SoTexture2);

  public:
    /// Texture model
    enum Model {
        MODULATE = SoTextureImageElement::MODULATE, ///< The texture color is
                                                    ///< multiplied by the
                                                    ///< surface color
        DECAL = SoTextureImageElement::DECAL, ///< The texture color replaces
                                              ///< the surface color
        BLEND =
            SoTextureImageElement::BLEND, ///< Blends between the surface color
                                          ///< and a specified blend color
        REPLACE =
            SoTextureImageElement::REPLACE ///< The texture color replaces the
                                           ///< surface color (OpenGL >= 1.1)
    };

    /// Texture wrap type
    enum Wrap {
        REPEAT = SoTextureImageElement::REPEAT, ///< Repeats texture outside 0-1
                                                ///< texture coordinate range
        CLAMP = SoTextureImageElement::CLAMP, ///< Clamps texture coordinates to
                                              ///< lie within 0-1 range
        CLAMP_TO_BORDER =
            SoTextureImageElement::CLAMP_TO_BORDER, ///< (OpenGL >= 1.3)
        CLAMP_TO_EDGE =
            SoTextureImageElement::CLAMP_TO_EDGE, ///< (OpenGL >= 1.2)
        MIRRORED_REPEAT =
            SoTextureImageElement::MIRRORED_REPEAT ///< (OpenGL >= 1.4)
    };

    /// Texture filter
    enum Filter {
        AUTO =
            SoTextureImageElement::AUTO, ///< Default. The texture filter depend
                                         ///< on SoComplexity::textureQuality.
        NEAREST = SoTextureImageElement::NEAREST,
        LINEAR = SoTextureImageElement::LINEAR,
        NEAREST_MIPMAP_NEAREST = SoTextureImageElement::NEAREST_MIPMAP_NEAREST,
        NEAREST_MIPMAP_LINEAR = SoTextureImageElement::NEAREST_MIPMAP_LINEAR,
        LINEAR_MIPMAP_NEAREST = SoTextureImageElement::LINEAR_MIPMAP_NEAREST,
        LINEAR_MIPMAP_LINEAR = SoTextureImageElement::LINEAR_MIPMAP_LINEAR
    };

    // Fields.

    /// Names file from which to read texture image. Currently only SGI .rgb
    /// files are supported. If the filename is not an absolute path name, the
    /// list of directories maintained by <tt>SoInput</tt> is searched. If the
    /// texture is not found in any of those directories, then the file is
    /// searched for relative to the directory from which the
    /// <tt>SoTexture2</tt> node was read. For example, if an
    /// <tt>SoTexture2</tt> node with a filename of <b>"../tofu.rgb"</b> is read
    /// from <b>/usr/people/bob/models/food.iv</b>, then
    /// <b>/usr/people/bob/tofu.rgb</b> will be read (assuming <b>tofu.rgb</b>
    /// isn't found in the directories maintained by <tt>SoInput</tt>).
    SoSFString filename;
    /// Contains an in-memory representation of the texture map. It is either
    /// the contents of the file read from #filename, an image read
    /// directly from an Inventor file, or an image set programmatically using
    /// the methods provided by <tt>SoSFImage</tt>.
    SoSFImage image;
    /// Indicates what to do when texture coordinates in the S (horizontal)
    /// direction lie outside the range 0-1.
    SoSFEnum wrapS;
    /// Indicates what to do when texture coordinates in the T (vertical)
    /// direction lie outside the range 0-1.
    SoSFEnum wrapT;
    /// Specifies how to map texture onto surface.
    SoSFEnum model;
    /// Color used for <b>BLEND</b> model.
    SoSFColor blendColor;

    /// Texture minifying function.
    SoSFEnum minFilter;

    /// Texture magnification function.
    SoSFEnum magFilter;

    /// Creates a texture node with default settings.
    SoTexture2();

    SoEXTENDER
  public:
    virtual void doAction(SoAction *action);
    virtual void GLRender(SoGLRenderAction *action);
    virtual void callback(SoCallbackAction *action);

    SoINTERNAL
  public:
    static void initClass();

  protected:
    // Reads stuff into instance. Returns FALSE on error.
    virtual SbBool readInstance(SoInput *in, unsigned short flags);

    virtual ~SoTexture2();

    int  getReadStatus() const { return readStatus; }
    void setReadStatus(int s) { readStatus = s; }

  private:
    int getMinFilter(float texQuality) const;
    int getMagFilter(float texQuality) const;

    // These keep the image and filename fields in sync.
    SoFieldSensor *imageSensor;
    static void    imageChangedCB(void *, SoSensor *);
    SoFieldSensor *filenameSensor;
    static void    filenameChangedCB(void *, SoSensor *);

    int readStatus;

    // Display list info for this texture:
    SoGLDisplayList *renderList;
    float            renderListQuality;
};


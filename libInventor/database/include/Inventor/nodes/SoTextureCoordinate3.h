#ifndef  _SO_TEXTURE_COORDINATE_3_
#define  _SO_TEXTURE_COORDINATE_3_

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoMFVec3f.h>

/// 3D texture coordinate node.
/// \ingroup Nodes
/// This node defines a set of 3D coordinates to be used to map textures
/// to subsequent vertex-based shapes (those derived from
/// <tt>SoVertexShape</tt>). It replaces the current texture coordinates in the
/// rendering state for the shapes to use.
///
/// The current texture coordinate binding (see
/// <tt>SoTextureCoordinateBinding</tt>) determines how texture coordinates are
/// mapped to vertices of shapes. An alternative to using explicit texture
/// coordinates is to generate them using a function; see
/// <tt>SoTextureCoordinateFunction</tt>.
///
/// Texture coordinates range from 0 to 1 across the texture. The
/// horizontal coordinate, called <b>S</b>, is specified first, followed by
/// the vertical coordinate, <b>T</b>, and the depth coordinate, <b>R<b>
///
/// \par Action behavior:
/// <b>SoGLRenderAction, SoCallbackAction</b>
/// Sets the current texture coordinates in the state.
///
/// \par File format/defaults:
/// \code
/// SoTextureCoordinate3 {
///    point	[  ]
/// }
/// \endcode
/// \sa SoTexture2,SoTextureCoordinateBinding,SoTextureCoordinateFunction,SoVertexShape
class INVENTOR_API SoTextureCoordinate3 : public SoNode {

    SO_NODE_HEADER(SoTextureCoordinate3);

  public:
    // Fields
    SoMFVec3f		point;		///< Texture coordinate points.

    /// Creates a texture coordinate node with default settings.
    SoTextureCoordinate3();

  SoEXTENDER public:
    virtual void	doAction(SoAction *action);
    virtual void	callback(SoCallbackAction *action);
    virtual void	GLRender(SoGLRenderAction *action);
    virtual void	pick(SoPickAction *action);

  SoINTERNAL public:
    static void		initClass();

  protected:
    virtual ~SoTextureCoordinate3();
};

#endif /* _SO_TEXTURE_COORDINATE_3_ */

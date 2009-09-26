#ifndef _SO_TRANSPARENCY_TYPE_
#define _SO_TRANSPARENCY_TYPE_

#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoTransparencyTypeElement.h>

class INVENTOR_API SoTransparencyType : public SoNode {

  SO_NODE_HEADER(SoTransparencyType);

public:
  /// Various levels of transparency rendering quality
  enum TransparencyType {
      SCREEN_DOOR         = SoTransparencyTypeElement::SCREEN_DOOR,	 ///< Use stipple patterns for screen-door transparency
      ADD                 = SoTransparencyTypeElement::ADD,                ///< Use additive alpha blending
      DELAYED_ADD         = SoTransparencyTypeElement::DELAYED_ADD,        ///< Uses additive blending, rendering all transparent objects after opaque ones
      SORTED_OBJECT_ADD   = SoTransparencyTypeElement::SORTED_OBJECT_ADD,  ///< Same as DELAYED_ADD, but sorts transparent objects by distances of bounding boxes from camera
      BLEND               = SoTransparencyTypeElement::BLEND,              ///< Uses multiplicative alpha blending
      DELAYED_BLEND       = SoTransparencyTypeElement::DELAYED_BLEND,      ///< Uses multiplicative alpha blending, rendering all transparent objects after opaque ones
      SORTED_OBJECT_BLEND = SoTransparencyTypeElement::SORTED_OBJECT_BLEND ///< Same as DELAYED_BLEND, but sorts transparent objects by distances of bounding boxes from camera
  };

  // Fields
  SoSFEnum type;

  /// Creates a transparency type node with default settings.
  SoTransparencyType(void);

SoEXTENDER public:
  virtual void doAction(SoAction * action);
  virtual void callback(SoCallbackAction * action);
  virtual void GLRender(SoGLRenderAction * action);

SoINTERNAL public:
  static void initClass(void);

protected:
  virtual ~SoTransparencyType();
};

#endif // _SO_TRANSPARENCY_TYPE_

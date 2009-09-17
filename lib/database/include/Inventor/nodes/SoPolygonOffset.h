#ifndef  _SO_POLYGON_OFFSET_
#define  _SO_POLYGON_OFFSET_

#include <Inventor/elements/SoPolygonOffsetElement.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFBitMask.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFFloat.h>

class SoAction;
class SoCallbackAction;
class SoGLRenderAction;

class INVENTOR_API SoPolygonOffset : public SoNode
{
   SO_NODE_HEADER(SoPolygonOffset);
   
   public:

      enum Style {
         FILLED = SoPolygonOffsetElement::FILLED,
         LINES  = SoPolygonOffsetElement::LINES, 
         POINTS = SoPolygonOffsetElement::POINTS
      };
      
      // Fields
      SoSFFloat    factor;
      SoSFFloat    units;
      SoSFBitMask  styles;
      SoSFBool     on;
      
      /// Creates a polygon offset node with default settings.
      SoPolygonOffset();

   SoEXTENDER public:

      virtual void doAction(SoAction *action);
      virtual void callback(SoCallbackAction *action);
      virtual void GLRender(SoGLRenderAction *action);

   SoINTERNAL public:

      static void initClass() ;

   protected:
      virtual ~SoPolygonOffset();

};

#endif  // _SO_POLYGON_OFFSET_

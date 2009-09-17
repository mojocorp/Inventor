#ifndef  _SO_POLYGON_OFFSET_ELEMENT_
#define  _SO_POLYGON_OFFSET_ELEMENT_

#include <Inventor/elements/SoReplacedElement.h>

//////////////////////////////////////////////////////////////////////////////
//
//  Class: SoPolygonOffsetElement
//
//  Element that stores the current polygon offset.
//
//  This class allows read-only access to the top element in the state
//  to make accessing several values in it more efficient. Individual
//  values must be accessed through this instance.
//
//////////////////////////////////////////////////////////////////////////////

SoEXTENDER class INVENTOR_API SoPolygonOffsetElement : public SoReplacedElement
{
   SO_ELEMENT_HEADER(SoPolygonOffsetElement);

   public:

      /// Polygon mode
      enum Style {
         FILLED = 0x01,
         LINES  = 0x02,
         POINTS = 0x04
      };

      /// Initializes element
      virtual void	init(SoState * state);

      /// Sets the current polygon offset
      static void	set(SoState *state, SoNode *node, float factor, float units, Style styles, SbBool on);
     
      /// Returns the polygon offset from an element
      static void	get(SoState *state, float &factor, float &units, Style &styles, SbBool &on);
      
      /// Returns the default polygon offset.
      static void	getDefault(float &factor, float &units, Style &styles, SbBool &on);

   protected:

      Style  style;
      SbBool active;
      float  offsetfactor;
      float  offsetunits;

      // Destructor.
      virtual		~SoPolygonOffsetElement();

      virtual void	setElt(float factor, float units, Style styles, SbBool on);

   SoINTERNAL public:

      // Initializes the SoPolygonOffsetElement class.
      static void       initClass();
};

#endif  // _SO_POLYGON_OFFSET_ELEMENT_

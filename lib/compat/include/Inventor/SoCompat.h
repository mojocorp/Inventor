#ifndef  _SO_COMPAT_
#define  _SO_COMPAT_

#include <Inventor/SbBasic.h>

/// Initializes Inventor compatibility classes.
class INVENTOR_API SoCompat {
  public:
    /// init all interaction classes
    static void		init();		
  private:
    static SbBool	initialized;	// has interaction been initialized
};

#endif /* _SO_COMPAT_ */
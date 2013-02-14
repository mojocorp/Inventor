#ifndef  _SO_GL_UNIFORM_ELEMENT
#define  _SO_GL_UNIFORM_ELEMENT

#include <Inventor/elements/SoAccumulatedElement.h>

//////////////////////////////////////////////////////////////////////////////
//
//  Class: SoGLUniformElement
//
//
//////////////////////////////////////////////////////////////////////////////

SoEXTENDER class INVENTOR_API SoGLUniformElement : public SoAccumulatedElement {

    SO_ELEMENT_HEADER(SoGLUniformElement);

public:
    enum UniformType {
        INT,
        FLOAT
    };

    /// Initializes element
    virtual void init(SoState *state);

    /// Adds a clip plane to the current set in the state
    //static void add(SoState *state, SoNode *node, UniformType type, const SbString & name, );

    /// Overrides push() method to copy values from next instance in the stack
    virtual void push(SoState *state);

    /// Overrides pop() method to free up planes that were added
    virtual void pop(SoState *state, const SoElement *prevTopElement);

    /// Returns the top (current) instance of the element in the state
    static const SoGLUniformElement * getInstance(SoState *state);

    /// Returns the number of planes in an instance
    int getNum() const;

    /// Returns the indexed plane an element as an SbPlane. The plane
    /// can be returned in object or world space.
    //const SbPlane & get(int index, SbBool inWorldSpace = TRUE) const;

    /// Prints element (for debugging)
    virtual void print(FILE *fp) const;

SoINTERNAL public:
    // Initializes the SoGLUniformElement class
    static void initClass();

protected:
    SbPList  planes;  // List of plane structures
    int   startIndex; // Index of 1st plane created

    virtual ~SoGLUniformElement();
};

#endif /* _SO_GL_UNIFORM_ELEMENT */

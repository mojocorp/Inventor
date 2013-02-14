#ifndef  _SO_GL_SHADERPROGRAM_ELEMENT
#define  _SO_GL_SHADERPROGRAM_ELEMENT

#include <Inventor/elements/SoReplacedElement.h>

class SbGLShaderProgram;
//////////////////////////////////////////////////////////////////////////////
//
//  Class: SoGLShaderProgramElement
//
//
//////////////////////////////////////////////////////////////////////////////

SoEXTENDER class INVENTOR_API SoGLShaderProgramElement : public SoReplacedElement {
    SO_ELEMENT_HEADER(SoGLShaderProgramElement);

public:
    /// Initializes element
    virtual void init(SoState *state);

    static void set(SoState *const state, SoNode *const node, SbGLShaderProgram * program);
    static SbGLShaderProgram * get(SoState * state);

    virtual void push(SoState *);
    virtual void pop(SoState * state, const SoElement *prevTopElement);

    /// Returns TRUE if the program matches given element
    virtual SbBool matches(const SoElement * element) const;

SoINTERNAL public:
    // Initializes the SoGLCoordinateElement class
    static void initClass();

protected:
    virtual ~SoGLShaderProgramElement();

    SbGLShaderProgram *shaderProgram;
};

#endif /* _SO_GL_SHADERPROGRAM_ELEMENT */

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
 |	SoDepthBufferElement
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#include <Inventor/elements/SoDepthBufferElement.h>

SO_ELEMENT_SOURCE(SoDepthBufferElement);

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Destructor.
//
// Use: private

SoDepthBufferElement::~SoDepthBufferElement()
//
////////////////////////////////////////////////////////////////////////
{
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Initializes SoDepthBufferElement class.
//
// Use: internal

void
SoDepthBufferElement::initClass()
//
////////////////////////////////////////////////////////////////////////
{
    SO_ELEMENT_INIT_CLASS(SoDepthBufferElement, SoElement);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Initializes element.
//
// Use: public

void
SoDepthBufferElement::init(SoState *)
//
////////////////////////////////////////////////////////////////////////
{
    test	= getDefaultTest();
    write	= getDefaultWrite();
    function    = getDefaultFunction();
    range       = getDefaultRange();
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//
//
// Use: public, static

void
SoDepthBufferElement::set(SoState * state, SbBool test, SbBool write,
                          DepthWriteFunction function, const SbVec2f & range)
//
////////////////////////////////////////////////////////////////////////
{
    SoDepthBufferElement	*elt;

    // Get an instance we can change (pushing if necessary)
    elt = (SoDepthBufferElement *) getElement(state, classStackIndex);

    if (elt != NULL)
        elt->setElt(test, write, function, range);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Virtual set method; the GL version of this element overrides
//    this to send stuff to GL, too...
//
// Use: protected

void
SoDepthBufferElement::setElt(SbBool _test, SbBool _write,
                             DepthWriteFunction _function, const SbVec2f & _range)
//
////////////////////////////////////////////////////////////////////////
{
    test        = _test;
    write       = _write;
    function    = _function;
    range       = _range;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Returns shape hints from state.
//
// Use: public, static

void
SoDepthBufferElement::get(SoState * state, SbBool & test_out, SbBool & write_out,
                          DepthWriteFunction & function_out, SbVec2f & range_out)
//
////////////////////////////////////////////////////////////////////////
{
    const SoDepthBufferElement *elt;

    elt = (const SoDepthBufferElement *)
        getConstElement(state, classStackIndex);

    test_out        = elt->test;
    write_out       = elt->write;
    function_out    = elt->function;
    range_out       = elt->range;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Overrides this method to compare shape hints.
//
// Use: public

SbBool
SoDepthBufferElement::matches(const SoElement *elt) const
//
////////////////////////////////////////////////////////////////////////
{
    const SoDepthBufferElement *shElt = (const SoDepthBufferElement *) elt;

    return (test        == shElt->test          	&&
            write       == shElt->write 		&&
            function    == shElt->function              &&
            range       == shElt->range);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//     Create a copy of this instance suitable for calling matches()
//     on.
//
// Use: protected

SoElement *
SoDepthBufferElement::copyMatchInfo() const
//
////////////////////////////////////////////////////////////////////////
{
    SoDepthBufferElement *result =
        (SoDepthBufferElement *)getTypeId().createInstance();

    result->test	= test;
    result->write	= write;
    result->function    = function;
    result->range	= range;

    return result;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Prints element for debugging.
//
// Use: public
//
////////////////////////////////////////////////////////////////////////

#ifdef DEBUG
void
SoDepthBufferElement::print(FILE *fp) const
{
    const char *functionName;

    SoElement::print(fp);

    switch (function) {
    case NEVER:     functionName = "NEVER"; break;
    case ALWAYS:    functionName = "ALWAYS"; break;
    case LESS:      functionName = "LESS"; break;
    case LEQUAL:    functionName = "LEQUAL"; break;
    case EQUAL:     functionName = "EQUAL"; break;
    case GEQUAL:    functionName = "GEQUAL"; break;
    case GREATER:   functionName = "GREATER"; break;
    case NOTEQUAL:  functionName = "NOTEQUAL"; break;
    default:	// Just to shut CC up
        break;
    }



    fprintf(fp,
            "\tDepth Buffer: test = %s, write = %s,\n"
            "\t\t     function = %s, range = [%f-%f]\n",
            test ? "TRUE" : "FALSE", write ? "TRUE" : "FALSE", functionName, range[0], range[1]);
}
#else  /* DEBUG */
void
SoDepthBufferElement::print(FILE *) const
{
}
#endif /* DEBUG */

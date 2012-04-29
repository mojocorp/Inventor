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
 |   $Revision $
 |
 |   Classes:
 |	SoFontNameElement class.
 |
 |   Author(s)		: Paul S. Strauss, Nick Thompson, Thad Beier
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#include <Inventor/elements/SoFontNameElement.h>
#include <Inventor/errors/SoDebugError.h>

std::map<std::string, std::string> SoFontNameElement::s_font_map;

SO_ELEMENT_SOURCE(SoFontNameElement);

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Destructor.
//
// Use: private

SoFontNameElement::~SoFontNameElement()
//
////////////////////////////////////////////////////////////////////////
{
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Initializes SoFontNameElement class.
//
// Use: internal

void
SoFontNameElement::initClass()
{
    SO_ELEMENT_INIT_CLASS(SoFontNameElement, SoReplacedElement);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Initializes element.
//
// Use: public

void
SoFontNameElement::init(SoState *)
//
////////////////////////////////////////////////////////////////////////
{
    fontName = getDefault();
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Sets the current fontName
//
// Use: public

void
SoFontNameElement::set(SoState *state, SoNode *node,
			 const SbName &fontName)
//
////////////////////////////////////////////////////////////////////////
{
    SoFontNameElement	*elt;

    // Get an instance we can change (pushing if necessary)
    elt = (SoFontNameElement *) getElement(state, classStackIndex, node);

    if (elt != NULL)
	elt->fontName = fontName;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Returns font name from state
//
// Use: public

const SbName &
SoFontNameElement::get(SoState *state)
//
////////////////////////////////////////////////////////////////////////
{
    const SoFontNameElement *elt;

    elt = (const SoFontNameElement *)
	getConstElement(state, classStackIndex);

    return elt->fontName;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Returns the font path
//
// Use: public
std::string
SoFontNameElement::getFontFileName(const std::string & fontName)
//
////////////////////////////////////////////////////////////////////////
{
    initFontMap();

    if (!s_font_map[fontName].empty()) {
        return s_font_map[fontName].c_str();
    }

#ifdef DEBUG
    SoDebugError::post("SoFontNameElement::getFontFileName",
                       "Couldn't find font %s, replacing with %s", fontName.c_str(), SoFontNameElement::getDefault().getString());
#endif

    return SoFontNameElement::getDefault().getString();
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Register a font
//
// Use: public
void
SoFontNameElement::addFontFileName(const std::string & fontName, const std::string & fontPath)
//
////////////////////////////////////////////////////////////////////////
{
    initFontMap();

    s_font_map[fontName] = fontPath;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Compares names
//
// Use: public

SbBool
SoFontNameElement::matches(const SoElement *elt) const
//
////////////////////////////////////////////////////////////////////////
{
    return (fontName ==
	    ((const SoFontNameElement *) elt)->fontName);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//     Create a copy of this instance suitable for calling matches()
//     on.
//
// Use: protected

SoElement *
SoFontNameElement::copyMatchInfo() const
//
////////////////////////////////////////////////////////////////////////
{
    SoFontNameElement *result =
	(SoFontNameElement *)getTypeId().createInstance();

    result->fontName = fontName;

    return result;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Prints contents of element for debugging.
//
// Use: public
//
////////////////////////////////////////////////////////////////////////

#ifdef DEBUG
void
SoFontNameElement::print(FILE *fp) const
{
    SoElement::print(fp);

    fprintf(fp, "\tFontName = %s\n", fontName.getString());
}
#else  /* DEBUG */
void
SoFontNameElement::print(FILE *) const
{
}
#endif /* DEBUG */

void
SoFontNameElement::initFontMap()
{
    static bool initialized = false;
    if (!initialized) {
        // In PDF, the following 14 fonts are defined:
        // Courier (Regular, Oblique, Bold, Bold Oblique)
        // Helvetica (Regular, Oblique, Bold, Bold Oblique)
        // Symbol
        // Times (Roman, Italic, Bold, Bold Oblique)
        // ITC Zapf Dingbats

#if defined(SB_OS_WIN)
            s_font_map["Courier"] = "c:\\Windows\\Fonts\\cour.ttf";
            s_font_map["Courier-Regular"] = "c:\\Windows\\Fonts\\cour.ttf";
            s_font_map["Courier-Bold"] = "c:\\Windows\\Fonts\\courbd.ttf";
            s_font_map["Courier-Oblique"] = "c:\\Windows\\Fonts\\couri.ttf";
            s_font_map["Courier-Bold Oblique"] = "c:\\Windows\\Fonts\\courbi.ttf";

            s_font_map["Helvetica"] = "c:\\Windows\\Fonts\\arial.ttf";
            s_font_map["Helvetica-Regular"] = "c:\\Windows\\Fonts\\arial.ttf";
            s_font_map["Helvetica-Bold"] = "c:\\Windows\\Fonts\\arialbd.ttf";
            s_font_map["Helvetica-Oblique"] = "c:\\Windows\\Fonts\\ariali.ttf";
            s_font_map["Helvetica-Bold Oblique"] = "c:\\Windows\\Fonts\\arialbi.ttf";

            s_font_map["Symbol"] = "c:\\Windows\\Fonts\\symbol.ttf";

            s_font_map["Times-Roman"] = "c:\\Windows\\Fonts\\times.ttf";
            s_font_map["Times-Bold"] = "c:\\Windows\\Fonts\\timesbd.ttf";
            s_font_map["Times-Oblique"] = "c:\\Windows\\Fonts\\timesi.ttf";
            s_font_map["Times-Bold Oblique"] = "c:\\Windows\\Fonts\\timesbi.ttf";

#elif defined(SB_OS_MACX)
            s_font_map["Courier"] = "/Library/Fonts/Courier New.ttf";
            s_font_map["Courier-Regular"] = "/Library/Fonts/Courier New.ttf";
            s_font_map["Courier-Oblique"] = "/Library/Fonts/Courier New Italic.ttf";
            s_font_map["Courier-Bold"] = "/Library/Fonts/Courier New Bold.ttf";
            s_font_map["Courier-Bold Oblique"] = "/Library/Fonts/Courier New Bold Italic.ttf";

            s_font_map["Helvetica"] = "/Library/Fonts/Arial.ttf";
            s_font_map["Helvetica-Regular"] = "/Library/Fonts/Arial.ttf";
            s_font_map["Helvetica-Oblique"] = "/Library/Fonts/Arial Italic.ttf";
            s_font_map["Helvetica-Bold"] = "/Library/Fonts/Arial Bold.ttf";
            s_font_map["Helvetica-Bold Oblique"] = "/Library/Fonts/Arial Bold Italic.ttf";

            s_font_map["Symbol"] = "/System/Library/Fonts/Symbol.ttf";

            s_font_map["Times-Roman"] = "/Library/Fonts/Times New Roman.ttf";
            s_font_map["Times-Oblique"] = "/Library/Fonts/Times New Roman Italic.ttf";
            s_font_map["Times-Bold"] = "/Library/Fonts/Times New Roman Bold.ttf";
            s_font_map["Times-Bold Oblique"] = "/Library/Fonts/Times New Roman Bold Italic.ttf";

#else
            s_font_map["Courier"] = "/usr/share/fonts/X11/Type1/c0419bt_.pfb";
            s_font_map["Courier-Regular"] = "/usr/share/fonts/X11/Type1/c0419bt_.pfb";
            s_font_map["Courier-Oblique"] = "/usr/share/fonts/X11/Type1/c0419bt_.pfb";
            s_font_map["Courier-Bold"] = "/usr/share/fonts/X11/Type1/c0583bt_.pfb";
            s_font_map["Courier-Bold Oblique"] = "/usr/share/fonts/X11/Type1/c0419bt_.pfb";

            s_font_map["Helvetica"] = "/usr/share/fonts/truetype/msttcorefonts/Arial.ttf";
            s_font_map["Helvetica-Regular"] = "/usr/share/fonts/truetype/msttcorefonts/Arial.ttf";
            s_font_map["Helvetica-Oblique"] = "/usr/share/fonts/truetype/msttcorefonts/Arial_Italic.ttf";
            s_font_map["Helvetica-Bold"] = "/usr/share/fonts/truetype/msttcorefonts/Arial_Bold.ttf";
            s_font_map["Helvetica-Bold Oblique"] = "/usr/share/fonts/truetype/msttcorefonts/Arial_Bold_Italic.ttf";

            s_font_map["Symbol"] = "/usr/share/fonts/truetype/?ttf-symbol-replacement/?symbol-replacement.ttf/?symbol.ttf";

            s_font_map["Times-Roman"] = "/usr/share/fonts/truetype/msttcorefonts/Times_New_Roman.ttf";
            s_font_map["Times-Oblique"] = "/usr/share/fonts/truetype/msttcorefonts/Times_New_Roman_Italic.ttf";
            s_font_map["Times-Bold"] = "/usr/share/fonts/truetype/msttcorefonts/Times_New_Roman_Bold.ttf";
            s_font_map["Times-Bold Oblique"] = "/usr/share/fonts/truetype/msttcorefonts/Times_New_Roman_Bold_Italic.ttf";
#endif
            initialized = true;
    }
}

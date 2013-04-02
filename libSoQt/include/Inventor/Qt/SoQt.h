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
* Copyright (C) 1990-93   Silicon Graphics, Inc.
*
_______________________________________________________________________
______________  S I L I C O N   G R A P H I C S   I N C .  ____________
|
|   $Revision: 1.2 $
|
|   This static class provides routines for Inventor/Xt compatibility.
|
|   Author(s): Nick Thompson, Paul Isaacs, David Mott, Gavin Bell
|		Alain Dumesny
|
______________  S I L I C O N   G R A P H I C S   I N C .  ____________
_______________________________________________________________________
*/

/*
 * Ported to Qt by Morgan Leborgne, 2010
 */

#ifndef _SO_QT_
#define _SO_QT_

#ifdef WIN32
#ifdef SOQT_EXPORTS
#   define SOQT_EXPORT __declspec(dllexport)
#else
#   define SOQT_EXPORT __declspec(dllimport)
#endif
#else
#   define SOQT_EXPORT
#endif

#include <Inventor/SbBasic.h>
#include <Inventor/SbLinear.h>

class QWidget;
class SoQtEventHandler;

class SOQT_EXPORT SoQt {
  public:
    // This binds Inventor with Qt so that they may work together.
    // It calls SoDB::init(), SoNodeKit::init(), SoInteraction::init
    static void init(const char *appName, const char *className = "Inventor");

    // Cleanup-up Inventor.
    static void finish();

    // This retrieves and dispatches events (loops forever).
    // It is equivalent to QApplication::exec().
    static int             mainLoop();
    

  private:
    static bool initialized;
    static SoQtEventHandler*  eventHandler;
};

#endif  /* _SO_QT_ */

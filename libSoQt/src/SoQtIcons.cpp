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
 |   Description:
 |	This file contains bitmaps which are used as icons for
 |  the push buttons in the viewers.
 |
 |   Author(s) : Alain Dumesny
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

/*
 * Ported to Qt by Morgan Leborgne, 2010
 */

#include <Inventor/Qt/SoQtIcons.h>

namespace SoQtIcon {

QIcon getIcon(IconType type)
{
    switch (type) {
    case PICK:
        return QIcon(":/resources/pickMode.png");
    case VIEW:
        return QIcon(":/resources/viewMode.png");
    case HELP:
        return QIcon(":/resources/help.png");
    case HOME:
        return QIcon(":/resources/gotoHome.png");
    case SET_HOME:
        return QIcon(":/resources/setHome.png");
    case VIEW_ALL:
        return QIcon(":/resources/viewAll.png");
    case SEEK:
        return QIcon(":/resources/seek.png");
    case ORTHO:
        return QIcon(":/resources/ortho.png");
    case PERSP:
        return QIcon(":/resources/persp.png");
    case X:
        return QIcon(":/resources/x.png");
    case Y:
        return QIcon(":/resources/y.png");
    case Z:
        return QIcon(":/resources/z.png");
    default:
        break;
    }

    return QIcon();
}

}

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
 |	This file contains the actual Qt cursors bitmaps. The file
 |  SoXtCursors.h contains cursors definitions (sizes and hot spot).
 |
 |   Author(s) : Alain Dumesny
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

/*
 * Ported to Qt by Morgan Leborgne, 2010
 */

#include <Inventor/Qt/SoQtCursors.h>

#include <QBitmap>
#include <QPainter>

namespace SoQtCursor {

#define so_qt_curved_hand_x_hot 4
#define so_qt_curved_hand_y_hot 2
#define so_qt_flat_hand_x_hot 13
#define so_qt_flat_hand_y_hot 4
#define so_qt_pointing_hand_x_hot 9
#define so_qt_pointing_hand_y_hot 4
#define so_qt_roll_x_hot 9
#define so_qt_roll_y_hot 11
#define so_qt_target_x_hot 11
#define so_qt_target_y_hot 11
#define so_qt_normal_vec_x_hot 12
#define so_qt_normal_vec_y_hot 18

///////////////////////////////////////////////////////////////////////////
//
// Walk viewer cursors
//
///////////////////////////////////////////////////////////////////////////

#define so_qt_walk_x_hot 15
#define so_qt_walk_y_hot 3
#define so_qt_pan_x_hot 9
#define so_qt_pan_y_hot 1
#define so_qt_tilt_x_hot 7
#define so_qt_tilt_y_hot 10

QCursor getCursor (CursorType type)
{
    QSize size;
    QBitmap bits;
    QBitmap mask;
    int hotX = -1;
    int hotY = -1;
    switch (type) {
    case VIEWING:
        return QCursor(Qt::ArrowCursor);
    case CURVED_HAND:
        hotX = 4; hotY = 2;
        bits = QBitmap(":/resources/so_qt_curved_hand_bits.png");
        mask = QBitmap(":/resources/so_qt_curved_hand_mask.png");
        break;
    case FLAT_HAND:
        hotX = 13; hotY = 4;
        bits = QBitmap(":/resources/so_qt_flat_hand_bits.png");
        mask = QBitmap(":/resources/so_qt_flat_hand_mask.png");
        break;
    case POINTING_HAND:
        hotX = 9; hotY = 4;
        bits = QBitmap(":/resources/so_qt_pointing_hand_bits.png");
        mask = QBitmap(":/resources/so_qt_pointing_hand_mask.png");
        break;
    case ROLL:
        hotX = 9; hotY = 11;
        bits = QBitmap(":/resources/so_qt_roll_bits.png");
        mask = QBitmap(":/resources/so_qt_roll_mask.png");
        break;
    case TARGET:
        hotX = 11; hotY = 11;
        bits = QBitmap(":/resources/so_qt_target_bits.png");
        mask = QBitmap(":/resources/so_qt_target_mask.png");
        break;
    case NORMAL_VEC:
        hotX = 12; hotY = 17;
        bits = QBitmap(":/resources/so_qt_normal_vec_bits.png");
        mask = QBitmap(":/resources/so_qt_normal_vec_mask.png");
        break;
    case WALK:
        hotX = 15; hotY = 3;
        bits = QBitmap(":/resources/so_qt_walk_bits.png");
        mask = QBitmap(":/resources/so_qt_walk_mask.png");
        break;
    case PAN:
        hotX = 9; hotY = 1;
        bits = QBitmap(":/resources/so_qt_pan_bits.png");
        mask = QBitmap(":/resources/so_qt_pan_mask.png");
        break;
    case TILT:
        hotX = 7; hotY = 10;
        bits = QBitmap(":/resources/so_qt_tilt_bits.png");
        mask = QBitmap(":/resources/so_qt_tilt_mask.png");
        break;
    }

    return QCursor (bits, mask, hotX, hotY);
}

}

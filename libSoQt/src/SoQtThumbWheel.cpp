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
 |   Classes:
 |	SoQtThumbWheel
 |
 |   Author(s)	: Alain Dumesny
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

/*
 * Ported to Qt by Morgan Leborgne, 2010
 */

#include <QMouseEvent>
#include <QPainter>

#define _USE_MATH_DEFINES
#include <math.h>

#include <Inventor/Qt/SoQtThumbWheel.h>

/*
 * Defines
 */

#define TICK_NUM    21
#define PART1	    4
#define PART2	    5
#define PART3	    7
#define PART4	    9
#define UI_THICK    0

#define WHITE_UI_COLOR      QColor(255,255,255)
#define BLACK_UI_COLOR      QColor(0,0,0)
#define MAIN_UI_COLOR       QColor(170,170,170)
#define DARK1_UI_COLOR      QColor(128,128,128)
#define DARK2_UI_COLOR      QColor(85,85,85)
#define DARK3_UI_COLOR      QColor(50,50,50)
#define LIGHT1_UI_COLOR     QColor(215,215,215)

////////////////////////////////////////////////////////////////////////
//
// Public constructor - build the widget right now
//
SoQtThumbWheel::SoQtThumbWheel(
    Qt::Orientation ori,
    QWidget *parent)
        : QWidget(parent)
//
////////////////////////////////////////////////////////////////////////
{
    interactive = false;
    value = 0.0f;
    orientation = ori;

    if (orientation == Qt::Horizontal) {
      setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
      setCursor(Qt::SizeHorCursor);
    } else {
      setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
      setCursor(Qt::SizeVerCursor);
    }
}

////////////////////////////////////////////////////////////////////////
//
//    Destructor.
//
SoQtThumbWheel::~SoQtThumbWheel()
//
////////////////////////////////////////////////////////////////////////
{

}

QSize SoQtThumbWheel::sizeHint() const
{
    return (orientation == Qt::Horizontal) ? QSize(130, 20) : QSize(20, 130);
}

////////////////////////////////////////////////////////////////////////
//
//  This routine draws the entire thumb wheel region.
//
// Use: virtual protected
//
void
SoQtThumbWheel::paintEvent(QPaintEvent *)
//
////////////////////////////////////////////////////////////////////////
{
    if (! isVisible())
	return;

    QPainter p(this);

    int size[] = { contentsRect().size().width(), contentsRect().size().height() };

    int i, n;
    short x, y, x1, y1, x2, y2;
    short mid, rad, l, d;
    float angle, ang_inc;
    
    x1 = y1 = 0;
    x2 = size[0] - 1;
    y2 = size[1] - 1;
    
    x1+=UI_THICK; y1+=UI_THICK; x2-=UI_THICK; y2-=UI_THICK;
    
    p.setPen(LIGHT1_UI_COLOR);
    p.drawRect(x1, y1, x2, y2);
    x1++; y1++; x2--; y2--;
    p.drawRect(x1, y1, x2, y2);
    x1++; y1++; x2--; y2--;
      
    if (orientation == Qt::Horizontal) {
        p.setPen(DARK3_UI_COLOR); p.drawLine(x1,y2,x2,y2);
        p.setPen(BLACK_UI_COLOR); p.drawLine(x1,y1,x2,y1);
	y1++; y2--;
	
	l = x2 - x1;
	d = 0;
        p.setPen(BLACK_UI_COLOR);
        p.drawLine(x1+d, y1,x2-d, y1);
        p.drawLine(x1+d, y2,x2-d, y2);
	d = (short)(l * .06);
        p.setPen(DARK2_UI_COLOR);
        p.drawLine(x1+d, y1,x2-d, y1);
        p.drawLine(x1+d, y2,x2-d, y2);
	d = (short)(l * .12);
        p.setPen(DARK1_UI_COLOR);
        p.drawLine(x1+d, y1,x2-d, y1);
        p.drawLine(x1+d, y2,x2-d, y2);
	d = (short)(l * .20);
        p.setPen(MAIN_UI_COLOR);
        p.drawLine(x1+d, y1,x2-d, y1);
        p.drawLine(x1+d, y2,x2-d, y2);
	d = (short)(l * .30);
        p.setPen(LIGHT1_UI_COLOR);
        p.drawLine(x1+d, y1,x2-d, y1);
        p.drawLine(x1+d, y2,x2-d, y2);
	d = (short)(l * .40);
        p.setPen(WHITE_UI_COLOR);
        p.drawLine(x1+d, y1,x2-d, y1);
        p.drawLine(x1+d, y2,x2-d, y2);
	x1++; y1++; x2--; y2--;
    }
    else {
        p.setPen(DARK3_UI_COLOR); p.drawLine(x1,y1,x1,y2);
        p.setPen(BLACK_UI_COLOR); p.drawLine(x2,y1,x2,y2);
	x1++; x2--;
	
	l = y2 - y1;
	d = 0;
        p.setPen(BLACK_UI_COLOR);
        p.drawLine(x1, y1+d,x1, y2-d);
        p.drawLine(x2, y1+d,x2, y2-d);
	d = (short)(l * .06);
        p.setPen(DARK2_UI_COLOR);
        p.drawLine(x1, y1+d,x1, y2-d);
        p.drawLine(x2, y1+d,x2, y2-d);
	d = (short)(l * .12);
        p.setPen(DARK1_UI_COLOR);
        p.drawLine(x1, y1+d,x1, y2-d);
        p.drawLine(x2, y1+d,x2, y2-d);
	d = (short)(l * .20);
        p.setPen(MAIN_UI_COLOR);
        p.drawLine(x1, y1+d,x1, y2-d);
        p.drawLine(x2, y1+d,x2, y2-d);
	d = (short)(l * .30);
        p.setPen(LIGHT1_UI_COLOR);
        p.drawLine(x1, y1+d,x1, y2-d);
        p.drawLine(x2, y1+d,x2, y2-d);
	d = (short)(l * .40);
        p.setPen(WHITE_UI_COLOR);
        p.drawLine(x1, y1+d,x1, y2-d);
        p.drawLine(x2, y1+d,x2, y2-d);
	x1++; y1++; x2--; y2--;
    }
    
    //p.setPen(MAIN_UI_COLOR);
    //p.drawRect(x1, y1, x2, y2);
    
    //
    // draw the tick marks
    //
    
    angle = value * M_PI / 180;
    ang_inc = M_PI / TICK_NUM;
    n = (int)(floorf(angle / ang_inc));
    angle -= n * ang_inc;
       
    if (orientation == Qt::Horizontal) {
        mid = size[0] / 2;
	rad = mid - UI_THICK - 2;
	
	for (i=0; i<TICK_NUM; i++) {
	    x = mid - (short)(cosf(angle) * rad) - 1;
	    
	    if (i<PART1 || i>(TICK_NUM - PART1)) {
                p.setPen(BLACK_UI_COLOR); p.drawLine(x, y1,x, y2);
		x++; 
                p.setPen(DARK1_UI_COLOR); p.drawLine(x, y1,x, y2);
	    }
	    else if (i<PART2 || i>(TICK_NUM - PART2)) {
                p.setPen(DARK3_UI_COLOR); p.drawLine(x, y1,x, y2);
	    }
	    else if (i<PART3 || i>(TICK_NUM - PART3)) {
                p.setPen(LIGHT1_UI_COLOR); p.drawLine(x, y1,x, y2);
		x++; 
                p.setPen(DARK2_UI_COLOR); p.drawLine(x, y1,x, y2);
	    }
	    else if (i<PART4 || i>(TICK_NUM - PART4)) {
                p.setPen(WHITE_UI_COLOR); p.drawLine(x, y1,x, y2);
		x++; 
                p.setPen(DARK2_UI_COLOR); p.drawLine(x, y1,x, y2);
	    }
	    else {
                p.setPen(WHITE_UI_COLOR);
                x--; p.drawLine(x, y1,x, y2);
                x++; p.drawLine(x, y1,x, y2);
                p.setPen(DARK2_UI_COLOR);
                x++; p.drawLine(x, y1,x, y2);
	    }
	    
	    angle += ang_inc;
	}
    }
    else {
        mid = size[1] / 2;
	rad = mid - UI_THICK - 2;
	
	for (i=0; i<TICK_NUM; i++) {
	    y = mid - (short)(cosf(angle) * rad);
	    
	    if (i<PART1 || i>(TICK_NUM - PART1)) {
                p.setPen(BLACK_UI_COLOR); p.drawLine(x1, y,x2, y);
		y--; 
                p.setPen(DARK1_UI_COLOR); p.drawLine(x1, y,x2, y);
	    }
	    else if (i<PART2 || i>(TICK_NUM - PART2)) {
                p.setPen(DARK3_UI_COLOR); p.drawLine(x1, y,x2, y);
	    }
	    else if (i<PART3 || i>(TICK_NUM - PART3)) {
                p.setPen(LIGHT1_UI_COLOR); p.drawLine(x1, y,x2, y);
		y--; 
                p.setPen(DARK2_UI_COLOR); p.drawLine(x1, y,x2, y);
	    }
	    else if (i<PART4 || i>(TICK_NUM - PART4)) {
                p.setPen(WHITE_UI_COLOR); p.drawLine(x1, y,x2, y);
		y--; 
                p.setPen(DARK2_UI_COLOR); p.drawLine(x1, y,x2, y);
	    }
	    else {
                p.setPen(WHITE_UI_COLOR);
                y++; p.drawLine(x1, y,x2, y);
                y--; p.drawLine(x1, y,x2, y);
                p.setPen(DARK2_UI_COLOR);
                y--; p.drawLine(x1, y,x2, y);
	    }
	    
	    angle += ang_inc;
	}
    }
}

////////////////////////////////////////////////////////////////////////
//
//  Process the passed Qt event.
//
// Use: virtual protected
//
void SoQtThumbWheel::mousePressEvent(QMouseEvent *e)
//
////////////////////////////////////////////////////////////////////////
{
    if (e->button() == Qt::LeftButton) {

        emit sliderPressed();
        interactive = true;

        // get starting point
        lastPosition = (orientation == Qt::Horizontal) ? e->x() : e->y();
    }
}

////////////////////////////////////////////////////////////////////////
//
//  Process the passed Qt event.
//
// Use: virtual protected
//
void SoQtThumbWheel::mouseReleaseEvent(QMouseEvent *e)
//
////////////////////////////////////////////////////////////////////////
{
    if (e->button() == Qt::LeftButton) {
        interactive = false;
        emit sliderReleased();
    }
}

////////////////////////////////////////////////////////////////////////
//
//  Process the passed Qt event.
//
// Use: virtual protected
//
void SoQtThumbWheel::mouseMoveEvent(QMouseEvent *e)
//
////////////////////////////////////////////////////////////////////////
{
    if (interactive) {
        float r;
        if (orientation == Qt::Horizontal)
            r = (e->x() - lastPosition) / float(width() - 2 * UI_THICK);
        else
            r = (e->y() - lastPosition) / float(height() - 2 * UI_THICK);

        // now rotate wheel
        if (r != 0.0) {
            setValue(value + r * 180);
        }

        lastPosition = (orientation == Qt::Horizontal) ? e->x() : e->y();
    }
}

////////////////////////////////////////////////////////////////////////
//
//  Sets the thumb wheel to this value..
//
// Use: public
//
void
SoQtThumbWheel::setValue(float v)
//
////////////////////////////////////////////////////////////////////////
{
    if (value == v)
    	return;
    
    value = v;
    
    emit valueChanged(value);

    update();
}



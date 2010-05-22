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

//  -*- C++ -*-

/*
 * Copyright (C) 1990-93   Silicon Graphics, Inc.
 *
 _______________________________________________________________________
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 |
 |   $Revision: 1.1 $
 |
 |   Description:
 |	This file contains the SoQtThumbWheel class which is a GL wheel
 |  viewed on it's side. It is mostly used to control things like rotation
 |  where a thumb wheel is better than a regular slider.
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

#ifndef  _SO_QT_THUMB_WHEEL_
#define  _SO_QT_THUMB_WHEEL_

#include <Inventor/Qt/SoQt.h>
#include <QWidget>

//////////////////////////////////////////////////////////////////////////////
//
//  Class: SoQtThumbWheel
//
//
//////////////////////////////////////////////////////////////////////////////

class SoQtThumbWheel : public QWidget {
    Q_OBJECT
public:
    
    SoQtThumbWheel(Qt::Orientation orientation, QWidget *parent = NULL);
    ~SoQtThumbWheel();
    
    //
    // Routines to specify the wheel value (a rotation given in degree) and get
    // the current wheel value.
    //
    // NOTE: setValue() will emit valueChanged signal if the value differs.
    //
    void    	setValue(float radians);
    float   	getValue() const { return value; }
    

    // true while the value is changing interactively
    bool  isInteractive() const { return interactive; }

    QSize sizeHint() const;
signals:
    void valueChanged(float);
    void sliderPressed();
    void sliderReleased();
protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);

private:
    // local variables
    Qt::Orientation orientation;
    float	value;
    int	    	lastPosition;
    
    bool interactive;
};

#endif  /* _SO_QT_THUMB_WHEEL_ */

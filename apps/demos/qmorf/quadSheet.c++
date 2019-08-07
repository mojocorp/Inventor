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

//
// Code to generate a flat sheet made out of a quad-mesh
//

#include <math.h>
#include "QuadThing.h"
#include <Inventor/SoDB.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/actions/SoWriteAction.h>

QuadThing *
createQuadSheet(int sizex, int sizey)
{
    SbVec3f *points = new SbVec3f[sizex*sizey];

    int row, column;

    for (row = 0; row < sizey; row++)
    {
	float y = 1.0 - row*2.0/(sizey-1);

	for (column = 0; column < sizex; column++)
	{
	    float x = column*2.0/(sizex-1) - 1.0;

	    points[row*sizex+column].setValue(x*.1, y*.1, 0.0);
	}
    }

    QuadThing *result = new QuadThing(sizex, sizey, points);

    return result;
}

int main(int argc, char **argv)
{
    SoDB::init();

    QuadThing *result = createQuadSheet(40, 40);

    // Now write it out to stdout
    SoWriteAction wa;

    wa.apply(result->getSceneGraph());

    return 0;
}

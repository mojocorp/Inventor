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

/*--------------------------------------------------------------
 *  This is an example from the Inventor Mentor
 *  chapter 2, example 2.
 *
 *  Use an engine to make the cone spin.
 *------------------------------------------------------------*/

#include <stdlib.h>
#include <Inventor/Qt/SoQt.h>
#include <Inventor/Qt/SoQtRenderArea.h>
#include <Inventor/engines/SoElapsedTime.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoRotationXYZ.h>
#include <Inventor/nodes/SoSeparator.h>

int
main(int , char **argv)
{
   // Initialize Inventor and Xt
   SoQt::init(argv[0]);

   SoSeparator *root = new SoSeparator;
   root->ref();
   SoPerspectiveCamera *myCamera = new SoPerspectiveCamera;
   root->addChild(myCamera);
   root->addChild(new SoDirectionalLight);

   // This transformation is modified to rotate the cone
   SoRotationXYZ *myRotXYZ = new SoRotationXYZ;
   root->addChild(myRotXYZ);

   SoMaterial *myMaterial = new SoMaterial;
   myMaterial->diffuseColor.setValue(1.0, 0.0, 0.0);   // Red
   root->addChild(myMaterial);
   root->addChild(new SoCone);

   // An engine rotates the object. The output of myCounter 
   // is the time in seconds since the program started.
   // Connect this output to the angle field of myRotXYZ
   myRotXYZ->axis = SoRotationXYZ::X;     // rotate about X axis
   SoElapsedTime *myCounter = new SoElapsedTime;
   myRotXYZ->angle.connectFrom(&myCounter->timeOut);

   SoQtRenderArea *myRenderArea = new SoQtRenderArea();
   myCamera->viewAll(root, myRenderArea->getViewportRegion());
   myRenderArea->setSceneGraph(root);
   myRenderArea->setTitle("Engine Spin");
   myRenderArea->show();

   return SoQt::mainLoop();
}

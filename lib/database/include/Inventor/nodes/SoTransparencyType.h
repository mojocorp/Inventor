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
 * Copyright (C) 1990,91   Silicon Graphics, Inc.
 *
 _______________________________________________________________________
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 |
 |   $Revision: 1.1 $
 |
 |   Description:
 |	SoTransparencyType
 |
 |   Author(s)		: Morgan Leborgne
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef _SO_TRANSPARENCY_TYPE_
#define _SO_TRANSPARENCY_TYPE_

#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoTransparencyTypeElement.h>

class SoTransparencyType : public SoNode {

  SO_NODE_HEADER(SoTransparencyType);

public:
  /// Various levels of transparency rendering quality
  enum TransparencyType {
      SCREEN_DOOR         = SoTransparencyTypeElement::SCREEN_DOOR,	 ///< Use stipple patterns for screen-door transparency
      ADD                 = SoTransparencyTypeElement::ADD,                ///< Use additive alpha blending
      DELAYED_ADD         = SoTransparencyTypeElement::DELAYED_ADD,        ///< Uses additive blending, rendering all transparent objects after opaque ones
      SORTED_OBJECT_ADD   = SoTransparencyTypeElement::SORTED_OBJECT_ADD,  ///< Same as DELAYED_ADD, but sorts transparent objects by distances of bounding boxes from camera
      BLEND               = SoTransparencyTypeElement::BLEND,              ///< Uses multiplicative alpha blending
      DELAYED_BLEND       = SoTransparencyTypeElement::DELAYED_BLEND,      ///< Uses multiplicative alpha blending, rendering all transparent objects after opaque ones
      SORTED_OBJECT_BLEND = SoTransparencyTypeElement::SORTED_OBJECT_BLEND ///< Same as DELAYED_BLEND, but sorts transparent objects by distances of bounding boxes from camera
  };

  // Fields
  SoSFEnum type;

  /// Creates a transparency type node with default settings.
  SoTransparencyType(void);

SoEXTENDER public:
  virtual void doAction(SoAction * action);
  virtual void callback(SoCallbackAction * action);
  virtual void GLRender(SoGLRenderAction * action);

SoINTERNAL public:
  static void initClass(void);

protected:
  virtual ~SoTransparencyType();
};

#endif // _SO_TRANSPARENCY_TYPE_

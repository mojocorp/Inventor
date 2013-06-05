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
 * Copyright (C) 1991   Silicon Graphics, Inc.
 *
 _______________________________________________________________________
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 |
 |   $Revision: 1.3 $
 |
 |   Classes:
 |	SoCallbackList
 |
 |   Author(s): David Mott
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#include <Inventor/misc/SoCallbackList.h>
#include <Inventor/errors/SoDebugError.h>

//////////////////////////////////////////////////////////////////////////////
//
//  Constructor
//
SoCallbackList::SoCallbackList()
//
//////////////////////////////////////////////////////////////////////////////
{
}

//////////////////////////////////////////////////////////////////////////////
//
//  Destructor
//
SoCallbackList::~SoCallbackList()
//
//////////////////////////////////////////////////////////////////////////////
{

}

//////////////////////////////////////////////////////////////////////////////
//
//  addCallback - adds the function f to the callback list, along with
//  userData. At invocation, f will be passed userData, along with callback
//  specific data.
//
void
SoCallbackList::addCallback(SoCallbackListCB *f, void *userData)
//
//////////////////////////////////////////////////////////////////////////////
{
    if (f == NULL)
    	return;
	
    SoCallbackStruct cb;
    cb.func = f;
    cb.userData = userData;
    
    list.push_back(cb);
}   

//////////////////////////////////////////////////////////////////////////////
//
//  removeCallback - removes the function f associated with userData from the.
//  callback list.
//
void
SoCallbackList::removeCallback(SoCallbackListCB *f, void *userData)
//
//////////////////////////////////////////////////////////////////////////////
{
    std::vector<SoCallbackStruct>::iterator it=list.begin();
    while (it != list.end()) {
        const SoCallbackStruct & cb = *it;
        if ((cb.func == f) && (cb.userData == userData)) {
            break;
        } else {
            ++it;
        }
    }
    
#ifdef DEBUG
    if (it == list.end()) {
        SoDebugError::post("SoCallbackList::removeCallback",
                           "Passed function and userData not found in "
                           "callback list");
    }
#endif
}   

//////////////////////////////////////////////////////////////////////////////
//
//  invokeCallbacks - invokes each callback func in the list, passing.
//  userData, and callbackData as the parameters.
//
void
SoCallbackList::invokeCallbacks(void *callbackData)
//
//////////////////////////////////////////////////////////////////////////////
{
    std::vector<SoCallbackStruct>::const_iterator it;
    for (it=list.begin(); it!=list.end(); ++it) {
        const SoCallbackStruct &cb = *it;
        (*cb.func) (cb.userData, callbackData);
    }
}   

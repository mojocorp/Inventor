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
 |   $Revision: 1.6 $
 |
 |   Classes:
 |	SoType
 |
 |   Author(s)		: Nick Thompson, Gavin Bell
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#include <Inventor/SoType.h>
#include <Inventor/SoLists.h>
#include <Inventor/errors/SoDebugError.h>
#ifdef SB_OS_WIN
#   include <windows.h>
#else
#   include <dlfcn.h>
#   include <unistd.h>
#endif
#include <stdlib.h>
#include <algorithm>

SoINTERNAL struct SoTypeData {
    SoType		type;
    SoType		parent;
    SbName		name;
    void		*(*createMethod)();
};

std::vector<SoTypeData>	SoType::typeData;

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Initialize the type system.  Called by SoDB::init.
//
// Use: public, static

void
SoType::init()
//
////////////////////////////////////////////////////////////////////////
{
    // This will change when expandTypeData() is called below
    typeData.reserve(128);

    // Initialize bad type at index 0. Make room first.
    SoTypeData td;
    td.type.storage.index = 0;
    td.type.storage.isPublic = 1;
    td.type.storage.data  = 0;
    typeData.push_back(td);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Clean-up the type system.  Called by SoDB::finish.
//
// Use: public, static

void
SoType::finish()
//
////////////////////////////////////////////////////////////////////////
{

}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Returns the "bad type" - used for type errors
//    
//
// Use: public, static

SoType
SoType::badType()
//
////////////////////////////////////////////////////////////////////////
{
    SoType t;

    t.storage.index = 0;
    t.storage.isPublic = 1;
    t.storage.data = 0;

    return t;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Returns TRUE if this type is derived from the argument type
//
// Use: public

SbBool
SoType::isDerivedFrom(SoType t) const
//
////////////////////////////////////////////////////////////////////////
{
    SoType thisType = *this;

    while (! thisType.isBad())
	if (thisType == t)
	    return TRUE;
	else
	    thisType = thisType.getParent();

    return FALSE;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Add all types derived from the given type to the given type
//    list.  Returns the number of types added.
//
// Use: public, static

int
SoType::getAllDerivedFrom(SoType type, SoTypeList &typeList)
//
////////////////////////////////////////////////////////////////////////
{
    // Gather all valid types into array (skip index 0 - badType)
    int numAdded = 0;
    for (size_t i = 1; i < typeData.size(); i++) {
        SoType curType = typeData[i].type;

        // See if the type corresponds to a non-abstract node class
        if (! curType.isBad() && curType.isDerivedFrom(type) &&
                (curType.storage.isPublic == 1)) {
            typeList.append(curType);
            ++numAdded;
        }
    }

    return numAdded;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Given a name, returns the appropriate SoType.
//
// Use: public, static

SoType
SoType::fromName(const SbName & name)
//
////////////////////////////////////////////////////////////////////////
{
    int b = 0;

    //
	// XXX Alex -- add additional layer of abstraction on top
 	// of this to make porting to other platforms easier.
	//
#ifdef SB_OS_WIN
#   define sgidlopen_version(a,b,c,d) LoadLibraryA(a)
#else
#   define sgidlopen_version(a,b,c,d) dlopen((a),(b))
#endif

    const char *nameChars = name.getString();
    SbString nameString(nameChars);  // For easier manipulation...

    // Look for an existing type; if the type begins with "So", then
    // look at a type matching the stuff after the "So", also.  If not
    // found, we'll try the DSO thing:
    b = find(name);
    SbBool notFound = (b == -1);
    if (notFound && (name.getLength() > 2)  && (nameString.getSubString(0,1) == "So")) {
        b = find(SbName(nameChars+2));
        notFound = (b == -1);
    }
    if (notFound) {
	// Do the DSO thing.
	// First, try the regular name, which will use the normal rld
	// search path (LD_LIBRARY_PATH followed by the system directories:
	//   /usr/lib or /usr/lib32 or /usr/lib64 and /lib   ).
	// If that fails, try:
	// Current directory (if not root)
	// /usr/local/lib/InventorDSO (if not root)  (or 32, 64)
	// /usr/lib/InventorDSO (always)  (or 32, 64)
#ifdef SB_OS_WIN
    HINSTANCE dsoHandle = NULL;
#else
	SbBool isRoot = ((geteuid()!=getuid()) || (getegid()!=getgid())
			 || (getuid() == 0));

	void *dsoHandle = NULL;
#endif
	// Temporary storage
	char DSOFile[101], dummyFunc[101];
#ifdef DEBUG
    const char *longestName = "/usr/lib/InventorDSO/.so";
	if (name.getLength()+strlen(longestName) > 100) {
	    SoDebugError::post("SoType::fromName",
			       "Type name '%s' is too long\n", nameChars);
	    return SoType::badType();
	}
	// Note: don't have to check dummyFunc, since "/usr/local..."
	// string is longer than the initClass__... string.
#endif

#ifdef SB_OS_WIN
    sprintf(DSOFile, "%s.dll", nameChars);
#else
    sprintf(DSOFile, "%s.so", nameChars);
#endif	
    dsoHandle = sgidlopen_version(DSOFile, RTLD_LAZY, "sgi3.0", 0);

#ifndef SB_OS_WIN
  const char *libDir = "lib";
  if (dsoHandle == NULL && !isRoot) {
      sprintf(DSOFile, "./%s.so", nameChars);
      dsoHandle = sgidlopen_version(DSOFile, RTLD_LAZY, "sgi3.0", 0);
  }     
  if (dsoHandle == NULL && !isRoot) {
      sprintf(DSOFile, "/usr/local/%s/InventorDSO/%s.so", 
                                 libDir, nameChars);
	    dsoHandle = sgidlopen_version(DSOFile, RTLD_LAZY, "sgi3.0", 0);
	}	    
	if (dsoHandle == NULL) {
	    sprintf(DSOFile,"/usr/%s/InventorDSO/%s.so", libDir, nameChars);
	    dsoHandle = sgidlopen_version(DSOFile, RTLD_LAZY, "sgi3.0", 0);
	}	    
#endif

	if (dsoHandle  == NULL)
	    return SoType::badType();

#ifdef SB_OS_WIN
  sprintf(dummyFunc, "?initClass@%s@@SAXXZ", nameChars);
#else

#if ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 1))
#define DUMMY_FUNC "_ZN%d%s9initClassEv"
#else
#define DUMMY_FUNC "initClass__%d%s%s"
#endif

    const char *abiName = "";
    sprintf(dummyFunc, DUMMY_FUNC, (int)name.getLength(), nameChars, abiName);

#endif  // SB_OS_WIN

  void (*dsoFunc)();
#ifdef SB_OS_WIN
  dsoFunc = (void (*)())GetProcAddress(dsoHandle, dummyFunc);
#else
  dsoFunc = (void (*)())dlsym(dsoHandle, dummyFunc);
#endif // SB_OS_WIN
  if (dsoFunc == NULL) {
#ifdef DEBUG
	    SoDebugError::post("SoType::fromName",
	       "Could not find %s::initClass in %s.",
			       nameChars, DSOFile);
#endif	    
        b = 0;
	} else {
	    (*dsoFunc)();  // Call initClass

	    // Now, try to find the type again.
        b = find(name);
        if (b == -1) {
#ifdef DEBUG
		SoDebugError::post("SoType::fromName",
			"%s::initClass did not initialize SoType!",
			nameChars);
#endif	    
        b = 0;
	    }
	}
    }

    if (b <= 0)
        return SoType::badType();

    SoType result = typeData[b].type;

    if (result.storage.isPublic == 0) {
#ifdef DEBUG
	SoDebugError::post("SoType::fromName", "%s is internal",
			   nameChars);
#endif
	return SoType::badType();
    }

    return result;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Create a new type, given it parent (badType if it has none), its
//    name, and, optionally, a method to create it and some data
//    associated with it.
//
// Use: extender, static

SoType
SoType::createType(SoType parent, SbName name,
                   void * (*createMethod)(),
                   short data)
//
////////////////////////////////////////////////////////////////////////
{
    SoType t;
    t.storage.index = typeData.size();
    t.storage.isPublic = 1;
    t.storage.data = data;

    SoTypeData td;
    td.type	= t;
    td.parent = parent;
    td.name	= name;
    td.createMethod	= createMethod;

    typeData.push_back(td);

    return t;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Make a new type act like an existing type.  "new" in this case
//    means a different subclass of an existing type-- for examply, a
//    user can create an MyCube subclass derived from SoCube.  By
//    registering it with the type system using this method, whenever
//    the database uses SoType::fromName and then
//    SoType::createInstance, an instance of MyCube will be created
//    instead of an SoCube.  Since the pointer returned from
//    createInstance may be cast to an SoCube, the instance created
//    must be a subclass of SoCube.
//
// Use: extender, static

SoType
SoType::overrideType(SoType oldType, void * (*createMethod)())
//
////////////////////////////////////////////////////////////////////////
{
#ifdef DEBUG
    if (oldType.isBad() || ! oldType.canCreateInstance())
	SoDebugError::post("SoType::overrideType",
			   "Type to override (%s) is bad: %s",
			   oldType.getName().getString());
#endif

    typeData[oldType.storage.index].createMethod = createMethod;

    return oldType;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Mark this type as internal.  This MUST be done just after the
//    type is first created.
//
// Use: private, static

void
SoType::makeInternal()
//
////////////////////////////////////////////////////////////////////////
{
    // This is gross, but necessary.  After creation, copies of the
    // type exist in two places:  the classes' classTypeId member, and
    // in the typeData array.  So, we need to change them both:
    storage.isPublic = 0;
    typeData[storage.index].type.storage.isPublic = 0;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//
//
//
// Use: private, static

size_t
SoType::getNumTypes()
//
////////////////////////////////////////////////////////////////////////
{
    return typeData.size();
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//
//
//
// Use: private, static

int
SoType::find(const SbName & name)
//
////////////////////////////////////////////////////////////////////////
{
    for (size_t i=0; i<typeData.size(); i++) {
        if (typeData[i].name == name)
            return i;
    }
    return -1;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Return the name of a given type.
//
// Use: public

SbName
SoType::getName() const
//
////////////////////////////////////////////////////////////////////////
{
    return typeData[storage.index].name;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Return the parent of a given type.
//
// Use: public

SoType
SoType::getParent() const
//
////////////////////////////////////////////////////////////////////////
{
    return typeData[storage.index].parent;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Test if we know how to create an instance
//
// Use: public

SbBool
SoType::canCreateInstance() const
//
////////////////////////////////////////////////////////////////////////
{
    return (typeData[storage.index].createMethod != NULL);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Use the creation method to create an object of the appropriate
//    type.
//
// Use: public

void *
SoType::createInstance() const
//
////////////////////////////////////////////////////////////////////////
{
    SoTypeData &data = typeData[storage.index];

    if (data.createMethod != NULL)
        return (*data.createMethod)();

    return NULL;
}

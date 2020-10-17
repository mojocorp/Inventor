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
 |      Defines the SoNodeKitListPart class.
 |      It behaves sort of like a group to the outsider, but it only allows
 |      certain types of nodes to be put beneath it.
 |
 |      It contains hidden children, and is not derived from SoGroup.
 |      The addChild(), insertChild(), etc. methods all check that the
 |      new node is allowable beneath it.
 |
 |      The container node sits beteen the SoNodeKitListPart and the children.
 |      It's type is taken in the constructor, and must be derived from
 |      SoGroup. By making the container an SoSeparator, SoSwitch, etc.,
 |      the SoNodeKitList can be made to encase its set of children in
 |      the appropriate behavior.
 |
 |   Author(s)          : Paul Isaacs
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
*/

#pragma once

#include <Inventor/misc/SoChildList.h>
#include <Inventor/fields/SoSFName.h>
#include <Inventor/fields/SoMFName.h>
#include <Inventor/fields/SoSFNode.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodekits/SoNodekitParts.h>

class SoGroup;

class SoGetMatrixAction;
class SoGLRenderAction;
class SoGetBoundingBoxAction;
class SoHandleEventAction;
class SoSearchAction;
class SoSearchAction;
class SoCallbackAction;

/// Group node with restricted children.
/// \ingroup Nodekits
/// This node class is very similar to <tt>SoGroup</tt> with the exception
/// that it specifies restrictions on the type of children that
/// it allows.  It is used by nodekits to restrict child types
/// within <em>list parts</em> (see the reference page for <tt>SoBaseKit</tt>).
///
///
/// By default, any kind of child may be added. Methods of this class
/// allow you to restrict the type of allowable children, and to lock
/// down the types so that this type list may no longer be altered.
///
///
/// Inside the <tt>SoNodeKitListPart</tt> is a <em>container</em> node, which in
/// turn contains the <em>children</em>. The <em>container</em> node is a hidden
/// child, and the type of node used may be set with
/// #setContainerType().
/// In this way, you can make the nodekitlist behave like a group, a
/// separator, or any other subclass of group. The <em>container</em> is not
/// accessible so that the nodekitlist may retain control over what kinds
/// of children are added.
///
/// \par File format/defaults:
/// \code
/// SoNodeKitListPart {
///    containerTypeName    "Group"
///    childTypeNames       ""
///    containerNode        NULL
/// }
/// \endcode
/// \sa
/// SoBaseKit,SoNodeKit,SoNodeKitDetail,SoNodeKitPath,SoNodekitCatalog,SoSceneKit,SoSeparatorKit,SoShapeKit,SoWrapperKit
class INVENTOR_API SoNodeKitListPart : public SoNode {

    SO_NODE_HEADER(SoNodeKitListPart);

  public:
    /// Deafult constructor
    SoNodeKitListPart();

    /// Gets the type of node used as the \e container.
    SoType getContainerType() const;
    /// Sets the type of node used as the \e container.
    void setContainerType(SoType newContainerType);

    /// Returns the permitted child node types. By default, any type of
    /// node is permitted, so the list contains one entry of
    /// type SoNode.
    const SoTypeList &getChildTypes() const;
    /// Permits the node type \a typeToAdd as a child.
    /// The first time the #addChildType() method is called,
    /// the default of SoNode is overridden and only the
    /// new \a typeToAdd is permitted.  In subsequent calls to
    /// #addChildType(), the \a typeToAdd is added to the existing types.
    void addChildType(SoType typeToAdd);

    /// Returns whether a node of type \a typeToCheck may be added as a child.
    SbBool isTypePermitted(SoType typeToCheck) const;
    /// Returns whether the node \a child may be added to this list.
    /// This will return TRUE if the type of \a child is one of the permissible
    /// child types.
    SbBool isChildPermitted(const SoNode *child) const;

    /// Sends a string to the SoGroup::set() method on the
    /// container node.
    /// This is how you can set the value of a switch node if the container
    /// node is an SoSwitch, for example.
    void containerSet(const char *fieldDataString);

    /// This function permanently locks the permitted child types and
    /// the container type permanently.
    /// Calls to #setContainerType() and #addChildType()
    /// will have no effect after this function is called.
    void lockTypes();
    /// Returns whether the permitted child types and the container type
    /// are locked (i.e. cannot be changed).  See #lockTypes()
    SbBool isTypeLocked() const { return areTypesLocked; }

    /// \name These are the functions used to edit the children.
    /// They parallel those of SoGroup, except that they always check
    /// the child types against those which are permissible.
    /// See SoGroup for details.
    /// @{
    void    addChild(SoNode *child);
    void    insertChild(SoNode *child, int childIndex);
    SoNode *getChild(int index) const;
    int     findChild(SoNode *child) const;
    int     getNumChildren() const;
    void    removeChild(int index);
    void    removeChild(SoNode *child) { removeChild(findChild(child)); }
    void    replaceChild(int index, SoNode *newChild);
    void    replaceChild(SoNode *oldChild, SoNode *newChild) {
        replaceChild(findChild(oldChild), newChild);
    }
    /// @}

    /// Depending on the type of the container, this may
    /// or may not affect the state.
    virtual SbBool affectsState() const;

    SoEXTENDER
  public:
    // These functions implement all actions for nodekits.
    virtual void doAction(SoAction *action);
    virtual void callback(SoCallbackAction *action);
    virtual void GLRender(SoGLRenderAction *action);
    virtual void getBoundingBox(SoGetBoundingBoxAction *action);
    virtual void getMatrix(SoGetMatrixAction *action);
    virtual void handleEvent(SoHandleEventAction *action);
    virtual void pick(SoPickAction *action);
    virtual void search(SoSearchAction *action);

    SoINTERNAL
  public:
    static void initClass();

    // Returns pointer to children
    virtual SoChildList *getChildren() const;

  protected:
    SoGroup *getContainerNode();

    // Reads in from file. Takes care of setting parts and stuff.
    virtual SbBool readInstance(SoInput *in, unsigned short flags);

    SoChildList *children;

    // Copies the contents of the given nodekit into this instance
    virtual void copyContents(const SoFieldContainer *fromFC,
                              SbBool                  copyConnections);

  protected:
    virtual ~SoNodeKitListPart();

  private:
    SoSFName containerTypeName;
    SoMFName childTypeNames;
    SoSFNode containerNode;

    SoTypeList childTypes;

    SbBool areTypesLocked;

    friend class SoBaseKit;
};

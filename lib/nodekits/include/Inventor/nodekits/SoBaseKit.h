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
 |      Defines the SoBaseKit class. A base class for all
 |      of the SoNodeKit classes
 |
 |   Author(s)          : Paul Isaacs, Thad Beier
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
*/

#ifndef  _SO_BASE_KIT_
#define  _SO_BASE_KIT_

#include <Inventor/misc/SoChildList.h>
#include <Inventor/fields/SoFieldData.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFName.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodekits/SoNodekitParts.h>
#include <Inventor/nodekits/SoSubKit.h>
#include <Inventor/SoNodeKitPath.h>

class SoSeparator;
class SbBox3f;
class SbDict;
class SoGroup;

////////////////////////////////////////////////////////////////////
//    Class: SoBaseKit -- 
//
//  This is the base class for all node kits.
//
// NOTE TO DEVELOPERS:
//     For info about the structure of SoBaseKit:
//     [1] compile: /usr/share/src/Inventor/samples/ivNodeKitStructure
//     [2] type:    ivNodeKitStructure SoBaseKit.
//     [3] The program prints a diagram of the scene graph and a table with 
//         information about each part.
//
//  It contains only 'this' and a callbackList beneath it.
//  It's catalog is made to describe the following structure:
//
//                            this
//                              |
//                            "callbackList"
//
////////////////////////////////////////////////////////////////////

/// Base class for all node kits.
/// \ingroup Nodekits
/// This is the base class from which all nodekit nodes are derived.
/// Nodekits provide a
/// convenient mechanism for creating groups of scene graph nodes with some
/// larger meaning. When you create a shape node such as an indexed face set,
/// for example, you almost always precede it with a coordinate node. You
/// may also want to add a transform node or specify properties with
/// material, drawing style, material binding, etc.
/// Instead of creating each of these nodes
/// individually and then arranging them into a subgraph,
/// you can use a nodekit of the appropriate type (in this case, <tt>SoShapeKit</tt>).
///
///
/// Each class of nodekit has a <em>nodekit catalog</em> (<tt>SoNodekitCatalog</tt>)
/// that describes the nodes in the subgraph, referred to as <em>parts</em>.
/// The catalog has an entry for each part, with information
/// such as the <em>partName</em>, <em>partType</em>, and <em>nullByDefault</em> (if FALSE
/// the constructor creates it).
/// The catalog also describes the arrangement of parts in the subgraph.
/// (Other information is described below; a complete description is in the
/// <tt>SoNodekitCatalog</tt> reference page.)
///
///
/// If we regard the scene graph arrangement as a branching
/// tree, then the top node (root) of the arrangement is always the
/// nodekit itself. The leaf nodes are those at the bottom (containing no
/// children).  Some leaves of the tree are defined in the catalog to be
/// <em>public</em> parts, while
/// other leaves are <em>private</em>. All non-leaf parts are considered internal to
/// the nodekit structure and are marked private.
/// Public parts are accessible; they may be requested, changed, or set by the
/// programmer with member functions
/// such as #getPart().  Private parts are not
/// accessible, so
/// methods such as #getPart() will have no effect on them. For example,
/// if you call #getPart() to retrieve a private part, <b>NULL</b> will be returned even when the part exists.
///
///
/// Every nodekit reference page has a Parts section describing
/// the function of each public part it adds to those inherited from its parent
/// class.
/// Also, a Catalog Parts section has tables of often-needed
/// information from the
/// catalog (part type, etc.). These tables include all public parts,
/// both new and inherited.
/// Only the public parts of a nodekit are described in the reference pages.
/// Nodekits take care of the rest for you; they automatically arrange the
/// subgraph, creating and deleting the private parts when necessary.
/// (The <tt>SoNodekitCatalog</tt> reference page has methods for finding out
///  the part names and arrangement of all parts, both public and private.)
///
///
/// The nodekit catalog is a template shared by all instances of a class.
/// They use the shared catalog  as a <em>guide</em> when creating parts
/// (i.e., constructing actual nodes),
/// but each instance stores its own parts separately.
/// Moreover, nodekits are <em>not</em> <tt>SoGroup</tt>
/// nodes, and parts are added as <em>hidden children</em>;
/// you can only access parts with the methods of <tt>SoBaseKit</tt> and its
/// derived classes.
///
///
/// Any public part may be retrieved with #getPart(),
/// installed with #setPart(),
/// or removed by giving a <b>NULL</b> argument to #setPart().
/// Paths from the nodekit down to a part can be created by
/// #createPathToPart().
///
///
/// By default, parts are not created until the user requests or sets them.
/// This keeps the subgraph uncluttered and efficient for traversal.
/// Additionally, removing a part (setting it to NULL) has the extra
/// effect of removing any internal parts that are no longer needed.
///
///
/// Since nodekits hide their children, any <tt>SoPath</tt> containing nodekits will
/// end at the topmost nodekit.  However, since nodekits may be nested within
/// other nodekits, you may wish to cast an <tt>(SoPath *)</tt> into an
/// <tt>(SoNodeKitPath *)</tt>. The methods of <tt>SoNodeKitPath</tt> allow you to view all
/// nodekits that lie on the path (see the reference page for <tt>SoNodeKitPath</tt>).
///
///
/// Public parts in the nodekit catalog fall into three categories:
///
///
/// [1] <em>regular nodes</em>
///
///
/// [2] <em>nodekits</em>, or <em>nested nodekits</em> (which may nest recursively).
/// Any node which is public in a nested nodekit is accessible to
/// the higher level nodekit(s) that contains it.  The description
/// of #getPart() below shows how to refer to nested
/// parts by name (e.g., \a "appearance.material").  This works for any
/// nodekit method that takes a part name for an argument.
///
///
/// [3] <em>lists</em>, or <em>list parts</em>. These parts group together children
/// (<em>list elements</em>) of a particular type or types.  As with nested nodekits,
/// you can refer to individual elements using notation described in
/// #getPart()
/// (e.g., \a "childList[0]", or if the list elements are in turn
/// nodekits, \a "childList[2].transform").
///
///
/// When the catalog denotes that a part is a list,
/// the part itself is always a node of type <tt>SoNodeKitListPart</tt>.
/// The catalog specifies a set of permissible \a listItemTypes and a
/// \a listContainerType for that part. It gives this information to
/// the <tt>SoNodeKitListPart</tt>
/// when it creates it. From then on, the list part will enforce
/// type checking. So even if you retrieve the <tt>SoNodeKitListPart</tt>
/// with #getPart(), you will not be able to
/// add illegal children. (See the <tt>SoNodeKitListPart</tt> reference page for more
/// information).  As an example, the <em>callbackList</em> part of <tt>SoBaseKit</tt>
/// has an <tt>SoSeparator</tt>
/// container and allows only <tt>SoCallback</tt> and <tt>SoEventCallback</tt> nodes
/// in the list.
/// Children may be added, retrieved, and removed from an <tt>SoNodeKitListPart</tt>
/// node using
/// methods that parallel those of <tt>SoGroup</tt>. However, type-checking is strictly
/// enforced.
///
///
/// Note that, although all public parts are leaves in the nodekit catalog,
/// you are free to add children to them (assuming that they are groups, nodekits,
/// or list parts).  A part's status as a leaf in the catalog
/// just means that the nodekit will not manage the part's  children.
/// For example, <tt>SoWrapperKit</tt> has a part called <em>contents</em> with a
/// part type of <tt>SoSeparator</tt>. You can put whatever you want underneath the
/// separator, as long as <em>contents</em> itself is an <tt>SoSeparator</tt>.
///
///
/// Thus, a nodekit only controls a section of the scene graph.  Above and
/// below that section, anything goes.
///
///
/// However, when nodekits are nested, they effectively create a larger
/// `known' section of the scene graph.
/// For example, the <em>appearance</em> part of the <tt>SoSeparatorKit</tt> is a leaf
/// node in the <tt>SoSeparatorKit</tt> catalog. But <em>appearance</em> is in turn an
/// <tt>SoAppearanceKit</tt>, containing parts such as <em>material</em> and <em>drawStyle</em>.
/// The two nodekits combine to make an even larger template, which the
/// <tt>SoSeparatorKit</tt> can examine by looking at the catalogs for both classes.
/// So an <tt>SoSeparatorKit</tt> can successfully return a part named \a "material";
/// first it finds (or creates)  the <em>appearance</em> part, then it gets the
/// <em>material</em> by calling #getPart()
/// on the <em>appearance</em>.
///
///
/// When the catalog defines the \a listItemTypes of a list part to be
/// nodekits, the name-able space expands further. For example,
/// <tt>SoSeparatorKit</tt> has a part <em>childList</em> which
/// permits only <tt>SoSeparatorKits</tt>, so each list element can
/// be further searched. Hence the name
/// #"childList[0].childList[1].childList[2].material" is perfectly legal.
///
/// \par Action behavior:
/// <b>SoGLRenderAction, SoCallbackAction, SoGetBoundingBoxAction, SoHandleEventAction</b>
/// Behaves like an <tt>SoGroup</tt>. Traverses each child in order.
/// <b>SoRayPickAction</b>
/// Traverses each child in order. Then, for any pick containing the kit
/// on its path, makes an <tt>SoNodeKitDetail</tt> as follows:
/// Sets the <b>"detailNodeKit"</b> (retrievable with
/// #SoNodeKitDetail::getNodeKit())
/// to be a pointer to itself.
/// Sets the <b>"detailPart"</b> (retrievable with
/// #SoNodeKitDetail::getPart())
/// to be a pointer to the kit's leaf-most part that lies on the pickPath.
///  Sets the <b>"detailPartName"</b> (retrievable with
/// #SoNodeKitDetail::getPartName())
/// to be the partName of that part, as found in the catalog.
///
///
/// Does not descend into nested nodekits. Each nodekit along the path
///  is the <b>"detailPart"</b> in its parent's detail.
///  However, if the pick path goes through a list part, a pointer to the child
///  is used for the <b>"detailPart"</b>, and  <b>"detailPartName"</b> is of the
///  form \a "listName[i]".
/// <b>SoGetMatrixAction</b>
/// Behaves like an <tt>SoGroup</tt>.
/// Does nothing unless the kit is in the middle of the path chain the
/// action is being applied to. If so, the children up to and including
/// the next node in the chain are traversed.
/// <b>SoSearchAction</b>
/// First, searches itself like an <tt>SoNode</tt>.
/// Then, checks the value of
/// #isSearchingChildren().
/// If TRUE, traverses the children in order. If FALSE, returns.
/// <b>SoWriteAction</b>
/// Begins by writing out regular fields, then writes out the parts.
/// A nodekit does <em>not</em> write out its parts the way an <tt>SoGroup</tt> writes
/// out its children.  Instead, it writes each part as an <tt>SoSFNode</tt> field.
/// First the partName is written, then the node being used for that part.
///
///
/// To keep the files terse, nodekits write out as few parts as possible.
/// However, nodekits <em>always</em> write a part if another instance or a path is
/// writing it. If this is not the case, parts are left out according to
/// the following rules:
///
///
/// [1] NULL parts only write if the catalog states they are created by default.
///
///
/// [2] Empty <tt>SoGroup</tt> and <tt>SoSeparator</tt> nodes do not write.
///
///
/// [3] Non-leaf parts only write if they have non-default field values.
///
///
/// [4] List parts only write if they have children or if the container node has
/// non-default field values.
///
///
/// [5] Nested nodekit parts only write if they need to write one or more parts,
/// or if they have non-default field values.
/// \par Nodekit structure:
/// \code
/// CLASS SoBaseKit
/// -->"this"
/// -->   "callbackList"
/// \endcode
///
/// \par File format/defaults:
/// \code
/// SoBaseKit {
///     callbackList	NULL
/// }
/// \endcode
/// \sa SoAppearanceKit,SoCameraKit,SoLightKit,SoNodeKit,SoNodeKitDetail,SoNodeKitListPart,SoNodeKitPath,SoNodekitCatalog,SoSceneKit,SoSeparatorKit,SoShapeKit,SoWrapperKit
class INVENTOR_API SoBaseKit : public SoNode {

    // Instead of calling SO_KIT_HEADER(className) here,
    // which is the norm for all nodekits, explicitly write the contents
    // of that macro, so as to expose the base class virtual function
    // getNodekitCatalog for the C-api...
    //
    SO_NODE_HEADER(SoBaseKit);

  private:
    static SoNodekitCatalog *nodekitCatalog; /* design of this class */
    static const SoNodekitCatalog **parentNodekitCatalogPtr; /* parent design */
  public:
    /// Returns the \c SoNodekitCatalog for the class \c SoBaseKit.
    static const SoNodekitCatalog *getClassNodekitCatalog()
	{ return nodekitCatalog; }

    /// eturns the \c SoNodekitCatalog for this instance of \c SoBaseKit.
    /// While each instance of a given class creates its own distinct set
    /// of parts (which are actual nodes), all instances
    /// share the same catalog (which
    /// describes the parts but contains no actual node pointers).
    virtual const SoNodekitCatalog *getNodekitCatalog() const;

  protected:
    static const SoNodekitCatalog **getClassNodekitCatalogPtr()
	{ return (const SoNodekitCatalog **)&nodekitCatalog; }

    // Define fields for the new parts of the kit...
    SO_KIT_CATALOG_ENTRY_HEADER(callbackList);

  public:

    /// Constructor.
    SoBaseKit();

    /// Searches the nodekit catalog (and those of all nested nodekits) for the part
    /// named \a partName.
    /// Returns a pointer to the part if a \e "match is found", the part is \e public,
    /// and the part has \e "already been built".
    /// If no match is found, or if the part is \e private, <b>NULL</b> is returned.
    /// If \a partName is in the catalog (or that of one of its nested nodekit parts),
    /// but the part has not been built yet, the argument \a makeIfNeeded
    /// determines the course of action. When \a makeIfNeeded is <b>FALSE</b>,
    /// <b>NULL</b> is returned; when \a makeIfNeeded is <b>TRUE</b>,
    /// #getPart() will create the part (as well as any
    /// necessary intermediary parts), put it in the
    /// correct place, and return a pointer to the newly created part.
    ///
    /// Elements of \e "list parts" and parts within nested nodekits can
    /// all be retrieved with #getPart().  The full syntax for legal \a partName
    /// arguments is given below.
    ///
    /// \e "Part name BNF notation":
    ///
    /// <b>partName</b> = singleName | compoundName
    ///
    /// <b>compoundName</b> = singleName | compoundName.singleName
    ///
    /// <b>singleName</b> = singlePartName | singleListElementName
    ///
    /// <b>singlePartName</b> = the name of any single part in the catalog (including
    /// those that are lists or nodekits), or in the recursively nested
    /// catalogs of any of its parts.
    ///
    /// <b>singleListElementName</b> = singleListName[index]
    ///
    /// <b>singleListName</b> = the name of any single list-type part in the
    /// catalog, or in the recursively nested catalogs of any of its parts.
    ///
    /// <b>index</b> = integer
    ///
    /// Examples of valid part names are:
    ///
    /// "transform", "appearance.material", "childList[2].drawStyle",
    /// "foot", "bird.leftLeg.foot", "octopus.leg[4].suctionCup[2].material"
    virtual SoNode *getPart( const SbName &partName, SbBool makeIfNeeded );

    /// Given a node or a path to a node, checks if the part exists in the
    /// nodekit, in a nested nodekit, or an element of a list part.
    /// If so, returns a string describing the part name; otherwise, returns an
    /// empty string ("").
    SbString getPartString( const SoBase *part );
		     
		     
    /// Returns a path that begins at this nodekit and ends at \a partName.
    /// Searching for the part is the same as in #getPart()
    /// <b>NULL</b> is returned if \a partName cannot be found,
    /// or if \a makeIfNeeded is <b>FALSE</b> and the part is not yet built.
    /// If the the part is retrieved and
    /// the argument \a pathToExtend is <b>NULL</b>,
    /// the path returned begins at <b>this</b> and ends at \a partName.
    /// If \a pathToExtend is not <b>NULL</b>, the path created is a copy of
    /// \a pathToExtend with entries appended all the way down to \a partName.
    /// It is okay for \a pathToExtend to go beyond the nodekit; extra nodes will
    /// be popped off the tail before continuing from <b>this</b> down
    /// to \a partName.
    virtual SoNodeKitPath *createPathToPart( const SbName &partName, 
		    SbBool makeIfNeeded, const SoPath *pathToExtend = NULL );
		   
		   
    /// Inserts the given node (not a copy) as the new part specified by
    /// \a partName.  See #getPart() for the syntax of \a partName.
    /// This method adds any extra nodes needed to fit the part into the
    /// nodekit's catalog. For example, if you call:
    /// \code
    /// mySepKit->setPart("childList[0]", myNewChild);
    /// \endcode
    /// the kit may need to create the part
    /// \e childList before it can install \a myNewChild.
    /// Run-time type checking verifies that the node type of \a newPart
    /// matches the type called for by \a partName.  For example,
    /// if \a partName was a \e material for an \c SoSeparatorKit, but
    /// \a newPart was an \c SoTransform node, then the node would not be installed,
    /// and <b>FALSE</b> would be returned.
    ///
    /// If \a newPart is <b>NULL</b>, then the node specified by \a partName is removed.
    /// If this renders any private parts useless (as occurs when you remove the last
    /// child of an \c SoGroup node), they will also be removed. Hence nodekits do not
    /// retain unnecessary nodes.
    ///
    /// <b>TRUE</b> is returned on success, and <b>FALSE</b> upon error.
    virtual SbBool setPart( const SbName &partName, SoNode *from );

    /// These functions allow field values of parts (nodes) to be set.
    /// If \a partName and \a parameters are used, then a single part is
    /// specified by \a partName; the field values
    /// are specified in \a parameters. The format of \a paramaters
    /// is the Inventor File Format syntax.  For example,
    /// \code
    /// mySepKit->set("material", "diffuseColor 1 0 0 shininess 0.6");
    /// \endcode
    /// sets the part \e material to the values
    /// <b>"diffuseColor 1 0 0 shininess 0.6"</b>  The values
    /// used in \a parameters must of course be appropriate for the node-type
    /// to which \a partName belongs.
    /// In this case, the nodekit \c SoSeparatorKit has a part named \e material
    /// which is of type \c SoMaterial.
    ///
    /// The \a nameValuePairs syntax can be used to set the field values
    /// in several different parts simultaneously.  In this case,
    /// the argument string, \a nameValuePairs contains \e name-value pairs:
    /// <b>"partName1 { parameters1 \} ... partNameN { parametersN \}"</b>.
    ///
    /// For example,
    /// \code
    /// mySepKit->set("material { diffuseColor 1 1 1 \}
    ///             transform { translation 4 3 .6 \}");
    /// mySepKit->set("childList[0].material { ambientColor .5 .5 .5 \}");
    /// \endcode
    SbBool set(const SbString & nameValuePairListString);
    SbBool set(char *partNameString, char *parameterString);

    /// Sets and queries if nodekit children are searched during \c SoSearchAction
    /// traversal.  By default, they are not.
    static SbBool isSearchingChildren() { return searchingChildren; }
    static void   setSearchingChildren( SbBool newVal );

  SoEXTENDER public:
    virtual void doAction( SoAction *action );
    // These functions implement all actions for nodekits.
    virtual void callback( SoCallbackAction *action );
    virtual void GLRender( SoGLRenderAction *action );
    virtual void getBoundingBox( SoGetBoundingBoxAction *action );
    virtual void getMatrix(SoGetMatrixAction *action );
    virtual void handleEvent( SoHandleEventAction *action );
    virtual void rayPick( SoRayPickAction *action );
    virtual void search( SoSearchAction *action );
    virtual void write( SoWriteAction *action );

  SoINTERNAL public:
    static void initClass();

    // Returns pointer to children
    virtual SoChildList *getChildren() const;

    static SoNode *typeCheck( const SbName &partName, const SoType &partType, 
			      SoNode *node );

    void printDiagram();
    void printSubDiagram( const SbName &rootName, int level );
    void printTable();

    // Overrides the default method to use countMyFields() instead of 
    // the regular SoFieldData writing mechanism.
    virtual void	addWriteReference(SoOutput *out,
					  SbBool isFromField = FALSE);

    friend class SoNodekitCatalogEntry;
    friend class SoNodekitParts;
    friend class SoV1BaseKit;

    // This returns TRUE if the nodekit intends to write out.
    //   [a] call shouldWrite(). If TRUE, trivial return.
    //   [b] If the kit thinks it shouldn't write, it first does a recursive
    //       call to its children.  If any children must write, then so must
    //       the kit.
    //   [c] If kit has changed its mind because of [b], then add a writeRef.
    //
    //   [d] If kit should not write, it will delete the fieldDataForWriting, 
    //       since there will no writing pass applied to take care of this.
    SbBool forceChildDrivenWriteRefs( SoOutput *out );

  protected:

    SoChildList *children;

    // Redefines this to add this node and all part nodes to the dictionary
    virtual SoNode *	addToCopyDict() const;

    // Copies the contents of the given nodekit into this instance
    virtual void	copyContents(const SoFieldContainer *fromFC,
				     SbBool copyConnections);

    // Returns the containerNode within the SoNodeKitListPart given 
    // by listName.
    SoGroup *getContainerNode( const SbName &listName,
			       SbBool makeIfNeeded = TRUE );

    // like their public versions, but are allowed access to non-leaf
    // and private parts.
    // These are virtual so subclasses may do extra things
    // when certain parts are requested.
    virtual SoNode *getAnyPart( const SbName &partName, SbBool makeIfNeeded, 
			SbBool leafCheck = FALSE, SbBool publicCheck = FALSE );
    virtual SoNodeKitPath *createPathToAnyPart(const SbName &partName, 
		    SbBool makeIfNeeded, 
		    SbBool leafCheck = FALSE, SbBool publicCheck = FALSE,
		   const SoPath *pathToExtend = NULL );
    virtual SbBool setAnyPart( const SbName &partName, SoNode *from, 
		       SbBool anyPart = TRUE );

    // parts list.
    SoNodekitParts	*nodekitPartsList;

    // called during construction to create parts list
    void createNodekitPartsList();
    // called during construction to create any parts that are created by
    // default (such as the cube in the SoCubeKit)
    virtual void createDefaultParts();

    // Return the node's partsList
    const SoNodekitParts *getNodekitPartsList() const 
			    { return nodekitPartsList; };

    // Prints an Error when there's trouble building a catalog.
    void catalogError();

    // detach/attach any sensors, callbacks, and/or field connections.
    // Called by:            start/end of SoBaseKit::readInstance
    // and on new copy by:   start/end of SoBaseKit::copyContents.
    // Classes that redefine must call setUpConnections(TRUE,TRUE) 
    // at end of constructor to add their own connections to the ones already
    // connected by the base classes.
    // The doItAlways flag can force the method to do the work.
    // But if (doItAlways == FALSE && onOff == connectionsSetUp), then
    // the method will return immediately without doing anything.
    // Returns the state of the node when this was called.
    virtual SbBool setUpConnections( SbBool onOff, SbBool doItAlways = FALSE );
    SbBool connectionsSetUp;

    // Reads in from file. Takes care of setting parts and stuff.
    virtual SbBool readInstance(SoInput *in, unsigned short flags);
    
    // This is called during countMyFields() method. It calls setDefault()
    // on those fields we do not wish to write out. setDefault does not change
    // the field value, but it sets a flag indicating that the field should
    // not get written by this node.
    // Fields that we do not write are [a] parts that are NULL and are defined
    // in the catalog to be NULL by default. [b] non-leaf parts for which
    // isNodeFieldValuesImportant() is FALSE.
    // [c] leaf parts that are empty groups, empty separators, 
    // [d] leaf parts that are empty lists (but they must have group or 
    //     separator container nodes.
    virtual void setDefaultOnNonWritingFields();

    // This is called during addWriteReference() to write the fields.
    // It sometimes treats fields for parts of the nodekit in a special way.
    // Under normal circumstances, they write like other fields.
    // The special case is when a part-field has a non-NULL value, but has 
    // also been set to default, indicating that we would rather not write it.
    // If the part is a regular node, we give it a field-connection style
    // write ref.  So it only writes if some other instance of the node forces
    // a write.
    // If the part is a nodekit, then we additionally write ref the fields of 
    // the nodekit, using this same method.
    // Later, in the WRITE stage, the kit-part will be written if at least
    // one of its fields or ancestor parts has shouldWrite() ==TRUE.
    // This way, if a nodekit part or any of its ancestors has non-NULL fields,
    // it will later be forced to write, even though it has been set to default.
    // Example: 
    //   Parent draggers attempt not to write out child draggers.
    //   But the parentDragger must at least traverse the childDragger to see 
    //   if any of the part geometry has been changed from its default. Such 
    //   changes must be written to file.
    void countMyFields(SoOutput *out);

    virtual ~SoBaseKit();

  private:
    void skipWhiteSpace(char *&string);

    // Returns FALSE if every field has either [a] hasDefaultValue() == TRUE or
    // [b] isDefault() == TRUE
    SbBool isNodeFieldValuesImportant( SoNode *n );

    // This creates a copy of fieldData with a few changes that make
    // it write out 'prettier'
    void createFieldDataForWriting();
    // Used to store field data during writing. Need this because a temporary
    // version of fieldData is made that differs from the real thing.
    SoFieldData *fieldDataForWriting;

    // Called by the readInstance routine...
    SbBool readMyFields(SoInput *in, SoFieldData *&unknownFieldData );

    // Called by write() after the (virtual) setDefaultOnNonWritingNodes() 
    // method.  This method looks at the part fields which have isDefault()
    // set to TRUE.    This method will setDefault(FALSE) on any part fields
    // that MUST write.   
    //     This happens when the part-field is for a part whose parent 
    //     is going to write out anyway. Therefore, it will appear in file as 
    //     a node within this kit, so we better write out the part field to 
    //     explain where the node belongs in the kit.
    void undoSetDefaultOnFieldsThatMustWrite();

    // Only nodekits themselves can access the private children.
    int  getNumChildren() const { return (children->getLength()); }
    void removeChild( int index );
    void removeChild( SoNode *child  ) { removeChild(findChild(child)); }
    void addChild( SoNode *child );
    int  findChild( const SoNode *child ) const;
    void insertChild( SoNode *child, int newChildIndex );
    SoNode *getChild( int index) const { return (*children)[index]; }
    void replaceChild( int index, SoNode *newChild);
    void replaceChild( SoNode *oldChild, SoNode *newChild)
	{ replaceChild(findChild(oldChild),newChild); }

    static SbBool searchingChildren;
};

// Macro for getting at parts without doing type casting yourself:
// for example, the code:
//  xf = (SoTransform *) myKit->getPart("transform",
//					SoTransform::getClassTypeId(), TRUE);
// becomes:
//  xf = SO_GET_PART( myKit, "transform", SoTransform );
//      
// The macro SO_CHECK_PART will not build the part if it is not already in the
// kit, since it sends 'FALSE' as the 'makeIfNeeded' argument.
//
// CAUTION:  These macros will not work in the C-api, and nothing will be
//           gained from making them.  So, don't make them.


#ifdef DEBUG
#define SO_GET_PART( kitContainingPart, partName, partClassName )              \
        ((partClassName *) SoBaseKit::typeCheck( partName,                     \
			       partClassName::getClassTypeId(),               \
			       kitContainingPart->getPart( partName, TRUE )))

#define SO_CHECK_PART( kitContainingPart, partName, partClassName )            \
        ((partClassName *) SoBaseKit::typeCheck( partName,                    \
			       partClassName::getClassTypeId(),               \
			       kitContainingPart->getPart( partName, FALSE )))

#define SO_GET_ANY_PART( kitContainingPart, partName, partClassName )         \
        ((partClassName *) SoBaseKit::typeCheck( partName,                    \
		partClassName::getClassTypeId(),                              \
		kitContainingPart->getAnyPart( partName, TRUE, FALSE, FALSE )))

#define SO_CHECK_ANY_PART( kitContainingPart, partName, partClassName )       \
        ((partClassName *) SoBaseKit::typeCheck( partName,                    \
		partClassName::getClassTypeId(),                              \
		kitContainingPart->getAnyPart( partName, FALSE, FALSE, FALSE )))
#else

#define SO_GET_PART( kitContainingPart, partName, partClassName )             \
        ((partClassName *) kitContainingPart->getPart( partName, TRUE ))
#define SO_CHECK_PART( kitContainingPart, partName, partClassName )           \
        ((partClassName *) kitContainingPart->getPart( partName, FALSE ))
#define SO_GET_ANY_PART( kitContainingPart, partName, partClassName )        \
        ((partClassName *) kitContainingPart->getAnyPart( partName, TRUE,    \
							  FALSE, FALSE ))
#define SO_CHECK_ANY_PART( kitContainingPart, partName, partClassName )      \
        ((partClassName *) kitContainingPart->getAnyPart( partName, FALSE,   \
							  FALSE, FALSE ))
#endif

#endif  /* _SO_BASE_KIT_ */

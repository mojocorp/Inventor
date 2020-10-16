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
 |   $Revision $
 |
 |   Description:
 |	This file contains definitions of the SoTranSender and SoTranReceiver
 |	classes used for transcribing Inventor data. Transcribing is the
 |	process of packaging changes to a database and sending them over a
 |	"wire" to another database.
 |
 |   Classes:
 |	SoTranSender, SoTranReceiver
 |
 |   Author(s)		: Paul S. Strauss
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#pragma once

#include <string>
#include <map>
#include <Inventor/misc/SoBasic.h>

class SoGroup;
class SoInput;
class SoNode;
class SoOutput;

//////////////////////////////////////////////////////////////////////////////
//
//  Class: SoTranSender
//
// This stores database changes that are to be transcribed. The
// changes are written to an SoOutput instance allocated and managed
// by the caller.
//
// These database change routines are supported:
//
//   INSERT node [parent n]
//	Creates and adds a node to other database. With 1 argument,
//	the node is added as the last child of the root of the
//	database. With 3 arguments, the node is added as the nth
//	child of the given parent. (The parent may be NULL to
//	indicate the root.) Note that if the inserted node is already
//	in the graph, a link is made from the parent to the existing
//	version of the node. (Therefore, if the new node is different
//	from the old one, any changes are lost.)
//
//   REMOVE parent n
//	Removes nth child from the given parent node. (The parent may
//	be NULL to indicate the root.)
//
//   REPLACE parent n newNode
//	This is exactly equivalent to "REMOVE parent n" followed by
//	"INSERT newNode parent n".
//
//   MODIFY node
//	Updates the field data for the given node to the new
//	contents. Note that this changes only field data; children of
//	groups are not affected, nor is any non-field instance data.
//
//////////////////////////////////////////////////////////////////////////////

/// Sends database changes for transcription.
/// \ingroup General
/// This class is used for transcribing Inventor data.  Transcription is
/// the process of packaging changes to a database and sending them over a
/// "wire" to another database.
///
///
/// The <tt>SoTranSender</tt> class is used on the sending side of
/// transcription.  It packages up changes to a Inventor database into a
/// file or memory area defined by an <tt>SoOutput</tt> instance. It supports a
/// limited set of changes to a database; each change is stored as a
/// command in the transcription area.  The <tt>SoTranReceiver</tt> class can
/// be used at the other end to interpret the transcribed commands.
/// \sa SoOutput, SoTranReceiver
class SoTranSender {
  public:
    /// The constructor takes a pointer to an SoOutput instance that
    /// determines what the transcription area is (file or memory).
    SoTranSender(SoOutput *output);

    /// Destructor
    ~SoTranSender() {}

    /// Returns pointer to current SoOutput instance.
    SoOutput *getOutput() const { return out; }

    /// Adds an INSERT command to the transcription area. The given node will
    /// be added as the last child of the root node on the receiving end.
    void insert(SoNode *node);

    /// Adds an INSERT command to the transcription area. The given node will
    /// be added as the \a n th child of the given parent node on the
    /// receiving end. A NULL parent node causes the node to be added to the
    /// receiving end's root node.
    void insert(SoNode *node, SoNode *parent, int n);

    /// Adds a REMOVE command to the transcription area. The \a n th child of
    /// the given (non-NULL) parent node on the receiving end will be removed.
    void remove(SoNode *parent, int n);

    /// Adds a REPLACE command to the transcription area. The \a n th child of
    /// the given (non-NULL) parent node on the receiving end will be replaced
    /// with \a newNode.
    void replace(SoNode *parent, int n, SoNode *newNode);

    /// Adds a MODIFY command to the transcription area. Updates the field
    /// data for the given node to the new contents. Note that this changes
    /// only field data; children of groups are not affected, nor is any
    /// non-field instance data.
    void modify(SoNode *node);

    /// Prepares a SoTranSender instance for transcription, making sure
    /// the transcription area is complete and all packaged to go. This must
    /// be called before the transcription can be performed.
    void prepareToSend();

  private:
    SoOutput *out;

    // Adding items to send
    void addBytes(const void *bytes, size_t nBytes);
    void addCommand(int command);
    void addInt(int n);
    void addNode(SoNode *node, SbBool addNames = TRUE);
    void addNodeNames(const SoNode *root);
    void addNodeRef(const SoNode *node);
    void addString(const char *cmdString);

    friend class SoTranReceiver;
};

/// Interprets database changes for transcription.
/// \ingroup General
/// This class is used for transcribing Inventor data. Transcription is
/// the process of packaging changes to a database and sending them over a
/// "wire" to another database.
///
///
/// The <tt>SoTranReceiver</tt> class is used on the receiving side of
/// transcription. It interprets changes to a Inventor database packaged
/// up by an <tt>SoTranSender</tt>.
/// \sa SoInput, SoTranSender
class SoTranReceiver {

  public:
    /// The constructor takes a pointer to an SoGroup instance that is the
    /// root node of the scene graph on the receiving end. All changes to the
    /// database are made relative to this root.
    SoTranReceiver(SoGroup *rootNode);

    /// Destructor
    ~SoTranReceiver();

    /// Interprets the transcription commands found in the given input stream.
    /// Returns FALSE on any error. Note: some errors (such as invalid node
    /// references) are recoverable, while others (such as syntax errors) are
    /// not.
    SbBool interpret(SoInput *in);

  private:
    SoGroup *root;
    struct SoTranDictEntry {
        SoNode *node;     // Pointer to node
        int32_t refCount; // Number of times node is added to dict
    };
    std::map<std::string, SoTranDictEntry>
        nameToEntryDict; // Maps node keyname to SoTranDictEntry
    std::map<SoNode *, std::string>
        nodeToNameDict; // Maps node pointer to node keyname

    // Interprets one database change command (with given code) from stream.
    // Sets done to TRUE if end command was found. Returns T/F error status.
    SbBool interpretCommand(int commandCode, SoInput *in, SbBool &done);

    // Gets a node and node names from the input
    SbBool getNodeAndNames(SoInput *in, SoNode *&node);

    // Gets node from input
    SbBool getNode(SoInput *in, SoNode *&root);

    // Recursively gets node names and sets up dictionaries.
    SbBool getNodeNames(SoInput *in, SoNode *root, SbBool lookForNode,
                        SoNode *&oldRoot);

    // Gets reference to a node, looks it up in dictionary, returns
    // node pointer.
    SbBool getNodeReference(SoInput *in, SoNode *&node);

    // Removes reference to node in dictionaries, recursively.
    void removeNodeReferences(SoNode *node);

    // Adds an entry to the dictionaries
    void addEntry(SoNode *node, const std::string &name);
};


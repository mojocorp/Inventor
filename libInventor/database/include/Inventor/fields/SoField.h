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
 |	Defines the SoField class, which is the base class for all fields.
 |
 |   Author(s)		: Paul S. Strauss, Nick Thompson, Gavin Bell
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef  _SO_FIELD_
#define  _SO_FIELD_

#include <Inventor/SbString.h>
#include <Inventor/SoType.h>
#include <Inventor/misc/SoNotification.h>

class SoEngineOutput;
class SoFieldContainer;
class SoFieldAuditorInfo;
class SoFieldConverter;
class SoFieldList;
class SoInput;
class SoNotList;
class SoOutput;

/// Base class for all fields.
/// \ingroup Fields
/// <tt>SoField</tt> is the abstract base class for all fields. Fields
/// are the data elements contained within nodes and are the input values
/// for engines.
/// Each node or engine class specifies a set of fields and associates a
/// name with each.
/// These names define the semantics of the field (e.g., the <tt>SoCube</tt>
/// node contains three float fields named width, height, and depth).
/// Field classes provide the access methods that indirectly allow editing
/// and querying of data within nodes.
///
/// There are two abstract subclasses of <tt>SoField</tt>: <tt>SoSField</tt>
/// is the base class for all single-valued field classes and
/// <tt>SoMField</tt> is the base class for all multiple-valued fields, which
/// contain dynamic arrays of values. Subclasses of <tt>SoSField</tt> have an
/// <tt>SoSF</tt> prefix, and subclasses of <tt>SoMField</tt> have an <tt>SoMF</tt>
/// prefix.  See the reference pages for <tt>SoSField</tt> and <tt>SoMField</tt> for
/// additional methods.
///
/// Fields are typically constructed only within node or engine
/// instances; if you need a field that is not part of a node or engine,
/// you can create a <tt>GlobalField</tt>; see the methods on <tt>SoDB</tt> for
/// creating global fields.
///
/// Fields can be connected either directly to another field, or can be
/// connected to the output of an engine.  The value of a field with a
/// connection will change when the thing it is connected to changes.  For
/// example, consider a field "A" that is connected from "B" (by
/// #A->connectFrom(B)).
/// When B's value is changed, A's value will also
/// change.  Note that A and B may have different values, even if they are
/// connected: if A's value is set after B's value, A's value will be
/// different from B's until B's value is set.
///
/// A field can be connected to several other fields, but can be connected
/// from only one source.
///
/// It is possible (and often useful) to create loops of field
/// connections (for example, A connected from B and B connected from A).
/// If there are loops, then the rule is that the last
/// #setValue() done
/// overrides any connections in to that value.  You can think of setting
/// the value of a field as immediately propagating that value forward
/// into all the fields it is connected to, with the propagation stopping
/// at the place where the original #setValue() occurred if there is a
/// connection loop.  (Actually, a more efficient mechanism than this is
/// used, but the semantics are the same.)
///
/// If you try to connect two fields of differing types, Inventor will
/// automatically try to insert a field converter engine between them to
/// convert values from one type into the other.  Inventor has most
/// reasonable conversions built-in (multiple-valued field to single-valued
/// and vice versa, anything to SoSFString, anything to
/// SoSFTrigger, float/short/unsigned short/int32_t/uint32_t/etc
/// numeric conversions, etc). You can add field converters using <tt>SoDB</tt>'s
/// extender method #addConverter(); see the SoDB.h header file for
/// details.  You can also find out if a converter is available with the
/// #SoDB::getConverter() method.
///
/// Fields each define their own file format for reading and being written
/// to files, but all fields follow the same conventions:
///
/// Fields in a node or engine are written as the name of the field
/// followed by the field's value; fields are not written if they have not
/// been modified since they were created (if they have their default
/// value).
///
/// The ignored flag is written as a "~" character after the field's
/// value (if the field's value is its default value, just the "~" is
/// written).
///
/// Field connections are written as an "=" followed by the container of
/// the field or engine output that the field is connected to, followed by
/// a "." and the name of the field or engine output.  For example:
///
/// DEF node1 Transform { translation 1 1 1 }
/// DEF node2 Scale { scaleFactor 1 1 1 = USE node1.translation }
///
/// Global fields are written as part of an internal <tt>SoFieldContainer</tt>
/// class called \cGlobalField\., which writes out an SoSFName field named
/// <b>type</b> whose value is the type of the global field, followed by
/// a field of that type whose name is the name of the global field.  For
/// example, a global uint32_t field called "FrameCounter" whose
/// value is 494 would be written as:
/// \code
/// GlobalField {
///     type SoSFUInt32
///     FrameCounter 494
/// }
/// \endcode
/// \sa SoSField, SoMField, SoNode, SoDB
class INVENTOR_API SoField {
  public:
    /// Destructor
    virtual ~SoField();

    /// Sets the ignore flag for this field. When a field's ignore flag
    /// is set to TRUE, the field is not used during traversal for rendering
    /// and other actions. The default value for this flag is FALSE.
    void		setIgnored(SbBool ig);

    /// Gets the ignore flag for this field.
    SbBool		isIgnored() const	{ return flags.ignored; }

    /// Gets the state of default flag of the field. This flag will be TRUE
    /// for any field whose value is not modified after construction and will
    /// be FALSE for those that have changed (each node or engine determines
    /// what the default values for its fields are).  Note: the state of this
    /// flag should not be set explicitly from within applications.
    SbBool		isDefault() const	{ return flags.hasDefault; }

    /// Return the type identifier for this field class.
    static SoType	getClassTypeId()	{ return classTypeId; }

    /// Return the type identifier for this field instance (SoField *).
    virtual SoType	getTypeId() const = 0;

    /// Returns TRUE if this field is the given type or derived from that
    /// type. This is typically used
    /// with the getClassTypeId() method to determine the type of an SoField *
    /// at run-time:
    /// \code
    /// SoField *field = ....;
    /// if (field->isOfType(SoSFFloat::getClassTypeId())) {
    ///    SoSFFloat *floatField = (SoSFFloat *)field);
    ///    floatField->setValue(4.5);
    /// }
    /// \endcode
    SbBool		isOfType(SoType type) const;

    /// Field connections may be enabled and disabled.  Disabling a field's
    /// connection is almost exactly like disconnecting it; the only
    /// difference is that you can later re-enable the connection by calling
    /// #enableConnection(TRUE).  Note that disconnecting an engine output can
    /// cause the engine's reference count to be decremented and the engine to
    /// be deleted, but disabling the connection does not decrement its
    /// reference count.
    ///
    /// Re-enabling a connection will cause the value of the field to be
    /// changed to the engine output or field to which it is connected.
    ///
    /// A field's connection-enabled status is maintained even if the field is
    /// disconnected or reconnected.  By default, connections are enabled.
    void		enableConnection(SbBool flag);

    /// Returns FALSE if connections to this field are disabled.  Note that this
    /// may return FALSE even if the field is not connected to anything.
    SbBool		isConnectionEnabled() const
	{ return flags.connectionEnabled; }

    /// Connects this field to an engine output.
    /// If the field was connected to something before, it will be
    /// automatically disconnected (a field may have only one connection
    /// writing into it at a time).  Unless connections to the field are
    /// disabled (see #enableConnection(), the field's value will be
    /// set to the value of the thing it is connected to.
    SbBool		connectFrom(SoEngineOutput *engineOutput);

    /// Connects this field to another field.
    /// If the field was connected to something before, it will be
    /// automatically disconnected (a field may have only one connection
    /// writing into it at a time).  Unless connections to the field are
    /// disabled (see #enableConnection(), the field's value will be
    /// set to the value of the thing it is connected to.
    SbBool		connectFrom(SoField *field);

    /// Disconnect the field from whatever it was connected to. This does
    /// nothing if the field was not connected.
    void		disconnect();

    /// Returns TRUE if the field is connected to anything.
    SbBool		isConnected() const	{ return flags.connected; }

    /// Returns TRUE if the field is connected to an engine's output.
    SbBool		isConnectedFromEngine() const
	{ return (flags.connected &&   flags.fromEngine); }

    /// Returns TRUE if the field is connected to another field.
    SbBool		isConnectedFromField() const
	{ return (flags.connected && ! flags.fromEngine); }

    /// Returns TRUE if this field is being written into by an engine, and
    /// returns the engine output it is connected to in \a engineOutput.  Returns
    /// FALSE and does not modify \a engineOutput if it is not connected to an
    /// engine.
    SbBool		getConnectedEngine(SoEngineOutput *&engineOutput) const;

    /// Returns TRUE if this field is being written into by another field, and
    /// returns the field it is connected to in \a writingField.  Returns
    /// FALSE and does not modify \a writingField if it is not connected to a
    /// field.
    SbBool		getConnectedField(SoField *&field) const;

    /// Adds pointers to all of the fields that this field is writing into
    /// (either fields in nodes, global fields or engine inputs) to the given
    /// field list, and returns the number of forward connections.
    int			getForwardConnections(SoFieldList &list) const;

    /// Returns the object that contains this field.  The type of the
    /// object will be either <tt>SoNode</tt>, <tt>SoEngine</tt>, or will be a
    /// global field container (note that the global field container
    /// class is internal to Inventor; see the methods for creating and accessing
    /// global fields on <tt>SoDB</tt>).  For example:
    /// \code
    /// SoFieldContainer *f = field->getContainer();
    /// if (f->isOfType(SoNode::getClassTypeId())) {
    ///     ... do something ...
    /// } else if (f->isOfType(SoEngine::getClassTypeId())) {
    ///     ... do someting else ...
    /// } else {
    ///     ... it must be a global field.  We can figure out its name, but that is about it:
    ///     const SbName &globalFieldName = f->getName();
    /// }
    /// \endcode
    SoFieldContainer *	getContainer() const;

    /// Sets the field to the given value, which is an ASCII string in the
    /// Inventor file format.  Each field subclass defines its own file
    /// format; see their reference pages for information on their file format.
    /// The string should contain only the field's value, <em>not</em> the field's
    /// name (e.g., "1.0", <em>not</em> "width 1.0"). This method
    /// returns TRUE if the string is valid, FALSE if it is not.
    SbBool		set(const char *valueString);

    /// Returns the value of the field in the Inventor file format, even if
    /// the field has its default value.
    void		get(SbString &valueString);

    /// Simulates a change to the field, causing attached sensors to fire,
    /// connected fields and engines to be marked as needing evaluation, and so
    /// forth. Calling #touch() on an instance of a derived field class is
    /// equivalent to calling #setValue(getValue()) using the derived class's
    /// methods, except that the field's #isDefault() status remains
    /// unchanged.
    virtual void	touch();

    /// Returns TRUE/FALSE if the given field is of the same type and
    /// has the same value(s) as this
    int			operator ==(const SoField &f) const
	{ return isSame(f); }
    int			operator !=(const SoField &f) const
	{ return ! isSame(f); }

  SoEXTENDER protected:
    // Constuctor:
    SoField();

  SoINTERNAL public:
    static void		initClass();

    // Initialize ALL Inventor node classes
    static void		initClasses();

    // Sets default flag
    void		setDefault(SbBool def)	{ flags.hasDefault = def; }

    // Initiates or propagates notification through container
    virtual void	startNotify();
    virtual void	notify(SoNotList *list);

    // Sets the containing node.  This also calls enableNotify(TRUE)
    // and setDefault(TRUE).
    void		setContainer(SoFieldContainer *cont);

    // Returns TRUE if the field really needs to be written out.
    // Fields with default values that aren't ignored and
    // aren't connected to anything will return FALSE.
    SbBool		shouldWrite() const;

    // Adds/removes an auditor to/from list
    void		addAuditor(void *auditor, SoNotRec::Type type);
    void		removeAuditor(void *auditor, SoNotRec::Type type);

    // Indicates whether notification will propagate as the result of
    // setting the field value. Engines turn this off when writing
    // results into fields, since notification has already propagated.
    SbBool		enableNotify(SbBool flag);
    SbBool		isNotifyEnabled() const
	{ return flags.notifyEnabled; }

    // Indicates to a field that a change has been made involving a
    // connection from it (as source) to another field. Passed the
    // number of things being connected to the field; the number will
    // be negative when things are disconnected.
    // The default method does nothing.
    virtual void	connectionStatusChanged(int numConnections);

    // If this returns TRUE, it means we're in the middle of doing a
    // setValue()+valueChanged() and values from an upstream
    // connection shouldn't write into this field.
    SbBool		isReadOnly() const { return flags.readOnly; }

    // Returns TRUE if the given field is of the same type and has the
    // same value(s) as this. Subclasses must define this as well as
    // an == operator.
    virtual SbBool	isSame(const SoField &f) const = 0;

    // Copies the value from one field to another, assuming same subclass
    virtual void	copyFrom(const SoField &f) = 0;

    // After a field value has been copied using copyFrom(),
    // this is called to allow fields to update the copy. This is used
    // by node, engine, and path fields to make sure instances are
    // handled properly. The default implementation does nothing.
    virtual void	fixCopy(SbBool copyConnections);

    // This returns TRUE if this field contains a reference to a node
    // or engine that is copied during a copy operation (i.e., it is
    // "inside"). The default method just checks if the field is
    // connected to such a node or engine. Subclasses may contain
    // other tests, such as those that contain pointers to nodes or
    // engines.
    virtual SbBool	referencesCopy() const;

    // Copies connection from one field to another. Assumes fields are
    // the same subclass and that this field is connected.
    void		copyConnection(const SoField *fromField);

    // Reads value of field (with given name) from file as defined by
    // SoInput. This does the work common to all fields, then calls
    // other read methods to do the rest.
    virtual SbBool	read(SoInput *in, const SbName &name);

    // Writes field (with given name) to file as defined by SoOutput
    virtual void	write(SoOutput *out, const SbName &name) const;

    // Counts write-references on field to prepare for writing
    virtual void	countWriteRefs(SoOutput *out) const;

    // Evaluates the field from whatever it's connected to. If
    // there's no connection or we don't need to evaluate, this does
    // nothing. This has to be const because it's used by getValue
    // methods.
    void		evaluate() const
	{ if (flags.dirty) evaluateConnection(); }

  protected:
    // Called by an instance to indicate that a value has changed. If
    // resetDefault is TRUE, this turns off default flag. Initiates
    // notification, if necessary.
    void		valueChanged(SbBool resetDefault = TRUE);

    // Evaluates the field or engine the field is connected to,
    // storing the result in the field. This is const because it is
    // called by evaluate().
    virtual void	evaluateConnection() const;

    // Reads value(s) of field
    virtual SbBool	readValue(SoInput *in) = 0;

    // Reads connection.
    virtual SbBool	readConnection(SoInput *in);

    // Writes value(s) of field
    virtual void	writeValue(SoOutput *out) const = 0;

    // Writes field connection
    virtual void	writeConnection(SoOutput *out) const;

  private:
    static SoType	classTypeId;

    // These are used by SoField::get() to hold the returned field string
    static char		*fieldBuf;
    static int		fieldBufSize;

    // The "flags" field contains several bit flags:
    struct {
	unsigned int hasDefault		: 1; // Field is set to default value
	unsigned int ignored		: 1; // Field value is to be ignored
	unsigned int connected		: 1; // Field connected from something
	unsigned int converted		: 1; // Connection required converter
	unsigned int fromEngine		: 1; // Connection is from engine
	unsigned int connectionEnabled	: 1; // Connection is enabled
	unsigned int notifyEnabled	: 1; // Notification is enabled
	unsigned int hasAuditors	: 1; // Connected, or FieldSensor
	unsigned int isEngineModifying	: 1; // Engine evaluating
	unsigned int readOnly		: 1; // Must not write into
					     // this field
	unsigned int dirty		: 1; // Field was notified and
					     // needs evaluation
    }			flags;

    // If no other auditors, the container for this field is stored
    // directly here.  If the field has other auditors (flags.hasAuditors)
    // then the connection information and the container are
    // stored in an opaque SoFieldAuditorInfo structure.  This is
    // done to save space in the common case.
    union {
	SoFieldContainer *container;
	SoFieldAuditorInfo *auditorInfo;
    };

    // Creates auditorInfo strucuture, if necessary:
    void		createAuditorInfo();

    // Creates a converter engine to convert from the given field
    // type to the type of this field. Returns NULL on error.
    SoFieldConverter	*createConverter(const SoType &fromFieldType) const;

    // Really disconnects field from whatever it's connected to
    void		reallyDisconnect();

    // Returns pointer to field converter, if fields was connected
    // through one
    SoFieldConverter *	getConverter() const;

    // Looks up what field connection is from (container and field/output name)
    void		getConnectionInfo(SoFieldContainer *&, SbName&) const;

    // This is used to reallocate the string buffer used by
    // SoField::get()
    static void *	reallocFieldBuf(void *ptr, size_t newSize);

friend class SoEngineOutput;
};

#endif /* _SO_FIELD_ */

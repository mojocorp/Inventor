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

#ifndef __glubin_h_
#define __glubin_h_

#include "../SoAddPrefix.h"

/*
 * bin.h - $Revision: 1.1 $
 */

#include "myassert.h"
#include "arc.h"
#include "defines.h"

struct Bin { /* a linked list of jordan arcs */
private:
    _SoNurbsArc *		head;		/* first arc on list */
    _SoNurbsArc *		current;	/* current arc on list */
public:
    			Bin();
			~Bin();
    inline _SoNurbsArc *	firstarc( void );
    inline _SoNurbsArc *	nextarc( void );
    inline _SoNurbsArc *	removearc( void );
    inline int		isnonempty( void ) { return (head ? 1 : 0); }
    inline void		addarc( _SoNurbsArc * );
    void 		remove_this_arc( _SoNurbsArc * );
    int			numarcs( void );
    void 		adopt( void );
    void		markall( void );
    void		show( const char * );
    void		listBezier( void );
};

/*----------------------------------------------------------------------------
 * Bin::addarc - add an Arc * to head of linked list of Arc *s
 *----------------------------------------------------------------------------
 */

inline void
Bin::addarc( _SoNurbsArc *jarc )
{
   jarc->link = head;
   head = jarc;
}

/*----------------------------------------------------------------------------
 * Bin::removearc - remove first Arc * from bin
 *----------------------------------------------------------------------------
 */

inline _SoNurbsArc *
Bin::removearc( void )
{
    _SoNurbsArc * jarc = head;

    if( jarc ) head = jarc->link;
    return jarc;
}


/*----------------------------------------------------------------------------
 * BinIter::nextarc - return current arc in bin and advance pointer to next arc
 *----------------------------------------------------------------------------
 */

inline _SoNurbsArc *
Bin::nextarc( void )
{
    _SoNurbsArc * jarc = current;

#ifdef DEBUG
    assert( jarc && jarc->check() );
#endif

    if( jarc ) current = jarc->link;
    return jarc;
}

/*----------------------------------------------------------------------------
 * BinIter::firstarc - set current arc to first arc of bin advance to next arc
 *----------------------------------------------------------------------------
 */

inline _SoNurbsArc *
Bin::firstarc( void )
{
    current = head;
    return nextarc( );
}

#endif /* __glubin_h_ */

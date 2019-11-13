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

const unsigned char SoPointLightDragger::geomBuffer[] = {
    0x23, 0x49, 0x6e, 0x76, 0x65, 0x6e, 0x74, 0x6f, 0x72, 0x20, 0x56, 0x32,
    0x2e, 0x31, 0x20, 0x62, 0x69, 0x6e, 0x61, 0x72, 0x79, 0x20, 0x20, 0xa,
    0x0,  0x0,  0x0,  0x9,  0x53, 0x65, 0x70, 0x61, 0x72, 0x61, 0x74, 0x6f,
    0x72, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x2,  0x0,  0x0,  0x0,  0x0,
    0x0,  0x0,  0x0,  0x7,  0x0,  0x0,  0x0,  0x3,  0x44, 0x45, 0x46, 0x0,
    0x0,  0x0,  0x0,  0x19, 0x70, 0x6f, 0x69, 0x6e, 0x74, 0x4c, 0x69, 0x67,
    0x68, 0x74, 0x4f, 0x76, 0x65, 0x72, 0x61, 0x6c, 0x6c, 0x4d, 0x61, 0x74,
    0x65, 0x72, 0x69, 0x61, 0x6c, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x8,
    0x4d, 0x61, 0x74, 0x65, 0x72, 0x69, 0x61, 0x6c, 0x0,  0x0,  0x0,  0x0,
    0x0,  0x0,  0x0,  0x2,  0x0,  0x0,  0x0,  0xc,  0x64, 0x69, 0x66, 0x66,
    0x75, 0x73, 0x65, 0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x0,  0x0,  0x0,  0x1,
    0x3f, 0x0,  0x0,  0x0,  0x3f, 0x0,  0x0,  0x0,  0x3f, 0x0,  0x0,  0x0,
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0xd,  0x65, 0x6d, 0x69, 0x73,
    0x73, 0x69, 0x76, 0x65, 0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x0,  0x0,  0x0,
    0x0,  0x0,  0x0,  0x1,  0x3f, 0x0,  0x0,  0x0,  0x3f, 0x0,  0x0,  0x0,
    0x3f, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x3,
    0x44, 0x45, 0x46, 0x0,  0x0,  0x0,  0x0,  0x25, 0x50, 0x4f, 0x49, 0x4e,
    0x54, 0x5f, 0x4c, 0x49, 0x47, 0x48, 0x54, 0x5f, 0x44, 0x52, 0x41, 0x47,
    0x47, 0x45, 0x52, 0x5f, 0x4c, 0x49, 0x4e, 0x45, 0x5f, 0x54, 0x52, 0x41,
    0x4e, 0x53, 0x4c, 0x41, 0x54, 0x4f, 0x52, 0x2b, 0x30, 0x0,  0x0,  0x0,
    0x0,  0x0,  0x0,  0x5,  0x47, 0x72, 0x6f, 0x75, 0x70, 0x0,  0x0,  0x0,
    0x0,  0x0,  0x0,  0x2,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x6,
    0x0,  0x0,  0x0,  0xa,  0x4c, 0x69, 0x67, 0x68, 0x74, 0x4d, 0x6f, 0x64,
    0x65, 0x6c, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x1,
    0x0,  0x0,  0x0,  0x5,  0x6d, 0x6f, 0x64, 0x65, 0x6c, 0x0,  0x0,  0x0,
    0x0,  0x0,  0x0,  0x5,  0x50, 0x48, 0x4f, 0x4e, 0x47, 0x0,  0x0,  0x0,
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x8,  0x54, 0x65, 0x78, 0x74,
    0x75, 0x72, 0x65, 0x32, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,
    0x0,  0x0,  0x0,  0x9,  0x44, 0x72, 0x61, 0x77, 0x53, 0x74, 0x79, 0x6c,
    0x65, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x1,
    0x0,  0x0,  0x0,  0x9,  0x6c, 0x69, 0x6e, 0x65, 0x57, 0x69, 0x64, 0x74,
    0x68, 0x0,  0x0,  0x0,  0x3f, 0x80, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,
    0x0,  0x0,  0x0,  0xa,  0x43, 0x6f, 0x6d, 0x70, 0x6c, 0x65, 0x78, 0x69,
    0x74, 0x79, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x1,
    0x0,  0x0,  0x0,  0x5,  0x76, 0x61, 0x6c, 0x75, 0x65, 0x0,  0x0,  0x0,
    0x3e, 0x99, 0x99, 0x9a, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x8,
    0x52, 0x6f, 0x74, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x0,  0x0,  0x0,  0x0,
    0x0,  0x0,  0x0,  0x1,  0x0,  0x0,  0x0,  0x8,  0x72, 0x6f, 0x74, 0x61,
    0x74, 0x69, 0x6f, 0x6e, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,
    0x3f, 0x7f, 0xff, 0xff, 0x3f, 0xc9, 0xf,  0xa6, 0x0,  0x0,  0x0,  0x0,
    0x0,  0x0,  0x0,  0x8,  0x43, 0x79, 0x6c, 0x69, 0x6e, 0x64, 0x65, 0x72,
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x2,  0x0,  0x0,  0x0,  0x6,
    0x72, 0x61, 0x64, 0x69, 0x75, 0x73, 0x0,  0x0,  0x3e, 0x4c, 0xcc, 0xcd,
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x6,  0x68, 0x65, 0x69, 0x67,
    0x68, 0x74, 0x0,  0x0,  0x40, 0x40, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,
    0x0,  0x0,  0x0,  0x3,  0x44, 0x45, 0x46, 0x0,  0x0,  0x0,  0x0,  0x22,
    0x70, 0x6f, 0x69, 0x6e, 0x74, 0x4c, 0x69, 0x67, 0x68, 0x74, 0x54, 0x72,
    0x61, 0x6e, 0x73, 0x6c, 0x61, 0x74, 0x6f, 0x72, 0x4c, 0x69, 0x6e, 0x65,
    0x54, 0x72, 0x61, 0x6e, 0x73, 0x6c, 0x61, 0x74, 0x6f, 0x72, 0x0,  0x0,
    0x0,  0x0,  0x0,  0x9,  0x53, 0x65, 0x70, 0x61, 0x72, 0x61, 0x74, 0x6f,
    0x72, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x2,  0x0,  0x0,  0x0,  0x1,
    0x0,  0x0,  0x0,  0xd,  0x72, 0x65, 0x6e, 0x64, 0x65, 0x72, 0x43, 0x61,
    0x63, 0x68, 0x69, 0x6e, 0x67, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x2,
    0x4f, 0x4e, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x2,
    0x0,  0x0,  0x0,  0x8,  0x4d, 0x61, 0x74, 0x65, 0x72, 0x69, 0x61, 0x6c,
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x2,  0x0,  0x0,  0x0,  0xc,
    0x64, 0x69, 0x66, 0x66, 0x75, 0x73, 0x65, 0x43, 0x6f, 0x6c, 0x6f, 0x72,
    0x0,  0x0,  0x0,  0x1,  0x3f, 0x0,  0x0,  0x0,  0x3f, 0x0,  0x0,  0x0,
    0x3f, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0xd,
    0x65, 0x6d, 0x69, 0x73, 0x73, 0x69, 0x76, 0x65, 0x43, 0x6f, 0x6c, 0x6f,
    0x72, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x1,  0x3f, 0x0,  0x0,  0x0,
    0x3f, 0x0,  0x0,  0x0,  0x3f, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,
    0x0,  0x0,  0x0,  0x3,  0x55, 0x53, 0x45, 0x0,  0x0,  0x0,  0x0,  0x25,
    0x50, 0x4f, 0x49, 0x4e, 0x54, 0x5f, 0x4c, 0x49, 0x47, 0x48, 0x54, 0x5f,
    0x44, 0x52, 0x41, 0x47, 0x47, 0x45, 0x52, 0x5f, 0x4c, 0x49, 0x4e, 0x45,
    0x5f, 0x54, 0x52, 0x41, 0x4e, 0x53, 0x4c, 0x41, 0x54, 0x4f, 0x52, 0x2b,
    0x30, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x3,  0x44, 0x45, 0x46, 0x0,
    0x0,  0x0,  0x0,  0x28, 0x70, 0x6f, 0x69, 0x6e, 0x74, 0x4c, 0x69, 0x67,
    0x68, 0x74, 0x54, 0x72, 0x61, 0x6e, 0x73, 0x6c, 0x61, 0x74, 0x6f, 0x72,
    0x4c, 0x69, 0x6e, 0x65, 0x54, 0x72, 0x61, 0x6e, 0x73, 0x6c, 0x61, 0x74,
    0x6f, 0x72, 0x41, 0x63, 0x74, 0x69, 0x76, 0x65, 0x0,  0x0,  0x0,  0x9,
    0x53, 0x65, 0x70, 0x61, 0x72, 0x61, 0x74, 0x6f, 0x72, 0x0,  0x0,  0x0,
    0x0,  0x0,  0x0,  0x2,  0x0,  0x0,  0x0,  0x1,  0x0,  0x0,  0x0,  0xd,
    0x72, 0x65, 0x6e, 0x64, 0x65, 0x72, 0x43, 0x61, 0x63, 0x68, 0x69, 0x6e,
    0x67, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x2,  0x4f, 0x4e, 0x0,  0x0,
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x2,  0x0,  0x0,  0x0,  0x8,
    0x4d, 0x61, 0x74, 0x65, 0x72, 0x69, 0x61, 0x6c, 0x0,  0x0,  0x0,  0x0,
    0x0,  0x0,  0x0,  0x2,  0x0,  0x0,  0x0,  0xc,  0x64, 0x69, 0x66, 0x66,
    0x75, 0x73, 0x65, 0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x0,  0x0,  0x0,  0x1,
    0x3f, 0x0,  0x0,  0x0,  0x3f, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0xd,  0x65, 0x6d, 0x69, 0x73,
    0x73, 0x69, 0x76, 0x65, 0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x0,  0x0,  0x0,
    0x0,  0x0,  0x0,  0x1,  0x3f, 0x0,  0x0,  0x0,  0x3f, 0x0,  0x0,  0x0,
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x3,
    0x55, 0x53, 0x45, 0x0,  0x0,  0x0,  0x0,  0x25, 0x50, 0x4f, 0x49, 0x4e,
    0x54, 0x5f, 0x4c, 0x49, 0x47, 0x48, 0x54, 0x5f, 0x44, 0x52, 0x41, 0x47,
    0x47, 0x45, 0x52, 0x5f, 0x4c, 0x49, 0x4e, 0x45, 0x5f, 0x54, 0x52, 0x41,
    0x4e, 0x53, 0x4c, 0x41, 0x54, 0x4f, 0x52, 0x2b, 0x30, 0x0,  0x0,  0x0,
    0x0,  0x0,  0x0,  0x3,  0x44, 0x45, 0x46, 0x0,  0x0,  0x0,  0x0,  0x26,
    0x50, 0x4f, 0x49, 0x4e, 0x54, 0x5f, 0x4c, 0x49, 0x47, 0x48, 0x54, 0x5f,
    0x44, 0x52, 0x41, 0x47, 0x47, 0x45, 0x52, 0x5f, 0x50, 0x4c, 0x41, 0x4e,
    0x45, 0x5f, 0x54, 0x52, 0x41, 0x4e, 0x53, 0x4c, 0x41, 0x54, 0x4f, 0x52,
    0x2b, 0x31, 0x0,  0x0,  0x0,  0x0,  0x0,  0x5,  0x47, 0x72, 0x6f, 0x75,
    0x70, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x2,  0x0,  0x0,  0x0,  0x0,
    0x0,  0x0,  0x0,  0xb,  0x0,  0x0,  0x0,  0xa,  0x4c, 0x69, 0x67, 0x68,
    0x74, 0x4d, 0x6f, 0x64, 0x65, 0x6c, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,
    0x0,  0x0,  0x0,  0x1,  0x0,  0x0,  0x0,  0x5,  0x6d, 0x6f, 0x64, 0x65,
    0x6c, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x5,  0x50, 0x48, 0x4f, 0x4e,
    0x47, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x8,
    0x54, 0x65, 0x78, 0x74, 0x75, 0x72, 0x65, 0x32, 0x0,  0x0,  0x0,  0x0,
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x9,  0x44, 0x72, 0x61, 0x77,
    0x53, 0x74, 0x79, 0x6c, 0x65, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,
    0x0,  0x0,  0x0,  0x1,  0x0,  0x0,  0x0,  0x9,  0x6c, 0x69, 0x6e, 0x65,
    0x57, 0x69, 0x64, 0x74, 0x68, 0x0,  0x0,  0x0,  0x3f, 0x80, 0x0,  0x0,
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0xa,  0x43, 0x6f, 0x6d, 0x70,
    0x6c, 0x65, 0x78, 0x69, 0x74, 0x79, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,
    0x0,  0x0,  0x0,  0x1,  0x0,  0x0,  0x0,  0x5,  0x76, 0x61, 0x6c, 0x75,
    0x65, 0x0,  0x0,  0x0,  0x3e, 0x99, 0x99, 0x9a, 0x0,  0x0,  0x0,  0x0,
    0x0,  0x0,  0x0,  0x6,  0x53, 0x70, 0x68, 0x65, 0x72, 0x65, 0x0,  0x0,
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x1,  0x0,  0x0,  0x0,  0x6,
    0x72, 0x61, 0x64, 0x69, 0x75, 0x73, 0x0,  0x0,  0x3f, 0x80, 0x0,  0x0,
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x9,  0x44, 0x72, 0x61, 0x77,
    0x53, 0x74, 0x79, 0x6c, 0x65, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,
    0x0,  0x0,  0x0,  0x2,  0x0,  0x0,  0x0,  0x5,  0x73, 0x74, 0x79, 0x6c,
    0x65, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x5,  0x4c, 0x49, 0x4e, 0x45,
    0x53, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x9,
    0x6c, 0x69, 0x6e, 0x65, 0x57, 0x69, 0x64, 0x74, 0x68, 0x0,  0x0,  0x0,
    0x3f, 0x80, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x9,
    0x50, 0x69, 0x63, 0x6b, 0x53, 0x74, 0x79, 0x6c, 0x65, 0x0,  0x0,  0x0,
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x1,  0x0,  0x0,  0x0,  0x5,
    0x73, 0x74, 0x79, 0x6c, 0x65, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0xa,
    0x55, 0x4e, 0x50, 0x49, 0x43, 0x4b, 0x41, 0x42, 0x4c, 0x45, 0x0,  0x0,
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0xd,  0x4e, 0x6f, 0x72, 0x6d,
    0x61, 0x6c, 0x42, 0x69, 0x6e, 0x64, 0x69, 0x6e, 0x67, 0x0,  0x0,  0x0,
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x1,  0x0,  0x0,  0x0,  0x5,
    0x76, 0x61, 0x6c, 0x75, 0x65, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x12,
    0x50, 0x45, 0x52, 0x5f, 0x56, 0x45, 0x52, 0x54, 0x45, 0x58, 0x5f, 0x49,
    0x4e, 0x44, 0x45, 0x58, 0x45, 0x44, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,
    0x0,  0x0,  0x0,  0xf,  0x4d, 0x61, 0x74, 0x65, 0x72, 0x69, 0x61, 0x6c,
    0x42, 0x69, 0x6e, 0x64, 0x69, 0x6e, 0x67, 0x0,  0x0,  0x0,  0x0,  0x0,
    0x0,  0x0,  0x0,  0x1,  0x0,  0x0,  0x0,  0x5,  0x76, 0x61, 0x6c, 0x75,
    0x65, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x7,  0x4f, 0x56, 0x45, 0x52,
    0x41, 0x4c, 0x4c, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0xb,
    0x43, 0x6f, 0x6f, 0x72, 0x64, 0x69, 0x6e, 0x61, 0x74, 0x65, 0x33, 0x0,
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x1,  0x0,  0x0,  0x0,  0x5,
    0x70, 0x6f, 0x69, 0x6e, 0x74, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x18,
    0x3f, 0x87, 0xc8, 0x4b, 0x3f, 0x87, 0xc8, 0x4b, 0x0,  0x0,  0x0,  0x0,
    0x3f, 0xa9, 0xba, 0x5e, 0x3f, 0xa9, 0xba, 0x5e, 0x0,  0x0,  0x0,  0x0,
    0xbf, 0x87, 0xc8, 0x4b, 0xbf, 0x87, 0xc8, 0x4b, 0x0,  0x0,  0x0,  0x0,
    0xbf, 0xa9, 0xba, 0x5e, 0xbf, 0xa9, 0xba, 0x5e, 0x0,  0x0,  0x0,  0x0,
    0xbf, 0x87, 0xc8, 0x4b, 0x3f, 0x87, 0xc8, 0x4b, 0x0,  0x0,  0x0,  0x0,
    0xbf, 0xa9, 0xba, 0x5e, 0x3f, 0xa9, 0xba, 0x5e, 0x0,  0x0,  0x0,  0x0,
    0x3f, 0x87, 0xc8, 0x4b, 0xbf, 0x87, 0xc8, 0x4b, 0x0,  0x0,  0x0,  0x0,
    0x3f, 0xa9, 0xba, 0x5e, 0xbf, 0xa9, 0xba, 0x5e, 0x0,  0x0,  0x0,  0x0,
    0x0,  0x0,  0x0,  0x0,  0x3f, 0x87, 0xc8, 0x4b, 0x3f, 0x87, 0xc8, 0x4b,
    0x0,  0x0,  0x0,  0x0,  0x3f, 0xa9, 0xba, 0x5e, 0x3f, 0xa9, 0xba, 0x5e,
    0x0,  0x0,  0x0,  0x0,  0xbf, 0x87, 0xc8, 0x4b, 0xbf, 0x87, 0xc8, 0x4b,
    0x0,  0x0,  0x0,  0x0,  0xbf, 0xa9, 0xba, 0x5e, 0xbf, 0xa9, 0xba, 0x5e,
    0x0,  0x0,  0x0,  0x0,  0xbf, 0x87, 0xc8, 0x4b, 0x3f, 0x87, 0xc8, 0x4b,
    0x0,  0x0,  0x0,  0x0,  0xbf, 0xa9, 0xba, 0x5e, 0x3f, 0xa9, 0xba, 0x5e,
    0x0,  0x0,  0x0,  0x0,  0x3f, 0x87, 0xc8, 0x4b, 0xbf, 0x87, 0xc8, 0x4b,
    0x0,  0x0,  0x0,  0x0,  0x3f, 0xa9, 0xba, 0x5e, 0xbf, 0xa9, 0xba, 0x5e,
    0x3f, 0x87, 0xc8, 0x4b, 0x0,  0x0,  0x0,  0x0,  0x3f, 0x87, 0xc8, 0x4b,
    0x3f, 0xa9, 0xba, 0x5e, 0x0,  0x0,  0x0,  0x0,  0x3f, 0xa9, 0xba, 0x5e,
    0xbf, 0x87, 0xc8, 0x4b, 0x0,  0x0,  0x0,  0x0,  0xbf, 0x87, 0xc8, 0x4b,
    0xbf, 0xa9, 0xba, 0x5e, 0x0,  0x0,  0x0,  0x0,  0xbf, 0xa9, 0xba, 0x5e,
    0xbf, 0x87, 0xc8, 0x4b, 0x0,  0x0,  0x0,  0x0,  0x3f, 0x87, 0xc8, 0x4b,
    0xbf, 0xa9, 0xba, 0x5e, 0x0,  0x0,  0x0,  0x0,  0x3f, 0xa9, 0xba, 0x5e,
    0x3f, 0x87, 0xc8, 0x4b, 0x0,  0x0,  0x0,  0x0,  0xbf, 0x87, 0xc8, 0x4b,
    0x3f, 0xa9, 0xba, 0x5e, 0x0,  0x0,  0x0,  0x0,  0xbf, 0xa9, 0xba, 0x5e,
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x7,  0x4c, 0x69, 0x6e, 0x65,
    0x53, 0x65, 0x74, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x1,
    0x0,  0x0,  0x0,  0xb,  0x6e, 0x75, 0x6d, 0x56, 0x65, 0x72, 0x74, 0x69,
    0x63, 0x65, 0x73, 0x0,  0x0,  0x0,  0x0,  0xc,  0x0,  0x0,  0x0,  0x2,
    0x0,  0x0,  0x0,  0x2,  0x0,  0x0,  0x0,  0x2,  0x0,  0x0,  0x0,  0x2,
    0x0,  0x0,  0x0,  0x2,  0x0,  0x0,  0x0,  0x2,  0x0,  0x0,  0x0,  0x2,
    0x0,  0x0,  0x0,  0x2,  0x0,  0x0,  0x0,  0x2,  0x0,  0x0,  0x0,  0x2,
    0x0,  0x0,  0x0,  0x2,  0x0,  0x0,  0x0,  0x2,  0x0,  0x0,  0x0,  0x0,
    0x0,  0x0,  0x0,  0x3,  0x44, 0x45, 0x46, 0x0,  0x0,  0x0,  0x0,  0x23,
    0x70, 0x6f, 0x69, 0x6e, 0x74, 0x4c, 0x69, 0x67, 0x68, 0x74, 0x54, 0x72,
    0x61, 0x6e, 0x73, 0x6c, 0x61, 0x74, 0x6f, 0x72, 0x50, 0x6c, 0x61, 0x6e,
    0x65, 0x54, 0x72, 0x61, 0x6e, 0x73, 0x6c, 0x61, 0x74, 0x6f, 0x72, 0x0,
    0x0,  0x0,  0x0,  0x9,  0x53, 0x65, 0x70, 0x61, 0x72, 0x61, 0x74, 0x6f,
    0x72, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x2,  0x0,  0x0,  0x0,  0x1,
    0x0,  0x0,  0x0,  0xd,  0x72, 0x65, 0x6e, 0x64, 0x65, 0x72, 0x43, 0x61,
    0x63, 0x68, 0x69, 0x6e, 0x67, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x2,
    0x4f, 0x4e, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x1,
    0x0,  0x0,  0x0,  0x3,  0x55, 0x53, 0x45, 0x0,  0x0,  0x0,  0x0,  0x26,
    0x50, 0x4f, 0x49, 0x4e, 0x54, 0x5f, 0x4c, 0x49, 0x47, 0x48, 0x54, 0x5f,
    0x44, 0x52, 0x41, 0x47, 0x47, 0x45, 0x52, 0x5f, 0x50, 0x4c, 0x41, 0x4e,
    0x45, 0x5f, 0x54, 0x52, 0x41, 0x4e, 0x53, 0x4c, 0x41, 0x54, 0x4f, 0x52,
    0x2b, 0x31, 0x0,  0x0,  0x0,  0x0,  0x0,  0x3,  0x44, 0x45, 0x46, 0x0,
    0x0,  0x0,  0x0,  0x29, 0x70, 0x6f, 0x69, 0x6e, 0x74, 0x4c, 0x69, 0x67,
    0x68, 0x74, 0x54, 0x72, 0x61, 0x6e, 0x73, 0x6c, 0x61, 0x74, 0x6f, 0x72,
    0x50, 0x6c, 0x61, 0x6e, 0x65, 0x54, 0x72, 0x61, 0x6e, 0x73, 0x6c, 0x61,
    0x74, 0x6f, 0x72, 0x41, 0x63, 0x74, 0x69, 0x76, 0x65, 0x0,  0x0,  0x0,
    0x0,  0x0,  0x0,  0x9,  0x53, 0x65, 0x70, 0x61, 0x72, 0x61, 0x74, 0x6f,
    0x72, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x2,  0x0,  0x0,  0x0,  0x1,
    0x0,  0x0,  0x0,  0xd,  0x72, 0x65, 0x6e, 0x64, 0x65, 0x72, 0x43, 0x61,
    0x63, 0x68, 0x69, 0x6e, 0x67, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x2,
    0x4f, 0x4e, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x2,
    0x0,  0x0,  0x0,  0x8,  0x4d, 0x61, 0x74, 0x65, 0x72, 0x69, 0x61, 0x6c,
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x2,  0x0,  0x0,  0x0,  0xc,
    0x64, 0x69, 0x66, 0x66, 0x75, 0x73, 0x65, 0x43, 0x6f, 0x6c, 0x6f, 0x72,
    0x0,  0x0,  0x0,  0x1,  0x3f, 0x0,  0x0,  0x0,  0x3f, 0x0,  0x0,  0x0,
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0xd,
    0x65, 0x6d, 0x69, 0x73, 0x73, 0x69, 0x76, 0x65, 0x43, 0x6f, 0x6c, 0x6f,
    0x72, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x1,  0x3f, 0x0,  0x0,  0x0,
    0x3f, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,
    0x0,  0x0,  0x0,  0x3,  0x55, 0x53, 0x45, 0x0,  0x0,  0x0,  0x0,  0x26,
    0x50, 0x4f, 0x49, 0x4e, 0x54, 0x5f, 0x4c, 0x49, 0x47, 0x48, 0x54, 0x5f,
    0x44, 0x52, 0x41, 0x47, 0x47, 0x45, 0x52, 0x5f, 0x50, 0x4c, 0x41, 0x4e,
    0x45, 0x5f, 0x54, 0x52, 0x41, 0x4e, 0x53, 0x4c, 0x41, 0x54, 0x4f, 0x52,
    0x2b, 0x31, 0x0,  0x0};

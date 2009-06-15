/*
 * Copyright (c) 2000 Silicon Graphics, Inc.  All Rights Reserved.
 * 
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 * 
 * This program is distributed in the hope that it would be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * 
 * Further, this software is distributed without any warranty that it is
 * free of the rightful claim of any third person regarding infringement
 * or the like.  Any license provided herein, whether implied or
 * otherwise, applies only to this software file.  Patent licenses, if
 * any, provided herein do not apply to combinations of this program with
 * other software, or any other product whatsoever.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write the Free Software Foundation, Inc., 59
 * Temple Place - Suite 330, Boston MA 02111-1307, USA.
 * 
 * Contact information: Silicon Graphics, Inc., 1600 Amphitheatre Pkwy,
 * Mountain View, CA  94043, or:
 * 
 * http://www.sgi.com 
 * 
 * For further information regarding this notice, see: 
 * 
 * http://oss.sgi.com/projects/GenInfo/NoticeExplan/
 */

#include <stdio.h>
#include <stdlib.h>
#ifdef WIN32
#include <windows.h>
#include <io.h>
#ifndef R_OK
#define R_OK 4
#endif
#else
#include <unistd.h>
#endif
#include <string.h>
#include "fl.h"

/* Implementation headers go here */
#include "flfreetype.h"

#define INIT(impl, ret)							\
  if (! impl->init && ! _flInitialize(impl))				\
    return ret

#define CHECK(fs, impl, func, ret)					\
  INIT(impl, ret);							\
  if (! fs || ! impl->func)						\
    return ret

/* Data type */
typedef struct FLfontImpl {
  const char *		   name;		/* vendor name */
  const char *		   version;		/* version string */

  FLInitializeFunc *	   initialize;		/* initialize handler */
  FLShutdownFunc *	   shutdown;		/* shutdown handler */
  FLCreateFontFunc *	   createFont;		/* font creation handler */
  FLDestroyFontFunc *	   destroyFont;		/* font destruction handler */
  FLGetBitmapFunc *	   getBitmap;		/* get bitmap handler */
  FLGetScalableBitmapFunc *getScalableBitmap;	/* get scalable bitmap handler */
  FLGetOutlineFunc *	   getOutline;		/* get outline handler */
  FLUniGetBitmapFunc *	   getUniBitmap;	/* get Unicode bitmap handler */
  FLUniGetOutlineFunc *	   getUniOutline;	/* get Unicode outline handler */

  GLboolean		   init;		/* initialized flag */
  GLboolean		   available;		/* availability flag */
} FLfontImpl;

/* Font implementation table */
FLfontImpl freetypeImpl = {
  "FreeType", "2.0.1",
  _flFTInitialize,
  _flFTShutdown,
  _flFTCreateFont,
  _flFTDestroyFont,
  _flFTGetBitmap,
  _flFTGetScalableBitmap,
  _flFTGetOutline,
  _flFTUniGetBitmap,
  _flFTUniGetOutline,
  FALSE, TRUE
};

static FLfontImpl *fontImplTable[] = {
  &freetypeImpl,
  NULL
};


#if defined(WIN32)
static const char *PATH_SEPARATOR = ";";
static const char *DEFAULT_FONTPATH = "c:\\Windows\\Fonts;c:\\Winnt\\Fonts";
static const char *DEFAULT_FONT = "arial.ttf";
#elif defined(__APPLE__)
static const char *PATH_SEPARATOR = ":";
static const char *DEFAULT_FONTPATH = "/System/Library/Fonts:/Library/Fonts";
static const char *DEFAULT_FONT = "Arial.ttf";
#else
static const char *PATH_SEPARATOR = ":";
static const char *DEFAULT_FONTPATH = "/usr/share/data/fonts";
static const char *DEFAULT_FONT = "Arial.ttf";
#endif


int fl_debug = FALSE;

/*
 * Initialize the given font implementation. Returns true if OK, false
 * otherwise.
 */
static GLboolean
_flInitialize(FLfontImpl *impl)
{
  GLboolean ret;
  char *ev;

  ev = getenv("FL_DEBUG");
  fl_debug = (ev && *ev != '0');

  if (impl->initialize)
    ret = impl->initialize();

  impl->init = TRUE;

  return ret;
}

/*
 * Search for a suitable font implementation. For now, simply returns 
 * FreeType's implementation.
 */
static FLfontImpl *
_flGetFontImpl(void)
{
  return fontImplTable[0];
}

/*
 * Search the given font name in font path, returns an allocated path if
 * found, else returns NULL.
 */
char *
_flSearchFont(const GLubyte *fontName)
{
  char buffer[1024];
  char *path;
  char *fontsearchpath;
  char tmp[1024];

  fontsearchpath = getenv ("FL_FONT_PATH");
  if (!fontsearchpath)
      fontsearchpath = DEFAULT_FONTPATH;

  strcpy(tmp, fontsearchpath);

  path = strtok (tmp,PATH_SEPARATOR);
  while (path != NULL) {
    sprintf(buffer, "%s/%s.ttf", path, fontName);
    if (access(buffer, R_OK) == 0) {
      path = strdup(buffer);
      break;
    }

    sprintf(buffer, "%s/%s.pfa", path, fontName);
    if (access(buffer, R_OK) == 0) {
      path = strdup(buffer);
      break;
    }

    sprintf(buffer, "%s/%s.pfb", path, fontName);
    if (access(buffer, R_OK) == 0) {
      path = strdup(buffer);
      break;
    }

    sprintf(buffer, "%s/%s.dfont", path, fontName);
    if (access(buffer, R_OK) == 0) {
      path = strdup(buffer);
      break;
    }
    path = strtok (NULL, PATH_SEPARATOR);
  }

  if (path==NULL) {
      strcpy(tmp, fontsearchpath);
      path = strtok (tmp,PATH_SEPARATOR);
      while (path != NULL) {
        sprintf(buffer, "%s/%s", path, DEFAULT_FONT);
        if (access(buffer, R_OK) == 0) {
          path = strdup(buffer);
          break;
        }
        path = strtok (NULL, PATH_SEPARATOR);
      }
  }
  TRACE(("_flSearchFont: path=[%s]\n", path));

  return path;
}

/*
 * Shut everything down.
 */
void
_flShutdown(void)
{
  FLfontImpl **impl;

  for (impl = fontImplTable; *impl; impl++)
    if ((*impl)->shutdown)
      (*impl)->shutdown();
}

FLfontStruct *
_flCreateFont(const GLubyte *fontName,
	      GLfloat	     mat[2][2],
	      GLint	     charNameCount,
	      GLubyte **     charNameVector)
{
  FLfontImpl *impl = fontImplTable[0];
  FLfontStruct *fs;

  INIT(impl, NULL);
  
  fs = impl->createFont(fontName, mat, charNameCount, charNameVector);
  if (! fs)
    return NULL;

  /* Initialize font structure */
  fs->direction		= FL_FONT_LEFTTORIGHT; /* hint about direction the font is painted */
  fs->min_char_or_byte2 = 32;	/* first character */
  fs->max_char_or_byte2 = 127;	/* last character */
  fs->min_byte1		= 0;	/* first row that exists */
  fs->max_byte1		= 0;	/* last row that exists */
  fs->all_chars_exist	= FALSE;/* flag if all characters have non-zero size */
  fs->default_char	= 32;	/* char to print for undefined character */
  fs->n_properties	= 0;	/* how many properties there are */
  fs->properties	= NULL;	/* pointer to array of additional properties */
#if 0
  fs->min_bounds;		/* minimum bounds over all existing char */
  fs->max_bounds;		/* maximum bounds over all existing char */
#endif
  fs->per_char		= NULL;	/* first_char to last_char information */
  fs->ascent		= 8;	/* log. extent above baseline for spacing */
  fs->descent		= 4;	/* log. descent below baseline for spacing */

  return fs;
}

void
_flDestroyFont(FLfontStruct *fs)
{
  FLfontImpl *impl = _flGetFontImpl();
#if defined(WIN32) || defined(__APPLE__)
  CHECK(fs, impl, destroyFont, NULL);
#else
  CHECK(fs, impl, destroyFont, /* nothing to return */);
#endif

  impl->destroyFont(fs);
}

FLbitmap *
_flGetBitmap(FLfontStruct *fs, GLuint c)
{
  FLfontImpl *impl = _flGetFontImpl();

  CHECK(fs, impl, getBitmap, NULL);

  return impl->getBitmap(fs, c);
}

FLscalableBitmap *
_flGetScalableBitmap(FLfontStruct *fs, GLuint c)
{
  FLfontImpl *impl = _flGetFontImpl();

  CHECK(fs, impl, getScalableBitmap, NULL);

  return impl->getScalableBitmap(fs, c);
}

FLoutline *
_flGetOutline(FLfontStruct *fs, GLuint c)
{
  FLfontImpl *impl = _flGetFontImpl();

  CHECK(fs, impl, getOutline, NULL);

  return impl->getOutline(fs, c);
}

FLbitmap *
_flUniGetBitmap(FLfontStruct **fsList, GLubyte *UCS2)
{
  FLfontImpl *impl = _flGetFontImpl();

  CHECK(fsList, impl, getUniBitmap, NULL);

  return impl->getUniBitmap(fsList, UCS2);
}

FLoutline *
_flUniGetOutline(FLfontStruct **fsList, GLubyte *UCS2)
{
  FLfontImpl *impl = _flGetFontImpl();

  CHECK(fsList, impl, getUniOutline, NULL);

  return impl->getUniOutline(fsList, UCS2);
}


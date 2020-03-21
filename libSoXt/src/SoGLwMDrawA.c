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

#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
#include <Xm/PrimitiveP.h>
#include "SoGLwMDrawAP.h"
#include <assert.h>
#include <stdio.h>

/* forward definitions */
/* resource default procs */
static void createColormap(SoGLwMDrawingAreaWidget w,
			   int offset, XrmValue *value);

/* widget methods */
static void Initialize(SoGLwMDrawingAreaWidget req, SoGLwMDrawingAreaWidget new,
		       ArgList args, Cardinal *num_args);
static void Realize(Widget w, Mask *valueMask,
		    XSetWindowAttributes *attributes);
static void Redraw(SoGLwMDrawingAreaWidget w, XEvent *event, Region region);
static void Resize(SoGLwMDrawingAreaWidget glw);
static void Destroy(SoGLwMDrawingAreaWidget glw);

/* errors and warnings */
static void error(Widget w, char *string);
static void warning(Widget w, char *string);

#define offset(field) XtOffset(SoGLwMDrawingAreaWidget, SoglwDrawingArea.field)


/*
 * There is a bit of unusual handling of the resources here.
 * Because Xt insists on allocating the colormap resource when it is
 * processing the core resources (even if we redeclare the colormap
 * resource here, we need to do a little trick.  When Xt first allocates
 * the colormap, we allow it to allocate the default one, since we have
 * not yet determined the appropriate visual (which is determined from
 * resources parsed after the colormap).  We also let it allocate colors
 * in that default colormap.
 *
 * In the initialize proc we calculate the actual visual.  Then, we
 * reobtain the colormap resource using XtGetApplicationResources in
 * the initialize proc.  If requested, we also reallocate colors in
 * that colormap using the same method.
 */

static XtResource resources[] = {

  /* the visual info is the only GL resource we allow */
  {SoGLwNvisualInfo, SoGLwCVisualInfo, SoGLwRVisualInfo, sizeof (XVisualInfo *),
       offset(visualInfo), XtRImmediate, (XtPointer) NULL},

/* miscellaneous resources */
  {SoGLwNinstallColormap, SoGLwCInstallColormap, XtRBoolean, sizeof (Boolean),
       offset(installColormap), XtRImmediate, (XtPointer) TRUE},

  {SoGLwNallocateBackground, SoGLwCAllocateColors, XtRBoolean, sizeof (Boolean),
       offset(allocateBackground), XtRImmediate, (XtPointer) FALSE},

  {SoGLwNallocateOtherColors, SoGLwCAllocateColors, XtRBoolean, sizeof (Boolean),
       offset(allocateOtherColors), XtRImmediate, (XtPointer) FALSE},

  {SoGLwNinstallBackground, SoGLwCInstallBackground, XtRBoolean, sizeof (Boolean),
       offset(installBackground), XtRImmediate, (XtPointer) TRUE},

  {SoGLwNginitCallback, SoGLwCCallback, XtRCallback, sizeof (XtCallbackList),
       offset(ginitCallback), XtRImmediate, (XtPointer) NULL},

  {SoGLwNinputCallback, SoGLwCCallback, XtRCallback, sizeof (XtCallbackList),
       offset(inputCallback), XtRImmediate, (XtPointer) NULL},

  {SoGLwNresizeCallback, SoGLwCCallback, XtRCallback, sizeof (XtCallbackList),
       offset(resizeCallback), XtRImmediate, (XtPointer) NULL},

  {SoGLwNexposeCallback, SoGLwCCallback, XtRCallback, sizeof (XtCallbackList),
       offset(exposeCallback), XtRImmediate, (XtPointer) NULL},

  /* Changes to Motif primitive resources */
  {XmNtraversalOn, XmCTraversalOn, XmRBoolean, sizeof (Boolean),
       XtOffset (SoGLwMDrawingAreaWidget, primitive.traversal_on), XmRImmediate,
       (XtPointer) FALSE},
  
  /* highlighting is normally disabled, as when Motif tries to disable
   * highlighting, it tries to reset the color back to the parent's
   * background (usually Motif blue).  Unfortunately, that is in a
   * different colormap, and doesn't work too well.
   */
  {XmNhighlightOnEnter, XmCHighlightOnEnter, XmRBoolean, sizeof (Boolean),
       XtOffset (SoGLwMDrawingAreaWidget, primitive.highlight_on_enter),
       XmRImmediate, (XtPointer) FALSE},
  
  {XmNhighlightThickness, XmCHighlightThickness, XmRHorizontalDimension,
       sizeof (Dimension),
       XtOffset (SoGLwMDrawingAreaWidget, primitive.highlight_thickness),
       XmRImmediate, (XtPointer) 0},
};

/* The following resources are reobtained using XtGetApplicationResources
 * in the initialize proc.
 */
/* The colormap */
static XtResource initializeResources[] = {
  /* reobtain the colormap with the new visual */
  {XtNcolormap, XtCColormap, XtRColormap, sizeof(Colormap),
       XtOffset(SoGLwMDrawingAreaWidget, core.colormap),
	 XtRCallProc,(XtPointer) createColormap},
};

/* reallocate any colors we need in the new colormap */
  
/* The background is obtained only if the allocateBackground resource is TRUE*/
static XtResource backgroundResources[] = {

#ifdef SB_OS_IRIX
#define USE_SGI_COLOR_RESOURCES
#endif /* SB_OS_IRIX */

#ifdef USE_SGI_COLOR_RESOURCES
   {
     XmNbackground, XmCBackground, XmRPixel, 
     sizeof (Pixel), XtOffset(SoGLwMDrawingAreaWidget, core.background_pixel),
     XmRCallProc, (XtPointer) _XmBackgroundColorDefault
   },
   {
     XmNbackgroundPixmap, XmCPixmap, XmRXmBackgroundPixmap, 
     sizeof (Pixmap), XtOffset(SoGLwMDrawingAreaWidget, core.background_pixmap),
     XmRImmediate, (XtPointer) XmUNSPECIFIED_PIXMAP
   },
#else	/* ! USE_SGI_COLOR_RESOURCES */
    {XtNbackground, XtCBackground, XtRPixel,sizeof(Pixel),
         XtOffset(SoGLwMDrawingAreaWidget,core.background_pixel),
	 XtRString, (XtPointer)"XtDefaultBackground"},
    {XtNbackgroundPixmap, XtCPixmap, XtRPixmap, sizeof(Pixmap),
         XtOffset(SoGLwMDrawingAreaWidget,core.background_pixmap),
	 XtRImmediate, (XtPointer)XtUnspecifiedPixmap},
#endif  /* USE_SGI_COLOR_RESOURCES */
};

/* The other colors such as the foreground are allocated only if
 * allocateOtherColors are set.  These resources only exist in Motif.
 */

#ifdef USE_SGI_COLOR_RESOURCES
static XtResource otherColorResources[] = {
   {
     XmNforeground, XmCForeground, XmRPixel, 
     sizeof (Pixel), XtOffset(SoGLwMDrawingAreaWidget, primitive.foreground),
     XmRCallProc, (XtPointer) _XmForegroundColorDefault
   },

   {
     XmNhighlightColor, XmCHighlightColor, XmRPixel, sizeof (Pixel),
     XtOffset(SoGLwMDrawingAreaWidget, primitive.highlight_color),
     XmRCallProc, (XtPointer) _XmHighlightColorDefault
   },

   {
     XmNhighlightPixmap, XmCHighlightPixmap, XmRPrimHighlightPixmap,
     sizeof (Pixmap),
     XtOffset(SoGLwMDrawingAreaWidget, primitive.highlight_pixmap),
     XmRCallProc, (XtPointer) _XmPrimitiveHighlightPixmapDefault
   },
};
#endif /* USE_SGI_COLOR_RESOURCES */

struct attribInfo
{
    int offset;
    int attribute;
};

#undef offset


SoGLwMDrawingAreaClassRec SoglwMDrawingAreaClassRec =
{
  { /* core fields */
    /* superclass		*/	(WidgetClass) &xmPrimitiveClassRec,
    /* class_name		*/	"SoGLwMDrawingArea",
    /* widget_size		*/	sizeof(SoGLwMDrawingAreaRec),
    /* class_initialize		*/	NULL,
    /* class_part_initialize	*/	NULL,
    /* class_inited		*/	FALSE,
    /* initialize		*/	(XtInitProc) Initialize,
    /* initialize_hook		*/	NULL,
    /* realize			*/	Realize,
    /* actions			*/	NULL,
    /* num_actions		*/	0,
    /* resources		*/	resources,
    /* num_resources		*/	XtNumber(resources),
    /* xrm_class		*/	NULLQUARK,
    /* compress_motion		*/	TRUE,
    /* compress_exposure	*/	TRUE,
    /* compress_enterleave	*/	TRUE,
    /* visible_interest		*/	TRUE,
    /* destroy			*/	(XtWidgetProc) Destroy,
    /* resize			*/	(XtWidgetProc) Resize,
    /* expose			*/	(XtExposeProc) Redraw,
    /* set_values		*/	NULL,
    /* set_values_hook		*/	NULL,
    /* set_values_almost	*/	XtInheritSetValuesAlmost,
    /* get_values_hook		*/	NULL,
    /* accept_focus		*/	NULL,
    /* version			*/	XtVersion,
    /* callback_private		*/	NULL,
    /* tm_table			*/	NULL,
    /* query_geometry		*/	XtInheritQueryGeometry,
    /* display_accelerator	*/	XtInheritDisplayAccelerator,
    /* extension		*/	NULL
  },
  {
    /* border_highlight		*/	XmInheritBorderHighlight,
    /* border_unhighlight	*/	XmInheritBorderUnhighlight,
    /* translations		*/	XtInheritTranslations,
    /* arm_and_activate		*/	NULL,
    /* get_resources		*/	NULL,
    /* num get_resources	*/	0,
    /* extension		*/	NULL,				
  }
};

WidgetClass SoglwMDrawingAreaWidgetClass = (WidgetClass)&SoglwMDrawingAreaClassRec;

static void 
error(Widget w, char *string)
{
    char buf[100];
    sprintf (buf, "SoGLwMDrawingArea: %s\n", string);
    XtAppError(XtWidgetToApplicationContext(w), buf);
}

static void 
warning(Widget w, char *string)
{
    char buf[100];
    sprintf (buf, "SoGLwMDraw: %s\n", string);
    XtAppWarning(XtWidgetToApplicationContext(w), buf);
}

/* Initialize the colormap based on the visual info.
 * This routine maintains a cache of visual-infos to colormaps.  If two
 * widgets share the same visual info, they share the same colormap.
 * This function is called by the callProc of the colormap resource entry.
 */
static void createColormap(SoGLwMDrawingAreaWidget w,
			   int offset, XrmValue *value)
{
    static struct cmapCache
    {
	Visual *visual;
	Colormap cmap;
        int      screen;
    } *cmapCache;
    static int cacheEntries=0;
    static int cacheMalloced=0;

    register int i;
    
    assert(w->SoglwDrawingArea.visualInfo);

    /* see if we can find it in the cache */
    for (i=0; i<cacheEntries; i++)
	if (cmapCache[i].visual == w->SoglwDrawingArea.visualInfo->visual &&
            cmapCache[i].screen == w->SoglwDrawingArea.visualInfo->screen)
	{
	    value->addr = (XtPointer) (&cmapCache[i].cmap);
	    return;
	}
    /* not in the cache, create a new entry */
    if (cacheEntries >= cacheMalloced)
    {
	/* need to malloc a new one.  Since we are likely to have only a
	 * few colormaps, we allocate one the first time, and double
	 * each subsequent time.
	 */
	if (cacheMalloced == 0)
	{
	    cacheMalloced = 1;
	    cmapCache = (struct cmapCache *)XtMalloc(sizeof(struct cmapCache));
	}
	else
	{
	    cacheMalloced <<= 1;
	    cmapCache = (struct cmapCache *)XtRealloc((char *) cmapCache,
						      sizeof(struct cmapCache)*
						      cacheMalloced);
	}
    }
       
    cmapCache[cacheEntries].cmap  = XCreateColormap (XtDisplay(w),
					RootWindow(XtDisplay(w),
					w->SoglwDrawingArea.visualInfo->screen),
					w->SoglwDrawingArea.visualInfo->visual,
					AllocNone);
    cmapCache[cacheEntries].visual = w->SoglwDrawingArea.visualInfo->visual;
    cmapCache[cacheEntries].screen = w->SoglwDrawingArea.visualInfo->screen;
    value->addr = (XtPointer) (&cmapCache[cacheEntries++].cmap);
}

static void
Initialize (SoGLwMDrawingAreaWidget req, SoGLwMDrawingAreaWidget new,
	    ArgList args, Cardinal *num_args)
{

    if (req->core.width == 0)
	new->core.width = 100;
    if (req->core.height == 0)
    new->core.height = 100;

    new->core.depth = new->SoglwDrawingArea.visualInfo->depth;

    /* Reobtain the colormap and colors in it using XtGetApplicationResources*/
    XtGetApplicationResources((Widget) new, new,
			      initializeResources,
			      XtNumber(initializeResources),
			      args, *num_args);

    /* obtain the color resources if appropriate */
    if (req->SoglwDrawingArea.allocateBackground)
	XtGetApplicationResources((Widget) new, new,
				  backgroundResources,
				  XtNumber(backgroundResources),
				  args, *num_args);

#ifdef USE_SGI_COLOR_RESOURCES
    if (req->SoglwDrawingArea.allocateOtherColors)
	XtGetApplicationResources((Widget) new, new,
				  otherColorResources,
				  XtNumber(otherColorResources),
				  args, *num_args);
#endif /* USE_SGI_COLOR_RESOURCES */

}

static void
Realize(Widget w, Mask *valueMask, XSetWindowAttributes *attributes)
{
    register SoGLwMDrawingAreaWidget glw = (SoGLwMDrawingAreaWidget)w;
    SoGLwDrawingAreaCallbackStruct cb;
   
    /* if we haven't requested that the background be both installed and
     * allocated, don't install it.
     */
    if (!(glw->SoglwDrawingArea.installBackground &&
	  glw->SoglwDrawingArea.allocateBackground))
	*valueMask &= ~CWBackPixel;
    
    XtCreateWindow (w, (unsigned int)InputOutput,
		    glw->SoglwDrawingArea.visualInfo->visual,
		    *valueMask, attributes);

    /* if appropriate, call XSetWMColormapWindows to install the colormap */
    if (glw->SoglwDrawingArea.installColormap)
    {
	Widget parentShell = XtParent(w);
	Status status;
	Window *windowsReturn;
	int countReturn;
	
	while (parentShell && !XtIsShell(parentShell))
	    parentShell = XtParent(parentShell);
	if (parentShell && XtWindow(parentShell))
	{
	    /* check to see if there is already a property */
	    status = XGetWMColormapWindows(XtDisplay(parentShell),
					   XtWindow(parentShell),
					   &windowsReturn, &countReturn);
	    
	    /* if no property, just create one */
	    if (!status)
	    {
		Window windows[2];
		windows[0] = XtWindow(w);
		windows[1] = XtWindow(parentShell);
		XSetWMColormapWindows(XtDisplay(parentShell),
				      XtWindow(parentShell),
				      windows, 2);
	    }
	    /* there was a property, add myself to the beginning */
	    else
	    {
		Window *windows = (Window *)XtMalloc((sizeof(Window))*
						     (countReturn+1));
		register int i;
		windows[0] = XtWindow(w);
		for (i=0; i<countReturn; i++)
		    windows[i+1] = windowsReturn[i];
		XSetWMColormapWindows(XtDisplay(parentShell),
				      XtWindow(parentShell),
				      windows, countReturn+1);
		XtFree((char *) windows);
		XFree((char *) windowsReturn);
	    }
	}
	else
	    warning(w, "Could not set colormap property on parent shell");
    }
    cb.reason = SoGLwCR_GINIT;
    cb.event = NULL;
    cb.width = glw->core.width;
    cb.height = glw->core.height;
    XtCallCallbackList((Widget) glw, glw->SoglwDrawingArea.ginitCallback, &cb);
}

static void
Redraw(SoGLwMDrawingAreaWidget w, XEvent *event, Region region)
{
   SoGLwDrawingAreaCallbackStruct cb;
   
   cb.reason = SoGLwCR_EXPOSE;
   cb.event = event;
   cb.width = w->core.width;
   cb.height = w->core.height;
   XtCallCallbackList ((Widget) w, w->SoglwDrawingArea.exposeCallback, &cb);
}

static void
Resize(SoGLwMDrawingAreaWidget glw)
{
    SoGLwDrawingAreaCallbackStruct cb;

    /* if we get a resize event before being realized, we can't handle it */
    if (!XtIsRealized((Widget)glw))
	return;
   cb.reason = SoGLwCR_RESIZE;
   cb.event = NULL;
   cb.width = glw->core.width;
   cb.height = glw->core.height;
   XtCallCallbackList ((Widget) glw, glw->SoglwDrawingArea.resizeCallback, &cb);
}

static void
Destroy(SoGLwMDrawingAreaWidget glw)
{
    /* if my colormap was installed, remove it */
    if (glw->SoglwDrawingArea.installColormap)
    {
	Widget parentShell = XtParent(glw);
	Status status;
	Window *windowsReturn;
	int countReturn;
	register int i;
    
	/* find the parent shell */
	while (parentShell && !XtIsShell(parentShell))
	    parentShell = XtParent(parentShell);

	if (parentShell && XtWindow(parentShell))
	{
	    /* make sure there is a property */
	    status = XGetWMColormapWindows(XtDisplay(parentShell),
					   XtWindow(parentShell),
					   &windowsReturn, &countReturn);
	    
	    /* if no property, just return.  If there was a property,
	     * continue
	     */
	    if (status)
	    {
		/* search for a match */
		for (i=0; i<countReturn; i++)
		{
		    if (windowsReturn[i] == XtWindow(glw))
		    {
			/* we found a match, now copu the rest down */
			for (i++; i<countReturn; i++)
			{
			    windowsReturn[i-1] = windowsReturn[i];
			}
			XSetWMColormapWindows(XtDisplay(parentShell),
					      XtWindow(parentShell),
					      windowsReturn, countReturn-1);
			break;	/* from outer for */
		    }
		}
		XFree((char *) windowsReturn);
	    }
	}
    }
}

/* Provide a Motif-style create routine */
Widget SoGLwCreateMDrawingArea(Widget parent, char *name,
			     ArgList arglist, Cardinal argcount)
{
    return (XtCreateWidget (name, SoglwMDrawingAreaWidgetClass, parent, arglist,
			    argcount));
}

#undef USE_SGI_COLOR_RESOURCES

TEMPLATE = lib
TARGET = glu

DESTDIR  = ../../build

CONFIG += opengl static
CONFIG -= qt

DEFINES += LIBRARYBUILD RESOLVE_3D_TEXTURE_SUPPORT _CRT_SECURE_NO_WARNINGS

CONFIG(release, debug|release) {
 DEFINES += NDEBUG
}

DEPENDPATH += . \
              sgi \
              include/GL \
              sgi/include \
              sgi/libtess \
              sgi/libutil \
              sgi/libnurbs/interface \
              sgi/libnurbs/internals \
              sgi/libnurbs/nurbtess
INCLUDEPATH += include \
               sgi/include \
               sgi/libtess \
               sgi/libutil \
               sgi/libnurbs/interface \
               sgi/libnurbs/internals \
               sgi/libnurbs/nurbtess

# Input
HEADERS += include/GL/glu.h \
           include/GL/glu_mangle.h \
           sgi/include/gluos.h \
           sgi/libtess/dict-list.h \
           sgi/libtess/dict.h \
           sgi/libtess/geom.h \
           sgi/libtess/memalloc.h \
           sgi/libtess/mesh.h \
           sgi/libtess/normal.h \
           sgi/libtess/priorityq-heap.h \
           sgi/libtess/priorityq-sort.h \
           sgi/libtess/priorityq.h \
           sgi/libtess/render.h \
           sgi/libtess/sweep.h \
           sgi/libtess/tess.h \
           sgi/libtess/tessmono.h \
           sgi/libutil/gluint.h \
           sgi/libnurbs/interface/bezierEval.h \
           sgi/libnurbs/interface/bezierPatch.h \
           sgi/libnurbs/interface/bezierPatchMesh.h \
           sgi/libnurbs/interface/glcurveval.h \
           sgi/libnurbs/interface/glimports.h \
           sgi/libnurbs/interface/glrenderer.h \
           sgi/libnurbs/interface/glsurfeval.h \
           sgi/libnurbs/interface/mystdio.h \
           sgi/libnurbs/interface/mystdlib.h \
           sgi/libnurbs/internals/arc.h \
           sgi/libnurbs/internals/arcsorter.h \
           sgi/libnurbs/internals/arctess.h \
           sgi/libnurbs/internals/backend.h \
           sgi/libnurbs/internals/basiccrveval.h \
           sgi/libnurbs/internals/basicsurfeval.h \
           sgi/libnurbs/internals/bezierarc.h \
           sgi/libnurbs/internals/bin.h \
           sgi/libnurbs/internals/bufpool.h \
           sgi/libnurbs/internals/cachingeval.h \
           sgi/libnurbs/internals/coveandtiler.h \
           sgi/libnurbs/internals/curve.h \
           sgi/libnurbs/internals/curvelist.h \
           sgi/libnurbs/internals/dataTransform.h \
           sgi/libnurbs/internals/defines.h \
           sgi/libnurbs/internals/displaylist.h \
           sgi/libnurbs/internals/displaymode.h \
           sgi/libnurbs/internals/flist.h \
           sgi/libnurbs/internals/flistsorter.h \
           sgi/libnurbs/internals/gridline.h \
           sgi/libnurbs/internals/gridtrimvertex.h \
           sgi/libnurbs/internals/gridvertex.h \
           sgi/libnurbs/internals/hull.h \
           sgi/libnurbs/internals/jarcloc.h \
           sgi/libnurbs/internals/knotvector.h \
           sgi/libnurbs/internals/mapdesc.h \
           sgi/libnurbs/internals/maplist.h \
           sgi/libnurbs/internals/mesher.h \
           sgi/libnurbs/internals/monotonizer.h \
           sgi/libnurbs/internals/myassert.h \
           sgi/libnurbs/internals/mymath.h \
           sgi/libnurbs/internals/mysetjmp.h \
           sgi/libnurbs/internals/mystring.h \
           sgi/libnurbs/internals/nurbsconsts.h \
           sgi/libnurbs/internals/nurbstess.h \
           sgi/libnurbs/internals/patch.h \
           sgi/libnurbs/internals/patchlist.h \
           sgi/libnurbs/internals/pwlarc.h \
           sgi/libnurbs/internals/quilt.h \
           sgi/libnurbs/internals/reader.h \
           sgi/libnurbs/internals/renderhints.h \
           sgi/libnurbs/internals/simplemath.h \
           sgi/libnurbs/internals/slicer.h \
           sgi/libnurbs/internals/sorter.h \
           sgi/libnurbs/internals/subdivider.h \
           sgi/libnurbs/internals/trimline.h \
           sgi/libnurbs/internals/trimregion.h \
           sgi/libnurbs/internals/trimvertex.h \
           sgi/libnurbs/internals/trimvertpool.h \
           sgi/libnurbs/internals/types.h \
           sgi/libnurbs/internals/uarray.h \
           sgi/libnurbs/internals/varray.h \
           sgi/libnurbs/nurbtess/definitions.h \
           sgi/libnurbs/nurbtess/directedLine.h \
           sgi/libnurbs/nurbtess/glimports.h \
           sgi/libnurbs/nurbtess/gridWrap.h \
           sgi/libnurbs/nurbtess/monoChain.h \
           sgi/libnurbs/nurbtess/monoPolyPart.h \
           sgi/libnurbs/nurbtess/monoTriangulation.h \
           sgi/libnurbs/nurbtess/mystdio.h \
           sgi/libnurbs/nurbtess/mystdlib.h \
           sgi/libnurbs/nurbtess/partitionX.h \
           sgi/libnurbs/nurbtess/partitionY.h \
           sgi/libnurbs/nurbtess/polyDBG.h \
           sgi/libnurbs/nurbtess/polyUtil.h \
           sgi/libnurbs/nurbtess/primitiveStream.h \
           sgi/libnurbs/nurbtess/quicksort.h \
           sgi/libnurbs/nurbtess/rectBlock.h \
           sgi/libnurbs/nurbtess/sampleComp.h \
           sgi/libnurbs/nurbtess/sampleCompBot.h \
           sgi/libnurbs/nurbtess/sampleCompRight.h \
           sgi/libnurbs/nurbtess/sampleCompTop.h \
           sgi/libnurbs/nurbtess/sampledLine.h \
           sgi/libnurbs/nurbtess/sampleMonoPoly.h \
           sgi/libnurbs/nurbtess/searchTree.h \
           sgi/libnurbs/nurbtess/zlassert.h \
           sgi/libtess/priorityq-heap.c
SOURCES += sgi/libtess/dict.c \
           sgi/libtess/geom.c \
           sgi/libtess/memalloc.c \
           sgi/libtess/mesh.c \
           sgi/libtess/normal.c \
           sgi/libtess/priorityq-heap.c \
           sgi/libtess/priorityq.c \
           sgi/libtess/render.c \
           sgi/libtess/sweep.c \
           sgi/libtess/tess.c \
           sgi/libtess/tessmono.c \
           sgi/libutil/error.c \
           sgi/libutil/glue.c \
           sgi/libutil/mipmap.c \
           sgi/libutil/project.c \
           sgi/libutil/quad.c \
           sgi/libutil/registry.c \
           sgi/libnurbs/interface/bezierEval.cc \
           sgi/libnurbs/interface/bezierPatch.cc \
           sgi/libnurbs/interface/bezierPatchMesh.cc \
           sgi/libnurbs/interface/glcurveval.cc \
           sgi/libnurbs/interface/glinterface.cc \
           sgi/libnurbs/interface/glrenderer.cc \
           sgi/libnurbs/interface/glsurfeval.cc \
           sgi/libnurbs/interface/incurveeval.cc \
           sgi/libnurbs/interface/insurfeval.cc \
           sgi/libnurbs/internals/arc.cc \
           sgi/libnurbs/internals/arcsorter.cc \
           sgi/libnurbs/internals/arctess.cc \
           sgi/libnurbs/internals/backend.cc \
           sgi/libnurbs/internals/basiccrveval.cc \
           sgi/libnurbs/internals/basicsurfeval.cc \
           sgi/libnurbs/internals/bin.cc \
           sgi/libnurbs/internals/bufpool.cc \
           sgi/libnurbs/internals/cachingeval.cc \
           sgi/libnurbs/internals/ccw.cc \
           sgi/libnurbs/internals/coveandtiler.cc \
           sgi/libnurbs/internals/curve.cc \
           sgi/libnurbs/internals/curvelist.cc \
           sgi/libnurbs/internals/curvesub.cc \
           sgi/libnurbs/internals/dataTransform.cc \
           sgi/libnurbs/internals/displaylist.cc \
           sgi/libnurbs/internals/flist.cc \
           sgi/libnurbs/internals/flistsorter.cc \
           sgi/libnurbs/internals/hull.cc \
           sgi/libnurbs/internals/intersect.cc \
           sgi/libnurbs/internals/knotvector.cc \
           sgi/libnurbs/internals/mapdesc.cc \
           sgi/libnurbs/internals/mapdescv.cc \
           sgi/libnurbs/internals/maplist.cc \
           sgi/libnurbs/internals/mesher.cc \
           sgi/libnurbs/internals/monotonizer.cc \
           sgi/libnurbs/internals/monoTriangulationBackend.cc \
           sgi/libnurbs/internals/mycode.cc \
           sgi/libnurbs/internals/nurbsinterfac.cc \
           sgi/libnurbs/internals/nurbstess.cc \
           sgi/libnurbs/internals/patch.cc \
           sgi/libnurbs/internals/patchlist.cc \
           sgi/libnurbs/internals/quilt.cc \
           sgi/libnurbs/internals/reader.cc \
           sgi/libnurbs/internals/renderhints.cc \
           sgi/libnurbs/internals/slicer.cc \
           sgi/libnurbs/internals/sorter.cc \
           sgi/libnurbs/internals/splitarcs.cc \
           sgi/libnurbs/internals/subdivider.cc \
           sgi/libnurbs/internals/tobezier.cc \
           sgi/libnurbs/internals/trimline.cc \
           sgi/libnurbs/internals/trimregion.cc \
           sgi/libnurbs/internals/trimvertpool.cc \
           sgi/libnurbs/internals/uarray.cc \
           sgi/libnurbs/internals/varray.cc \
           sgi/libnurbs/nurbtess/directedLine.cc \
           sgi/libnurbs/nurbtess/gridWrap.cc \
           sgi/libnurbs/nurbtess/monoChain.cc \
           sgi/libnurbs/nurbtess/monoPolyPart.cc \
           sgi/libnurbs/nurbtess/monoTriangulation.cc \
           sgi/libnurbs/nurbtess/partitionX.cc \
           sgi/libnurbs/nurbtess/partitionY.cc \
           sgi/libnurbs/nurbtess/polyDBG.cc \
           sgi/libnurbs/nurbtess/polyUtil.cc \
           sgi/libnurbs/nurbtess/primitiveStream.cc \
           sgi/libnurbs/nurbtess/quicksort.cc \
           sgi/libnurbs/nurbtess/rectBlock.cc \
           sgi/libnurbs/nurbtess/sampleComp.cc \
           sgi/libnurbs/nurbtess/sampleCompBot.cc \
           sgi/libnurbs/nurbtess/sampleCompRight.cc \
           sgi/libnurbs/nurbtess/sampleCompTop.cc \
           sgi/libnurbs/nurbtess/sampledLine.cc \
           sgi/libnurbs/nurbtess/sampleMonoPoly.cc \
           sgi/libnurbs/nurbtess/searchTree.cc

set(HEADERS
    IfAssert.h
    IfBuilder.h
    IfCollector.h
    IfCondenser.h
    IfFixer.h
    IfFlattener.h
    IfHasher.h
    IfHolder.h
    IfMerger.h
    IfReplacer.h
    IfReporter.h
    IfShape.h
    IfShapeList.h
    IfSorter.h
    IfStripper.h
    IfTypes.h
    IfWeeder.h
)

set(SOURCES
    ivfix.c++
    IfBuilder.c++
    IfCollector.c++
    IfCondenser.c++
    IfFixer.c++
    IfFlattener.c++
    IfHasher.c++
    IfHolder.c++
    IfMerger.c++
    IfReplacer.c++
    IfReporter.c++
    IfShape.c++
    IfShapeList.c++
    IfSorter.c++
    IfStripper.c++
    IfTypes.c++
    IfWeeder.c++
)

add_executable(ivfix ${HEADERS} ${SOURCES})
target_link_libraries(ivfix Inventor)

install(TARGETS ivfix DESTINATION bin)

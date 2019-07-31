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

add_executable(ivfix ${SOURCES})
target_link_libraries(ivfix Inventor)

install(TARGETS ivfix DESTINATION bin)
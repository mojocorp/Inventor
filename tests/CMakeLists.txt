set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    main.c++
    utils.h
    utils.c++
    testUtils.c++
    testVec2f.c++
    testVec3f.c++
    testColor.c++
    testBox3f.c++
    testIO.c++
    testCreation.c++
    testMatrix.c++
    testName.c++
    testString.c++
    testEndian.c++
    testFile.c++
    testFields.c++
    testOffscreen.c++
    testModels.c++
    testImage.c++
    stb_image_write.h

)

if(APPLE)
    add_definitions(-DSB_OS_MACOS)
elseif(WIN32)
    add_definitions(-DSB_OS_WIN)
elseif(UNIX)
    add_definitions(-DSB_OS_LINUX)
else()
    message( FATAL_ERROR "Inventor has not been ported to this OS." )
endif()

add_executable(ivtest ${SOURCES})
target_link_libraries(ivtest Inventor gmock_main pdiff)
set_target_properties(ivtest PROPERTIES FOLDER "tests")

install(TARGETS ivtest DESTINATION bin)


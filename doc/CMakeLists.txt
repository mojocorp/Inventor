find_package(Doxygen REQUIRED)

set(DOXYGEN_GENERATE_HTML YES)
set(DOXYGEN_GENERATE_MAN NO)

set(DOXYGEN_PROJECT_NAME "SGI Open Inventor")
set(DOXYGEN_PROJECT_NUMBER ${INVENTOR_VERSION})
set(DOXYGEN_EXTRACT_ALL YES)
set(DOXYGEN_JAVADOC_AUTOBRIEF YES)
set(DOXYGEN_HTML_TIMESTAMP YES)
set(DOXYGEN_FILE_PATTERNS *.h *.dox)
set(DOXYGEN_STRIP_FROM_INC_PATH ${PROJECT_SOURCE_DIR}/lib/database/include
                                ${PROJECT_SOURCE_DIR}/lib/interaction/include
                                ${PROJECT_SOURCE_DIR}/lib/nodekits/include
                                ${PROJECT_SOURCE_DIR}/libSoXt/include/Inventor/Xt/viewers)
set(DOXYGEN_EXCLUDE ${PROJECT_SOURCE_DIR}/lib/database/include/Inventor/misc/upgraders 
                    ${PROJECT_SOURCE_DIR}/lib/database/include/Inventor/elements 
                    ${PROJECT_SOURCE_DIR}/lib/database/include/Inventor/lists 
                    ${PROJECT_SOURCE_DIR}/lib/nodekits/include/Inventor/misc/upgraders)
set(DOXYGEN_IGNORE_PREFIX Sb So)
set(DOXYGEN_MACRO_EXPANSION YES)
set(DOXYGEN_PREDEFINED INVENTOR_API=)
set(DOXYGEN_EXPAND_AS_DEFINED SO__FIELD_HEADER
                              SO__SFIELD_RW_HEADER
                              SO__MFIELD_RW_HEADER
                              SO_SFIELD_REQUIRED_HEADER
                              SO_SFIELD_CONSTRUCTOR_HEADER
                              SO_SFIELD_VALUE_HEADER
                              SO_SFIELD_DERIVED_VALUE_HEADER
                              SO_SFIELD_HEADER
                              SO_SFIELD_DERIVED_HEADER
                              SO_MFIELD_REQUIRED_HEADER
                              SO_MFIELD_CONSTRUCTOR_HEADER
                              SO_MFIELD_VALUE_HEADER
                              SO_MFIELD_DERIVED_VALUE_HEADER
                              SO_MFIELD_HEADER
                              SO_MFIELD_DERIVED_HEADER)
set(DOXYGEN_SKIP_FUNCTION_MACROS NO)

doxygen_add_docs(
    doxygen
    ${CMAKE_CURRENT_SOURCE_DIR}/modules.dox
    ${CMAKE_CURRENT_SOURCE_DIR}/mainpage.dox
    ${PROJECT_SOURCE_DIR}/lib/
    ${PROJECT_SOURCE_DIR}/libSoXt/include/Inventor/Xt/viewers
    ALL
    COMMENT "Generating API documentation"
)
set_target_properties(doxygen PROPERTIES FOLDER "doc/html")
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html DESTINATION doc)

add_subdirectory(help)
add_subdirectory(man)

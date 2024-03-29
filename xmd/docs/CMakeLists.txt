# https://devblogs.microsoft.com/cppblog/clear-functional-c-documentation-with-sphinx-breathe-doxygen-cmake/
find_package(Doxygen REQUIRED)
find_package(Sphinx REQUIRED)

set(DOXYGEN_INPUT "\"${PROJECT_SOURCE_DIR}/xmd/include\" \"${PROJECT_SOURCE_DIR}/xmd/src\"")

set(DOXYGEN_INPUT_DIR ${PROJECT_SOURCE_DIR}/xmd)
set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/doxygen)
set(DOXYGEN_INDEX_FILE ${DOXYGEN_OUTPUT_DIR}/html/index.html)
set(DOXYFILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
set(DOXYFILE_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)

file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR})
add_custom_command(
    OUTPUT ${DOXYGEN_INDEX_FILE}
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
    MAIN_DEPENDENCY ${DOXYFILE_OUT} ${DOXYFILE_IN}
    COMMENT "Generating docs")

add_custom_target(Doxygen ALL DEPENDS ${DOXYGEN_INDEX_FILE})

set(SPHINX_SOURCE ${CMAKE_CURRENT_LIST_DIR})
set(SPHINX_BUILD ${CMAKE_CURRENT_BINARY_DIR}/sphinx)
set(SPHINX_INDEX_FILE ${SPHINX_BUILD}/index.html)

add_custom_command(OUTPUT ${SPHINX_INDEX_FILE}
    COMMAND
    ${SPHINX_EXECUTABLE} -b html
    -Dbreathe_projects.xmd=${DOXYGEN_OUTPUT_DIR}/xml
    ${SPHINX_SOURCE} ${SPHINX_BUILD}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/index.rst
        ${DOXYGEN_INDEX_FILE}
    MAIN_DEPENDENCY ${SPHINX_SOURCE}/conf.py
    COMMENT "Generating documentation with Sphinx")

set(LOCAL_SPHINX_BUILD ${PROJECT_SOURCE_DIR}/docs)
set(LOCAL_SPHINX_INDEX ${LOCAL_SPHINX_BUILD}/index.html)

add_custom_command(OUTPUT ${LOCAL_SPHINX_INDEX}
    COMMAND
    ${CMAKE_COMMAND} -E rm -rf ${LOCAL_SPHINX_BUILD}
    COMMAND
    ${CMAKE_COMMAND} -E copy_directory ${SPHINX_BUILD}/ ${LOCAL_SPHINX_BUILD}
    COMMAND
    ${CMAKE_COMMAND} -E touch ${LOCAL_SPHINX_BUILD}/.nojekyll
    DEPENDS ${SPHINX_INDEX_FILE}
    COMMENT "Copying the generated docs to _build")

add_custom_target(Sphinx ALL DEPENDS ${LOCAL_SPHINX_INDEX})

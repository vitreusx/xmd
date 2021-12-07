file(GLOB_RECURSE PRIVATE_SOURCES CONFIGURE_DEPENDS
    src/*.cpp src/*.cxx
    src/*.hpp src/*.h src/*.inl)

file(GLOB_RECURSE PUBLIC_SOURCES CONFIGURE_DEPENDS
    include/*.hpp include/*.h include/*.inl)

set(TARGET_NAME "xmd")
set(TARGET_ROOT_DIR "xmd")

add_executable(${TARGET_NAME})

target_sources(${TARGET_NAME}
    PRIVATE ${PRIVATE_SOURCES}
    PUBLIC ${PUBLIC_SOURCES})

target_compile_features(${TARGET_NAME}
    PUBLIC cxx_std_17)

target_include_directories(${TARGET_NAME}
    PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/${TARGET_ROOT_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include/${TARGET_ROOT_DIR}>
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

target_compile_options(${TARGET_NAME}
    PRIVATE
    $<$<CONFIG:Debug>:-g;$<$<COMPILE_LANGUAGE:CXX>:-Og>>
    $<$<CONFIG:Release>:$<$<COMPILE_LANGUAGE:CXX>:-Ofast;-march=native>>
    $<$<COMPILE_LANGUAGE:CXX>:-Wall;-Wextra;-Wpedantic>)

find_package(Eigen3 REQUIRED)
find_package(yaml-cpp REQUIRED)

target_link_libraries(${TARGET_NAME}
    PUBLIC Eigen3::Eigen yaml-cpp)

file(COPY data DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

add_custom_command(
    TARGET ${TARGET_NAME}
    POST_BUILD
    COMMAND /usr/bin/zsh ${PROJECT_SOURCE_DIR}/scripts/gen_asm.zsh
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
file(GLOB_RECURSE PRIVATE_SOURCES CONFIGURE_DEPENDS
    src/*.cpp src/*.cxx
    src/*.hpp src/*.h src/*.inl)

file(GLOB_RECURSE PUBLIC_SOURCES CONFIGURE_DEPENDS
    include/*.hpp include/*.h include/*.inl)

set(TARGET_NAME "cpu_ref")
set(TARGET_ROOT_DIR "xmd")

add_library(${TARGET_NAME} INTERFACE)

target_sources(${TARGET_NAME}
    INTERFACE ${PUBLIC_SOURCES})

target_compile_features(${TARGET_NAME}
    INTERFACE cxx_std_17)

target_include_directories(${TARGET_NAME}
    INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

target_compile_options(${TARGET_NAME}
    INTERFACE
    $<$<CONFIG:Debug>:-g;$<$<COMPILE_LANGUAGE:CXX>:-Og>>
    $<$<CONFIG:Release>:$<$<COMPILE_LANGUAGE:CXX>:-Ofast;-march=native>>
    $<$<COMPILE_LANGUAGE:CXX>:-Wall;-Wextra;-Wpedantic>)

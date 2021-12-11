add_executable(sandbox main.cpp)
target_link_libraries(sandbox PRIVATE xmd)
file(COPY data DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

add_executable(cparse-test cparse-test.cpp)
target_link_libraries(cparse-test PRIVATE cparse)

add_executable(taskflow-tests taskflow.cpp)
target_link_libraries(taskflow-tests PRIVATE Taskflow)
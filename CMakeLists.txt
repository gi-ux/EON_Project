cmake_minimum_required(VERSION 3.17)
project(EON_Project)

set(CMAKE_CXX_STANDARD 14)

include_directories(dependencies/amplapi/include)
link_directories(dependencies/amplapi/lib)
link_libraries(ampl-2.0.4.0)

add_executable(EON_Project src/main.cpp src/Path.cpp src/Path.h src/Link.cpp src/Link.h src/Data.cpp src/NDArray.cpp src/Demand.cpp src/Demand.h src/PathsForT.cpp src/PathsForT.h src/Data.h src/Parser.cpp src/HeuristicGreedy.cpp)

add_custom_command(TARGET EON_Project POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/dependencies/ampl"
        $<TARGET_FILE_DIR:EON_Project>/ampl )


add_custom_command(TARGET EON_Project POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PROJECT_SOURCE_DIR}/dependencies/amplapi/bin/ampl-2.0.4.0.dll"
        $<TARGET_FILE_DIR:EON_Project>)
cmake_minimum_required(VERSION 3.0.0)
project(par_sort VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include(CTest)
enable_testing()

add_executable(par_sort main.cpp)
# Find TBB
find_package(TBB REQUIRED)
target_link_libraries(par_sort PRIVATE TBB::tbb)

# Find Threads
find_package(Threads REQUIRED)

# Link against the Threads library
target_link_libraries(par_sort PRIVATE Threads::Threads)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
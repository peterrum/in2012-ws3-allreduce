cmake_minimum_required(VERSION 2.8.0)

option(CLUSTER "Are we building on the cluster?" OFF)

project(BONSAI)
cmake_policy(SET CMP0015 NEW)

get_cmake_property(CACHE_VARS CACHE_VARIABLES)
foreach(CACHE_VAR ${CACHE_VARS})
  get_property(CACHE_VAR_HELPSTRING CACHE ${CACHE_VAR} PROPERTY HELPSTRING)
  if(CACHE_VAR_HELPSTRING STREQUAL "No help, variable specified on the command line.")
    get_property(CACHE_VAR_TYPE CACHE ${CACHE_VAR} PROPERTY TYPE)
    if(CACHE_VAR_TYPE STREQUAL "UNINITIALIZED")
      set(CACHE_VAR_TYPE)
    else()
      set(CACHE_VAR_TYPE :${CACHE_VAR_TYPE})
    endif()
    set(CMAKE_ARGS "${CMAKE_ARGS} -D${CACHE_VAR}${CACHE_VAR_TYPE}=\"${${CACHE_VAR}}\"")
  endif()
endforeach()
message("CMAKE_ARGS: ${CMAKE_ARGS}")

if(DEFINED ${n})
    message(abba)
else()
    set(n "12")
    message(abbba)
endif()

message(${n})



# Set up compiler
if (CLUSTER)
 set(CMAKE_CXX_COMPILER "mpiCC")
 add_compile_options(-std=c++14)
else()
 set(CMAKE_CXX_COMPILER "mpic++")
 add_compile_options(-std=c++14)
endif()

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS} -pthread -lgomp")
endif()

#set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread -lgomp")

# Create executable
add_executable(main src/main.cpp)




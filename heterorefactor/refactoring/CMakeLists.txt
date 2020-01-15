cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

file(GLOB SRC "src/*.cpp" "src/*/*.cpp")
file(GLOB RUNTIME_ALLOC "runtime/hls-buddy-alloc.c")

function(make_includable input_file output_file)
    file(READ ${input_file} content)
    set(delim "for_c++_include")
    set(content "R\"${delim}(\n${content})${delim}\"")
    file(WRITE ${output_file} "${content}")
endfunction(make_includable)

make_includable(${RUNTIME_ALLOC}
    ${CMAKE_CURRENT_BINARY_DIR}/generated/runtime_alloc.frag)

include_directories("./include/"
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_SOURCE_DIR}/../../libraries/rose-compiler/dist/include
    ${CMAKE_SOURCE_DIR}/../../libraries/rose-compiler/dist/include/rose
    ${CMAKE_SOURCE_DIR}/../../libraries/rose-compiler/build/boost/include)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fdata-sections -ffunction-sections -Wno-deprecated-declarations")

add_executable(heterorefactor ${SRC})

# Comment the following lines to build dynamic linked program (1/2)
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--gc-sections -static -pthread -Wl,--whole-archive -lpthread -Wl,--no-whole-archive -Wl,--unresolved-symbols=ignore-in-object-files")
target_link_libraries(heterorefactor
    ${CMAKE_SOURCE_DIR}/../../libraries/rose-compiler/dist/lib/librose.a
    ${CMAKE_SOURCE_DIR}/../../libraries/rose-compiler/build/boost/lib/libboost_system.a
    ${CMAKE_SOURCE_DIR}/../../libraries/rose-compiler/build/boost/lib/libboost_regex.a
    ${CMAKE_SOURCE_DIR}/../../libraries/rose-compiler/build/boost/lib/libboost_thread.a
    ${CMAKE_SOURCE_DIR}/../../libraries/rose-compiler/build/boost/lib/libboost_filesystem.a
    ${CMAKE_SOURCE_DIR}/../../libraries/rose-compiler/build/boost/lib/libboost_wave.a
    ${CMAKE_SOURCE_DIR}/../../libraries/rose-compiler/build/boost/lib/libboost_iostreams.a
    dl)
# Comment the above lines to build dynamic linked program (1/2)

# Uncomment this line to build dynamic linked program (2/2)
# target_link_libraries(heterorefactor rose boost_system boost_regex)
# Remember to set your LD_LIBRARY_PATH to ../../libraries/rose-compiler/dist/lib/
#   and ../../libraries/rose-compiler/build/boost/lib/

target_compile_features(heterorefactor PRIVATE cxx_auto_type)
target_compile_features(heterorefactor PRIVATE cxx_range_for)
target_compile_features(heterorefactor PRIVATE cxx_lambda_init_captures)
install(TARGETS heterorefactor DESTINATION bin)

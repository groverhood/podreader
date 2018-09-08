cmake_minimum_required(VERSION 3.10)
project(podreader)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER /usr/bin/g++-8)

include_directories(podreader/src)
include_directories(podreader/src/json)
include_directories(podreader/src/meta)

add_executable(podreadertest1
        podreader/src/json/jsonreader.hxx
        podreader/src/json/operator.hxx
        podreader/src/json/string.hxx
        podreader/src/json/value.hxx
        podreader/src/meta/meta.hxx
        podreader/test/test1.cxx)

add_executable(podreadertest2
        podreader/src/json/jsonreader.hxx
        podreader/src/json/operator.hxx
        podreader/src/json/string.hxx
        podreader/src/json/value.hxx
        podreader/src/meta/meta.hxx
        podreader/test/test2.cxx)

add_executable(podreadertest3
        podreader/src/json/jsonreader.hxx
        podreader/src/json/operator.hxx
        podreader/src/json/string.hxx
        podreader/src/json/value.hxx
        podreader/src/meta/meta.hxx
        podreader/test/test3.cxx)

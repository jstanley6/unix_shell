cmake_minimum_required(VERSION 3.3)
project(Lab8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        mssh.c
        pipes/pipes.h
        pipes/pipes.c
        process/process.h
        tokenize/makeArgs.h
        tokenize/makeArgs.c
        process/process.c
        utils/myUtils.c
        utils/myUtils.h
        utils/fileUtil.h
        pipes/pipes.c
        linkedlist/listUtils.c
        linkedlist/linkedList.c
        linkedlist/linkedList.h
        linkedlist/listUtils.h
        linkedlist/genericData.h
        linkedlist/requiredIncludes.h
        history/history.h
        history/history.c
        alias/alias.c alias/alias.h)
add_executable(Lab8 ${SOURCE_FILES})
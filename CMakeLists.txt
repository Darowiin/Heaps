cmake_minimum_required(VERSION 3.11)

project(Heaps CXX)

set(CMAKE_CXX_STANDARD 23)

# Специфичные настройки для компилятора Visual Studio
if(MSVC)
    # Генерируем статическую заглушку для динамических библиотек
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS true)

    # Включаем высокий уровень предупреждений
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()
    set(CMAKE_CXX_FLAGS " /Ob0 ${CMAKE_CXX_FLAGS} ")
endif()

# Объявляем библиотеку
add_library(FibonacciHeap SHARED "include/Heaps/FibonacciHeap.h" "src/FibonacciHeap.cpp")
target_include_directories(FibonacciHeap PUBLIC "include")

add_library(BinomialHeap SHARED "include/Heaps/BinomialHeap.h" "src/BinomialHeap.cpp")
target_include_directories(BinomialHeap PUBLIC "include")

# Подключаем googletest
include(FetchContent)
FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/6b63c98ac43efc992122f1da12aaf0a0e0658206.zip)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Объявляем тесты
add_executable(FibonacciHeapTests "tests/FibonacciHeapTests.cpp")
target_link_libraries(FibonacciHeapTests FibonacciHeap GTest::gtest_main)

add_executable(BinomialHeapTests "tests/BinomialHeapTests.cpp")
target_link_libraries(BinomialHeapTests BinomialHeap GTest::gtest_main)

add_executable(measureTime "tests/measureTime.cpp")
target_link_libraries(measureTime FibonacciHeap BinomialHeap GTest::gtest_main)
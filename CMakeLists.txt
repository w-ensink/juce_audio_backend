cmake_minimum_required(VERSION 3.18)
project(juce_audio_backend LANGUAGES CXX)

add_subdirectory(JUCE)
add_library(${PROJECT_NAME} STATIC include/juce_audio.h source/juce_audio.cpp)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_14)
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(${PROJECT_NAME} PRIVATE juce::juce_audio_devices)
target_compile_definitions(${PROJECT_NAME} PRIVATE -DJUCE_USE_CURL=0)

#-----------------------------------------

if (BUILD_TEST)
    add_executable(audio_test test/test_main.cpp)
    target_compile_features(audio_test PRIVATE cxx_std_14)
    target_link_libraries(audio_test PRIVATE ${PROJECT_NAME})
endif()
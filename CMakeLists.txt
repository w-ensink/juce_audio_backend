cmake_minimum_required(VERSION 3.21)
project(juce_audio_backend LANGUAGES CXX)

add_subdirectory(JUCE)
add_library(${PROJECT_NAME} STATIC juce_audio.h juce_audio.cpp)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_14)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE juce::juce_audio_devices)


#-----------------------------------------

add_executable(audio_test test_main.cpp)
target_compile_features(audio_test PRIVATE cxx_std_14)
target_link_libraries(audio_test PRIVATE ${PROJECT_NAME})
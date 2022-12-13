cmake_minimum_required(VERSION 3.15)

project(Lab)
find_package(OpenGL REQUIRED)

file(GLOB_RECURSE files_code
         RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
         ${CMAKE_CURRENT_SOURCE_DIR}/Lab/*.cpp 
         ${CMAKE_CURRENT_SOURCE_DIR}/Lab/*.h)

file(GLOB_RECURSE ASSETS  
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/*
)

add_subdirectory(external/glad)
add_subdirectory(external/glfw)
add_subdirectory(external/glm)


include_directories(external/glfw/include/)
include_directories(external/stb/)
include_directories(external/glm/)
include_directories(external/glad/include/)

add_executable(${PROJECT_NAME} ${files_code})
target_link_libraries(${PROJECT_NAME} PUBLIC glfw glad OpenGL::GL)

target_link_libraries(${PROJECT_NAME} PUBLIC ${EXTRA_LIBS})
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_BINARY_DIR}" ${EXTRA_INCLUDES})

target_compile_definitions(${PROJECT_NAME} PRIVATE STB_IMAGE_IMPLEMENTATION)
target_compile_definitions(${PROJECT_NAME} PRIVATE STB_IMAGE_STATIC)

#set_target_properties(${PROJECT_NAME} PROPERTIES
#    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})


add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/resources
                ${CMAKE_CURRENT_BINARY_DIR}/resources)


target_compile_definitions(${PROJECT_NAME} PRIVATE
  TEXTURES_DIR="resources/textures/")

target_compile_definitions(${PROJECT_NAME} PRIVATE
  SHADERS_DIR="resources/shaders/")

target_sources(${PROJECT_NAME} PRIVATE ${ASSETS})
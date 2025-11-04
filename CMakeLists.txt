# CMakeLists.txt
cmake_minimum_required(VERSION 3.15)
project(skyRenderer LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(CMAKE_CXX_STANDARD 20)

# Add SDL as a subdirectory
add_subdirectory(external/SDL EXCLUDE_FROM_ALL)

# Add vk-bootstrap as a subdirectory
add_subdirectory(external/vk-bootstrap)

# Add Dear ImGUI as a library 
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui)
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdl3.cpp
)

add_library(imgui STATIC ${IMGUI_SOURCES})

target_include_directories(imgui PUBLIC ${IMGUI_DIR} ${IMGUI_DIR}/backends)
target_link_libraries(imgui PRIVATE SDL3::SDL3)

# Add Vulkan Memory Allocator as a library
add_library(VulkanMemoryAllocator INTERFACE)
target_include_directories(VulkanMemoryAllocator INTERFACE ${CMAKE_SOURCE_DIR}/external/VulkanMemoryAllocator/include)

# Add GLM as a library
add_library(glm INTERFACE)
target_include_directories(glm INTERFACE external/glm)

# Find packages
find_package(Vulkan REQUIRED)

# Compile shaders
set(SHADER_DIR "${CMAKE_SOURCE_DIR}/shaders")
set(SPIRV_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders_spv")

file(MAKE_DIRECTORY ${SPIRV_DIR})

set(SHADER_FILES

)

set(SPIRV_FILES "")

foreach(SHADER_META ${SHADER_FILES})
    string(REGEX MATCHALL "[^|]+" SHADER_PARTS "${SHADER_META}")
    list(GET SHADER_PARTS 0 SHADER_FILE)
    list(GET SHADER_PARTS 1 COMBINED_ENTRY_POINTS)

    string(REGEX MATCHALL "[^,]+" ENTRY_POINTS "${COMBINED_ENTRY_POINTS}")

    set(ENTRY_ARGS "")
    foreach(ENTRY ${ENTRY_POINTS})
        string(REGEX MATCHALL "[^:]+" ENTRY_SPLIT ${ENTRY})
        list(GET ENTRY_SPLIT 0 STAGE)
        list(GET ENTRY_SPLIT 1 ENTRY_NAME)

        list(APPEND ENTRY_ARGS -entry ${ENTRY_NAME} -stage ${STAGE})
    endforeach()

    get_filename_component(BASE_NAME ${SHADER_FILE} NAME_WE)
    set(SPIRV_FILE "${SPIRV_DIR}/${BASE_NAME}.spv")

    add_custom_command(
        OUTPUT ${SPIRV_FILE}
        COMMAND slangc ${SHADER_DIR}/${SHADER_FILE} -target spirv -o ${SPIRV_FILE} ${ENTRY_ARGS}
        DEPENDS ${SHADER_DIR}/${SHADER_FILE}
        COMMENT "Compiling ${BASE_NAME} stage ${STAGE} entry ${ENTRY_NAME}"
        VERBATIM
    )
    list(APPEND SPIRV_FILES ${SPIRV_FILE})
    
endforeach()

add_custom_target(Shaders ALL DEPENDS ${SPIRV_FILES})

# Compile executable
file(GLOB SOURCES CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)

add_executable(skyRenderer ${SOURCES})
add_dependencies(skyRenderer Shaders)

target_include_directories(skyRenderer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(skyRenderer PRIVATE
    ${Vulkan_LIBRARIES}
    SDL3::SDL3
    vk-bootstrap::vk-bootstrap
    imgui
    VulkanMemoryAllocator
    glm
)
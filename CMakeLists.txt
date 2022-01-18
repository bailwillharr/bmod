cmake_minimum_required(VERSION 3.10)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

project(bmod VERSION 1.0)
add_executable(bmod src/main.c src/game.c src/window.c src/logger.c src/game_input.c src/game_camera.c src/render/renderer.c src/render/shader.c src/render/camera.c src/object/object.c src/object/component.c src/object/component/component_mesh.c src/object/component/component_mesh_renderer.c)

add_subdirectory(modelconv)

add_subdirectory(glad)
target_link_libraries(bmod PRIVATE glad)

if (NOT MINGW)
	target_link_libraries(bmod PRIVATE ld)
endif()

target_link_libraries(bmod PRIVATE glfw3 m)

configure_file(config.h.in config.h)
target_include_directories(bmod PRIVATE ${PROJECT_BINARY_DIR})

add_custom_command(
	TARGET bmod POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E create_symlink
	${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:bmod>/res)

install(TARGETS bmod DESTINATION bin)
install(DIRECTORY res/ DESTINATION share/bmod)

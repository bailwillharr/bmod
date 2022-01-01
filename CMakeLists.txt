cmake_minimum_required(VERSION 3.10)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

project(bmod VERSION 1.0)
add_executable(bmod src/main.c src/game.c src/window.c src/logger.c src/game_input.c src/game_camera.c src/glad/glad.c src/render/renderer.c src/render/shader.c src/render/camera.c)

find_library(GLFW glfw)
target_link_libraries(bmod PUBLIC ${GLFW} m dl)

add_custom_command(
	TARGET bmod POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E create_symlink
	${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:bmod>/res)

install(TARGETS bmod DESTINATION bin)
install(DIRECTORY res/ DESTINATION share/bmod)

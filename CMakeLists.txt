cmake_minimum_required (VERSION 3.3)

project (t1)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
cmake_policy(SET CMP0015 NEW)

set(HELLO_SRC_DIR "src")

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	include_directories(/usr/local/include /usr/include)
	link_directories(/usr/local/lib /usr/lib64 /usr/lib)
	find_library(GL_LIB GL HINTS /usr/local/lib )
	find_library(GLFW_LIB glfw HINTS /usr/local/lib)
	find_library(GLEW_LIB GLEW HINTS /usr/lib64)
	set(LINK_LIBS ${GL_LIB} ${GLEW_LIB} ${GLFW_LIB})
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	include_directories(/opt/local/include)
	link_directories(/opt/local/lib)
	link_directories(./src/kazmath/kazmath)

	#find libs
	find_package(OpenGL)
	find_package(GLEW)
	find_package(PkgConfig REQUIRED)
	pkg_search_module(GLFW REQUIRED glfw3)

	list(APPEND LINK_LIBS "") #create empty list
	if(OPENGL_FOUND)
		list(APPEND LINK_LIBS ${OPENGL_LIBRARIES})
	endif()
	if(GLEW_FOUND)
		list(APPEND LINK_LIBS ${GLEW_LIBRARIES})
	endif()
	if(GLFW_FOUND)
		list(APPEND LINK_LIBS ${GLFW_LIBRARIES})
	endif()
endif()

add_custom_target(BUILD_KAZMATH_LIB ALL)

add_custom_command(TARGET BUILD_KAZMATH_LIB
	PRE_BUILD 
	COMMAND echo "${CMAKE_CURRENT_SOURCE_DIR}"
	COMMENT "This command will be executed before building target BUILD_KAZMATH_LIB"

	COMMAND git -C ${CMAKE_CURRENT_SOURCE_DIR} submodule update --init
	COMMAND cd ${CMAKE_CURRENT_SOURCE_DIR}/src/kazmath && cmake . && make kazmath
	)

set(LEARN_BUILD_TARGETS 
	001_hello_window 
	002_triangle_1 002_triangle_2 
	003_shader_1 003_shader_2 003_shader_3 
	004_texture_1 004_texture_2
	005_transform_1 005_transform_2
	006_coordinate_1 006_coordinate_2 006_coordinate_3 006_coordinate_4
	007_camera_1 007_camera_2 007_camera_3)

set(NEED_KAZMATH_TARGETS "005" "006" "007")

foreach(target ${LEARN_BUILD_TARGETS})
	add_executable(${target} ${HELLO_SRC_DIR}/${target}.c)
	string(REGEX MATCH "([0-9]+)" target_index ${target})
	if(CMAKE_MATCH_1) 
		list(APPEND ${target_index}_extra_lib ${LINK_LIBS})
		if(${target_index} IN_LIST NEED_KAZMATH_TARGETS)
			list(APPEND ${target_index}_extra_lib "kazmath")
			add_dependencies(${target} BUILD_KAZMATH_LIB)
		endif()
	endif()
	target_link_libraries(${target} ${${target_index}_extra_lib})
endforeach()


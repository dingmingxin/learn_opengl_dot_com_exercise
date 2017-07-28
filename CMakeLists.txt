cmake_minimum_required (VERSION 2.8)

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
	message(${LINK_LIBS})
endif()

add_executable(001_hello_window ${HELLO_SRC_DIR}/001_hello_window.c)
target_link_libraries(001_hello_window ${LINK_LIBS})

add_executable(002_triangle_1 ${HELLO_SRC_DIR}/002_triangle_1.c)
target_link_libraries(002_triangle_1 ${LINK_LIBS})

add_executable(002_triangle_2 ${HELLO_SRC_DIR}/002_triangle_2.c)
target_link_libraries(002_triangle_2 ${LINK_LIBS})

add_executable(003_shader_1 ${HELLO_SRC_DIR}/003_shader_1.c)
target_link_libraries(003_shader_1 ${LINK_LIBS})

add_executable(003_shader_2 ${HELLO_SRC_DIR}/003_shader_2.c)
target_link_libraries(003_shader_2 ${LINK_LIBS})

add_executable(003_shader_3 ${HELLO_SRC_DIR}/003_shader_3.c)
target_link_libraries(003_shader_3 ${LINK_LIBS})

add_executable(004_texture_1 ${HELLO_SRC_DIR}/004_texture_1.c)
target_link_libraries(004_texture_1 ${LINK_LIBS})

add_executable(004_texture_2 ${HELLO_SRC_DIR}/004_texture_2.c)
target_link_libraries(004_texture_2 ${LINK_LIBS})

add_executable(005_transform_1 ${HELLO_SRC_DIR}/005_transform_1.c)
target_link_libraries(005_transform_1 ${LINK_LIBS} kazmath)

add_executable(005_transform_2 ${HELLO_SRC_DIR}/005_transform_2.c)
target_link_libraries(005_transform_2 ${LINK_LIBS} kazmath)

add_executable(006_coordinate_1 ${HELLO_SRC_DIR}/006_coordinate_1.c)
target_link_libraries(006_coordinate_1 ${LINK_LIBS} kazmath)

add_executable(006_coordinate_2 ${HELLO_SRC_DIR}/006_coordinate_2.c)
target_link_libraries(006_coordinate_2 ${LINK_LIBS} kazmath)

add_executable(006_coordinate_3 ${HELLO_SRC_DIR}/006_coordinate_3.c)
target_link_libraries(006_coordinate_3 ${LINK_LIBS} kazmath)

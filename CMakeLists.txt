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

list(APPEND LINK_LIBS "kazmath")

add_custom_target(BUILD_KAZMATH_LIB ALL)

add_custom_command(TARGET BUILD_KAZMATH_LIB
	PRE_BUILD 
	COMMAND echo "${CMAKE_CURRENT_SOURCE_DIR}"
	COMMENT "This command will be executed before building target BUILD_KAZMATH_LIB"

	COMMAND git -C ${CMAKE_CURRENT_SOURCE_DIR} submodule update --init
	COMMAND cd ${CMAKE_CURRENT_SOURCE_DIR}/src/kazmath && cmake . && make kazmath
	)

set(CHAPTERS
    1.getting_started
	2.lighting
	3.model_loading
	4.advanced_opengl
	5.advanced_lighting
	6.pbr
	7.in_practice
)

set(1.getting_started   
    1.1.hello_window
	1.2.hello_window_clear
	2.1.hello_triangle
	2.2.hello_triangle_indexed
	2.3.hello_triangle_exercise1
	2.4.hello_triangle_exercise2
	2.5.hello_triangle_exercise3
	3.1.shaders_uniform
	3.2.shaders_interpolation
	3.3.shaders_class
	4.1.textures
	4.2.textures_combined
	4.3.textures_exercise2
	4.4.textures_exercise3
	4.5.textures_exercise4
	5.1.transformations
	5.2.transformations_exercise2
	6.1.coordinate_systems
	6.2.coordinate_systems_depth
	6.3.coordinate_systems_multiple
	7.1.camera_circle
	7.2.camera_keyboard_dt
	7.3.camera_mouse_zoom
	7.4.camera_class
)

set(2.lighting
	1.colors
	2.1.basic_lighting_diffuse
	2.2.basic_lighting_specular
	3.1.materials
	3.2.materials_exercise1
	4.1.lighting_maps_diffuse_map
	4.2.lighting_maps_specular_map
	4.3.lighting_maps_exercise4
	5.1.light_casters_directional
	5.2.light_casters_point
	5.3.light_casters_spot
	5.4.light_casters_spot_soft
	6.multiple_lights
)

set(3.model_loading
	1.model_loading
)

set(4.advanced_opengl
	1.1.depth_testing
	1.2.depth_testing_view
	2.stencil_testing
	3.1.blending_discard
	3.2.blending_sort
	5.1.framebuffers
	5.2.framebuffers_exercise1
	6.1.cubemaps_skybox
	6.2.cubemaps_environment_mapping
	8.advanced_glsl_ubo
	9.1.geometry_shader_houses
	9.2.geometry_shader_exploding
	9.3.geometry_shader_normals
	10.1.instancing_quads
	10.2.asteroids
	10.3.asteroids_instanced
	11.anti_aliasing_offscreen
)

set(5.advanced_lighting
	1.advanced_lighting
	2.gamma_correction
	3.1.1.shadow_mapping_depth
	3.1.2.shadow_mapping_base
	3.1.3.shadow_mapping
	3.2.1.point_shadows
	3.2.2.point_shadows_soft
	4.normal_mapping
	5.1.parallax_mapping
	5.2.steep_parallax_mapping
	5.3.parallax_occlusion_mapping
	6.hdr
	7.bloom
	8.1.deferred_shading
	8.2.deferred_shading_volumes
	9.ssao
)

set(6.pbr
	1.1.lighting
	1.2.lighting_textured
	2.1.1.ibl_irradiance_conversion
	2.1.2.ibl_irradiance
	2.2.1.ibl_specular
	2.2.2.ibl_specular_textured
)

set(7.in_practice
	1.debugging
	# 2.text_rendering
)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src")

# then create a project file per tutorial
foreach(CHAPTER ${CHAPTERS})
    foreach(DEMO ${${CHAPTER}})
		set(SRC_DIR "src/${CHAPTER}/${DEMO}")
		if(EXISTS ${SRC_DIR})
			file(GLOB SOURCE
				"${SRC_DIR}/*.h"
				"${SRC_DIR}/*.c"
				)
			set(NAME "${CHAPTER}__${DEMO}")
			list(LENGTH SOURCE SOURCE_FILES_LEN)
			if(${SOURCE_FILES_LEN} GREATER 0)
				add_executable(${NAME} ${SOURCE})
				add_dependencies(${NAME} BUILD_KAZMATH_LIB)
				target_link_libraries(${NAME} ${LINK_LIBS})
				set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin/${CHAPTER}")
				# copy shader files to build directory
				file(GLOB SHADERS 
					"${SRC_DIR}/*.vs"
					"${SRC_DIR}/*.fs"
					"${SRC_DIR}/*.gs"
					)
				foreach(SHADER ${SHADERS})
					file(COPY ${SHADER} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin/${CHAPTER})			
				endforeach(SHADER)
			endif()
		else()
			MESSAGE(WARNING "mkdir -p ${SRC_DIR}")
			file(MAKE_DIRECTORY ${SRC_DIR})
		endif()
	endforeach(DEMO)
endforeach(CHAPTER)


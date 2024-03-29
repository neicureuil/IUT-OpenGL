#CMakeLists demoGLEngine
project(demoGLEngine)
cmake_minimum_required(VERSION 3.8.2)

set(RESOURCES_DIRECTORY "${PROJECT_SOURCE_DIR}/resources/")
# set(RESOURCES_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/resources/")
set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")
set(INC_DIR "${PROJECT_SOURCE_DIR}/include/")

#Configure config.hpp.in
configure_file(
	${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/config.hpp.in
  ${CMAKE_CURRENT_BINARY_DIR}/include/${PROJECT_NAME}/config.hpp @ONLY
)

include_directories(${CMAKE_CURRENT_BINARY_DIR}/include BEFORE)

set(SRC
	${SRC_DIR}/myapp.cpp
	${SRC_DIR}/glfwEventWrapper.cpp
	${SRC_DIR}/main.cpp
	${SRC_DIR}/particleSystem.cpp
	${SRC_DIR}/utils.cpp
	${SRC_DIR}/offLoader.cpp
	${SRC_DIR}/bulletManager.cpp
	${SRC_DIR}/collisionManager.cpp
	${SRC_DIR}/target.cpp
	${SRC_DIR}/glmesh2.cpp
	${SRC_DIR}/cube.cpp
)


set(HEADER
	${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/config.hpp.in
	${INC_DIR}/${PROJECT_NAME}/myapp.hpp
	${INC_DIR}/${PROJECT_NAME}/glfwEventWrapper.hpp
	${INC_DIR}/${PROJECT_NAME}/particle.hpp
	${INC_DIR}/${PROJECT_NAME}/particleSystem.hpp
	${INC_DIR}/${PROJECT_NAME}/utils.hpp
	${INC_DIR}/${PROJECT_NAME}/offLoader.hpp
	${INC_DIR}/${PROJECT_NAME}/bullet.hpp
	${INC_DIR}/${PROJECT_NAME}/bulletManager.hpp
	${INC_DIR}/${PROJECT_NAME}/collisionManager.hpp
	${INC_DIR}/${PROJECT_NAME}/target.hpp
	${INC_DIR}/${PROJECT_NAME}/glmesh2.hpp
	${INC_DIR}/${PROJECT_NAME}/cube.hpp
)

set(RESOURCE_FILES	
	${PROJECT_SOURCE_DIR}/resources/shaders/lighting.frag
	${PROJECT_SOURCE_DIR}/resources/shaders/lighting.vert
	${PROJECT_SOURCE_DIR}/resources/shaders/lightCube.frag
	${PROJECT_SOURCE_DIR}/resources/shaders/lightCube.vert
	${PROJECT_SOURCE_DIR}/resources/shaders/planShader.frag
	${PROJECT_SOURCE_DIR}/resources/shaders/planShader.vert
	${PROJECT_SOURCE_DIR}/resources/shaders/colorFramebuffersScreen.frag
	${PROJECT_SOURCE_DIR}/resources/shaders/foggyColorScreenShader.frag
	${PROJECT_SOURCE_DIR}/resources/shaders/depthFramebuffersScreen.frag
	${PROJECT_SOURCE_DIR}/resources/shaders/framebuffersScreen.vert
	${PROJECT_SOURCE_DIR}/resources/shaders/particle.frag
	${PROJECT_SOURCE_DIR}/resources/shaders/particle.vert
	${PROJECT_SOURCE_DIR}/resources/shaders/nightVisionScreen.frag
	${PROJECT_SOURCE_DIR}/resources/shaders/target.frag
	${PROJECT_SOURCE_DIR}/resources/shaders/target.vert
	${PROJECT_SOURCE_DIR}/resources/shaders/bullet.frag
	${PROJECT_SOURCE_DIR}/resources/shaders/bullet.vert
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/resources" PREFIX "Resources Files" FILES ${RESOURCE_FILES})

add_executable(${PROJECT_NAME} ${SRC} ${HEADER} ${RESOURCE_FILES})
include_directories(${INC_DIR} AFTER)

#SET(VS_STARTUP_PROJECT ${EXECUTABLE_NAME})
set_property(DIRECTORY ${PROJECT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

#Copy resources
add_custom_command(
	TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/resources
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
)

install(
	TARGETS ${PROJECT_NAME}
	RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
	# RUNTIME DESTINATION "bin/${PROJECT_NAME}"
)

install(
	DIRECTORY ${PROJECT_SOURCE_DIR}/resources
	# DESTINATION "bin/${PROJECT_NAME}"
	DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Detect and add SFML (don't forghet to defined SFML_ROOT in cmake)
# find_package(SFML 2.5 COMPONENTS main system window graphics audio REQUIRED)
# include_directories(${SFML_INCLUDE_DIR})

# Linking
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARY} stbimage glad glfw glm glengine)

add_subdirectory(doc)
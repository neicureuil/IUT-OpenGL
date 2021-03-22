#include "demoGLEngine/myapp.hpp"
#include "demoGLEngine/glfwEventWrapper.hpp"
#include "glengine/shader.hpp"
#include "demoGLEngine/config.hpp"
#include <stdexcept>
#include <vector>

// #define STB_IMAGE_IMPLEMENTATION
#include "stbimage/stb_image.h"


MyApp::MyApp(int _width, int _height, const char* _title)
	:width(_width), height(_height), title(_title), window(nullptr), framebuffer(0), textureColorbuffer(0), rbo(0),
	camera(nullptr), bulletManager(nullptr), collisionManager(nullptr),
	deltaTime(0.0f), lastFrame(0.0f), firstMouse(true),
	lastX(0.0f), lastY(0.0f), leftButtonPressed(false), leftButtonReleased(false),
	displayState(DisplayStyle::normal) {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (window == NULL) {		
		glfwTerminate();
		throw std::runtime_error("Failed to create GLFW window");
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		throw std::runtime_error("Failed to initialize GLAD");
	}
}

void MyApp::initGLFWCallBack() {
	GLFWEventWarpper::bind(this);
	glfwSetFramebufferSizeCallback(window, GLFWEventWarpper::resizeCallBack);

	glfwSetMouseButtonCallback(window, GLFWEventWarpper::onMouseButton);
	glfwSetCursorPosCallback(window, GLFWEventWarpper::onMouseMove);
	glfwSetScrollCallback(window, GLFWEventWarpper::onMouseScroll);

	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void MyApp::onMouseButton(int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		leftButtonPressed = true;
		leftButtonReleased = false;
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		leftButtonPressed = false;
		leftButtonReleased = true;
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		shoot();
	}
}

void MyApp::onMouseMove(double xpos, double ypos) {
	if (leftButtonReleased) {
		lastX = (float)xpos;
		lastY = (float)ypos;
	}

	if (leftButtonPressed) {
		if (firstMouse) {
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = (float)xpos - lastX;
		float yoffset = lastY - (float)ypos; // reversed since y-coordinates go from bottom to top

		lastX = (float)xpos;
		lastY = (float)ypos;

		camera->ProcessMouseMovement(xoffset, yoffset, true);
	}	
}

void MyApp::onMouseScroll(double xoffset, double yoffset) {
	camera->ProcessMouseScroll((float)yoffset);
}

void MyApp::processInput() {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS) {
		displayState = DisplayStyle::depthBuffer;
	}

	if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS) {
		displayState = DisplayStyle::normal;
	}

	if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS) {
		displayState = DisplayStyle::Foggy;
	}

	if (glfwGetKey(window, GLFW_KEY_F4) == GLFW_PRESS) {
		displayState = DisplayStyle::NightVision;
	}

	if (camera) {
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera->ProcessKeyboard(Camera::Movement::FORWARD, deltaTime);

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera->ProcessKeyboard(Camera::Movement::BACKWARD, deltaTime);

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera->ProcessKeyboard(Camera::Movement::LEFT, deltaTime);

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera->ProcessKeyboard(Camera::Movement::RIGHT, deltaTime);
	}	
}

void MyApp::buildGeometry() {
	shaders["boxShader"] = Shader(
		std::string(_resources_directory).append("shaders/lighting.vert").c_str(),
		std::string(_resources_directory).append("shaders/lighting.frag").c_str()
	);	

	shaders["lightProxyShader"] = Shader(
		std::string(_resources_directory).append("shaders/lightCube.vert").c_str(),
		std::string(_resources_directory).append("shaders/lightCube.frag").c_str()
	);

	shaders["targetShader"] = Shader(
		std::string(_resources_directory).append("shaders/target.vert").c_str(),
		std::string(_resources_directory).append("shaders/target.frag").c_str()
	);

	shaders["bulletShader"] = Shader(
		std::string(_resources_directory).append("shaders/bullet.vert").c_str(),
		std::string(_resources_directory).append("shaders/bullet.frag").c_str()
	);

	std::vector<float> cubeVertices = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f, // 0  0
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f, // 1  1
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f, // 2  2
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f, // 3  3

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f, // 0  4
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f, // 1  5
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f, // 2  6
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f, // 3  7

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f, // 0  8
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f, // 1  9
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f, // 2 10
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f, // 3 11

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f, // 0 12
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, // 1 13
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, // 2 14
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f, // 3 15

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f, // 0 16
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f, // 1 17
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f, // 2 18
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f, // 3 19

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f, // 0 20
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f, // 1 21
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f, // 2 22
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f  // 3 23
	};

	std::vector<unsigned int> cubeIndices = {
		 0,  1,  2,
		 2,  3,  0,

		 4,  5, 6,
		 6,  7, 4,

		 8,  9, 10,
		10, 11,  8,

		12, 13, 14,
		14, 15,  12,

		16, 17, 18,
		18, 19, 16,

		20, 21, 22,
		22, 23, 20
	};

	meshes["box"] = glMesh2(cubeVertices, cubeIndices, GL_STATIC_DRAW);

	OFFObject* target = OFFLoader::loadFile(std::string(_resources_directory).append("objects/100x100pointsUV.off").c_str());
	meshes["target"] = glMesh2(target->vertices, {}, target->uvs, target->triangles, GL_STATIC_DRAW);
	delete target;

	// load and create a texture 
	// -------------------------
	textures["textureDiffuse"] = Texture(std::string(_resources_directory).append("textures/container2_gris.png").c_str());
	textures["textureSpecular"] = Texture(std::string(_resources_directory).append("textures/container2_specular2.png").c_str());
	textures["textureMask"] = Texture(std::string(_resources_directory).append("textures/container2_diffuse_mask.png").c_str());
	textures["textureNormal"] = Texture(std::string(_resources_directory).append("textures/container2_normal.png").c_str());
	
	textures["textureTarget"] = Texture(std::string(_resources_directory).append("textures/cible.png").c_str());
	
	// shader configuration
	// --------------------
	const Shader &boxShader = shaders["boxShader"];
	boxShader.bind();
	boxShader.setInt("textureDiffuse", 0);
	boxShader.setInt("textureSpecular", 1);
	boxShader.setInt("textureMask", 2);
	boxShader.setInt("textureNormal", 10);

	// second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
	meshes["lightProxy"] = glMesh2(cubeVertices, cubeIndices, GL_STATIC_DRAW);

	// Plan
	std::vector<float> planVertices = {
		// positions			// Normal			// texture coords
		 0.5f,  0.f,   0.5f,	0.0f, 1.0f, 0.0f,	80.0f, 80.0f,
		 0.5f,  0.0f, -0.5f,	0.0f, 1.0f, 0.0f,	0.0f, 80.0f,
		-0.5f,  0.0f, -0.5f,	0.0f, 1.0f, 0.0f,	0.0f, 0.0f,
		-0.5f,  0.0f,  0.5f,	0.0f, 1.0f, 0.0f,	80.0f, 0.0f
	};

	std::vector<unsigned int> planIndices = {
		0, 1, 3,   // premier triangle
		1, 2, 3    // second triangle
	};

	meshes["ground"] = glMesh2(planVertices, planIndices, GL_STATIC_DRAW);
	textures["ground"] = Texture(std::string(_resources_directory).append("textures/wall.jpg").c_str());

	shaders["groundShader"] = Shader(
		std::string(_resources_directory).append("shaders/planShader.vert").c_str(),
		std::string(_resources_directory).append("shaders/planShader.frag").c_str()
	);

	const Shader &groundShader = shaders["groundShader"];
	groundShader.bind();
	groundShader.setInt("ground", 3);
}

void MyApp::loop() {
	initGLFWCallBack();

	// configure global opengl state
	glEnable(GL_DEPTH_TEST);
	camera = new Camera(glm::vec3(0.0f, 2.1f, 12.0f));
	
	buildGeometry();
	frameBufferConfiguration();

	// configure app component
	target = new Target(glm::vec3(-10, 3.5f, -10), meshes["target"], shaders["targetShader"]);
	bulletManager = new BulletManager(shaders["bulletShader"]);
	collisionManager = new CollisionManager();

	double lag = 0.0;
	while (!glfwWindowShouldClose(window)) {
		// per-frame time logic
		// --------------------
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
		lag += deltaTime;
		while (lag >= MS_PER_UPDATE) {
			update(MS_PER_UPDATE);
			lag -= MS_PER_UPDATE;
		}

		// input
		processInput();

		// render
		render();

		// check and call events and swap the buffers
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
}

void MyApp::update(const double& dt) {
	bulletManager->update(dt);
	target->update(glfwGetTime());
	collisionManager->testTargetCollisions(target, bulletManager);
}

void MyApp::frameBufferConfiguration() {
	// Clean previous states
	if (framebuffer)
		glDeleteFramebuffers(1, &framebuffer);

	if (textureColorbuffer)
		glDeleteTextures(1, &textureColorbuffer);

	if (rbo)
		glDeleteTextures(1, &rbo);



	std::vector<float> quadVertices = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions 
		-1.0f, -1.0f, 0.0f, // 0
		 1.0f, -1.0f, 0.0f, // 1
		 1.0f,  1.0f, 0.0f, // 2
	   -1.0f,  1.0f, 0.0f  // 3
	};

	std::vector<float> quadUV = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// texCoords		
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};

	std::vector<unsigned int> quadIndices = {
		0, 1, 2,
		0, 2, 3
	};

	const glMesh2& screenQuad = meshes["screenQuad"] = glMesh2(quadVertices, {}, quadUV, quadIndices);
	screenQuad.bind();


	// framebuffer configuration
	// -------------------------
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	
	// create a color attachment texture
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
		
	// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	/* GLuint rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo)
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it  */

	glGenTextures(1, &rbo);
	glBindTexture(GL_TEXTURE_2D, rbo);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, rbo, 0);

	// Set the list of draw buffers.
	GLenum DrawBuffers[2] = { GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT };
	glDrawBuffers(2, DrawBuffers); // "1" is the size of DrawBuffers

	// glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, textureColorbuffer);
	// glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

	// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		throw std::runtime_error("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	shaders["colorScreenShader"] = Shader(
		std::string(_resources_directory).append("shaders/framebuffersScreen.vert").c_str(),
		std::string(_resources_directory).append("shaders/colorFramebuffersScreen.frag").c_str()
	);

	shaders["foggyColorScreenShader"] = Shader(
		std::string(_resources_directory).append("shaders/framebuffersScreen.vert").c_str(),
		std::string(_resources_directory).append("shaders/foggyColorScreenShader.frag").c_str()
	);

	shaders["depthScreenShader"] = Shader(
		std::string(_resources_directory).append("shaders/framebuffersScreen.vert").c_str(),
		std::string(_resources_directory).append("shaders/depthFramebuffersScreen.frag").c_str()
	);	

	shaders["nightVisionScreenShader"] = Shader(
		std::string(_resources_directory).append("shaders/framebuffersScreen.vert").c_str(),
		std::string(_resources_directory).append("shaders/nightVisionScreen.frag").c_str()
	);


	const Shader& foggyColorScreenShader = shaders["foggyColorScreenShader"];
	foggyColorScreenShader.bind();
	foggyColorScreenShader.setInt("screenTexture", 4);
	foggyColorScreenShader.setInt("screenDepth", 5);
	foggyColorScreenShader.setFloat("viewPort.near", 0.1f);
	foggyColorScreenShader.setFloat("viewPort.far", 100.0f);
	foggyColorScreenShader.setVec3("fog.color", 1.0f, 1.0f, 1.0f);
	foggyColorScreenShader.setFloat("fog.density", 0.15f);

	const Shader& colorScreenShader = shaders["colorScreenShader"];
	colorScreenShader.bind();
	colorScreenShader.setInt("screenTexture", 4);
	colorScreenShader.setInt("screenDepth", 5);
	colorScreenShader.setFloat("viewPort.near", 0.1f);
	colorScreenShader.setFloat("viewPort.far", 100.0f);

	const Shader& depthScreenShader = shaders["depthScreenShader"];
	depthScreenShader.bind();
	depthScreenShader.setInt("screenTexture", 4);
	depthScreenShader.setInt("screenDepth", 5);
	depthScreenShader.setFloat("viewPort.near", 0.1f);
	depthScreenShader.setFloat("viewPort.far", 100.0f);

	const Shader& nightVisionScreenShader = shaders["nightVisionScreenShader"];
	nightVisionScreenShader.bind();
	nightVisionScreenShader.setInt("screenTexture", 4);
}

void MyApp::render() {
	// bind to framebuffer and draw scene as we normally would to color texture 
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

	renderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
	
	// clear all relevant buffers
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
	glClear(GL_COLOR_BUFFER_BIT);

	
	switch (displayState) {
	case DisplayStyle::normal:
		shaders["colorScreenShader"].bind();
		break;
	case DisplayStyle::depthBuffer:
		shaders["depthScreenShader"].bind();
		break;
	case DisplayStyle::Foggy:
		shaders["foggyColorScreenShader"].bind();
		break;
	case DisplayStyle::NightVision:
		shaders["nightVisionScreenShader"].bind();
		break;
	default:
		shaders["colorScreenShader"].bind();
		break;
	}
	
	const glMesh2& screenQuad = meshes["screenQuad"];
	screenQuad.bind();
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// use the color attachment texture as the texture of the quad plane

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, rbo);
	screenQuad.render();
}

void MyApp::renderScene() {
	// rendering commands here
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// bind textures on corresponding texture units
	textures["textureDiffuse"].bind(GL_TEXTURE0);
	textures["textureSpecular"].bind(GL_TEXTURE1);
	textures["textureMask"].bind(GL_TEXTURE2);
	textures["ground"].bind(GL_TEXTURE3);

	textures["textureNormal"].bind(GL_TEXTURE10);
	textures["textureTarget"].bind(GL_TEXTURE11);

	// be sure to activate shader when setting uniforms/drawing objects
	const Shader& boxShader = shaders["boxShader"];
	boxShader.bind();

	glm::vec3 lightPos(2.0f, 3.0f, 6.0f);	
	// light properties
	glm::vec3 lightColor(0.9f, 0.9f, 1.0f); 
	glm::vec3 diffuseColor = lightColor * glm::vec3(1.0f); // decrease the influence or not...
	glm::vec3 ambientColor = diffuseColor * glm::vec3(0.15f); // low influence

	const Shader& lightProxyShader = shaders["lightProxyShader"];
	lightProxyShader.bind();
	lightProxyShader.setVec3("light.position", lightPos);
	lightProxyShader.setVec3("light.ambient", ambientColor);
	lightProxyShader.setVec3("light.diffuse", diffuseColor);
	lightProxyShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

	boxShader.bind();
	boxShader.setVec3("lightPos", lightPos);
	boxShader.setVec3("viewPos", camera->Position);
	boxShader.setVec3("light.position", lightPos);
	boxShader.setVec3("light.ambient", ambientColor);
	boxShader.setVec3("light.diffuse", diffuseColor);
	boxShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

	// material properties
	boxShader.setVec3("material.specular", 1.0f, 1.0f, 1.0f); 
	boxShader.setFloat("material.shininess", 128.0f);


	// projection transformations
	glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)width / (float)height, 0.1f, 100.0f);
	boxShader.setMat4("projection", projection);
	

	// camera/view transformation
	glm::mat4 view = camera->GetViewMatrix();
	boxShader.setMat4("view", view);


	unsigned int nbCubes = 10;
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.5f,  0.0f),
		glm::vec3(1.1f,  0.5f, 0.0f),
		glm::vec3(0.5f, 0.5f, 1.17f),
		glm::vec3(0.5f, 1.5f, 0.5f),
		glm::vec3(2.4f, 0.5f, -3.5f),
		glm::vec3(-1.7f,  0.5f, 3.5f),
		glm::vec3(-1.3f, 0.5f, 2.3f),
		glm::vec3(-2.7f,  0.5f, 2.5f),
		glm::vec3(-2.0f,  1.5f, 2.8f),
		glm::vec3(1.5f,  0.5f, 3.5f)
	};

	float cubeRotation[] = {
		0.0f,
		-4.0f,
		10.0f,
		-25.0f,
		-70.0f,
		-10.0f,
		15.0f,
		-30.0f,
		-35.0f,
		45.0f
	};

	
	// cubes colors
	glm::vec3 cubeColors[] = {
		glm::vec3(1.0f, 0.5f, 0.31f),
		glm::vec3(0.18f, 0.73f, 0.69f),
		glm::vec3(0.97f, 0.79f, 0.31f),
		glm::vec3(0.45f, 0.6f, 0.51f),
		glm::vec3(0.72f, 0.22f, 0.22f),
		glm::vec3(0.36f, 0.83f, 0.36f),
		glm::vec3(0.0f, 0.51f, 0.76f),
		glm::vec3(0.87f, 0.56f, 0.64f),
		glm::vec3(0.99f, 0.65f, 0.02f),
		glm::vec3(0.95f, 1.0f, 0.9f)
	};


	glm::mat4  model = glm::mat4(1.0f);

	// glBindVertexArray(VAO);
	meshes["box"].bind();

	for (unsigned int i = 0; i < nbCubes; i++) {
		// calculate the model matrix for each object and pass it to shader before drawing
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		model = glm::translate(model, cubePositions[i]);
		model = glm::rotate(model, glm::radians(cubeRotation[i]), glm::vec3(0.0f, 1.0f, 0.0f));
		boxShader.setMat4("model", model);

		boxShader.setVec3("material.ambient", glm::vec3(1.0));
		boxShader.setVec3("material.diffuse", cubeColors[i]);

		meshes["box"].render();
	}

	// also draw the lamp object
	lightProxyShader.bind();
	lightProxyShader.setMat4("projection", projection);
	lightProxyShader.setMat4("view", view);
	model = glm::mat4(1.0f);
	model = glm::translate(model, lightPos);
	model = glm::scale(model, glm::vec3(0.1f)); // a smaller cube
	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 1.0f)); // a smaller cube	
	lightProxyShader.setMat4("model", model);

	const glMesh2& lightProxy = meshes["lightProxy"];
	lightProxy.bind();
	lightProxy.render();
	
	
	const Shader &groundShader = shaders["groundShader"];
	groundShader.bind();
	groundShader.setMat4("projection", projection);
	groundShader.setMat4("view", view);
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(100.0f)); 
	groundShader.setMat4("model", model);

	groundShader.setVec3("viewPos", camera->Position);
	groundShader.setVec3("light.position", lightPos);
	groundShader.setVec3("light.ambient", ambientColor);
	groundShader.setVec3("light.diffuse", diffuseColor);
	groundShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
	groundShader.setVec3("material.specular", 1.0f, 1.0f, 1.0f);
	groundShader.setFloat("material.shininess", 8);	
	
	const glMesh2& ground = meshes["ground"];
	ground.bind();
	ground.render();

	target->render(view, projection);
	bulletManager->render(view, projection);
}

MyApp::~MyApp() {
	if (camera) delete camera;
	if (bulletManager) delete bulletManager;
	if (collisionManager) delete collisionManager;
	if (target) delete target;
}

void MyApp::onResize(int _width, int _height) {
	width = _width;
	height = _height;

	int w, h;
	glViewport(0, 0, width, height);
	frameBufferConfiguration();
}

void MyApp::shoot() {
	bulletManager->addBullet(camera->Position + camera->Front, camera->Front);
}
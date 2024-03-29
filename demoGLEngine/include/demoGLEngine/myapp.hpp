#ifndef __MYAPP_HPP_
#define __MYAPP_HPP_
#include <string>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "glengine/camera.hpp"
#include "glengine/shader.hpp"
#include "glengine/texture.hpp"
#include "glengine/glmesh.hpp"

#include "collisionManager.hpp"
#include "bulletManager.hpp"
#include "offLoader.hpp"
#include "target.hpp"
#include "glmesh2.hpp"
#include "particleSystem.hpp"
#include "cube.hpp"

#include <unordered_map>


class MyApp {
public:
	MyApp(int _width, int _height, const char* _title);
	~MyApp();

	void onMouseButton(int button, int action, int mods);
	void onMouseMove(double xpos, double ypos);
	void onMouseScroll(double xoffset, double yoffset);
	void onResize(int _width, int _height);
	
	void loop();

private:
	std::vector<Cube> cubes; // Vecteurs des cubes de la scene
	BulletManager* bulletManager; // Gestionnaire des balles
	CollisionManager* collisionManager; // Gestionnaire de collision
	Target* target; // La cible de la scene
	ParticleSystem* fireParticles; // Systeme de particule de feu dans la scene
	ParticleSystem* elecParticles; // Systeme de particule d'electricit� dans la scene

private:
	enum class DisplayStyle { normal, depthBuffer, Foggy, NightVision };

	void update(const double& dt);
	void frameBufferConfiguration();
	void render();
	void renderScene();

	void processInput();
	void buildGeometry();
	void initGLFWCallBack();

	/*
	* Fonction qui initialise tous les composants cr�es et utilis� pendant le projet
	*/
	void buildProject();
	/* 
	* Fonction appel� lors que l'on desir tirer une balle
	*/
	void shoot();
	

	int width;
	int height;
	const char* title;
	GLFWwindow* window;

	static constexpr double MS_PER_UPDATE = 33.3*(1.0/1000.0);
	double deltaTime;
	double lastFrame;
	float lastX;
	float lastY;
	bool leftButtonPressed, leftButtonReleased;
	bool firstMouse;
	DisplayStyle displayState;

	Camera* camera;	
	std::unordered_map<std::string, Texture> textures;
	std::unordered_map<std::string, Shader> shaders;
	std::unordered_map<std::string, glMesh2> meshes;
	GLuint framebuffer, textureColorbuffer, rbo;
};

#endif // !__MYAPP_HPP_

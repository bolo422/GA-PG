#pragma once

#include <iostream>
#include <string>
#include <assert.h>
#include <cmath>
#include <chrono>
#include <ctime> 

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// stb
#include "stb_image.h"

#include <vector>

//#include "Shader.h"
#include "Player.h"
#include "Timer.h"



using namespace std;

class GameManager
{
public:
	GameManager() {}
	~GameManager(){}

	void initialize();
	void assignTextures();
	void createObjects();
	

	void run();
	void finish();


	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

	int loadTexture(string path);


	


protected:
	
	Timer speedTimer;
	float speed = 0;

	GLFWwindow* window = 0;
	int width = 0, height = 0;
	Shader* shader = 0;

	glm::mat4 projection;
	GLint projLoc = 0;

	//Objects and Textures
	GLuint texID1 = 0, texID2 = 0, texID3 = 0, texID4 = 0;
	Object bg, bg2, mg, mg2, ground, ground2;
	Player player;



	const GLuint WIDTH = 1024, HEIGHT = 768;
	const float pi = 3.14159;
};


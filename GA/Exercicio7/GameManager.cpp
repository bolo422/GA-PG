#include "GameManager.h"

static bool keys[1024];

void GameManager::initialize()
{
	// Inicialização da GLFW
	glfwInit();

	// Criação da janela GLFW
	window = glfwCreateWindow(WIDTH, HEIGHT, "Ola Triangulo!", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da função de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros d funções da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	// Obtendo as informações de versão
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;


	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Definindo as dimensões da viewport com as mesmas dimensões da janela da aplicação
	//int width, height;

	shader = new Shader("./shaders/transforms.vs", "./shaders/transforms.fs");


	shader->Use();


	projection = glm::mat4(1);
	double xmin = 0.0, xmax = 1024.0, ymin = 0.0, ymax = 768.0;

	projection = glm::ortho(xmin, xmax, ymin, ymax, -1.0, 1.0);

	projLoc = glGetUniformLocation(shader->Program, "projection");
	glUniformMatrix4fv(projLoc, 1, FALSE, glm::value_ptr(projection));


	glm::mat4 model = glm::mat4(1);


	model = glm::translate(model, glm::vec3(400, 300, 0.0));
	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(960.0f, 1280.0f, 1.0));

	GLint modelLoc = glGetUniformLocation(shader->Program, "model");

	glUniform1i(glGetUniformLocation(shader->Program, "ourTexture1"), 0);

	createObjects();



	speedTimer.setInitialTime(1500);
}

void GameManager::assignTextures()
{
}

void GameManager::createObjects()
{
	//TEXTURES
	//texID = loadTexture("./textures/Background/middleground.png");
	//texID = loadTexture("./textures/Background/background.png");
	//texID = loadTexture("./textures/Player/player-idle-1.png");
	//texID = loadTexture("./textures/Background/ground.png");
	//texID = loadTexture("./textures/textures/Enemy/bee-1.png");


	//Background
	Object *obj = new Object;
	texID = loadTexture("./textures/Background/background.png");
	obj = new Object("bg", glm::vec3(WIDTH / 2, HEIGHT / 2, 0), glm::vec3(1024, 768, 1.0), texID, shader);
	objects.push_back(obj);
	obj = new Object("bg", glm::vec3(WIDTH * 1.5, HEIGHT / 2, 0), glm::vec3(1024, 768, 1.0), texID, shader);
	objects.push_back(obj);


	//MiddleGround
	texID = loadTexture("./textures/Background/middleground.png");
	obj = new Object("mg", glm::vec3(WIDTH / 2, HEIGHT / 2, 0), glm::vec3(1024, 768, 1.0), texID, shader);
	objects.push_back(obj);
	obj = new Object("mg", glm::vec3(WIDTH * 1.5, HEIGHT / 2, 0), glm::vec3(1024, 768, 1.0), texID, shader);
	objects.push_back(obj);

	//Ground
	texID = loadTexture("./textures/Background/ground.png");
	obj = new Object("ground", glm::vec3(WIDTH / 2, 15, 0), glm::vec3(1024, 30, 1.0), texID, shader);
	objects.push_back(obj);
	obj = new  Object("ground", glm::vec3(WIDTH * 1.5, 15, 0), glm::vec3(1024, 30, 1.0), texID, shader);
	objects.push_back(obj);

	//Enemytest
	/*texID = loadTexture("./textures/textures/Enemy/bee-1.png");
	obj = new Object("enemy", glm::vec3(WIDTH / 2, 70, 0), glm::vec3(1024, 30, 1.0), texID, shader);
	objects.push_back(obj);
	obj = new Object("enemy", glm::vec3(WIDTH / 2, 70, 0), glm::vec3(1024, 30, 1.0), texID, shader);
	objects.push_back(obj);*/
	

	//Player
	texID = loadTexture("./textures/Player/player-idle-1.png");
	player.initialize();
	player.setTag("player");
	player.setPosition(glm::vec3(200, 62, 0));
	player.setDimensions(glm::vec3(74, 64, 1.0));
	player.setAngle(glm::radians(0.0f));
	player.setTexture(texID);
	player.setShader(shader);
}

void GameManager::run()
{
	while (!glfwWindowShouldClose(window)) {
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
		glfwPollEvents();
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);
		glUniformMatrix4fv(projLoc, 1, FALSE, glm::value_ptr(projection));


		for (int i = 0; i < objects.size(); i++)
		{
			objects[i]->update();
			objects[i]->draw();
		}


		player.update();
		player.draw();

		


		// REFAZER O PULO

		/*if (holding) {
			jumpForce += 4;
		}
		else {
			player.jump(jumpForce, 1);
			jumpForce = 0;
		}*/

		//Movimento MG
		{
			objects[2]->addPositionX(-2 - speed);
			objects[3]->addPositionX(-2 - speed);

			if (objects[2]->getPosition().x <= -512) {
				objects[2]->setPositionX(WIDTH * 1.5);
			}

			if (objects[3]->getPosition().x <= -512) {
				objects[3]->setPositionX(WIDTH * 1.5);
			}

			if (objects[2]->getPosition().x - objects[3]->getPosition().x != -1024 && objects[2]->getPosition().x < objects[3]->getPosition().x)
			{
				objects[2]->setPositionX(512);
				objects[3]->setPositionX(1536);
			}
			else if (objects[3]->getPosition().x - objects[2]->getPosition().x != -1024 && objects[3]->getPosition().x < objects[2]->getPosition().x)
			{
				objects[2]->setPositionX(512);
				objects[3]->setPositionX(1536);
			}
		}

		//Movimento BG
		{
			objects[0]->addPositionX(-0.5 - speed);
			objects[1]->addPositionX(-0.5 - speed);

			if (objects[0]->getPosition().x <= -512) {
				objects[0]->setPositionX(WIDTH * 1.5);
			}

			if (objects[1]->getPosition().x <= -512) {
				objects[1]->setPositionX(WIDTH * 1.5);
			}

			if (objects[0]->getPosition().x - objects[1]->getPosition().x != -1024 && objects[0]->getPosition().x < objects[1]->getPosition().x)
			{
				objects[0]->setPositionX(512);
				objects[1]->setPositionX(1536);
			}
			else if (objects[1]->getPosition().x - objects[0]->getPosition().x != -1024 && objects[1]->getPosition().x < objects[0]->getPosition().x)
			{
				objects[0]->setPositionX(512);
				objects[1]->setPositionX(1536);
			}
		}

		//Movimento Ground
		{
			objects[4]->addPositionX(-3 - speed);
			objects[5]->addPositionX(-3 - speed);

			if (objects[4]->getPosition().x <= -512) {
				objects[4]->setPositionX(WIDTH * 1.5);
			}

			if (objects[5]->getPosition().x <= -512) {
				objects[5]->setPositionX(WIDTH * 1.5);
			}

			if (objects[4]->getPosition().x - objects[5]->getPosition().x != -1024 && objects[4]->getPosition().x < objects[5]->getPosition().x)
			{
				objects[4]->setPositionX(512);
				objects[5]->setPositionX(1536);
			}
			else if (objects[5]->getPosition().x - objects[4]->getPosition().x != -1024 && objects[5]->getPosition().x < objects[4]->getPosition().x)
			{
				objects[4]->setPositionX(512);
				objects[5]->setPositionX(1536);
			}
		}




		speedTimer.tick();
		if (speedTimer.over()) {
			speed++;
			speedTimer.restart(speedTimer.getInitialTime() * 0.95);
			cout << "Incremento de tempo : Speed = " << speed << " Ticks: " << speedTimer.getRemainingTime() << endl;
		}


		// Troca os buffers da tela
		glfwSwapBuffers(window);

	}
}

void GameManager::finish()
{
	glfwTerminate();
}

void GameManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	//APAGAR ISTO:
	bool holding = true;
	/////

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	//   ATEN��O!!!!!!!!!!
	//   Implementar estes ifs no in�cio do update e lembrar de voltar o array de keys para 0!!!!



	/*if (key == GLFW_KEY_SPACE && action == GLFW_PRESS && !player.getFalling() && !player.getJumped())
		holding = true;

	if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE && !player.getFalling() && !player.getJumped()) {
		holding = false;
		player.jump(true);
	}*/
}

int GameManager::loadTexture(string path)
{
	GLuint texID;

	//identificador de textura da memória
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	//Ajusta os parâmetros de wrapping e filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Carregamento da imagem
	int width, height, nrChannels;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		if (nrChannels == 3) //jpg, bmp
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else //png
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, 0);

	return texID;
}



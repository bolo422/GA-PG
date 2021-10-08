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
	texID1 = loadTexture("./textures/Background/middleground.png");
	texID2 = loadTexture("./textures/Background/background.png");
	texID3 = loadTexture("./textures/Player/player-idle-1.png");
	texID4 = loadTexture("./textures/Background/ground.png");


	//Background
	bg = Object(glm::vec3(WIDTH / 2, HEIGHT / 2, 0), glm::vec3(1024, 768, 1.0), texID2, shader);
	bg2 = Object(glm::vec3(WIDTH * 1.5, HEIGHT / 2, 0), glm::vec3(1024, 768, 1.0), texID2, shader);
	
	//MiddleGround
	mg = Object(glm::vec3(WIDTH / 2, HEIGHT / 2, 0), glm::vec3(1024, 768, 1.0), texID1, shader);
	mg2 = Object(glm::vec3(WIDTH * 1.5, HEIGHT / 2, 0), glm::vec3(1024, 768, 1.0), texID1, shader);

	//Ground
	ground = Object(glm::vec3(WIDTH / 2, 15, 0), glm::vec3(1024, 30, 1.0), texID4, shader);
	ground2 = Object(glm::vec3(WIDTH * 1.5, 15, 0), glm::vec3(1024, 30, 1.0), texID4, shader);

	//Player
	player.initialize();
	player.setPosition(glm::vec3(200, 62, 0));
	player.setDimensions(glm::vec3(74, 64, 1.0));
	player.setAngle(glm::radians(0.0f));
	player.setTexture(texID3);
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


		bg.update();
		bg.draw();

		bg2.update();
		bg2.draw();

		mg.update();
		mg.draw();

		mg2.update();
		mg2.draw();

		ground.update();
		ground.draw();

		ground2.update();
		ground2.draw();

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
			mg.addPositionX(-2 - speed);
			mg2.addPositionX(-2 - speed);

			if (mg.getPosition().x <= -512) {
				mg.setPositionX(WIDTH * 1.5);
			}

			if (mg2.getPosition().x <= -512) {
				mg2.setPositionX(WIDTH * 1.5);
			}

			if (mg.getPosition().x - mg2.getPosition().x != -1024 && mg.getPosition().x < mg2.getPosition().x)
			{
				mg.setPositionX(512);
				mg2.setPositionX(1536);
			}
			else if (mg2.getPosition().x - mg.getPosition().x != -1024 && mg2.getPosition().x < mg.getPosition().x)
			{
				mg.setPositionX(512);
				mg2.setPositionX(1536);
			}
		}

		//Movimento BG
		{
			bg.addPositionX(-0.5 - speed);
			bg2.addPositionX(-0.5 - speed);

			if (bg.getPosition().x <= -512) {
				bg.setPositionX(WIDTH * 1.5);
			}

			if (bg2.getPosition().x <= -512) {
				bg2.setPositionX(WIDTH * 1.5);
			}

			if (bg.getPosition().x - bg2.getPosition().x != -1024 && bg.getPosition().x < bg2.getPosition().x)
			{
				bg.setPositionX(512);
				bg2.setPositionX(1536);
			}
			else if (bg2.getPosition().x - bg.getPosition().x != -1024 && bg2.getPosition().x < bg.getPosition().x)
			{
				bg.setPositionX(512);
				bg2.setPositionX(1536);
			}
		}

		//Movimento Ground
		{
			ground.addPositionX(-3 - speed);
			ground2.addPositionX(-3 - speed);

			if (ground.getPosition().x <= -512) {
				ground.setPositionX(WIDTH * 1.5);
			}

			if (ground2.getPosition().x <= -512) {
				ground2.setPositionX(WIDTH * 1.5);
			}

			if (ground.getPosition().x - ground2.getPosition().x != -1024 && ground.getPosition().x < ground2.getPosition().x)
			{
				ground.setPositionX(512);
				ground2.setPositionX(1536);
			}
			else if (ground2.getPosition().x - ground.getPosition().x != -1024 && ground2.getPosition().x < ground.getPosition().x)
			{
				ground.setPositionX(512);
				ground2.setPositionX(1536);
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



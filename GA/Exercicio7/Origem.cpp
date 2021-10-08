/* Hello Triangle - código adaptado de https://learnopengl.com/#!Getting-started/Hello-Triangle
 *
 * Adaptado por Rossana Baptista Queiroz
 * para a disciplina de Processamento Gráfico - Jogos Digitais - Unisinos
 * Versão inicial: 7/4/2017
 * Última atualização em 09/08/2021
 *
 */

#include <iostream>
#include <string>
#include <assert.h>
#include <cmath>
#include <chrono>
#include <ctime> 

using namespace std;

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

// Shaders
#include "Shader.h"
#include "Object.h"
#include "Timer.h"

// Protótipo da função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Protótipos das funções
int setupGeometry();
int loadTexture(string path);
GLuint createSprite();



//Timers
Timer speedTimer(1500);



// Dimens�es da janela (pode ser alterado em tempo de execu��o)
const GLuint WIDTH = 1024, HEIGHT = 768;

const float pi = 3.14159;

int nvertices = 100 + 1 + 1; //+ centro +cópia do 1

using namespace std;

Object player;

bool holding = false;
float jumpForce = 0;
float speed = 0;

// Função MAIN
int main()
{
	// Inicialização da GLFW
	glfwInit();

	//Muita atenção aqui: alguns ambientes não aceitam essas configurações
	//Você deve adaptar para a versão do OpenGL suportada por sua placa
	//Sugestão: comente essas linhas de código para desobrir a versão e
	//depois atualize (por exemplo: 4.5 com 4 e 5)
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Essencial para computadores da Apple
//#ifdef __APPLE__
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//#endif

	// Criação da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ola Triangulo!", nullptr, nullptr);
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
	int width, height;
	
	// Compilando e buildando o programa de shader
	//GLuint shaderID = setupShader();
	Shader* shader = new Shader("./shaders/transforms.vs", "./shaders/transforms.fs");

	GLuint texID1 = loadTexture("./textures/Background/middleground.png");
	GLuint texID2 = loadTexture("./textures/Background/background.png");
	GLuint texID3 = loadTexture("./textures/Player/player-idle-1.png");
	GLuint texID4 = loadTexture("./textures/Background/ground.png");


	Object bg;
	bg.initialize();
	bg.setPosition(glm::vec3(WIDTH / 2, HEIGHT / 2, 0));
	bg.setDimensions(glm::vec3(1024, 768, 1.0));
	bg.setTexture(texID2);
	bg.setShader(shader);

	Object bg2;
	bg2.initialize();
	bg2.setPosition(glm::vec3(WIDTH * 1.5, HEIGHT / 2, 0));
	bg2.setDimensions(glm::vec3(1024, 768, 1.0));
	bg2.setTexture(texID2);
	bg2.setShader(shader);

	Object mg;
	mg.initialize();
	mg.setPosition(glm::vec3(WIDTH/2, HEIGHT/2, 0));
	mg.setDimensions(glm::vec3(1024, 768, 1.0));
	mg.setTexture(texID1);
	mg.setShader(shader);

	Object mg2;
	mg2.initialize();
	mg2.setPosition(glm::vec3(WIDTH * 1.5, HEIGHT/2, 0));
	mg2.setDimensions(glm::vec3(1024, 768, 1.0));
	mg2.setTexture(texID1);
	mg2.setShader(shader);

	Object ground;
	ground.initialize();
	ground.setPosition(glm::vec3(WIDTH / 2, 15, 0));
	ground.setDimensions(glm::vec3(1024, 30, 1.0));
	ground.setTexture(texID4);
	ground.setShader(shader);

	Object ground2;
	ground2.initialize();
	ground2.setPosition(glm::vec3(WIDTH * 1.5, 15, 0));
	ground2.setDimensions(glm::vec3(1024, 30, 1.0));
	ground2.setTexture(texID4);
	ground2.setShader(shader);

	player.initialize();
	player.setPosition(glm::vec3(200, 62, 0));
	player.setDimensions(glm::vec3(74, 64, 1.0));
	player.setAngle(glm::radians(0.0f));
	player.setTexture(texID3);
	player.setShader(shader);
	//player.jump(true);



	// Gerando um buffer simples, com a geometria de um triângulo
	//GLuint VAO = setupGeometry();
	GLuint VAO = createSprite();

	shader->Use();

	// Enviando a cor desejada (vec4) para o fragment shader
	// Utilizamos a variáveis do tipo uniform em GLSL para armazenar esse tipo de info
	// que não está nos buffers
	/*GLint colorLoc = glGetUniformLocation(shader->Program, "inputColor");
	assert(colorLoc > -1);*/

	glm::mat4 projection = glm::mat4(1);
	double xmin=0.0, xmax=1024.0, ymin=0.0, ymax=768.0;

	projection = glm::ortho(xmin, xmax, ymin, ymax, -1.0, 1.0);

	GLint projLoc = glGetUniformLocation(shader->Program, "projection");
	glUniformMatrix4fv(projLoc, 1, FALSE, glm::value_ptr(projection));


	glm::mat4 model = glm::mat4(1);

	
	model = glm::translate(model, glm::vec3(400, 300, 0.0));
	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(960.0f, 1280.0f, 1.0));

	GLint modelLoc = glGetUniformLocation(shader->Program, "model");

	glUniform1i(glGetUniformLocation(shader->Program, "ourTexture1"), 0);

	
	// Loop da aplicação - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
		glfwPollEvents();

		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		// Limpa o buffer de cor
		glClearColor(0.8f, 0.8f, 0.8f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(10);

		model = glm::mat4(1); //matriz identidade

		model = glm::translate(model, glm::vec3(400, 300, 0.0));
		model = glm::rotate(model, (GLfloat)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(200, 200, 1.0));
		

		//glm::radians(45.0f)

		glUniformMatrix4fv(modelLoc, 1, FALSE, glm::value_ptr(model));

		//Aqui o código de correção do aspecto 
		/*double ratio = width / (float) height;
		if (width >= height)
		{
			projection = glm::ortho(xmin*ratio, xmax*ratio, ymin, ymax, -1.0, 1.0);
		}
		else
		{
			ratio = height / (float) width;
			projection = glm::ortho(xmin, xmax, ymin*ratio, ymax*ratio, -1.0, 1.0);
		}*/

		
		glUniformMatrix4fv(projLoc, 1, FALSE, glm::value_ptr(projection));



		/*glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texID);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);*/

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

		
		if (holding) {
			jumpForce += 4;
		}
		else {
			player.jump(jumpForce, 1);
			jumpForce = 0;
		}

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
	// Pede pra OpenGL desalocar os buffers
	glDeleteVertexArrays(1, &VAO);
	// Finaliza a execução da GLFW, limpando os recursos alocados por elav
	glfwTerminate();
	return 0;
}

// Função de callback de teclado - só pode ter uma instância (deve ser estática se
// estiver dentro de uma classe) - É chamada sempre que uma tecla for pressionada
// ou solta via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS && !player.getFalling() && !player.getJumped())
		holding = true;

	if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE && !player.getFalling() && !player.getJumped()) {
		holding = false;
		player.jump(true);
	}


}

// Esta função está bastante harcoded - objetivo é criar os buffers que armazenam a 
// geometria de um triângulo
// Apenas atributo coordenada nos vértices
// 1 VBO com as coordenadas, VAO com apenas 1 ponteiro para atributo
// A função retorna o identificador do VAO
int setupGeometry()
{
	// Aqui setamos as coordenadas x, y e z do triângulo e as armazenamos de forma
	// sequencial, já visando mandar para o VBO (Vertex Buffer Objects)
	// Cada atributo do vértice (coordenada, cores, coordenadas de textura, normal, etc)
	// Pode ser arazenado em um VBO único ou em VBOs separados
	//GLfloat vertices[] = {
	//	-0.5, -0.5, 0.0,
	//	 0.5, -0.5, 0.0,
	//	 0.0, 0.5, 0.0,
	//	 //outro triangulo vai aqui
	//};

	GLfloat* vertices;

	vertices = new GLfloat[nvertices * 3]; //* nro de valores do(s) atributo(s)

	//ponto do centro -- origem
	vertices[0] = 0.0; //x
	vertices[1] = 0.0; //y
	vertices[2] = 0.0; //z

	float angulo = 0.0;
	float deltaAngulo = 2 * pi / (nvertices - 2);

	int i = 3; //posição em vértices onde começa o segundo ponto

	float raio = 0.5;

	while (i < nvertices * 3)
	{
		vertices[i] = raio * cos(angulo); //x
		vertices[i + 1] = raio * sin(angulo); //y
		vertices[i + 2] = 0.0;

		i += 3;
		angulo += deltaAngulo;
	}

	GLuint VBO, VAO;

	//Geração do identificador do VBO
	glGenBuffers(1, &VBO);
	//Faz a conexão (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, (nvertices * 3) * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	//Geração do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);
	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de vértices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);
	//Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando: 
	// Localização no shader * (a localização dos atributos devem ser correspondentes no layout especificado no vertex shader)
	// Numero de valores que o atributo tem (por ex, 3 coordenadas xyz) 
	// Tipo do dado
	// Se está normalizado (entre zero e um)
	// Tamanho em bytes 
	// Deslocamento a partir do byte zero 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Observe que isso é permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de vértice 
	// atualmente vinculado - para que depois possamos desvincular com segurança
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO (é uma boa prática desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);

	return VAO;
}

int loadTexture(string path)
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

GLuint createSprite()
{
	GLuint VAO, VBO, EBO;

	float vertices[] = {
		// positions // colors // texture coords
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0, // top right
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
		-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0 // top left
	};
	unsigned int indices[] = {
	0, 1, 3, // first triangle
	1, 2, 3 // second triangle
	};

	glGenVertexArrays(1, &VAO); 
	glGenBuffers(1, &VBO); 
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);





	return VAO;
}






#include<iostream>
#include<cmath>
#include<chrono>
#include<stdbool.h>

#include<glad/glad.h>
#include<GLFW/glfw3.h>

const unsigned int winInitW = 800;
const unsigned int winInitH = 800;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void toggleWireframeCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processInput(GLFWwindow *window);

const char *vertexShaderSource =
R"(#version 330 core
layout (location = 0) in vec3 aPos;

void main() {
   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
)";

const char *fragmentShaderSource =
R"(#version 330 core
out vec4 FragColor;

void main() {
    FragColor = vec4(gl_FragCoord.x/800, gl_FragCoord.y/600, 0.2f, 1.0f);
}
)";


int main (int argc, char *argv[]) {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Force floating window on tiling wm
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	GLFWwindow* window = glfwCreateWindow(winInitW, winInitH, "engine", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);


	// Glad init
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}    


	glViewport(0, 0, winInitW, winInitH);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  

	// Geometry
	float vertices[] = {
		 0.5f,  0.5f, 0.0f, // tr
		 0.5f, -0.5f, 0.0f, // br
		-0.5f, -0.5f, 0.0f, // bl
		-0.5f,  0.5f, 0.0f  // tl
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	}; 

	// Vertex shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int  success;
	char infoLog[512];

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "Vertex shader failed to compile:\n" << infoLog << std::endl;
		return -1;
	}

	// Fragment shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "Fragment shader failed to compile:\n" << infoLog << std::endl;
		return -1;
	}

	// Shader program
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "Failed to link shader program:\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	unsigned int EBO;
	glGenBuffers(1, &EBO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);

	// VAO
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Buffer inits
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	// Buffers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glfwSetKeyCallback(window, toggleWireframeCallback);

	glBindVertexArray(VAO);

	auto st = std::chrono::steady_clock::now();

	// Main loop
	while(!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(29/255.0, 32/255.0, 33/255.0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		auto nt = std::chrono::steady_clock::now();
		float m = std::chrono::duration_cast<std::chrono::milliseconds>(nt-st).count();
		vertices[0] = sin(m/1000);
		vertices[1] = cos(m/1000);
		vertices[3] = cos(m/1000);
		vertices[4] = -sin(m/1000);
		vertices[6] = -sin(m/1000);
		vertices[7] = -cos(m/1000);
		vertices[9] = -cos(m/1000);
		vertices[10] = sin(m/1000);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glUseProgram(shaderProgram);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window); // "Swap" colour buffer onto screen
		glfwPollEvents();    
	}

	// Clean up and exit
	glfwTerminate();
	std::cout << "Terminated successfully" << std::endl;
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}  

void toggleWireframeCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_W && action == GLFW_PRESS) {
		GLint polygonMode[2];
		glGetIntegerv(GL_POLYGON_MODE, polygonMode);
		if (polygonMode[0] == GL_FILL)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void processInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

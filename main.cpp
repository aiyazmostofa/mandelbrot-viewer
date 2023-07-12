#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// Method headers
static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

// Vertex source
static const char* vertexShaderSource =
R"(
#version 330 core
layout (location = 0) in vec2 aPos;
uniform vec4 gB;
out vec2 cPos;
void main()
{
	cPos = vec2((aPos.x + 1.0) / 2.0 * (gB.y - gB.x) + gB.x, (aPos.y + 1.0) / 2.0 * (gB.w - gB.z) + gB.z);
	gl_Position = vec4(aPos.x, -aPos.y, 0.0, 1.0);
}
)";

// Fragment source
static const char* fragmentShaderSource =
R"(
#version 330 core
in vec2 cPos;
out vec4 FragColor;

vec3 mandelbrot();

void main()
{
	FragColor = vec4(mandelbrot(),1.0);
}

vec3 mandelbrot() 
{
	float x = 0.0;
	float y = 0.0;
	int iteration = 0;
	float x2 = 0.0;
	float y2 = 0.0;
	while (x*x + y*y <= 4 && iteration < 1000) 
	{
		y = 2*x*y + cPos.y;
		x = x2 - y2 + cPos.x;
		x2 = x * x;
		y2 = y * y;
		iteration = iteration + 1;
	}
	float f = iteration / 1000.0;
	return vec3(f, f, f);
}
)";

// Graph and mouse coordinates
static float minX = -2.0f;
static float maxX = 2.0f;
static float minY = -1.5f;
static float maxY = 1.5f;
static float mouseX = 0.0f;
static float mouseY = 0.0f;
static bool drag = false;

// "Main" method
int main()
{
	// Initialize GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create GLFW Window
	GLFWwindow* window = glfwCreateWindow(800, 600, "Deez Fractals", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Set viewport
	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Set up mouse input
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);


	// Error log variables
	int  success;
	char infoLog[512];

	// Compile vertex shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// Check vertex shader errors
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		return -1;
	}

	// Compile fragment shader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Check fragment shader errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		return -1;
	}

	// Create shader program
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Check shader program errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
		return -1;
	}

	// Delete shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Set vertices
	float vertices[] = {
		// x, y
		1.0f, 1.0f,
		1.0f, -1.0f,
		-1.0f, -1.0f,
		-1.0f, 1.0f,
	};

	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	// Create arrays and buffers
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Manage VAO
	glBindVertexArray(VAO);

	// Manage VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Manage EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Manage position attribute
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Demanage arrays and buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);



	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		// Draw
		glUseProgram(shaderProgram);
		int gBLocation = glGetUniformLocation(shaderProgram, "gB");
		glUniform4f(gBLocation, minX, maxX, minY, maxY);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// Swap buffers
		glfwSwapBuffers(window);
		// GLFW processing
		glfwPollEvents();
	}

	// Deallocate memory
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);

	// Terminate program
	glfwTerminate();

	return 0;
}

// Change viewport according to current screen size
static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// Handle dragging when mouse moving
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (drag) {
		// We add the difference between the current mouse position and the previously recorded mouse position.
		float temp_mouseX = (float)(xpos / 800.0f * (maxX - minX) + minX);
		float temp_mouseY = (float)(ypos / 600.0f * (maxY - minY) + minY);
		minX = minX + (mouseX - temp_mouseX);
		maxX = maxX + (mouseX - temp_mouseX);
		minY = minY + (mouseY - temp_mouseY);
		maxY = maxY + (mouseY - temp_mouseY);
	}
	else {
		mouseX = (float)(xpos / 800.0f * (maxX - minX) + minX);
		mouseY = (float)(ypos / 600.0f * (maxY - minY) + minY);
	}
}

// Activate dragging when needed
static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) drag = true;
	else drag = false;
}

// Handle zooming
/*
	This method works by calculating the domain and range of the graph, then adding both ends to the current center.
	For example, say we have the ends of -10 and 10 and the new domain is 8.
	We first find the middle of the ends, which is 0, then evenly distribute the domain to both parts.
	So our new ends are -4 and 4;
*/
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	float width;
	float height;
	if (yoffset > 0) {
		width = (maxX - minX) * 5.0f / 6.0f;
		height = (maxY - minY) * 5.0f / 6.0f;
	}
	else {
		width = (maxX - minX) * 6.0f / 5.0f;
		height = (maxY - minY) * 6.0f / 5.0f;
	}
	if (width < 0.000001) return;
	float temp_minX = (maxX + minX - width) / 2.0f;
	maxX = (maxX + minX + width) / 2.0f;
	minX = temp_minX;
	float temp_minY = (maxY + minY - height) / 2.0f;
	maxY = (maxY + minY + height) / 2.0f;
	minY = temp_minY;
}

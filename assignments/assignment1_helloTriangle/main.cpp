#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

// Triangle Vertices
float vertices[9] = {
	//x   //y	//z
   -0.5, -0.5, 0.0,
	0.5, -0.5, 0.0,
	0.0,  0.5, 0.0
};

// Vertex Shader Source
const char* vertexShaderSource = R"(
	#version 450
	layout(location = 0) in vec3 vPos;
	void main(){
		gl_Position = vec4(vPos,1.0);
	}
)";

// Fragment Shader Source
const char* fragmentShaderSource = R"(
	#version 450
	out vec4 FragColor;
	void main(){
		FragColor = vec4(1.0, 0.0, 0.0, 1.0);
	}
)";

int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello Triangle", NULL, NULL);
	if (window == NULL) {
		printf("GLFW failed to create window");
		return 1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}

	// gladLoadGL
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//Define position attribute (3 floats)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (const void*)0);
	glEnableVertexAttribArray(0);

	//Creates a new vertex array object with vertex data
	unsigned int createVAO(float* vertexData, int numVertices);
	
	// Defining a new buffer object;
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// Allocate space for + send vertex data to GPU
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	// Create a new vertex shader object
	unsigned int vertexShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Supply the shader object with source code
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	// Compile the shader object
	glCompileShader(vertexShader);

	int success;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		// 512 is an arbitrary length, but should be plenty of characters for our error message.
		char infoLog[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		printf("Failed to compile shader: %s", infoLog);
	}

	// Create a new fragment shader object
	unsigned int fragmentShader = glCreateShader(GL_VERTEX_SHADER);
	// Supply the shader object with source code
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	// Compile the shader object
	glCompileShader(fragmentShader);

	int success;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		// 512 is an arbitrary length, but should be plenty of characters for our error message.
		char infoLog[512];
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		printf("Failed to compile shader: %s", infoLog);
	}

	//The linked program now contains our compiled code, so we can delete these intermediate objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//Creates a new shader program with vertex + fragment stages
	//Returns id of new shader program if successful, 0 if failed
	unsigned int createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);

	unsigned int shaderProgram = glCreateProgram();
	//Attach each stage
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	//Link all the stages together
	glLinkProgram(shaderProgram);

	unsigned int shader = createShaderProgram(vertexShaderSource, fragmentShaderSource);
	unsigned int vao = createVAO(vertices, 3);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shader);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(window);
	}

	printf("Shutting down...");
}

#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>

#include "Functions.h"

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

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

	const int arrayLength = 21;

	float vertices[arrayLength] = {
		//x   //y  //z   //r  //g  //b  //a
		-0.5, -0.5, 0.0, 1.0, 0.0, 0.0, 1.0, //Bottom left
		 0.5, -0.5, 0.0, 0.0, 1.0, 0.0, 1.0, //Bottom right
		 0.0,  0.5, 0.0, 0.0, 0.0, 1.0, 1.0  //Top center
	};


	const char* vertexShaderSource = R"(
	#version 450
	layout(location = 0) in vec3 vPos;
	layout(location = 1) in vec4 vColor;
	out vec4 Color;
	void main(){
		Color = vColor;
		gl_Position = vec4(vPos,1.0);
	}
	)";

	const char* fragmentShaderSource = R"(
	#version 450
	out vec4 FragColor;
	in vec4 Color;
	void main(){
		FragColor = Color;
	}
	)";


	// Creates a new shader of a given type.
	// Possible types: GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, etc
	// Returns id of the shader object

	
	unsigned int shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);
	unsigned int vao = createVAO(vertices, arrayLength, 3);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
	}

	printf("Shutting down...");
}
#include <stdio.h>
#include <math.h>
#include <time.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <jesseT/shader.h>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

struct Vertex {
	float x, y, z;
	float u, v;
};

unsigned int createVAO(Vertex* vertexData, int numVertices, unsigned int* indicesData, int numIndices);


Vertex vertices[4] = {
	//x   //y  //z   //u   //v
	//triangle 1
	{-1.0, -1.0, 0.0, 0.0, 0.0},
	{ 1.0, -1.0, 0.0, 1.0, 0.0},
	{ 1.0,  1.0, 0.0, 1.0, 1.0},
	{ -1.0, 1.0, 0.0, 0.0, 1.0}
};



unsigned int indicies[6] = {
	0, 1, 2, // triangle 1
	0, 2, 3, // triangle 22
};


float sunColor[3] = { 1.0, 0.8, 0.0 };
float buildingColor1[3] = { 0.1, 0.1, 0.1 };
float buildingColor2[3] = { 0.15,0.15,0.15 };
float buildingColor3[3] = { 0.2, 0.2, 0.2 };
float triangleBrightness = 1.0f;
float iTime = (float)glfwGetTime();
float timeSped = 1.0;

bool showImGUIDemoWindow = true;


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
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}

	//Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();


	jesseT::Shader shader ("assets/vertexShader.vert", "assets/fragmentShader.frag");
	
	unsigned int vao = createVAO(vertices, 4, indicies, 6);

	shader.use();

	glBindVertexArray(vao);

	//Wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		iTime = (float)glfwGetTime();
		//Set uniforms
		shader.setFloat("_iTime", iTime * timeSped);
		shader.setVec2("_aspectRatio", SCREEN_WIDTH, SCREEN_HEIGHT);
		shader.setVec3("_sunColor", sunColor[0], sunColor[1], sunColor[2]);
		shader.setVec3("_buildingColor1", buildingColor1[0], buildingColor1[1], buildingColor1[2]);
		shader.setVec3("_buildingColor2", buildingColor2[0], buildingColor2[1], buildingColor2[2]);
		shader.setVec3("_buildingColor3", buildingColor3[0], buildingColor3[1], buildingColor3[2]);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

		//Render UI
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Settings");
			ImGui::Checkbox("Show Demo Window", &showImGUIDemoWindow);
			ImGui::SliderFloat("Time Speed", &timeSped, 0.0f, 3.0f);
			ImGui::ColorEdit3("Sun Color", sunColor);
			ImGui::ColorEdit3("Back Buildings", buildingColor1);
			ImGui::ColorEdit3("Middle Buildings", buildingColor2);
			ImGui::ColorEdit3("Front Buildings", buildingColor3);
			ImGui::End();
			if (showImGUIDemoWindow) {
				ImGui::ShowDemoWindow(&showImGUIDemoWindow);
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}



unsigned int createVAO(Vertex* vertexData, int numVertices, unsigned int* indicesData, int numIndices) {
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned int ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numIndices, indicesData, GL_STATIC_DRAW);

	//Define a new buffer id
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//Allocate space for + send vertex data to GPU.
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numVertices * sizeof(Vertex), vertexData, GL_STATIC_DRAW);

	//Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, x));
	glEnableVertexAttribArray(0);

	//UV
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(offsetof(Vertex, u)));
	glEnableVertexAttribArray(1);

	return vao;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


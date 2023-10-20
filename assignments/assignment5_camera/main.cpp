#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <ew/shader.h>
#include <ew/procGen.h>
#include <ew/transform.h>
#include <../core/jesseT/camera.h>
#include <../core/jesseT/transformations.h>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);

void clamp(float maxNum, float minNum, float &x) {
	if (x < minNum)
	{
		x = minNum;
	}
	if (x > maxNum)
	{
		x = maxNum;
	}

}

// Move Camera
void moveCamera(GLFWwindow* window, jesseT::Camera* camera, jesseT::CameraControls* controls, float deltaTime) {
	if (!glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2)) {
		//Release cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		controls->firstMouse = true;
		return;
	}
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Get screen mouse position this frame
	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);

	//If we just started right clicking, set prevMouse values to current position.
	//This prevents a bug where the camera moves as soon as we click.
	if (controls->firstMouse) {
		controls->firstMouse = false;
		controls->prevMouseX = mouseX;
		controls->prevMouseY = mouseY;
	}

	// Get mouse position delta for this frame
	controls->pitch += mouseY - controls->prevMouseY;//Y
	controls->yaw += mouseX - controls->prevMouseX;	//X
		
	clamp(89, -89, controls->pitch);
	
	//Remember previous mouse position
	controls->prevMouseX = mouseX;
	controls->prevMouseY = mouseY;

	//Construct Forward Vector
	ew::Vec3 forward = ew::Vec3(
	( sin(ew::Radians(controls->yaw)) * cos(ew::Radians(controls->pitch))),
	( -sin(ew::Radians(controls->pitch))),
	( -cos(ew::Radians(controls->yaw)) * cos(ew::Radians(controls->pitch)))
	);

	// Get Right and Up
	
	ew::Vec3 right = ew::Normalize( ew::Cross(forward, ew::Vec3(0,1,0)));
	ew::Vec3 up = ew::Normalize( ew::Cross(right, forward));

	// Keyboard inputs

	controls->moveSpeed = controls->initialMoveSpeed * deltaTime;

	//increase speed
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) {
		controls->moveSpeed = controls->shiftMoveSpeed * deltaTime;
	}
	//forward
	if (glfwGetKey(window, GLFW_KEY_W)) {
		camera->position += forward * controls->moveSpeed;
	}
	//left
	if (glfwGetKey(window, GLFW_KEY_A)) {
		camera->position += right * -controls->moveSpeed;
	}
	//back
	if (glfwGetKey(window, GLFW_KEY_S)) {
		camera->position += forward * -controls->moveSpeed;
	}
	//right
	if (glfwGetKey(window, GLFW_KEY_D)) {
		camera->position += right * controls->moveSpeed;
	}
	//up
	if (glfwGetKey(window, GLFW_KEY_SPACE)) {
		camera->position += up * controls->moveSpeed;
	}
	//down
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)) {
		camera->position += up * -controls->moveSpeed;
	}

	// Set Camera Position & Target

	camera->target = camera->position + forward;
};

                          
//Projection will account for aspect ratio!
const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

const int NUM_CUBES = 4;
ew::Transform cubeTransforms[NUM_CUBES];

int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Camera", NULL, NULL);
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

	//Enable back face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//Depth testing - required for depth sorting!
	glEnable(GL_DEPTH_TEST);

	ew::Shader shader("assets/vertexShader.vert", "assets/fragmentShader.frag");
	
	//Cube mesh
	ew::Mesh cubeMesh(ew::createCube(0.5f));

	//Cube positions
	for (size_t i = 0; i < NUM_CUBES; i++)
	{
		cubeTransforms[i].position.x = i % (NUM_CUBES / 2) - 0.5;
		cubeTransforms[i].position.y = i / (NUM_CUBES / 2) - 0.5;
	}

	//Camera creation
	// clamp(x, a, b) { return min(max(x,a), b)

	//norm p-e
	//projection needs screen H&W


	// Camera


	jesseT::Camera theCamera;
	theCamera.position = ew::Vec3(0.0, 0.0, 5.0);
	theCamera.target = ew::Vec3(0.0, 0.0, 0.0);
	theCamera.fov = 60.0;
	theCamera.aspectRatio = (float)SCREEN_WIDTH / SCREEN_HEIGHT;
	theCamera.nearPlane = 0.1;
	theCamera.farPlane = 100.0;
	theCamera.orthographic = false;
	theCamera.orthoSize = 6.0;

	// Camera controls
	jesseT::CameraControls cameraControls;

	// Time
	float prevTime = 0;

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		//Calc Time
		float time = (float)glfwGetTime(); //Timestamp of current frame
		float deltaTime = time - prevTime;
		prevTime = time;

		moveCamera(window, &theCamera, &cameraControls, deltaTime);
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		//Clear both color buffer AND depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		
		//Set uniforms
		shader.use();

		//TODO: Set model matrix uniform
		for (size_t i = 0; i < NUM_CUBES; i++)
		{
			//Construct model matrix
			shader.setMat4("_Model", cubeTransforms[i].getModelMatrix());
			cubeMesh.draw();
		}

		//Camera
		shader.setMat4("_View", theCamera.ViewMatrix());
		shader.setMat4("_Projection", theCamera.ProjectionMatrix());

		//Camera Movement
		{

		}

		//Render UI
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Settings");
			ImGui::Text("Cubes");
			for (int i = 0; i < NUM_CUBES; i++)
			{
				ImGui::PushID(i);
				if (ImGui::CollapsingHeader("Transform")) {
					ImGui::DragFloat3("Position", &cubeTransforms[i].position.x, 0.05f);
					ImGui::DragFloat3("Rotation", &cubeTransforms[i].rotation.x, 1.0f);
					ImGui::DragFloat3("Scale", &cubeTransforms[i].scale.x, 0.05f);
				}
				ImGui::PopID();
			}
			ImGui::Text("Camera");
			ImGui::DragFloat3("Position", &theCamera.position.x, 0.05f);
			ImGui::DragFloat3("Target", &theCamera.target.x, 0.05f);
			ImGui::DragFloat("FOV", &theCamera.fov, 0.05f);
			ImGui::Checkbox("Orthographic", &theCamera.orthographic);
			ImGui::DragFloat("Ortho Height", &theCamera.orthoSize, 0.05f);
			ImGui::DragFloat("Near Plane", &theCamera.nearPlane, 0.05f);
			ImGui::DragFloat("Far Plane", &theCamera.farPlane, 0.05f);
			if (ImGui::Button("Reset")) {

				theCamera.position = ew::Vec3(0.0, 0.0, 5.0);
				theCamera.target = ew::Vec3(0.0, 0.0, 0.0);
				theCamera.fov = 60.0;
				theCamera.aspectRatio = (float)SCREEN_WIDTH / SCREEN_HEIGHT;
				theCamera.nearPlane = 0.1;
				theCamera.farPlane = 100.0;
				theCamera.orthographic = false;
				theCamera.orthoSize = 6.0;

				cameraControls.pitch = 0;
				cameraControls.yaw = 0;


			}

			ImGui::End();
			
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}


		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


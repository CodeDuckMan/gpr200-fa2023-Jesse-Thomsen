#pragma once
#include "../ew/ewMath/ewMath.h"
#include "../ew/ewMath/mat4.h"
#include "../ew/ewMath/vec3.h"
#include "../jesseT/transformations.h"
namespace jesseT {
	struct Camera 
	{
		ew::Vec3 position = ew::Vec3(0.0, 0.0, 0.5); //Camera body position
		ew::Vec3 target = ew::Vec3(0.0, 0.0, 0.0); //Position to look at
		ew::Vec3 up = ew::Vec3(0.0, 1.0, 0.0);
		float fov; //Vertical field of view in degrees
		float aspectRatio; //Screen width / Screen height
		float nearPlane; //Near plane distance (+Z)
		float farPlane; //Far plane distance (+Z)
		bool orthographic; //Perspective or orthographic?
		float orthoSize; //Height of orthographic frustum
		ew::Mat4 ViewMatrix() { //World->View
			return jesseT::LookAt(position, target, up);
		};
		ew::Mat4 ProjectionMatrix() { //View->Clip
			if (orthographic)
			{
				return jesseT::Orthographic(orthoSize, aspectRatio, nearPlane, farPlane);
			}
			else
			{
				return jesseT::Perspective(ew::Radians(fov), aspectRatio, nearPlane, farPlane);
			}
		};
	};
	//Camera aiming variables
	struct CameraControls 
	{
		double prevMouseX, prevMouseY;
		float yaw = 0, pitch = 0;
		float mouseSensitivity = 0.1f;
		bool firstMouse = true;
		float moveSpeed = 5.0f;
		float initialMoveSpeed = 5.0f;
		float shiftMoveSpeed = 10.0f;
	};
}
#pragma once
#include <sstream>
#include <fstream>
#include <C:\Users\jesse\OneDrive\Desktop\Repositories\gpr200-fa2023-Jesse-Thomsen\core\ew\external\glad.h>
#include <C:\Users\jesse\OneDrive\Desktop\Repositories\gpr200-fa2023-Jesse-Thomsen\core\ew\ewMath\ewMath.h>

namespace jesseT {
	std::string loadShaderSourceFromFile(const std::string& filePath);

	class Shader {
	public:
		Shader(const std::string& vertexShader, const std::string& fragmentShader);
		
		unsigned int createShader(GLenum shaderType, const char* sourceCode);
		unsigned int createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);

		void use();
		void setInt(const std::string& name, int v) const;
		void setFloat(const std::string& name, float v) const;
		void setVec2(const std::string& name, float x, float y) const;
		void setVec3(const std::string& name, float x, float y, float z) const;
		void setVec4(const std::string& name, float x, float y, float z, float w) const;

	private:
		unsigned int m_id;
	};
}
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Utils.h"

#include <string>

class Shader {
public:
	Shader(const char *vertexFilepath, const char *fragmentFilepath);
	~Shader();

	void use();

	unsigned int getId() const;

	void setMat4(const char *uniformName, const glm::mat4 &mat4);
	void setMat3(const char *uniformName, const glm::mat3 &mat3);
	void setMat2(const char *uniformName, const glm::mat2 &mat2);

	void setVec4(const char *uniformName, const glm::vec4 &vec4);
	void setVec3(const char *uniformName, const glm::vec3 &vec3);
	void setVec2(const char *uniformName, const glm::vec2 &vec2);

	void setInt(const char *uniformName, int integer);
	void setFloat(const char *uniformName, float flt);
private:
	bool isShaderCompiled(unsigned int &shader);
	bool isShaderProgramLinked(unsigned int &program);

	unsigned int createShader(const char *filepath, unsigned int type);
	void createShaderProgram(const char *vertexFilepath, const char *fragmentFilepath);

	unsigned int m_id;
};

#endif
#include "Shader.h"

Shader::Shader(const char *vertexFilepath, const char *fragmentFilepath) {
	createShaderProgram(vertexFilepath, fragmentFilepath);
}

Shader::~Shader() {
	glDeleteProgram(m_id);
}

void Shader::use() {
	glUseProgram(m_id);
}

unsigned int Shader::getId() const {
	return m_id;
}

void Shader::setMat4(const char *uniformName, const glm::mat4 &mat4) {
	glUniformMatrix4fv(glGetUniformLocation(m_id, uniformName), 1, GL_FALSE, glm::value_ptr(mat4));
}

void Shader::setMat3(const char *uniformName, const glm::mat3 &mat3) {
	glUniformMatrix3fv(glGetUniformLocation(m_id, uniformName), 1, GL_FALSE, glm::value_ptr(mat3));
}

void Shader::setMat2(const char *uniformName, const glm::mat2 &mat2) {
	glUniformMatrix2fv(glGetUniformLocation(m_id, uniformName), 1, GL_FALSE, glm::value_ptr(mat2));
}

void Shader::setVec4(const char *uniformName, const glm::vec4 &vec4) {
	glUniform4fv(glGetUniformLocation(m_id, uniformName), 1, glm::value_ptr(vec4));
}

void Shader::setVec3(const char *uniformName, const glm::vec3 &vec3) {
	glUniform3fv(glGetUniformLocation(m_id, uniformName), 1, glm::value_ptr(vec3));
}

void Shader::setVec2(const char *uniformName, const glm::vec2 &vec2) {
	glUniform2fv(glGetUniformLocation(m_id, uniformName), 1, glm::value_ptr(vec2));
}

void Shader::setInt(const char *uniformName, int integer) {
	glUniform1i(glGetUniformLocation(m_id, uniformName), integer);
}

void Shader::setFloat(const char *uniformName, float flt) {
	glUniform1f(glGetUniformLocation(m_id, uniformName), flt);
}

bool Shader::isShaderCompiled(unsigned int &shader) {
	int success;
	char infoLog[512];

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		const std::string inf = infoLog;
		std::cout << "GLSL Shader error:\n\t" << inf;

		return false;
	}

	return true;
}

bool Shader::isShaderProgramLinked(unsigned int &program) {
	int success;
	char infoLog[512];

	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, nullptr, infoLog);
		const std::string inf = infoLog;
		std::cout << "Shader program error:\n\t" << inf;

		return false;
	}

	return true;
}

unsigned int Shader::createShader(const char *filepath, unsigned int type) {
	std::string shaderCode = readFileContents(filepath);
	const char *shaderSource = shaderCode.c_str();

	unsigned int shader = glCreateShader(type);
	glShaderSource(shader, 1, &shaderSource, nullptr);
	glCompileShader(shader);

	if (!isShaderCompiled(shader)) {
		std::cout << "Failed to compile shader.\n";
		return NULL;
	}

	return shader;
}

void Shader::createShaderProgram(const char *vertexFilepath, const char *fragmentFilepath) {
	unsigned int vertexShader = createShader(vertexFilepath, GL_VERTEX_SHADER);
	unsigned int fragmentShader = createShader(fragmentFilepath, GL_FRAGMENT_SHADER);

	m_id = glCreateProgram();
	glAttachShader(m_id, vertexShader);
	glAttachShader(m_id, fragmentShader);
	glLinkProgram(m_id);

	if (!isShaderProgramLinked(m_id)) {
		std::cout << "Failed to link program.\n";
		return;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}
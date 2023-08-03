#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <string.h>
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader{
public:
	//着色器程序ID
	unsigned int ID;
	//着色器程序类的构造函数
	Shader(const char* vertexPath, const char* fragPath) {
		std::string vertexCode;
		std::string fragmentCode;

		std::ifstream vShderFile;
		std::ifstream fShaderFile;

		vShderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try {
			//打开文件
			vShderFile.open(vertexPath);
			fShaderFile.open(fragPath);

			//读取文件流
			std::stringstream vShaderString, fShaderString;
			vShaderString << vShderFile.rdbuf();
			fShaderString << fShaderFile.rdbuf();

			//关闭文件
			vShderFile.close();
			fShaderFile.close();

			//将文件流转化为string
			vertexCode = vShaderString.str();
			fragmentCode = fShaderString.str();

			//std::cout << vertexCode << std::endl;
		}
		catch (std::ifstream::failure e) {
			std::cout << "Failed to read file stream" << e.what() << std::endl;
		}
		
		//通过string类的c_str()函数能够把string对象转换成c语言中的char字符串的样式
		const char* vertexShaderCode = vertexCode.c_str();
		const char* fragmentShaderCode = fragmentCode.c_str();

		/*std::cout << vertexShaderCode << std::endl;
		std::cout << fragmentShaderCode << std::endl;*/

		unsigned int vertexShaderS, fragmentShaderS;
		vertexShaderS = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShaderS, 1, &vertexShaderCode, NULL);
		glCompileShader(vertexShaderS);

		fragmentShaderS = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShaderS, 1, &fragmentShaderCode, NULL);
		glCompileShader(fragmentShaderS);

		int success;
		char infoLog[512];

		glGetShaderiv(vertexShaderS, GL_COMPILE_STATUS,&success);

		if (!success) {
			std::cout << "Failed to create vertex shader" << std::endl;
			glGetShaderInfoLog(vertexShaderS, 512, NULL, infoLog);
		}

		glGetShaderiv(fragmentShaderS, GL_COMPILE_STATUS, &success);
		if (!success) {
			std::cout << "Failed to create fragment shader" << std::endl;
			glGetShaderInfoLog(fragmentShaderS, 512, NULL, infoLog);
		}

		//创建着色器程序
		ID = glCreateProgram();
		glAttachShader(ID, vertexShaderS);
		glAttachShader(ID, fragmentShaderS);
		glLinkProgram(ID);

		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success) {
			std::cout << "Failed to create shader program" << std::endl;
			glGetProgramInfoLog(ID, 512, NULL, infoLog);
		}

		//在着色器程序创建并编译成功后，删除之前创建的着色器
		glDeleteShader(vertexShaderS);
		glDeleteShader(fragmentShaderS);
	}

	Shader(const char* vertexPath, const char* geometryPath, const char* fragPath) {
		std::string vertexCode;
		std::string geometryCode;
		std::string fragmentCode;

		std::ifstream vShderFile;
		std::ifstream gShaderFile;
		std::ifstream fShaderFile;

		vShderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try {
			//打开文件
			vShderFile.open(vertexPath);
			gShaderFile.open(geometryPath);
			fShaderFile.open(fragPath);

			//读取文件流
			std::stringstream vShaderString, gShaderString ,fShaderString;
			vShaderString << vShderFile.rdbuf();
			gShaderString << gShaderFile.rdbuf();
			fShaderString << fShaderFile.rdbuf();

			//关闭文件
			vShderFile.close();
			gShaderFile.close();
			fShaderFile.close();

			//将文件流转化为string
			vertexCode = vShaderString.str();
			geometryCode = gShaderString.str();
			fragmentCode = fShaderString.str();

			//std::cout << vertexCode << std::endl;
		}
		catch (std::ifstream::failure e) {
			std::cout << "Failed to read file stream" << e.what() << std::endl;
		}

		//通过string类的c_str()函数能够把string对象转换成c语言中的char字符串的样式
		const char* vertexShaderCode = vertexCode.c_str();
		const char* geometryShaderCode = geometryCode.c_str();
		const char* fragmentShaderCode = fragmentCode.c_str();

		/*std::cout << vertexShaderCode << std::endl;
		std::cout << fragmentShaderCode << std::endl;*/

		unsigned int vertexShaderS, geometryShaderS, fragmentShaderS;
		vertexShaderS = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShaderS, 1, &vertexShaderCode, NULL);
		glCompileShader(vertexShaderS);
		checkCompileErrors(vertexShaderS, "VERTEX");

		geometryShaderS = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometryShaderS, 1, &geometryShaderCode, NULL);
		glCompileShader(geometryShaderS);
		checkCompileErrors(geometryShaderS, "GEOMETRY");

		fragmentShaderS = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShaderS, 1, &fragmentShaderCode, NULL);
		glCompileShader(fragmentShaderS);
		checkCompileErrors(fragmentShaderS, "FRAGMENT");

		//创建着色器程序
		ID = glCreateProgram();
		glAttachShader(ID, vertexShaderS);				
		glAttachShader(ID, geometryShaderS);
		glAttachShader(ID, fragmentShaderS);
		glLinkProgram(ID);
		checkCompileErrors(ID, "PROGRAM");

		//在着色器程序创建并编译成功后，删除之前创建的着色器
		glDeleteShader(vertexShaderS);
		glDeleteShader(geometryShaderS);
		glDeleteShader(fragmentShaderS);
	}
	//使用程序
	void use() {
		glUseProgram(ID);
	}

	//设置uniform值，将函数设为const是为了不改变对象中的任何变量，所有变量只能读，不能写
	void SetInt(const std::string& name, int value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}

	void SetBool(const std::string& name, bool value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}

	void SetFloat(const std::string& name, float value) const {
		//std::cout << "enter setfloat，the name is " << name.c_str() << std::endl;
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}

	void SetVec2(const std::string& name, const float x, const float y) {
		//glUniform3f(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, v3[0]);
		glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
	}

	void setVec2(const std::string& name, glm::vec2 v2) {
		glUniform2f(glGetUniformLocation(ID, name.c_str()), v2.x, v2.y);
	}

	void SetVec3(const std::string& name, const float x, const float y, const float z) {
		//glUniform3f(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, v3[0]);
		glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
	}

	void setVec3(const std::string& name, glm::vec3 v3) {
		//glUniform3f(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, v3[0]);
		glUniform3f(glGetUniformLocation(ID, name.c_str()), v3.x, v3.y, v3.z);
	}

	void SetMat4(const std::string& name, const glm::mat4 mat) {
		glUniformMatrix4fv(glGetUniformLocation(ID,name.c_str()), 1, GL_FALSE, &mat[0][0]);		
	}

	void deleteProgram() {
		glDeleteProgram(ID);
	}

private:
	void checkCompileErrors(GLuint shader, std::string type)
	{
		GLint success;
		GLchar infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}
};


#endif // !SHADER_H




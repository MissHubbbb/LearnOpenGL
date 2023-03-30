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
	//��ɫ������ID
	unsigned int ID;
	//��ɫ��������Ĺ��캯��
	Shader(const char* vertexPath, const char* fragPath) {
		std::string vertexCode;
		std::string fragmentCode;

		std::ifstream vShderFile;
		std::ifstream fShaderFile;

		vShderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try {
			//���ļ�
			vShderFile.open(vertexPath);
			fShaderFile.open(fragPath);

			//��ȡ�ļ���
			std::stringstream vShaderString, fShaderString;
			vShaderString << vShderFile.rdbuf();
			fShaderString << fShaderFile.rdbuf();

			//�ر��ļ�
			vShderFile.close();
			fShaderFile.close();

			//���ļ���ת��Ϊstring
			vertexCode = vShaderString.str();
			fragmentCode = fShaderString.str();

			//std::cout << vertexCode << std::endl;
		}
		catch (std::ifstream::failure e) {
			std::cout << "Failed to read file stream" << e.what() << std::endl;
		}
		
		//ͨ��string���c_str()�����ܹ���string����ת����c�����е�char�ַ�������ʽ
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

		//������ɫ������
		ID = glCreateProgram();
		glAttachShader(ID, vertexShaderS);
		glAttachShader(ID, fragmentShaderS);
		glLinkProgram(ID);

		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success) {
			std::cout << "Failed to create shader program" << std::endl;
			glGetProgramInfoLog(ID, 512, NULL, infoLog);
		}

		//����ɫ�����򴴽�������ɹ���ɾ��֮ǰ��������ɫ��
		glDeleteShader(vertexShaderS);
		glDeleteShader(fragmentShaderS);
	}
	//ʹ�ó���
	void use() {
		glUseProgram(ID);
	}

	//����uniformֵ����������Ϊconst��Ϊ�˲��ı�����е��κα��������б���ֻ�ܶ�������д
	void SetInt(const std::string& name, int value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}

	void SetBool(const std::string& name, bool value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}

	void SetFloat(const std::string& name, float value) const {
		//std::cout << "enter setfloat��the name is " << name.c_str() << std::endl;
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}

	void SetVec3(const std::string& name, const float x, const float y, const float z) {
		//glUniform3f(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, v3[0]);
		glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
	}

	void SetMat4(const std::string& name, const glm::mat4 mat) {
		glUniformMatrix4fv(glGetUniformLocation(ID,name.c_str()), 1, GL_FALSE, &mat[0][0]);		
	}

	void deleteProgram() {
		glDeleteProgram(ID);
	}
};


#endif // !SHADER_H




#define STB_IMAGE_IMPLEMENTATION
#include <map>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"
#include "Camera.h"
#include "Shader.h"

class MyTexture
{
public:
	unsigned int textureID;

	MyTexture(const char* name) {
		textureID = LoadTexture(name);
	}
	//һ���뷨����������Ҳ������������Ŀǰc++������Ϥ����Ҫ�������β�ת��Ϊ�����ڵı�����ʹ��map�����洢��ֵ�ԣ�
	//���һ��漰��const char*תΪstring���о���string���������ܣ�����뷨���Ҹ���
	MyTexture(const char* mapName, const char* name) {
		
	}

	unsigned int LoadTexture(const char* name) {
		unsigned int texture;
		//����Ҳ��ʹ��ID���õģ���һ������������������������ڶ�����ID��(����������)	
		//glGenTextures(1, &texture);
		glGenTextures(1, &texture);

		//���ز���������
		int width1, height1, nrChannels1;
		//����ͼ��ǰ����תͼ���y��
		stbi_set_flip_vertically_on_load(true);
		//����ͼƬ����ȣ��߶ȣ���ɫͨ���ĸ���
		unsigned char* data = stbi_load(name, &width1, &height1, &nrChannels1, 0);

		if (data) {
			GLenum format;
			if (nrChannels1 == 1)
				format = GL_RED;
			else if (nrChannels1 == 3)
				format = GL_RGB;
			else if (nrChannels1 == 4)
				format = GL_RGBA;

			//�������Ա����������ָ�������в���
			glBindTexture(GL_TEXTURE_2D, texture);

			//ʹ��ǰ���ͼƬ������������������glTexImage2Dʱ����ǰ�󶨵��������ͻᱻ����������ͼ��
			glTexImage2D(GL_TEXTURE_2D, 0, format, width1, height1, 0, format, GL_UNSIGNED_BYTE, data);

			//Ϊ��ǰ�󶨵������Զ�����������Ҫ�Ķ༶��Զ����
			glGenerateMipmap(GL_TEXTURE_2D);

			//Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		else {
			std::cout << "Failed to load texture1" << std::endl;
		}

		//�ͷ��ڴ�
		stbi_image_free(data);

		return texture;
	}

	void use(unsigned int tex,int num) {
		if (num == 0) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, tex);
		}
		else if (num == 1) {
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, tex);
		}
		else if (num == 2) {
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, tex);
		}
		else if (num == 3) {
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, tex);
		}
		else if (num == 4) {
			glActiveTexture(GL_TEXTURE4);
			glBindTexture(GL_TEXTURE_2D, tex);
		}
		else {
			glActiveTexture(GL_TEXTURE5);
			glBindTexture(GL_TEXTURE_2D, tex);
		}
	}

	//~MyTexture() {};

private:

};

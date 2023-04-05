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
	//一个想法，将变量名也传进来，但是目前c++还不熟悉，需要将函数形参转化为函数内的变量名使用map容器存储键值对，
	//并且还涉及到const char*转为string，感觉用string更消耗性能，这个想法暂且搁置
	MyTexture(const char* mapName, const char* name) {
		
	}

	unsigned int LoadTexture(const char* name) {
		unsigned int texture;
		//纹理也是使用ID引用的，第一个参数是生成纹理的数量，第二个是ID号(可以是数组)	
		//glGenTextures(1, &texture);
		glGenTextures(1, &texture);

		//加载并生成纹理
		int width1, height1, nrChannels1;
		//加载图像前，反转图像的y轴
		stbi_set_flip_vertically_on_load(true);
		//加载图片：宽度，高度，颜色通道的个数
		unsigned char* data = stbi_load(name, &width1, &height1, &nrChannels1, 0);

		if (data) {
			GLenum format;
			if (nrChannels1 == 1)
				format = GL_RED;
			else if (nrChannels1 == 3)
				format = GL_RGB;
			else if (nrChannels1 == 4)
				format = GL_RGBA;

			//绑定纹理，以便后续的纹理指令对其进行操作
			glBindTexture(GL_TEXTURE_2D, texture);

			//使用前面的图片数据生成纹理，当调用glTexImage2D时，当前绑定的纹理对象就会被附加上纹理图像
			glTexImage2D(GL_TEXTURE_2D, 0, format, width1, height1, 0, format, GL_UNSIGNED_BYTE, data);

			//为当前绑定的纹理自动生成所有需要的多级渐远纹理
			glGenerateMipmap(GL_TEXTURE_2D);

			//为当前绑定的纹理对象设置环绕、过滤方式
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		else {
			std::cout << "Failed to load texture1" << std::endl;
		}

		//释放内存
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

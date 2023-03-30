/*
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const unsigned int SRCT_WIDTH = 800;
const unsigned int SRCT_HEIGHT = 600;
float mixValue = 0.2f;

const char* vertexshadersourceT = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
	"layout (location = 1) in vec3 aColor;\n"
	"layout (location = 2) in vec2 aTexCoord;\n"
	"out vec3 ourColor;\n"
	"out vec2 TexCoord;\n"
	"uniform mat4 transform;\n"
    "void main()\n"
    "{\n"
   // "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"   gl_Position = transform * vec4(aPos,1.0f);\n"
	"   ourColor = aColor;\n"
	"   TexCoord = aTexCoord;\n"
	//��תy�ᣬ�������stbi_set_flip_vertically_on_load(true)�Ĺ����ظ���
	//"   TexCoord = vec2(aTexCoord.x, 1.0 - aTexCoord.y);\n"
    "}\0";

const char* vertexshadersourceM = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"layout (location = 2) in vec2 aTexCoord;\n"
"out vec3 ourColor;\n"
"out vec2 TexCoord;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"   gl_Position = projection * view * model * vec4(aPos,1.0f);\n"
"   ourColor = aColor;\n"
"   TexCoord = aTexCoord;\n"
"}\0";

const char* fragmentshadersourceT = "#version 330 core\n"
    "out vec4 FragColor;\n"
	"in vec3 ourColor;\n"
	"in vec2 TexCoord;\n"
	"//uniform sampler2D ourTexture;\n"
	"uniform sampler2D texture1;\n"
	"uniform sampler2D texture2;\n"
	"uniform float mixValue;"
    "void main()\n"
    "{\n"
    "   //FragColor = texture(ourTexture,TexCoord);\n"
	"   //FragColor = texture(ourTexture,TexCoord) * vec4(ourColor,1.0);\n"
	"   FragColor = mix(texture(texture1,TexCoord),texture(texture2,TexCoord),mixValue);\n"
	//��תx��
	//"   FragColor = mix(texture(texture1,TexCoord),texture(texture2,vec2(1-TexCoord.x,TexCoord.y)),mixValue);\n"
	"}\n\0";

void framebuffer_size_callbackT(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

//�����ڴ����е�����
void processInputT(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}	

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		mixValue += 0.05f;
		if (mixValue >= 1.0f) {
			mixValue = 1.0f;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		mixValue -= 0.05f;
		if (mixValue <= 0.0f) {
			mixValue = 0.0f;
		}
	}
}

int main4(void) {
	//��һ������(1, 0, 0)λ��(1, 1, 0)����λ
	//glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
	//glm::mat4 trans = glm::mat4(1.0f);	//4x4�ĵ�λ����	
	//trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
	//vec = trans * vec;
	//std::cout << vec.x << vec.y << vec.z << std::endl;

	//glm::mat4 trans = glm::mat4(1.0f);
	////��z����ת90�ȣ�GLMϣ���Ƕ��ǻ����Ƶģ�����ʹ��glm::radians���Ƕ�ת��Ϊ����
	//trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
	//trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
	////��Ϊ���ǰ�������󴫵ݸ���GLM��ÿ��������GLM���Զ���������ˣ����صĽ����һ�������˶���任�ı任����
	////������������������XYƽ���ϵģ�����������Ҫ��������z����ת


	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* windowT = glfwCreateWindow(SRCT_WIDTH, SRCT_HEIGHT, "Texture Practice1", NULL, NULL);
	if (!windowT) {
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}

	//�����Ǵ����Ĵ��ڵ���ǰ������
	glfwMakeContextCurrent(windowT);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initiate GLAD" << std::endl;
		return -1;
	}

	//����������ɫ��	
	unsigned int vertexShaderT;	
	vertexShaderT = glCreateShader(GL_VERTEX_SHADER);

	//������ɫ��Դ�룬������
	glShaderSource(vertexShaderT, 1, &vertexshadersourceM, NULL);
	glCompileShader(vertexShaderT);

	int success;
	char infoLog[512];

	glad_glGetShaderiv(vertexShaderT, GL_COMPILE_STATUS, &success);

	if (!success) {
		std::cout << "Failed to create vertex shader" << std::endl;
		glGetShaderInfoLog(vertexShaderT, 512, NULL, infoLog);
	}

	//����Ƭ����ɫ��	
	unsigned int fragShaderT;
	fragShaderT = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragShaderT, 1, &fragmentshadersourceT, NULL);
	glCompileShader(fragShaderT);

	glad_glGetShaderiv(fragShaderT, GL_COMPILE_STATUS, &success);

	if (!success) {
		std::cout << "Failed to create fragment shader" << std::endl;
		glGetShaderInfoLog(fragShaderT, 512, NULL, infoLog);
	}

	unsigned int shaderProgramT;
	shaderProgramT = glCreateProgram();
	
	glAttachShader(shaderProgramT,vertexShaderT);
	glAttachShader(shaderProgramT, fragShaderT);
	glLinkProgram(shaderProgramT);

	glad_glGetProgramiv(shaderProgramT, GL_LINK_STATUS, &success);

	if (!success) {
		std::cout << "Failed to Link shader program" << std::endl;
		glGetProgramInfoLog(shaderProgramT, 512, NULL, infoLog);
	}

	glDeleteShader(vertexShaderT);
	glDeleteShader(fragShaderT);

	glViewport(0, 0, SRCT_WIDTH, SRCT_HEIGHT);
	glfwSetFramebufferSizeCallback(windowT, framebuffer_size_callbackT);

	float vertices[] = {
		//     ---- λ�� ----       ---- ��ɫ ----     - �������� -
			 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // ����
			 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // ����
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // ����
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // ����
	};

	//���������������ȡֵ����Χ��(0,1)��Ϊ(0,2)
	//float vertices[] = {
	//	//     ---- λ�� ----       ---- ��ɫ ----     - �������� -
	//		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   2.0f, 2.0f,   // ����
	//		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   2.0f, 0.0f,   // ����
	//		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // ����
	//		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 2.0f    // ����
	//};

	//Ϊ����ʾ�������صĻ���
	//float vertices[] = {
	//	// positions          // colors           // texture coords (note that we changed them to 'zoom in' on our texture image)
	//	 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   0.55f, 0.55f, // top right
	//	 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   0.55f, 0.45f, // bottom right
	//	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.45f, 0.45f, // bottom left
	//	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.45f, 0.55f  // top left 
	//};

	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	unsigned int VBO, VAO, EBO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//����λ��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//������ɫ
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//������������
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//VAO���
	glBindVertexArray(0);

	//--------------------------����ͼƬ����������ͼ
	//unsigned int texture;
	 unsigned int texture1,texture2;
	//����Ҳ��ʹ��ID���õģ���һ������������������������ڶ�����ID��(����������)	
	//glGenTextures(1, &texture);
	 glGenTextures(1, &texture1);	 
	//�������Ա����������ָ�������в���
	//glBindTexture(GL_TEXTURE_2D, texture);
	 glBindTexture(GL_TEXTURE_2D, texture1);

	//Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//���ز���������
	int width, height, nrChannels;
	//����ͼ��ǰ����תͼ���y��
	//stbi_set_flip_vertically_on_load(true);	
	//����ͼƬ����ȣ��߶ȣ���ɫͨ���ĸ���
	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);	

	if (data) {
		//ʹ��ǰ���ͼƬ������������������glTexImage2Dʱ����ǰ�󶨵��������ͻᱻ����������ͼ��
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		//Ϊ��ǰ�󶨵������Զ�����������Ҫ�Ķ༶��Զ����
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture1" << std::endl;
	}	

	//�ͷ��ڴ�
	stbi_image_free(data);

	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	//Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture2" << std::endl;
	}

	stbi_image_free(data);

	glUseProgram(shaderProgramT);
	glUniform1i(glGetUniformLocation(shaderProgramT, "texture1"), 0);	//���������λ��ֵ
	glUniform1i(glGetUniformLocation(shaderProgramT, "texture2"), 1);
	glUniform1f(glGetUniformLocation(shaderProgramT, "mixValue"), mixValue);

	while (!glfwWindowShouldClose(windowT)) {
		processInputT(windowT);
		//upAnddown(windowT, shaderProgramT);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
						
		//glBindTexture(GL_TEXTURE_2D, texture);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		
		glUniform1f(glGetUniformLocation(shaderProgramT, "mixValue"), mixValue);
		glUseProgram(shaderProgramT);

		//��һ���ķ���
		//glm::mat4 trans = glm::mat4(1.0f);
		////	�ڶ���������ʾ��ת���ȣ�������������ʾ������������������ת
		//trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		//trans = glm::translate(trans, glm::vec3(0.5f, -0.5, 0.0f));		
		//unsigned int transformLoc = glGetUniformLocation(shaderProgramT, "transform");
		////�ڶ��������Ǹ���opengl���ǽ�Ҫ���Ͷ��ٸ����󣬵��������������Ƿ�Ҫ�Ծ������ת�ã����һ�������������ľ������ݣ�����GLM���Դ��ĺ���value_ptr���任��Щ����ΪOpenGL�ɽ��ܵĸ�ʽ
		////OpenGL������ͨ��ʹ��һ���ڲ����󲼾֣�����������(Column-major Ordering)���֡�GLM��Ĭ�ϲ��־������������Բ�����Ҫת�þ���������GL_FALSE
		//glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));		
		//glBindVertexArray(VAO);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);		

		//�ڶ����ķ���
		//glm::mat4 trans1 = glm::mat4(1.0f);
		//trans1 = glm::translate(trans1, glm::vec3(-0.5f, 0.5f,0.0f));
		//float scaleAmount = static_cast<float>(sin(glfwGetTime()));
		//trans1 = glm::scale(trans1, glm::vec3(scaleAmount,scaleAmount,scaleAmount));
		////glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans1));
		//glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &trans1[0][0]);				
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		//	�ڶ���������ʾ��ת���ȣ�������������ʾ������������������ת
		model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		projection = glm::perspective(glm::radians(45.0f), (float)(width / height), 0.1f, 100.0f);

		unsigned int modelLoc = glGetUniformLocation(shaderProgramT, "model");
		//�ڶ��������Ǹ���opengl���ǽ�Ҫ���Ͷ��ٸ����󣬵��������������Ƿ�Ҫ�Ծ������ת�ã����һ�������������ľ������ݣ�����GLM���Դ��ĺ���value_ptr���任��Щ����ΪOpenGL�ɽ��ܵĸ�ʽ
		//OpenGL������ͨ��ʹ��һ���ڲ����󲼾֣�����������(Column-major Ordering)���֡�GLM��Ĭ�ϲ��־������������Բ�����Ҫת�þ���������GL_FALSE
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		unsigned int viewLoc = glGetUniformLocation(shaderProgramT, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		unsigned int projectionLoc = glGetUniformLocation(shaderProgramT, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(windowT);
		glfwPollEvents();
	}
	glDeleteProgram(shaderProgramT);
	
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	
	glfwTerminate();

	return 0;
}
*/
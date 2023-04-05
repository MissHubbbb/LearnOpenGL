/*
#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"
#include "Camera.h"
#include "Shader.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "MYGUICLASS.h"

Camera cam1;

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);		//ȫ�ֱ�������Դ��λ��

unsigned int loadTexture(char const* path);
void framebuffer_size_callbackTr(GLFWwindow* window, int width, int height);
void processInputTr(GLFWwindow* window);
void DrawGUI(GLFWwindow* window);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

const unsigned int SRCT_WIDTH = 800;
const unsigned int SRCT_HEIGHT = 600;
float mixValue = 0.2f;

float deltaTime = 0.0f;	//��ǰ֡����һ֡��ʱ���
float lastFrame = 0.0f;	//��һ֡��ʱ��
//���ĳ�ʼλ�ã���Ļ���ģ�
float lastX = 400, lastY = 300;
//yaw��x�����¸�����pitch��y������ƫ�ƣ������ȡֵ�ǽǶ�
float yaw, pitch;

//�ǲ��ǵ�һ�ν����������
bool firstMouse = true;
bool isFirstEnter = false;

ImVec4 objectColor = ImVec4(1.0f, 0.5f, 0.31f, 1.0f);
ImVec4 lightColor = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);

glm::vec3 ambientColor(0.2f, 0.2f, 0.2f);
glm::vec3 diffuseColor(0.5f, 0.5f, 0.5f);

//const char* glsl_version = "#version 130";

M_GUI my;

int main(void) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* windowTr = glfwCreateWindow(SRCT_WIDTH, SRCT_HEIGHT, "Texture Practice1", NULL, NULL);
	if (!windowTr) {
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}

	//�����Ǵ����Ĵ��ڵ���ǰ������
	glfwMakeContextCurrent(windowTr);
	glfwSwapInterval(1);

	my.SetWindow(windowTr);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initiate GLAD" << std::endl;
		return -1;
	}

	//glfwSetInputMode(windowTr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	Shader colorShader("light4_LightMap.vs.txt", "light4_LightMap.fs.txt");
	Shader lightShader("light1.vs.txt", "light1_light.fs.txt");

	glViewport(0, 0, SRCT_WIDTH, SRCT_HEIGHT);
	glfwSetFramebufferSizeCallback(windowTr, framebuffer_size_callbackTr);

	//ǰ�����Ƕ���������Ϣ���м������Ƕ����Ӧ�ķ�������Ϣ������������uv����
	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};

	unsigned int VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);
	//����λ��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//������
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//��ͼ����
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	//VAO���
	//glBindVertexArray(0);	

	unsigned int LightVAO;
	glGenVertexArrays(1, &LightVAO);
	glBindVertexArray(LightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//����Ҫ�ٴζ�ȡ���ݣ������Ѿ�����VBO5����
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	unsigned int diffuseMap = loadTexture("container2.png");
	unsigned int specularMap = loadTexture("container2_specular.png");		//������ͼ
	//unsigned int specularMap = loadTexture("lighting_maps_specular_color.png");		//��ɫ������ͼ
	unsigned int emissionMap = loadTexture("matrix.jpg");		//�������ͼ

	colorShader.use();
	colorShader.SetInt("material.diffuse", 0);
	colorShader.SetInt("material.specular", 1);
	colorShader.SetInt("material.emissionMap", 2);

	while (!glfwWindowShouldClose(windowTr)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInputTr(windowTr);

		//glfwSetCursorPosCallback(windowTr, mouse_callback);
		glfwSetScrollCallback(windowTr, scroll_callback);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//�ù�Դ������
		//lightPos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
		//lightPos.y = cos(glfwGetTime() / 2.0f) * 1.0f;
		//lightPos.z = sin(glfwGetTime() / 2.0f) * 1.0f;

		colorShader.use();
		colorShader.setVec3("light.position", lightPos);
		colorShader.setVec3("viewPos", cam1.Position);	//��ʱ���������λ������ռ��еģ����Բ���Ҫ����ת��
		//colorShader.SetVec3("objectColor", objectColor.x, objectColor.y, objectColor.z);
		//colorShader.SetVec3("lightColor", lightColor.x, lightColor.y, lightColor.z);

		//colorShader.SetVec3("material.ambient", 0.0f, 0.1f, 0.06f);		
		//colorShader.SetVec3("material.diffuse", 0.0f, 0.50980392f, 0.50980392f);
		colorShader.SetVec3("material.specular", 0.50196078f, 0.50196078f, 0.50196078f);
		colorShader.SetFloat("material.shininess", 32.0f);

		colorShader.setVec3("light.ambient", ambientColor);
		colorShader.setVec3("light.diffuse", diffuseColor); // �����յ�����һЩ�Դ��䳡��
		colorShader.SetVec3("light.specular", 1.0f, 1.0f, 1.0f);

		//������ͼ�����ͶӰ����		
		glm::mat4 view1 = glm::mat4(1.0f);
		glm::mat4 projection1 = glm::mat4(1.0f);
		view1 = cam1.GetViewMatrix();
		projection1 = glm::perspective(glm::radians(cam1.Zoom), (float)(SRCT_WIDTH / SRCT_HEIGHT), 0.1f, 100.0f);

		colorShader.SetMat4("view", view1);
		colorShader.SetMat4("projection", projection1);

		glm::mat4 model1 = glm::mat4(1.0f);
		colorShader.SetMat4("model", model1);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, emissionMap);

		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		lightShader.use();
		lightShader.SetMat4("view", view1);
		lightShader.SetMat4("projection", projection1);
		lightShader.SetVec3("lightColor", lightColor.x, lightColor.y, lightColor.z);
		//lightShader.SetVec3("objectColor", 1.0, 1.0, 1.0);

		model1 = glm::mat4(1.0f);
		model1 = glm::translate(model1, lightPos);
		model1 = glm::scale(model1, glm::vec3(0.2f));
		lightShader.SetMat4("model", model1);
		glBindVertexArray(LightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		bool enable = (glfwGetKey(windowTr, GLFW_KEY_SPACE) == GLFW_PRESS);
		if (enable || isFirstEnter) {
			isFirstEnter = true;

			DrawGUI(windowTr);
		}

		glfwSwapBuffers(windowTr);
		glfwPollEvents();
	}

	my.DeleteGUI();

	colorShader.deleteProgram();

	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &LightVAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();

	return 0;
}

void framebuffer_size_callbackTr(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

//�����ڴ����е�����
void processInputTr(GLFWwindow* window) {
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

	//ws����ǰ������ƶ���ad������������ƶ�
	//float cameraSpeed = 0.05f;
	float cameraSpeed = deltaTime * 2.5f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cam1.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cam1.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cam1.ProcessKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cam1.ProcessKeyboard(RIGHT, deltaTime);
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse) {
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = -(ypos - lastY);
	lastX = (float)xpos;
	lastY = (float)ypos;

	cam1.ProcessMouseMovement(xoffset, yoffset, GL_TRUE);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	cam1.ProcessMouseScroll(yoffset);
}

void DrawGUI(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
		isFirstEnter = false;

	my.StartGUIFrame();

	my.Begin("Hello, world!");

	my.Text("This is some useful text.");               // Display some text (you can use a format strings too)
	my.DrawOneFloat("lightColor_x", &lightColor.x);
	my.DrawOneFloat("lightColor_y", &lightColor.y);
	my.DrawOneFloat("lightColor_z", &lightColor.z);

	my.DrawOneFloat("lightPos_x", &lightPos.x);
	my.DrawOneFloat("lightPos_y", &lightPos.y);
	my.DrawOneFloat("lightPos_z", &lightPos.z);

	my.DrawOneFloat("cameraPos_x", &cam1.Position.x);
	my.DrawOneFloat("cameraPos_y", &cam1.Position.y);
	my.DrawOneFloat("cameraPos_z", &cam1.Position.z);

	//my.DrawOneColor("objectColor", (float*)&objectColor);
	//my.DrawOneColor("lightColor", (float*)&lightColor);
	my.DrawOneColor("ambientColor", (float*)&ambientColor);
	my.DrawOneColor("diffuseColor", (float*)&diffuseColor);

	if (ImGui::Button("Close Tab"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		isFirstEnter = false;

	my.End();

	my.RenderGUI();
}


unsigned int loadTexture(char const* path) {
	unsigned int texture;
	//����Ҳ��ʹ��ID���õģ���һ�������������������������ڶ�����ID��(����������)	
	//glGenTextures(1, &texture);
	glGenTextures(1, &texture);	

	//���ز���������
	int width1, height1, nrChannels1;
	//����ͼ��ǰ����תͼ���y��
	stbi_set_flip_vertically_on_load(true);
	//����ͼƬ�����ȣ��߶ȣ���ɫͨ���ĸ���
	unsigned char* data = stbi_load(path, &width1, &height1, &nrChannels1, 0);

	if (data) {
		GLenum format;
		if (nrChannels1 == 1)
			format = GL_RED;
		else if (nrChannels1 == 3)
			format = GL_RGB;
		else if (nrChannels1 == 4)
			format = GL_RGBA;

		//���������Ա����������ָ�������в���
		glBindTexture(GL_TEXTURE_2D, texture);

		//ʹ��ǰ���ͼƬ��������������������glTexImage2Dʱ����ǰ�󶨵���������ͻᱻ����������ͼ��
		glTexImage2D(GL_TEXTURE_2D, 0, format, width1, height1, 0, format, GL_UNSIGNED_BYTE, data);

		//Ϊ��ǰ�󶨵������Զ�����������Ҫ�Ķ༶��Զ����
		glGenerateMipmap(GL_TEXTURE_2D);

		//Ϊ��ǰ�󶨵������������û��ơ����˷�ʽ
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
*/
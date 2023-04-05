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
#include "MyTexture.h"

Camera cam1;

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);		//全局变量，光源的位置
glm::vec3 lightDir(-0.2f, -1.0f, -0.3f);

unsigned int loadTexture(char const* path);
void framebuffer_size_callbackTr(GLFWwindow* window, int width, int height);
void processInputTr(GLFWwindow* window);
void DrawGUI(GLFWwindow* window);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

const unsigned int SRCT_WIDTH = 800;
const unsigned int SRCT_HEIGHT = 600;
float mixValue = 0.2f;

float deltaTime = 0.0f;	//当前帧与上一帧的时间差
float lastFrame = 0.0f;	//上一帧的时间
//鼠标的初始位置（屏幕中心）
float lastX = 400, lastY = 300;
//yaw绕x轴上下俯仰，pitch绕y轴左右偏移，这里的取值是角度
float yaw, pitch;

//是不是第一次进入这个窗口
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

	//绑定我们创建的窗口到当前上下文
	glfwMakeContextCurrent(windowTr);
	glfwSwapInterval(1);

	my.SetWindow(windowTr);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initiate GLAD" << std::endl;
		return -1;
	}

	//glfwSetInputMode(windowTr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	Shader colorShader("light4_LightMap.vs.txt", "light4_LightCast_Dir.fs.txt");
	Shader lightShader("light1.vs.txt", "light1_light.fs.txt");

	glViewport(0, 0, SRCT_WIDTH, SRCT_HEIGHT);
	glfwSetFramebufferSizeCallback(windowTr, framebuffer_size_callbackTr);

	//前三个是顶点坐标信息，中间三个是顶点对应的法向量信息，后面两个是uv坐标
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

	glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	unsigned int VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);
	//顶点位置
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//法向量
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//贴图坐标
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	//VAO解绑
	//glBindVertexArray(0);	

	unsigned int LightVAO;
	glGenVertexArrays(1, &LightVAO);
	glBindVertexArray(LightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//不需要再次读取数据，数据已经存在VBO5中了
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	MyTexture te1("container2.png");
	MyTexture te2("container2_specular.png");

	unsigned int diffuseMap = te1.textureID;
	unsigned int specularMap = te2.textureID;		//镜面贴图

	colorShader.use();
	colorShader.SetInt("material.diffuse", 0);
	colorShader.SetInt("material.specular", 1);
	colorShader.SetFloat("light.constant", 1.0f);
	colorShader.SetFloat("light.linear", 0.09f);
	colorShader.SetFloat("light.quadratic", 0.032f);

	while (!glfwWindowShouldClose(windowTr)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInputTr(windowTr);

		glfwSetCursorPosCallback(windowTr, mouse_callback);
		glfwSetScrollCallback(windowTr, scroll_callback);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		colorShader.use();		
		//colorShader.setVec3("light.position", lightPos);		//光源方块作为手电筒的起点
		colorShader.SetVec3("light.position", cam1.Position.x, cam1.Position.y, cam1.Position.z);	//摄像机位置作为手电筒的起点
		//colorShader.setVec3("light.direction", lightDir);
		colorShader.setVec3("light.direction", cam1.Front);
		colorShader.SetFloat("light.cutOff", glm::cos(glm::radians(12.5f)));		//传入cos值，直接与LightDir和SpotDir向量的点积结果cos值进行比较，就不需要求反余弦值，是个开销很大的运算
		colorShader.SetFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));
		colorShader.setVec3("viewPos", cam1.Position);	//此时的摄像机是位于世界空间中的，所以不需要进行转换

		colorShader.SetVec3("material.specular", 0.50196078f, 0.50196078f, 0.50196078f);
		colorShader.SetFloat("material.shininess", 32.0f);

		colorShader.setVec3("light.ambient", ambientColor);
		colorShader.setVec3("light.diffuse", diffuseColor); // 将光照调暗了一些以搭配场景
		colorShader.SetVec3("light.specular", 1.0f, 1.0f, 1.0f);

		//设置视图矩阵和投影矩阵		
		glm::mat4 view1 = glm::mat4(1.0f);
		glm::mat4 projection1 = glm::mat4(1.0f);
		view1 = cam1.GetViewMatrix();
		projection1 = glm::perspective(glm::radians(cam1.Zoom), (float)(SRCT_WIDTH / SRCT_HEIGHT), 0.1f, 100.0f);

		colorShader.SetMat4("view", view1);
		colorShader.SetMat4("projection", projection1);

		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, diffuseMap);
		te1.use(diffuseMap,0);
		te2.use(specularMap,1);
		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, specularMap);

		glBindVertexArray(cubeVAO);
		
		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model1 = glm::mat4(1.0f);
			model1 = glm::translate(model1, cubePositions[i]);
			float angle = 20.0f * i;
			model1 = glm::rotate(model1, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			colorShader.SetMat4("model", model1);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}				

		lightShader.use();
		lightShader.SetMat4("view", view1);
		lightShader.SetMat4("projection", projection1);
		lightShader.SetVec3("lightColor", lightColor.x, lightColor.y, lightColor.z);
		//lightShader.SetVec3("objectColor", 1.0, 1.0, 1.0);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		lightShader.SetMat4("model", model);
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

//处理在窗口中的输入
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

	//ws是向前和向后移动，ad是向左和向右移动
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
	//my.SameLine();

	my.DrawOneFloat("lightPos_x", &lightPos.x);
	my.DrawOneFloat("lightPos_y", &lightPos.y);
	my.DrawOneFloat("lightPos_z", &lightPos.z);
	//my.SameLine();

	my.DrawOneFloat("lightDir_x", &lightDir.x);
	my.DrawOneFloat("lightDir_y", &lightDir.y);
	my.DrawOneFloat("lightDir_z", &lightDir.z);
	//my.SameLine();

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


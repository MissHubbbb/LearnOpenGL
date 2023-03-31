//使用该脚本前需要先注释Light_1_light.cpp脚本

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "Shader.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "MYGUICLASS.h"

Camera cam1;

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);		//全局变量，光源的位置

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

// Our state
bool show_demo_window = true;
bool show_another_window = false;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

ImVec4 objectColor = ImVec4(1.0f, 0.5f, 0.31f, 1.0f);
ImVec4 lightColor = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);

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


	//// Setup Dear ImGui context
	//IMGUI_CHECKVERSION();
	//ImGui::CreateContext();
	//ImGuiIO& io = ImGui::GetIO(); (void)io;

	//// Setup Dear ImGui style
	//ImGui::StyleColorsDark();

	//// Setup Platform/Renderer backends
	//ImGui_ImplGlfw_InitForOpenGL(windowTr, true);
	//ImGui_ImplOpenGL3_Init(glsl_version);	

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initiate GLAD" << std::endl;
		return -1;
	}

	//glfwSetInputMode(windowTr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	Shader colorShader("light2_diffuseLight.vs.txt", "light2_diffuseLight.fs.txt");
	Shader lightShader("light1.vs.txt", "light1_light.fs.txt");

	glViewport(0, 0, SRCT_WIDTH, SRCT_HEIGHT);
	glfwSetFramebufferSizeCallback(windowTr, framebuffer_size_callbackTr);

	//前三个是顶点坐标信息，后三个是顶点对应的法向量信息
	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	unsigned int VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);
	//顶点位置
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//法向量
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//VAO解绑
	//glBindVertexArray(0);


	unsigned int LightVAO;
	glGenVertexArrays(1, &LightVAO);
	glBindVertexArray(LightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//不需要再次读取数据，数据已经存在VBO5中了
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);	

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

		//让光源动起来
		/*lightPos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
		lightPos.y = cos(glfwGetTime() / 2.0f) * 1.0f;
		lightPos.z = sin(glfwGetTime() / 2.0f) * 1.0f;*/

		colorShader.use();
		colorShader.setVec3("lightPos", lightPos);
		colorShader.setVec3("viewPos", cam1.Position);	//此时的摄像机是位于世界空间中的，所以不需要进行转换
		colorShader.SetVec3("objectColor", objectColor.x, objectColor.y, objectColor.z);
		colorShader.SetVec3("lightColor", lightColor.x, lightColor.y, lightColor.z);
		//设置视图矩阵和投影矩阵		
		glm::mat4 view1 = glm::mat4(1.0f);
		glm::mat4 projection1 = glm::mat4(1.0f);
		view1 = cam1.GetViewMatrix();
		projection1 = glm::perspective(glm::radians(cam1.Zoom), (float)(SRCT_WIDTH / SRCT_HEIGHT), 0.1f, 100.0f);

		colorShader.SetMat4("view", view1);
		colorShader.SetMat4("projection", projection1);

		glm::mat4 model1 = glm::mat4(1.0f);
		colorShader.SetMat4("model", model1);
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		lightShader.use();
		lightShader.SetMat4("view", view1);
		lightShader.SetMat4("projection", projection1);

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

	//// Cleanup
	//ImGui_ImplOpenGL3_Shutdown();
	//ImGui_ImplGlfw_Shutdown();
	//ImGui::DestroyContext();
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
	my.DrawOneFloat("lightPos_x", &lightPos.x);
	my.DrawOneFloat("lightPos_y", &lightPos.y);
	my.DrawOneFloat("lightPos_z", &lightPos.z);

	my.DrawOneColor("objectColor", (float*)&objectColor);
	my.DrawOneColor("lightColor", (float*)&lightColor);

	if (ImGui::Button("Close Tab"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		isFirstEnter = false;

	my.End();

	my.RenderGUI();
}
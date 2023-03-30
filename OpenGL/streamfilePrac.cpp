#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"

void framebuffer_size_callback2(GLFWwindow* window, int width, int height);
void processInput2(GLFWwindow* window);

const unsigned int SRC_WIDTH1 = 800;
const unsigned int SRC_HEIGHT1 = 600;

int main3(void) {
	//初始化
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window2 = glfwCreateWindow(SRC_WIDTH1, SRC_HEIGHT1, "Practice1", NULL, NULL);
	if (!window2) {
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}

	//绑定我们创建的窗口到当前上下文
	glfwMakeContextCurrent(window2);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initiate GLAD" << std::endl;
		return -1;
	}

	Shader ourShader("shaderColorEquPos.vs.txt", "shaderColorEquPos.fs.txt");
	

	float vertices1[] = {
		// 位置              // 颜色
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
	};

	unsigned int VBOF, VAOF;
	glGenBuffers(1, &VBOF);
	glGenVertexArrays(1, &VAOF);

	glBindVertexArray(VAOF);

	glBindBuffer(GL_ARRAY_BUFFER, VBOF);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);	//解绑VAO

	while (!glfwWindowShouldClose(window2)) {
		processInput2(window2);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		float offset = 0.5f;
		ourShader.SetFloat("xOffset", offset);
		ourShader.use();
		glBindVertexArray(VAOF);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(0);

		glfwSwapBuffers(window2);
	}

	glDeleteVertexArrays(1, &VAOF);
	glDeleteBuffers(1, &VBOF);
	glfwTerminate();

	return 0;
}

//调整窗口大小的回调函数
void framebuffer_size_callback2(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

//处理在窗口中的输入
void processInput2(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}
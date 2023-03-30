
//ע�⣡���������ó�����entry_6_texture�ļ����ܹ��棬����ǰӦ��ɾ��entry_6_texture������
/*
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "Shader.h"

glm::mat4 LookAtInCam(glm::vec3 camPos, glm::vec3 targetPos, glm::vec3 worldUp);

const unsigned int SRCT_WIDTH = 800;
const unsigned int SRCT_HEIGHT = 600;
float mixValue = 0.2f;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float deltaTime = 0.0f;	//��ǰ֡����һ֡��ʱ���
float lastFrame = 0.0f;	//��һ֡��ʱ��

//���ĳ�ʼλ�ã���Ļ���ģ�
float lastX = 400, lastY = 300;

//yaw��x�����¸�����pitch��y������ƫ�ƣ������ȡֵ�ǽǶ�
float yaw, pitch;

//�ǲ��ǵ�һ�ν����������
bool firstMouse = true;

//��Ұfield of view
//float fov = 45.0f;

Camera cam1;

void framebuffer_size_callbackTr(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = -(ypos - lastY);
	lastX = xpos;
	lastY = ypos;

	cam1.ProcessMouseMovement(xoffset,yoffset,GL_TRUE);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	cam1.ProcessMouseScroll(yoffset);
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
		//cameraPos += cameraSpeed * cameraFront;
		cam1.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		//cameraPos -= cameraSpeed * cameraFront;
		cam1.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		//cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		cam1.ProcessKeyboard(LEFT,deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		//cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		cam1.ProcessKeyboard(RIGHT, deltaTime);
	}
}

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

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initiate GLAD" << std::endl;
		return -1;
	}

	glfwSetInputMode(windowTr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);	
	
	Shader shader1("transform.vs.txt", "transform.fs.txt");

	glViewport(0, 0, SRCT_WIDTH, SRCT_HEIGHT);
	glfwSetFramebufferSizeCallback(windowTr, framebuffer_size_callbackTr);

	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	//ʮ��������������ռ��е�λ��
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

	//unsigned int indices[] = {
	//	0, 1, 3, // first triangle
	//	1, 2, 3  // second triangle
	//};

	unsigned int VBO5, VAO5;
	glGenBuffers(1, &VBO5);
	glGenVertexArrays(1, &VAO5);	

	glBindVertexArray(VAO5);

	glBindBuffer(GL_ARRAY_BUFFER, VBO5);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);	

	//����λ��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//������������
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//VAO���
	glBindVertexArray(0);

	//--------------------------����ͼƬ����������ͼ
	//unsigned int texture;
	unsigned int texture11, texture22;
	//����Ҳ��ʹ��ID���õģ���һ������������������������ڶ�����ID��(����������)	
	//glGenTextures(1, &texture);
	glGenTextures(1, &texture11);
	//�������Ա����������ָ�������в���
	//glBindTexture(GL_TEXTURE_2D, texture);
	glBindTexture(GL_TEXTURE_2D, texture11);

	//Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//���ز���������
	int width1, height1, nrChannels1;
	//����ͼ��ǰ����תͼ���y��
	stbi_set_flip_vertically_on_load(true);
	//����ͼƬ����ȣ��߶ȣ���ɫͨ���ĸ���
	unsigned char* data = stbi_load("container.jpg", &width1, &height1, &nrChannels1, 0);

	if (data) {
		//ʹ��ǰ���ͼƬ������������������glTexImage2Dʱ����ǰ�󶨵��������ͻᱻ����������ͼ��
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		//Ϊ��ǰ�󶨵������Զ�����������Ҫ�Ķ༶��Զ����
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture1" << std::endl;
	}

	//�ͷ��ڴ�
	stbi_image_free(data);

	glGenTextures(1, &texture22);
	glBindTexture(GL_TEXTURE_2D, texture22);

	//Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	data = stbi_load("awesomeface.png", &width1, &height1, &nrChannels1, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width1, height1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture2" << std::endl;
	}

	stbi_image_free(data);

	//glUseProgram(shaderProgramT);
	shader1.use();
	//glUniform1i(glGetUniformLocation(shader1.ID, "texture1"), 0);	//���������λ��ֵ
	//glUniform1i(glGetUniformLocation(shader1.ID, "texture2"), 1);
	//glUniform1f(glGetUniformLocation(shader1.ID, "mixValue"), mixValue);

	shader1.SetInt("texture1", 0);
	shader1.SetInt("texture2", 1);
	shader1.SetFloat("mixValue", mixValue);	

	float camX = cam1.Position.x, camZ = cam1.Position.z;

	while (!glfwWindowShouldClose(windowTr)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwSetCursorPosCallback(windowTr, mouse_callback);
		glfwSetScrollCallback(windowTr, scroll_callback);

		processInputTr(windowTr);
		//upAnddown(windowT, shaderProgramT);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glBindTexture(GL_TEXTURE_2D, texture);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture11);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture22);

		shader1.use();

		shader1.SetFloat("mixValue", mixValue);
		//glUniform1f(glGetUniformLocation(shader1.ID, "mixValue"), mixValue);
		//glUseProgram(shader1.ID);		

		//������ͼ�����ͶӰ����		
		glm::mat4 view1 = glm::mat4(1.0f);
		glm::mat4 projection1 = glm::mat4(1.0f);
		//	�ڶ���������ʾ��ת���ȣ�������������ʾ������������������ת
		//model1 = glm::rotate(model1, (float)glfwGetTime() * glm::radians(-50.0f), glm::vec3(0.5f, 1.0f, 0.0f));		
		
		//view1 = glm::translate(view1, glm::vec3(0.0f, 0.0f, -3.0f));	//�����ƶ����ǳ��������������
		//view1 = glm::translate(view1, glm::vec3(0.0f, -0.5f, -3.0f));		//������ǵߵ���Ч��

		//������LookAt������ʵ��ԭ��
		////���λ��
		//glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		////ָ��Ŀ�꣨Ҳ�����������ķ�����z�Ḻ����
		//glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
		//glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
		////����
		//glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		//glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
		////����
		//glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

		
		//view1 = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		//view1 = cam1.GetViewMatrix();
		
		view1 = LookAtInCam(cam1.Position,cam1.Position + cam1.Front,cam1.WorldUp);
		//view1 = glm::lookAt(cameraPos, cameraFront - cameraPos, cameraUp);		//����ʲôʱ�򶼻ῴ��cameraFront

		//projection1 = glm::perspective(glm::radians(45.0f), (float)(width1 / height1), 0.1f, 100.0f);
		projection1 = glm::perspective(glm::radians(cam1.Zoom), (float)(width1 / height1), 0.1f, 100.0f);

		//unsigned int modelLoc1 = glGetUniformLocation(shader1.ID, "model");
		////�ڶ��������Ǹ���opengl���ǽ�Ҫ���Ͷ��ٸ����󣬵��������������Ƿ�Ҫ�Ծ������ת�ã����һ�������������ľ������ݣ�����GLM���Դ��ĺ���value_ptr���任��Щ����ΪOpenGL�ɽ��ܵĸ�ʽ
		////OpenGL������ͨ��ʹ��һ���ڲ����󲼾֣�����������(Column-major Ordering)���֡�GLM��Ĭ�ϲ��־������������Բ�����Ҫת�þ���������GL_FALSE
		//glUniformMatrix4fv(modelLoc1, 1, GL_FALSE, glm::value_ptr(model1));

		//unsigned int viewLoc1 = glGetUniformLocation(shader1.ID, "view");
		//glUniformMatrix4fv(viewLoc1, 1, GL_FALSE, glm::value_ptr(view1));
		shader1.SetMat4("view", view1);

		//unsigned int projectionLoc1 = glGetUniformLocation(shader1.ID, "projection");
		//glUniformMatrix4fv(projectionLoc1, 1, GL_FALSE, glm::value_ptr(projection1));
		shader1.SetMat4("projection", projection1);

		glBindVertexArray(VAO5);
		for (int i = 0; i < 10; i++) {
			glm::mat4 model1 = glm::mat4(1.0f);
			model1 = glm::translate(model1, cubePositions[i]);
			float angle = i * 20.0f;
			if (i % 3 == 0 || i == 0)
				angle = glfwGetTime() * 25.0f;							

			model1 = glm::rotate(model1, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			shader1.SetMat4("model", model1);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}						

		glfwSwapBuffers(windowTr);
		glfwPollEvents();
	}
	//glDeleteProgram(shaderProgramT);
	shader1.deleteProgram();

	glDeleteVertexArrays(1, &VAO5);
	glDeleteBuffers(1, &VBO5);	

	glfwTerminate();

	return 0;
}

glm::mat4 LookAtInCam(glm::vec3 camPos, glm::vec3 targetPos, glm::vec3 worldUp) {
	glm::vec3 zaxis = glm::normalize(camPos - targetPos);
	// 3. Get positive right axis vector
	glm::vec3 xaxis = glm::normalize(glm::cross(glm::normalize(worldUp), zaxis));
	// 4. Calculate camera up vector
	glm::vec3 yaxis = glm::cross(zaxis, xaxis);

	// Create translation and rotation matrix
	// In glm we access elements as mat[col][row] due to column-major layout
	glm::mat4 translation = glm::mat4(1.0f); // Identity matrix by default
	translation[3][0] = -camPos.x; // Third column, first row
	translation[3][1] = -camPos.y;
	translation[3][2] = -camPos.z;
	glm::mat4 rotation = glm::mat4(1.0f);
	rotation[0][0] = xaxis.x; // First column, first row
	rotation[1][0] = xaxis.y;
	rotation[2][0] = xaxis.z;
	rotation[0][1] = yaxis.x; // First column, second row
	rotation[1][1] = yaxis.y;
	rotation[2][1] = yaxis.z;
	rotation[0][2] = zaxis.x; // First column, third row
	rotation[1][2] = zaxis.y;
	rotation[2][2] = zaxis.z;

	// Return lookAt matrix as combination of translation and rotation matrix
	return rotation * translation;
}
*/
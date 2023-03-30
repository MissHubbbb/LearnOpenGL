#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback1(GLFWwindow* window, int width, int height);
void processInput1(GLFWwindow* window);

const unsigned int SRC_WIDTH = 800;
const unsigned int SRC_HEIGHT = 600;

const char* vertexshadersource1 = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main() {\n"
"gl_Position = vec4(aPos.x,aPos.y,aPos.z,1.0f);\n"
"}\n\0";

const char* vertexshadersourceN = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n"
"void main() {\n"
"gl_Position = vec4(aPos,1.0f);\n"
"ourColor = aColor;\n"
"}\n\0";

const char* fragmentshadersource1 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main() {\n"
"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

const char* fragmentshadersource2 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main() {\n"
"FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
"}\n\0";

const char* fragmentshadersourceU = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 ourColor;\n"
"void main() {\n"
"FragColor = ourColor;\n"
"}\n\0";

const char* fragmentshadersourceN = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"void main() {\n"
"FragColor = vec4(ourColor,1.0f);\n"
"}\n\0";

int main2(void) {
	//��ʼ��
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window1 = glfwCreateWindow(SRC_WIDTH, SRC_HEIGHT, "Practice1", NULL, NULL);
	if (!window1) {
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}

	//�����Ǵ����Ĵ��ڵ���ǰ������
	glfwMakeContextCurrent(window1);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initiate GLAD" << std::endl;
		return -1;
	}

	//����������ɫ��
	//unsigned int vertexShader;
	unsigned int vertexShaderN;
	//vertexShader = glCreateShader(GL_VERTEX_SHADER);
	vertexShaderN = glCreateShader(GL_VERTEX_SHADER);

	//������ɫ��Դ�룬������
	glShaderSource(vertexShaderN, 1, &vertexshadersourceN, NULL);
	glCompileShader(vertexShaderN);

	int success;
	char infoLog[512];

	glad_glGetShaderiv(vertexShaderN, GL_COMPILE_STATUS, &success);

	if (!success) {
		std::cout << "Failed to create vertex shader" << std::endl;
		glGetShaderInfoLog(vertexShaderN, 512, NULL, infoLog);
	}

	//����Ƭ����ɫ��
	//unsigned int fragShader,fragShader1, fragShaderU;
	unsigned int fragShaderN;
	/*fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	fragShader1 = glCreateShader(GL_FRAGMENT_SHADER);
	fragShaderU = glCreateShader(GL_FRAGMENT_SHADER);*/
	fragShaderN = glCreateShader(GL_FRAGMENT_SHADER);

	/*glShaderSource(fragShader, 1, &fragmentshadersource1, NULL);
	glShaderSource(fragShader1, 1, &fragmentshadersource2, NULL);
	glShaderSource(fragShaderU, 1, &fragmentshadersourceU, NULL);*/
	glShaderSource(fragShaderN, 1, &fragmentshadersourceN, NULL);


	/*glCompileShader(fragShader);
	glCompileShader(fragShader1);
	glCompileShader(fragShaderU);*/
	glCompileShader(fragShaderN);

	int success1;
	char infoLog1[512];

	glad_glGetShaderiv(fragShaderN, GL_COMPILE_STATUS, &success1);

	if (!success1) {
		std::cout << "Failed to create fragment shader1" << std::endl;
		glGetShaderInfoLog(fragShaderN, 512, NULL, infoLog1);
	}

	/*int success11;
	char infoLog11[512];

	glad_glGetShaderiv(fragShader1, GL_COMPILE_STATUS, &success11);

	if (!success11) {
		std::cout << "Failed to create fragment shader2" << std::endl;
		glGetShaderInfoLog(fragShader1, 512, NULL, infoLog11);
	}

	int successU;
	char infoLogU[512];

	glad_glGetShaderiv(fragShaderU, GL_COMPILE_STATUS, &successU);

	if (!success11) {
		std::cout << "Failed to create fragment shaderU" << std::endl;
		glGetShaderInfoLog(fragShaderU, 512, NULL, infoLogU);
	}*/

	//������ɫ������
	//unsigned int shaderProgram,shaderProgram1, shaderProgramU;
	unsigned int shaderProgramN;
	shaderProgramN = glCreateProgram();
	/*shaderProgram1 = glCreateProgram();
	shaderProgramU = glCreateProgram();*/

	glAttachShader(shaderProgramN, vertexShaderN);
	glAttachShader(shaderProgramN, fragShaderN);
	glLinkProgram(shaderProgramN);	

	/*glAttachShader(shaderProgram1, vertexShader);
	glAttachShader(shaderProgram1, fragShader1);
	glLinkProgram(shaderProgram1);

	glAttachShader(shaderProgramU, vertexShader);
	glAttachShader(shaderProgramU, fragShaderU);
	glLinkProgram(shaderProgramU);*/

	//int success2,success22, successU1;
	int success2;
	//char infoLog2[512], infoLog22[512], infoLogU1[512];
	char infoLog2[512];

	glad_glGetProgramiv(shaderProgramN, GL_LINK_STATUS, &success2);
	/*glad_glGetProgramiv(shaderProgram1, GL_LINK_STATUS, &success22);
	glad_glGetProgramiv(shaderProgramU, GL_LINK_STATUS, &successU1);*/

	if (!success2) {
		std::cout << "Failed to link the shader program1" << std::endl;
		glGetProgramInfoLog(shaderProgramN, 512, NULL, infoLog2);
	}

	/*if (!success22) {
		std::cout << "Failed to link the shader program2" << std::endl;
		glGetProgramInfoLog(shaderProgram1, 512, NULL, infoLog22);
	}

	if (!successU1) {
		std::cout << "Failed to link the shader programU" << std::endl;
		glGetProgramInfoLog(shaderProgramU, 512, NULL, infoLogU1);
	}*/

	//��ɫ�����򴴽��ɹ�������Ҳ�������������ɫ������ɾ����������ɫ��
	glDeleteShader(vertexShaderN);
	glDeleteShader(fragShaderN);
	/*glDeleteShader(fragShader1);
	glDeleteShader(fragShaderU);*/

	//�����ӿ�
	glViewport(0, 0, SRC_WIDTH, SRC_HEIGHT);

	//���õ������ڹ��Ļص�����
	glfwSetFramebufferSizeCallback(window1, framebuffer_size_callback1);	

#pragma region 1.��Ӹ��ඥ�㵽�����У�ʹ��glDrawArrays�����������˴�������������
	/*
	* float vertices[] = {
		// first triangle
	   -0.9f, -0.5f, 0.0f,  // left 
	   -0.0f, -0.5f, 0.0f,  // right
	   -0.45f, 0.5f, 0.0f,  // top 
	   // second triangle
		0.0f, -0.5f, 0.0f,  // left
		0.9f, -0.5f, 0.0f,  // right
		0.45f, 0.5f, 0.0f   // top 
	};

	//����VBO(���㻺�����),VAO(�����������)��EBO(Ԫ�ػ������)
	unsigned int VBO, VAO, EBO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);

	//��VAO����ʾ֮���VBO�Ľ�������洢��VAO��
	glBindVertexArray(VAO);

	//��һ��ʼ����õ����ݸ��Ƶ�VBO��
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//ʹ��buffer�е�������VAO������0�Ŷ����ָ��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);

	//����ָ��ĵ�0������
	glEnableVertexAttribArray(0);

	//���VAO
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window1)) {
		processInput1(window1);

		//����glClear�����������Ļ����ɫ���壬������glClearColor�����������Ļ���õ���ɫ��
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//��Ⱦ����...
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);

		//����������
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//���VAO
		glBindVertexArray(0);

		//˫���彻��
		glfwSwapBuffers(window1);
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);	
	glDeleteProgram(shaderProgram);
	*/
#pragma endregion

#pragma region 2.������ͬ�����������Σ��������ǵ�����ʹ�ò�ͬ��VAO��VBO
	/*
	float firstTriangle[] = {
	   -0.9f, -0.5f, 0.0f,  // left 
	   -0.0f, -0.5f, 0.0f,  // right
	   -0.45f, 0.5f, 0.0f,  // top 
	};
	float secondTriangle[] = {
		0.0f, -0.5f, 0.0f,  // left
		0.9f, -0.5f, 0.0f,  // right
		0.45f, 0.5f, 0.0f   // top 
	};

	unsigned int VBOs[2], VAOs[2];
	glGenBuffers(2, VBOs);
	glGenVertexArrays(2, VAOs);

	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);		//�����ĵ�0������
	glBindVertexArray(VAOs[0]);

	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);		//�����ĵ�0������
	glBindVertexArray(VAOs[1]);

	while (!glfwWindowShouldClose(window1)) {
		processInput1(window1);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//��Ⱦ����
		glUseProgram(shaderProgram);
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		glfwSwapBuffers(window1);
	}

	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);
	glDeleteProgram(shaderProgram);
	*/
#pragma endregion

#pragma region 3.����������ɫ�����򣬵ڶ�������ʹ��һ����ͬ��Ƭ����ɫ���������ɫ
/*
	float firstTriangle[] = {
	   -0.9f, -0.5f, 0.0f,  // left 
	   -0.0f, -0.5f, 0.0f,  // right
	   -0.45f, 0.5f, 0.0f,  // top 
	};
	float secondTriangle[] = {
		0.0f, -0.5f, 0.0f,  // left
		0.9f, -0.5f, 0.0f,  // right
		0.45f, 0.5f, 0.0f   // top 
	};

	unsigned int VBOs[2], VAOs[2];
	glGenBuffers(2, VBOs);
	glGenVertexArrays(2, VAOs);

	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);		//�����ĵ�0������
	glBindVertexArray(VAOs[0]);

	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);		//�����ĵ�0������
	glBindVertexArray(VAOs[1]);

	while (!glfwWindowShouldClose(window1)) {
		processInput1(window1);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//��Ⱦ����
		glUseProgram(shaderProgram);
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		glUseProgram(shaderProgram1);
		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		glfwSwapBuffers(window1);
	}

	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);
	glDeleteProgram(shaderProgram);
	glDeleteProgram(shaderProgram1);
	*/
#pragma endregion

#pragma region ����uniform��ֵ
/*
float vertices[] = {
		-0.5f,-0.5f,0.0f,
		0.5f,-0.5f,0.0f,
		0.0f,0.5f,0.0f
};

	//����VBO(���㻺�����),VAO(�����������)��EBO(Ԫ�ػ������)
unsigned int VBO, VAO, EBO;
glGenBuffers(1, &VBO);
glGenVertexArrays(1, &VAO);
glGenBuffers(1, &EBO);

//��VAO����ʾ֮���VBO�Ľ�������洢��VAO��
glBindVertexArray(VAO);

//��һ��ʼ����õ����ݸ��Ƶ�VBO��
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

//ʹ��buffer�е�������VAO������0�Ŷ����ָ��
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);

//����ָ��ĵ�0������
glEnableVertexAttribArray(0);

//���VAO
glBindVertexArray(0);

while (!glfwWindowShouldClose(window1)) {
	processInput1(window1);

	//����glClear�����������Ļ����ɫ���壬������glClearColor�����������Ļ���õ���ɫ��
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//��Ⱦ����...
	glUseProgram(shaderProgramU);
	glBindVertexArray(VAO);

	//����uniform��ֵҪ��������ɫ������󣬻���ͼ��֮ǰ
	double timeValue = glfwGetTime();
	float greenValue = sin(timeValue) / 2.0f + 0.5f;
	int vertexColorLocation = glGetUniformLocation(shaderProgramU, "ourColor");
	glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

	//����������
	glDrawArrays(GL_TRIANGLES, 0, 3);

	//���VAO
	glBindVertexArray(0);

	//˫���彻��
	glfwSwapBuffers(window1);
}
glDeleteVertexArrays(1, &VAO);
glDeleteBuffers(1, &VBO);
glDeleteProgram(shaderProgram);
*/
#pragma endregion

#pragma region ���ö����λ�ú���ɫ����
	float vertices[] = {
		// λ��              // ��ɫ
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // ����
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // ����
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // ����
	};

	unsigned int VBO, VAO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);	//���VAO

	while (!glfwWindowShouldClose(window1)) {
		processInput1(window1);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(VAO);
		glUseProgram(shaderProgramN);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(0);

		glfwSwapBuffers(window1);
	}

#pragma endregion


	glfwTerminate();
	return 0;

}



//�������ڴ�С�Ļص�����
void framebuffer_size_callback1(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

//�����ڴ����е�����
void processInput1(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}
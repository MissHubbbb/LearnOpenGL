#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

//#pragma comment(lib,"glew32.lib")

void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char* vertexshadersource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main() {\n"
"gl_Position = vec4(aPos.x,aPos.y,aPos.z,1.0f);\n"
"}\0";

const char* fragmentshadersource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main() {\n"
"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

int main1(void)
{
    //��ʼ��GLFW
    glfwInit();

    //����GLFW����һ������Ϊѡ�������(���ԴӺܶ���GLFW_��ͷ��ö��ֵ��ѡ��)���ڶ��������������ø�ѡ���ֵ
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  //���汾��
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);  //�ΰ汾��
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  //����GLFW����ʹ�õ��Ǻ���ģʽ(Core-profile)

    //����һ�����ڶ���������ڶ����������кʹ�����ص����ݣ����һᱻGLFW����������Ƶ�����õ�
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    //�����괰�����ǾͿ���֪ͨGLFW�����Ǵ��ڵ�����������Ϊ��ǰ�̵߳���������
    glfwMakeContextCurrent(window);

    //GLAD����������OpenGL�ĺ���ָ��ģ������ڵ����κ�OpenGL�ĺ���֮ǰ������Ҫ��ʼ��GLAD
    //��GLAD��������������ϵͳ��ص�OpenGL����ָ���ַ�ĺ���
    //GLFW�����ǵ���glfwGetProcAddress�����������Ǳ����ϵͳ��������ȷ�ĺ���
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //����������ɫ������
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);  

    //����ɫ��Դ�븽�ӵ���ɫ����,Ȼ�������
    glShaderSource(vertexShader, 1, &vertexshadersource, NULL);
    glCompileShader(vertexShader);

    int  successV;
    char infoLog[512];
    //����Ƿ����ɹ�
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &successV);

    if (!successV) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //����Ƭ����ɫ������
    unsigned int fragShader;
    fragShader = glCreateShader(GL_FRAGMENT_SHADER);    

    glShaderSource(fragShader, 1, &fragmentshadersource, NULL);
    glCompileShader(fragShader);
    
    int successF;
    char infoLogF[512];
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &successF);

    if (!successF) {
        glGetShaderInfoLog(fragShader, 512, NULL, infoLogF);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLogF << std::endl;
    }

    //������ɫ��������ɫ����Ҫ���ӵ���������ϣ�Ȼ������
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragShader);
    glLinkProgram(shaderProgram);

    int successP;
    char infoLogP[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &successP);

    if (!successP) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLogP);
        std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLogF << std::endl;
    }   

    //�ڳ��������������ɫ����ǰ���£��Ϳ���ɾ����������ɫ��
    glDeleteShader(vertexShader);
    glDeleteShader(fragShader);

    //glViewport����ǰ�����������ƴ������½ǵ�λ�á��������͵��ĸ�����������Ⱦ���ڵĿ�Ⱥ͸߶ȣ����أ�
    glViewport(0, 0, 800, 600);

    //ע�ᴰ�ڵ����ص�����������GLFW����ϣ��ÿ�����ڵ�����С��ʱ������������
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

   /* float vertices[] = {
        -0.5f,-0.5f,0.0f,
        0.5f,-0.5f,0.0f,
        0.0f,0.5f,0.0f
    };*/

    //float vertices[] = {
    //    // ��һ��������
    //    0.5f, 0.5f, 0.0f,   // ���Ͻ�
    //    0.5f, -0.5f, 0.0f,  // ���½�
    //    -0.5f, 0.5f, 0.0f,  // ���Ͻ�
    //    // �ڶ���������
    //    0.5f, -0.5f, 0.0f,  // ���½�
    //    -0.5f, -0.5f, 0.0f, // ���½�
    //    -0.5f, 0.5f, 0.0f   // ���Ͻ�
    //};

    float vertices[] = {
    0.5f, 0.5f, 0.0f,   // ���Ͻ�
    0.5f, -0.5f, 0.0f,  // ���½�
    -0.5f, -0.5f, 0.0f, // ���½�
    -0.5f, 0.5f, 0.0f   // ���Ͻ�
    };

    unsigned int indices[] = {
        //������0��ʼ
        //��������ֱ�ʾ����vertices���±�
        //�����������±깹�ɾ���
        0,1,3,
        1,2,3
    };

    //ʹ�ú����ͻ���ID����һ��VBO(���㻺�����)
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    //���������������VAO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    //����Ԫ��(����)�������
    unsigned int EBO;
    glGenBuffers(1, &EBO);

    //��VAO
    glBindVertexArray(VAO);

    //�Ѵ����Ļ���VBO�󶨵�GL_ARRAY_BUFFERĿ����
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    //��֮ǰ����õĶ������ݸ��Ƶ���ǰ�󶨻���
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //�Ѵ����Ļ���EBO�󶨵�GL_ELEMENT_ARRAY_BUFFERĿ����
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //��֮ǰ����õ��������ݸ��Ƶ���ǰ�󶨻���
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //���ö�������ָ��
    //����OpenGL����ν����������ݣ�Ӧ�õ��������������
    //ʹ��buffer�е�������VAO����0�Ŷ������Ե�ָ��
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //�Զ�������λ��ֵ��Ϊ���������ö������ԣ���������Ĭ���ǽ��õ�
    glEnableVertexAttribArray(0);

    //�����������
    //glUseProgram(shaderProgram);
    
    //���VAO,��Ϊ��ʱ�󶨶�����0���൱�ڽ��
    glBindVertexArray(0);
    

    //glfwWindowShouldClose����������ÿ��ѭ���Ŀ�ʼǰ���һ��GLFW�Ƿ�Ҫ���˳�
    while (!glfwWindowShouldClose(window)) {
        //����
        processInput(window);

        //��Ⱦָ��......
        //����glClear�����������Ļ����ɫ���壬������glClearColor�����������Ļ���õ���ɫ��������glClear�����������ɫ����֮��������ɫ���嶼�ᱻ���ΪglClearColor�������õ���ɫ
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);        
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);     //��VAO


        //���¶��ǻ���ָ��
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        
        //��һ������ָ�������ǻ��Ƶ�ģʽ�������glDrawArrays��һ����
        //�ڶ������������Ǵ�����ƶ���ĸ�����������6��Ҳ����˵����һ����Ҫ����6�����㡣
        //���������������������ͣ�������GL_UNSIGNED_INT��
        //���һ�����������ǿ���ָ��EBO�е�ƫ���������ߴ���һ���������飬�������ǵ��㲻��ʹ��������������ʱ�򣩣��������ǻ���������д0��
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        //����ģʽ
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);    //�߿�ģʽ
        //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    //���

        //glfwPollEvents�����û�д���ʲô�¼�������������롢����ƶ��ȣ������´���״̬�������ö�Ӧ�Ļص�����������ͨ���ص������ֶ����ã�
        glfwPollEvents();

        glBindVertexArray(0);   //���VAO

        //glfwSwapBuffers�����ύ����ɫ���壬Ҳ����˫���塣
        //ǰ���屣�������������ͼ�񣬻�����Ļ��ʾ�������е���Ⱦָ����ں󻺳��ϻ��ƣ���������Ⱦָ��ִ����ϣ��ͽл�ǰ����ͺ󻺳壬�����Ͳ�����ֲ���ʵ��
        glfwSwapBuffers(window);       
    }

    //����Ⱦѭ��������������Ҫ��ȷ�ͷ�/ɾ��֮ǰ�ķ����������Դ
    glDeleteVertexArrays(1, &VAO);      //ɾ�������������
    glDeleteBuffers(1, &VBO);               //ɾ�����㻺�����
    glfwTerminate();
    return 0;
}

//����
void processInput(GLFWwindow* window) {
    //glfwGetKey������Ҫһ�������Լ�һ��������Ϊ����,���᷵����������Ƿ����ڱ�����
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

//���ھ����ӿڣ������ǵ������ӻ����ڵ�ʱ�򣬳�����ӿڵĳߴ�ҲӦ�ø��ŵ��������ԶԴ���ע��һ���ص�����
//���֡�����С������Ҫһ��GLFWwindow��Ϊ���ĵ�һ���������Լ�����������ʾ���ڵ���ά��
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

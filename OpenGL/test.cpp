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
    //初始化GLFW
    glfwInit();

    //配置GLFW，第一个参数为选项的名称(可以从很多以GLFW_开头的枚举值中选择)，第二个参数用来设置该选项的值
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  //主版本号
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);  //次版本号
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  //告诉GLFW我们使用的是核心模式(Core-profile)

    //创建一个窗口对象，这个窗口对象存放了所有和窗口相关的数据，而且会被GLFW的其他函数频繁地用到
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    //创建完窗口我们就可以通知GLFW将我们窗口的上下文设置为当前线程的主上下文
    glfwMakeContextCurrent(window);

    //GLAD是用来管理OpenGL的函数指针的，所以在调用任何OpenGL的函数之前我们需要初始化GLAD
    //给GLAD传入了用来加载系统相关的OpenGL函数指针地址的函数
    //GLFW给我们的是glfwGetProcAddress，它根据我们编译的系统定义了正确的函数
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //创建顶点着色器对象
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);  

    //将着色器源码附加到着色器上,然后编译它
    glShaderSource(vertexShader, 1, &vertexshadersource, NULL);
    glCompileShader(vertexShader);

    int  successV;
    char infoLog[512];
    //检查是否编译成功
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &successV);

    if (!successV) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //创建片段着色器对象
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

    //创建着色器程序，着色器需要附加到这个程序上，然后链接
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

    //在程序包括这两个着色器的前提下，就可以删除这两个着色器
    glDeleteShader(vertexShader);
    glDeleteShader(fragShader);

    //glViewport函数前两个参数控制窗口左下角的位置。第三个和第四个参数控制渲染窗口的宽度和高度（像素）
    glViewport(0, 0, 800, 600);

    //注册窗口调整回调函数，告诉GLFW我们希望每当窗口调整大小的时候调用这个函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

   /* float vertices[] = {
        -0.5f,-0.5f,0.0f,
        0.5f,-0.5f,0.0f,
        0.0f,0.5f,0.0f
    };*/

    //float vertices[] = {
    //    // 第一个三角形
    //    0.5f, 0.5f, 0.0f,   // 右上角
    //    0.5f, -0.5f, 0.0f,  // 右下角
    //    -0.5f, 0.5f, 0.0f,  // 左上角
    //    // 第二个三角形
    //    0.5f, -0.5f, 0.0f,  // 右下角
    //    -0.5f, -0.5f, 0.0f, // 左下角
    //    -0.5f, 0.5f, 0.0f   // 左上角
    //};

    float vertices[] = {
    0.5f, 0.5f, 0.0f,   // 右上角
    0.5f, -0.5f, 0.0f,  // 右下角
    -0.5f, -0.5f, 0.0f, // 左下角
    -0.5f, 0.5f, 0.0f   // 左上角
    };

    unsigned int indices[] = {
        //索引从0开始
        //这里的数字表示的是vertices的下标
        //这样就能由下标构成矩形
        0,1,3,
        1,2,3
    };

    //使用函数和缓冲ID生成一个VBO(顶点缓冲对象)
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    //创建顶点数组对象VAO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    //创建元素(索引)缓冲对象
    unsigned int EBO;
    glGenBuffers(1, &EBO);

    //绑定VAO
    glBindVertexArray(VAO);

    //把创建的缓冲VBO绑定到GL_ARRAY_BUFFER目标上
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    //将之前定义好的顶点数据复制到当前绑定缓冲
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //把创建的缓冲EBO绑定到GL_ELEMENT_ARRAY_BUFFER目标上
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //将之前定义好的索引数据复制到当前绑定缓冲
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //设置顶点属性指针
    //告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上
    //使用buffer中的数据在VAO生成0号顶点属性的指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //以顶点属性位置值作为参数，启用顶点属性；顶点属性默认是禁用的
    glEnableVertexAttribArray(0);

    //激活这个程序
    //glUseProgram(shaderProgram);
    
    //解绑VAO,因为此时绑定对象变成0，相当于解绑
    glBindVertexArray(0);
    

    //glfwWindowShouldClose函数在我们每次循环的开始前检查一次GLFW是否被要求退出
    while (!glfwWindowShouldClose(window)) {
        //输入
        processInput(window);

        //渲染指令......
        //调用glClear函数来清空屏幕的颜色缓冲，调用了glClearColor来设置清空屏幕所用的颜色。当调用glClear函数，清除颜色缓冲之后，整个颜色缓冲都会被填充为glClearColor里所设置的颜色
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);        
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);     //绑定VAO


        //以下都是绘制指令
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        
        //第一个参数指定了我们绘制的模式，这个和glDrawArrays的一样。
        //第二个参数是我们打算绘制顶点的个数，这里填6，也就是说我们一共需要绘制6个顶点。
        //第三个参数是索引的类型，这里是GL_UNSIGNED_INT。
        //最后一个参数里我们可以指定EBO中的偏移量（或者传递一个索引数组，但是这是当你不在使用索引缓冲对象的时候），但是我们会在这里填写0。
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        //绘制模式
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);    //线框模式
        //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    //填充

        //glfwPollEvents检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数（可以通过回调方法手动设置）
        glfwPollEvents();

        glBindVertexArray(0);   //解绑VAO

        //glfwSwapBuffers函数会交换颜色缓冲，也就是双缓冲。
        //前缓冲保存着最终输出的图像，会在屏幕显示，而所有的渲染指令都会在后缓冲上绘制，当所有渲染指令执行完毕，就叫唤前缓冲和后缓冲，这样就不会出现不真实感
        glfwSwapBuffers(window);       
    }

    //当渲染循环结束后我们需要正确释放/删除之前的分配的所有资源
    glDeleteVertexArrays(1, &VAO);      //删除顶点数组对象
    glDeleteBuffers(1, &VBO);               //删除顶点缓冲对象
    glfwTerminate();
    return 0;
}

//输入
void processInput(GLFWwindow* window) {
    //glfwGetKey函数需要一个窗口以及一个按键作为输入,将会返回这个按键是否正在被按下
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

//窗口就是视口，当我们调整可视化窗口的时候，程序的视口的尺寸也应该跟着调整，所以对窗口注册一个回调函数
//这个帧缓冲大小函数需要一个GLFWwindow作为它的第一个参数，以及两个整数表示窗口的新维度
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

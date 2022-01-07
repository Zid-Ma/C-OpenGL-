#pragma comment(lib,"opengl32.lib")
#include <glad.h>
#include <glfw3.h>
#include <opencv2/opencv.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#define STB_IMAGE_IMPLEMENTATION
#include <stb-master/stb_image.h>

#include <ME/shader_me.h>
//#include <ME/camera_me.h>
#include <mesh_me.h>
#include <mode_me.h>
#include <atlimage.h>

//#pragma comment(lib,"assimp-mdd.lib")
//#pragma comment(lib,"zlibstaticd.lib")
//#include <vgl.h>
//#include <LoadShaders.h>
#include <gl.h>


#pragma region 回调函数

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//鼠标位置改变
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//鼠标滚轮
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

#pragma endregion

#pragma region 函数初始化
    unsigned int loadCubemap(vector<std::string> faces);

#pragma endregion

#pragma region 全局变量

const char* path = "F:/Resources/C++/Hello Under World-Cube/";

//GLFW窗体对象
GLFWwindow* window;

//多重采样
int multisampling = 16;

//窗体句柄
HDC hdc = (HDC)0x11D47;
HGLRC hglrc = (HGLRC)0x10000;

//窗体宽度
const unsigned int SCR_WIDTH = 1920;
//窗体高度
const unsigned int SCR_HEIGHT = 1080;

//正方体各顶点
float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};
//各正方体位置
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
//点光源位置
glm::vec3 pointLightPositions[] = {
    glm::vec3(0.7f,  0.2f,  2.0f),
    glm::vec3(2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3(0.0f,  0.0f, -3.0f)
};

//摄像机位置
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
//摄像机方向向量
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
//摄像机上向量
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

//是否第一次鼠标进入窗体
bool firstMouse = true;
//偏航角
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
//俯仰角
float pitch = 0.0f;
//鼠标在X轴的上一帧位置
float lastX = 800.0f / 2.0;
//鼠标在Y轴的上一帧位置
float lastY = 600.0 / 2.0;
//摄像机视野
float fov = 45.0f;

//上一帧到当前帧时间
float deltaTime = 0.0f;
//上一帧的时间
float lastFrame = 0.0f;

#pragma endregion

//------
//首次使用初始化
//------
void chushihua(void) {
	
    std:: cout << "首次使用初始化" << endl;

#pragma region GLFW初始化

    //GLFW初始化
    glfwInit();
    //配置GLFW，调试选项
    //OpenGL主版本号（GLFW_CONTEXT_VERSION_MAJOR）
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//要设置的hint的名字（使用GLFW常量），值
    //OpenGL副版本号（GLFW_CONTEXT_VERSION_MINOR）
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //OpenGL模式（GLFW_OPENGL_PROFILE）
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //窗口是否可调整大小（GLFW_RESIZABLE）
    glfwWindowHint(GLFW_RESIZABLE, TRUE);
#pragma region 多重采样
    //使用一个包含N个样本的多重采样缓冲 
    glfwWindowHint(GLFW_SAMPLES, multisampling);//GLFW会自动创建一个每像素4个子采样点的深度和样本缓冲。这也意味着所有缓冲的大小都增长了4倍。
#pragma endregion

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

#pragma endregion

#pragma region GLFW窗体对象创建

    //为窗体对象赋值
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hello Under World", NULL, NULL);

    //如果窗体对象出现问题
    if (window == NULL) {
        std::cout << "创建窗体对象时失败" << std::endl;
    }
    //设置窗口的上下文为当前线程的主上下文
    glfwMakeContextCurrent(window);
    //设置window窗体不需要被关闭
    glfwSetWindowShouldClose(window, false);

    //绑定相应回调函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    //设置鼠标在当前窗体的模式（鼠标隐藏且不改变位置）
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

#pragma endregion

#pragma region 设置OpenGL上下文

#pragma region 创建Windows窗体
    ////注册WNDCLASS类（结构体）
    //WNDCLASS    wc; // Windows Class Structure  

    //HINSTANCE hInstance = GetModuleHandle(NULL);  // Grab An Instance For Our Window  
    //// Redraw On Size, And Own DC For Window.  
    //wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    //wc.lpfnWndProc = (WNDPROC)window; // WndProc Handles Messages  
    //wc.cbClsExtra = 0;                    // No Extra Window Data  
    //wc.cbWndExtra = 0;                    // No Extra Window Data  
    //wc.hInstance = hInstance;           // Set The Instance  
    //wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);    // Load The Default Icon  
    //wc.hCursor = LoadCursor(NULL, IDC_ARROW);   // Load The Arrow Pointer  
    //wc.hbrBackground = NULL;                     // No Background Required For GL  
    //wc.lpszMenuName = NULL;                     // We Don't Want A Menu  
    //wc.lpszClassName = (LPCWSTR)"OpenGL";                 // Set The Class Name  

    //if (!RegisterClass(&wc)) // Attempt To Register The Window Class  
    //{
    //    cout << "Failed to register the window." << endl;
    //}

    ////创建窗体
    //HWND hWnd = CreateWindow((LPCWSTR)"OpenGL", (LPCWSTR)"Hello World.",
    //    WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW,
    //    CW_USEDEFAULT, CW_USEDEFAULT, 500, 500, NULL, NULL, GetModuleHandle(NULL), NULL);
    //if (!hWnd)
    //{
    //    cerr << "Create window failed." << endl;
    //}

    //ShowWindow(hWnd, SW_SHOW);

#pragma endregion

#pragma region 销毁窗体
    //if (hdc && !ReleaseDC(hWnd, hdc)) // Are We Able To Release The DC  
    //{
    //    cerr << "Release Device Context Failed." << endl;
    //    hdc = NULL; // Set DC To NULL  
    //}

    //if (hWnd && !DestroyWindow(hWnd)) // Are We Able To Destroy The Window?  
    //{
    //    cerr << "Could Not Release hWnd." << endl;
    //    hWnd = NULL; // Set hWnd To NULL  
    //}

    //if (!UnregisterClass((LPCWSTR)"OpenGL", hInstance))   // Are We Able To Unregister Class  
    //{
    //    cerr << "Could Not Unregister Class." << endl;
    //    hInstance = NULL; // Set hInstance To NULL  
    //}
#pragma endregion


    ////设置一个新的窗口句柄
    //hdc = (HDC)0x11D47;
    //hglrc = (HGLRC)0x10000;
    ////将OpenGL上下文设置为当前线程的当前上下文
    //hdc = GetDC(hWnd);
    //printf("%p", hWnd);

    //填写PIXELFORMATDESCRIPTOR结构体
    //static PIXELFORMATDESCRIPTOR pfd = {
    //sizeof(PIXELFORMATDESCRIPTOR),  // Size Of This Pixel Format Descriptor  
    //1,                              // Version Number  
    //PFD_DRAW_TO_WINDOW |            // Format Must Support Window  
    //PFD_SUPPORT_OPENGL |            // Format Must Support OpenGL  
    //PFD_DOUBLEBUFFER,               // Must Support Double Buffering  
    //PFD_TYPE_RGBA,                  // Request An RGBA Format  
    //32,                           // Select Our Color Depth  
    //0, 0, 0, 0, 0, 0,               // Color Bits Ignored  
    //0,                              // No Alpha Buffer  
    //0,                              // Shift Bit Ignored  
    //0,                              // No Accumulation Buffer  
    //0, 0, 0, 0,                     // Accumulation Bits Ignored  
    //16,                             // 16Bit Z-Buffer (Depth Buffer)    
    //0,                              // No Stencil Buffer  
    //0,                              // No Auxiliary Buffer  
    //PFD_MAIN_PLANE,                 // Main Drawing Layer  
    //0,                              // Reserved  
    //0, 0, 0                         // Layer Masks Ignored  
    //};

    ////选择合适的像素格式
    //// get the best available match of pixel format for the device context  
    //int  iPixelFormat = ChoosePixelFormat(hdc, &pfd);
    //if (0 == iPixelFormat)
    //{
    //    cerr << "Choose pixel format failed." << endl;
    //}

    ////设置像素格式
    //// make that the pixel format of the device context  
    //if (!SetPixelFormat(hdc, iPixelFormat, &pfd))
    //{
    //    cerr << "Set pixel format failed." << endl;
    //}

    //wglMakeCurrent(hdc, wglCreateContext(hdc));//wglCreateContext(hdc)
    ////window = (GLFWwindow*)x;//0x0055C148

    ////显示OpenGL渲染内容
    ////交换缓冲
    ////SwapBuffers(hdc);
#pragma endregion

#pragma region GLAD初始化

    //GLAD初始化,用来管理OpenGL函数指针的，调用任何OpenGL的函数之前都需要初始化GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        //返回初始化失败信息
        std::cout << "Failed to initialize GLAD" << std::endl;
        //return -1;
    }

#pragma endregion

#pragma region OpenGL初始化

    //设置OpenGL渲染窗体的尺寸大小，即视口(Viewport)
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    //设置OpenGL的全局状态（）
    //-----------------------------------
    //启用深度测试
    glEnable(GL_DEPTH_TEST);
    //指定“目标像素与当前像素在z方向上值大小比较”的函数，符合该函数关系的目标像素才进行绘制，否则对目标像素不予绘制。
    glDepthFunc(GL_LESS);//GL_LESS,如果输入的深度值小于参考值，则通过
    //模板缓冲
    //------------
    //启用模板测试
    glEnable(GL_STENCIL_TEST);
    //告诉OpenGL，只要一个片段的模板值等于(GL_EQUAL)参考值1，片段将会通过测试并被绘制，否则会被丢弃。
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    //指定更新模板缓冲的时机与行为
    //sfail：模板测试失败时采取的行为。
    //dpfail：模板测试通过，但深度测试失败时采取的行为。
    //dppass：模板测试和深度测试都通过时采取的行为。
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    //混合
    //--------
    //启用混合
    glEnable(GL_BLEND);
    //设置源和目标因子
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //也可以使用glBlendFuncSeparate为RGB和alpha通道分别设置不同的选项：
    //glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
    //OpenGL甚至给了我们更多的灵活性，允许我们改变方程中源和目标部分的运算符。当前源和目标是相加的，但如果愿意的话，我们也可以让它们相减。glBlendEquation(GLenum mode)允许我们设置运算符，它提供了三个选项：
    //GL_FUNC_ADD：默认选项，将两个分量相加：。
    //GL_FUNC_SUBTRACT：将两个分量相减： 。
    //GL_FUNC_REVERSE_SUBTRACT：将两个分量相减，但顺序相反：。
    //面剔除
    //---------
    //启用面剔除
    //glEnable(GL_CULL_FACE);
    //剔除背面
    //glCullFace(GL_BACK);
    //顺时针为正向面(初始为CL_CWW逆时针为正向)
    //glFrontFace(GL_CW);
    //点大小
    //------
    //启用顶点着色器点大小修改
    glEnable(GL_PROGRAM_POINT_SIZE);
    //多重采样
    // -------
    //启用多重采样
    glEnable(GL_MULTISAMPLE);
    //gamma校正
    //--------
    //启用sRGB颜色空间//OpenGL将自动执行gamma校正，包括默认帧缓冲
    //glEnable(GL_FRAMEBUFFER_SRGB);
#pragma endregion


}

#pragma region 函数
//------
//数据初始化
//------
void shujuchushihua(void) {
    //定义好正方体的顶点
    float v[] = {
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
    //为数组赋值
    memcpy(vertices, v, sizeof(v));

    // world space positions of our cubes
    glm::vec3 cubeP[] = {
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
    memcpy(cubePositions, cubeP, sizeof(cubeP));

    //摄像机位置
    cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    //摄像机方向向量
    cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    //摄像机上向量
    cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);


}

//------
//OpenCV读取图片数据（图片路径，返回图片的宽度，返回图片的高度）
//------
GLubyte* Image_(const char* path, int* width, int* height) {
    using namespace cv;
    //从文件中读取相应图像作为纹理

    cv::Mat img = cv::imread(path, -1);//-1表示读取原图，0表示读取为灰度,大于0表示返回三通道彩色图
    if (img.empty() == true) {
        std::cout << "图片读取失败:" << *path << std::endl;
    }

    //获取图像的宽和高
    *width = img.cols;
    *height = img.rows;
    //获取图像指针大小
    int pixellength = *width * *height * 3;
    //创建一个相应大小的存储空间，用于存储图像数据
    GLubyte* pixels = new GLubyte[pixellength];
    //unsigned char* image = new GLubyte[pixellength];
    std::cout << "width = " << *width << "height = " << *height << std::endl;
    //std::cout << img.data;
    //获取图像数据到pixels中
    memcpy(pixels, img.data, pixellength * sizeof(unsigned char));

    return pixels;
}

//------
//纹理对象初始化（纹理对象，图片路径）
//------
void wenliduixiang(unsigned int* texture, const char* path) {
    //生成纹理对象,根据纹理参数返回n个纹理索引(目前没有用到的纹理索引)
    glGenTextures(1, texture);
    //绑定纹理对象texture1（绑定模式为2D）// 将一个命名的纹理绑定到一个纹理目标上
    glBindTexture(GL_TEXTURE_2D, *texture);
    //设置纹理包裹参数(set the texture wrapping parameters)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //设置纹理过滤参数(set texture filtering parameters)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //图片的宽度、高度
    int width, height;
    //获取图片数据
    unsigned char* image = Image_(path, &width, &height);
    //target ：指定纹理单元的类型是哪一种，必须指定为 GL_TEXTURE_2D, GL_PROXY_TEXTURE_2D, GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, or GL_PROXY_TEXTURE_CUBE_MAP中的一个。二维纹理需要指定为GL_TEXTURE_2D
    //
    //level：指定纹理单元的层次，非mipmap纹理level设置为0，mipmap纹理设置为纹理的层级(Mipmap是一种电脑图形图像技术，用于在三维图像的二维代替物中达到立体感效应。)
    //
    //internalFormat：指定OpenGL是如何管理纹理单元中数据格式的。网络上很多解释说这个参数必须和后面的format参数一样，这个说法是不正确的
    //
    //width：指定纹理单元的宽度
    //
    //height：指定纹理单元的高度
    //
    //border：指定纹理单元的边框，如果包含边框取值为1，不包含边框取值为0
    //
    //format：指定data所指向的数据的格式
    //
    //type：指定data所指向的数据的类型
    //
    //data：实际指向的数据是什么，这个参数有两种解释
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);//指定一个二维纹理图像
    //生成Mipmap
    glGenerateMipmap(GL_TEXTURE_2D);
    //绑定纹理
    glBindTexture(GL_TEXTURE_2D, 0); //(纹理要绑定到的目标,纹理名)// Unbind texture when done, so we won't accidentily mess up our texture.
}

//------
//带透明通道纹理对象初始化（纹理对象，图片路径）
//------
void wenliduixiang_alpha(unsigned int* texture, const char* path) {
    //生成纹理对象,根据纹理参数返回n个纹理索引//生成纹理ID
    glGenTextures(1, texture);
    //绑定纹理对象texture1（绑定模式为2D）//绑定纹理对象到相应的索引//创建纹理对象
    glBindTexture(GL_TEXTURE_2D, *texture);
    //指定纹理属性
    //------------
    //设置纹理包裹参数(set the texture wrapping parameters)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //设置纹理过滤参数(set texture filtering parameters)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //图片的宽度、高度、读取到的图像类型
    int width, height, nrComponents;
    //获取图片数据
    unsigned char* image = stbi_load(path, &width, &height, &nrComponents, 0);
    GLenum format = 3;
    if (nrComponents == 1)
        format = GL_RED;
    else if (nrComponents == 3)
        format = GL_RGB;
    else if (nrComponents == 4)
        format = GL_RGBA;
    else
        format = GL_RGB;

    std::cout << "Texture_alpha failed to load at path: " << nrComponents << std::endl;
    //target ：指定纹理单元的类型是哪一种，必须指定为 GL_TEXTURE_2D, GL_PROXY_TEXTURE_2D, GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, or GL_PROXY_TEXTURE_CUBE_MAP中的一个。二维纹理需要指定为GL_TEXTURE_2D
    //
    //level：指定纹理单元的层次，非mipmap纹理level设置为0，mipmap纹理设置为纹理的层级(Mipmap是一种电脑图形图像技术，用于在三维图像的二维代替物中达到立体感效应。)
    //
    //internalFormat：指定OpenGL是如何管理纹理单元中数据格式的。网络上很多解释说这个参数必须和后面的format参数一样，这个说法是不正确的
    //
    //width：指定纹理单元的宽度
    //
    //height：指定纹理单元的高度
    //
    //border：指定纹理单元的边框，如果包含边框取值为1，不包含边框取值为0
    //
    //format：指定data所指向的数据的格式
    //
    //type：指定data所指向的数据的类型
    //
    //data：实际指向的数据是什么，这个参数有两种解释
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);//指定一个二维纹理图像//指定纹理内容
    //生成Mipmap
    glGenerateMipmap(GL_TEXTURE_2D);
    //绑定纹理//激活纹理功能
    glBindTexture(GL_TEXTURE_2D, *texture); //(纹理要绑定到的目标,纹理名)// Unbind texture when done, so we won't accidentily mess up our texture.

    //提供纹理坐标
    //glTexCoord * ();
}

//------
//设置着色器中uniform对象的值（着色器对象, 对象名, 值）
//------
void zhaoseqizhi(GLuint* program, const char* name, glm::mat4 x) {
    //获取到着色器程序对象中相应uniform对象的位置
    GLuint transformLoc = glGetUniformLocation(*program, name);
    //如果未找到相应对象
    if (transformLoc == -1)
    {
        std::cout << "\n我顶::" << transformLoc;
    }
    else
    {
        std::cout << "已成功找到uniform变量的位置";
    }
    //设置uniform对象的值

    //通过一致变量（uniform修饰的变量）引用将一致变量值传入渲染管线。
    //
    //location: uniform的位置。
    //count : 需要加载数据的数组元素的数量或者需要修改的矩阵的数量。
    //transpose : 指明矩阵是列优先(column major)矩阵（GL_FALSE）还是行优先(row major)矩阵（GL_TRUE）。
    //value : 指向由count个元素的数组的指针。
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(x));//uniform的位置值,告诉OpenGL我们将要发送多少个矩阵,是否希望对矩阵进行置换(Transpose),矩阵数据//value_ptr来变换这些数据为OpenGL所接受的格式
}

void lightVAO_Set(Shader lightingShader, unsigned int* VBO, unsigned int* lightVAO)
{
    // 在此之前不要忘记首先 use 对应的着色器程序（来设定uniform）
    lightingShader.use();
    lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);//设置相应的uniform对象值
    lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
    glGenVertexArrays(1, lightVAO);
    glBindVertexArray(*lightVAO);
    // 只需要绑定VBO不用再次设置VBO的数据，因为箱子的VBO数据中已经包含了正确的立方体顶点数据
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    // 设置灯立方体的顶点属性（对我们的灯来说仅仅只有位置数据）
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}
#pragma endregion

int main(int argc, char** argv) {

    chushihua();

    shujuchushihua();

//#pragma region 冯氏光照着色器对象
//    //构建并编译着色器对象
//    Shader ourShader("F:/Resources/C++/Hello Under World-Cube/cube.vert", "F:/Resources/C++/Hello Under World-Cube/cube.frag");
//#pragma region 原program
//    //记录着色器信息
//    //ShaderInfo shaders[] = {
//    //    {GL_VERTEX_SHADER, "cube.vert"},//顶点着色器名称(路径)
//    //    {GL_FRAGMENT_SHADER, "cube.frag"},//片段着色器名称(路径)
//    //    {GL_NONE, NULL}
//    //};
//    ////构建并编译着色器对象
//    //GLuint program = LoadShaders(shaders);
//    //使用当前着色器程序
//    //glUseProgram(program);
//#pragma endregion
//
//    //普通方块对象
//    //------------
//    //创建顶点缓冲对象、顶点数组对象（两者的位置信息都为1）
//    unsigned int cubeVBO, cubeVAO;
//    glGenVertexArrays(1, &cubeVAO);
//    glGenBuffers(1, &cubeVBO);
//
//    //绑定顶点数组对象
//    glBindVertexArray(cubeVAO);
//    //绑定顶点缓冲对象
//    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
//    //创建并初始化缓冲区对象的数据存储
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//    //OpenGL确保至少有16个包含4分量的顶点属性可用，但是有些硬件或许允许更多的顶点属性
//    int nrAttributes;
//    //查询顶点属性的最大支持量
//    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
//    std::cout << "支持顶点属性的最大数量： " << nrAttributes << std::endl;
//
//    //对顶点缓冲对象内容进行解析定义
//    // -----------------------------
//    //位置属性
//    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);//启用顶点属性0
//    //法向量属性
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
//    glEnableVertexAttribArray(1);
//    //纹理坐标属性
//    //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
//    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
//    glEnableVertexAttribArray(2);//启用顶点属性1
//
//
//    //创建相应的纹理对象
//    unsigned int texture1, texture2;
//    //对纹理对象参数初始化
//    wenliduixiang(&texture1, "F:/Resources/Image/Map/WoodenBox-DiffuseMap.bmp");
//    wenliduixiang(&texture2, "F:/Resources/Image/Map/WoodenBox-SpecularMap.bmp");
//
//    // 告诉opengl每个采样器它所属的纹理单元（只需执行一次）//tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
//    // -------------------------------------------------------------------------------------------------------------------------------------------------
//    ourShader.use();
//    ourShader.setInt("texture1", 0);
//    ourShader.setInt("texture2", 1);
//    ourShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);//设置相应的uniform对象值
//    ourShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
//    ourShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
//    ourShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
//    ourShader.setFloat("material.shininess", 32.0f);
//    ourShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
//    ourShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f); // 将光照调暗了一些以搭配场景
//    ourShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
//#pragma endregion

#pragma region 光照贴图着色器对象

    std::cout << "光照贴图着色器对象" << endl;

    //构建并编译着色器对象
    Shader ourShader("F:/Resources/C++/Hello Under World-Cube/Debug/Shader/cube-multiple.vert", "F:/Resources/C++/Hello Under World-Cube/Debug/Shader/cube-multiple.frag");
#pragma region 原program
    //记录着色器信息
    //ShaderInfo shaders[] = {
    //    {GL_VERTEX_SHADER, "cube.vert"},//顶点着色器名称(路径)
    //    {GL_FRAGMENT_SHADER, "cube.frag"},//片段着色器名称(路径)
    //    {GL_NONE, NULL}
    //};
    ////构建并编译着色器对象
    //GLuint program = LoadShaders(shaders);
    //使用当前着色器程序
    //glUseProgram(program);
#pragma endregion

    //普通方块对象
    //------------
    //创建顶点缓冲对象、顶点数组对象（两者的位置信息都为1）
    unsigned int cubeVBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);

    //绑定顶点数组对象
    glBindVertexArray(cubeVAO);
    //绑定顶点缓冲对象
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    //创建并初始化缓冲区对象的数据存储
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //OpenGL确保至少有16个包含4分量的顶点属性可用，但是有些硬件或许允许更多的顶点属性
    int nrAttributes;
    //查询顶点属性的最大支持量
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "支持顶点属性的最大数量： " << nrAttributes << std::endl;
    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &nrAttributes);
    std::cout << "支持uniform的最大数量： " << nrAttributes << std::endl;
    

    //对顶点缓冲对象内容进行解析定义
    // -----------------------------
    //位置属性
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);//启用顶点属性0
    //法向量属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    //纹理坐标属性
    //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);//启用顶点属性1


    //创建相应的纹理对象
    unsigned int texture1, texture2;
    //对纹理对象参数初始化
    wenliduixiang(&texture1, "F:/Resources/C++/Hello Under World-Cube/Debug/Image/Map/WoodenBox-DiffuseMap.bmp");
    wenliduixiang(&texture2, "F:/Resources/C++/Hello Under World-Cube/Debug/Image/Map/WoodenBox-SpecularMap.bmp");

#pragma region ourShader着色器对象设置
    // 告诉opengl每个采样器它所属的纹理单元（只需执行一次）//tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
// -------------------------------------------------------------------------------------------------------------------------------------------------
    ourShader.use();
    //ourShader.setInt("texture1", 0);
    //ourShader.setInt("texture2", 1);
    //ourShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);//设置相应的uniform对象值
    //ourShader.setFloat("material.shininess", 32.0f);
    //ourShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
    //ourShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f); // 将光照调暗了一些以搭配场景
    //ourShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

    //ourShader.setInt("material.diffuse", 0);
    //ourShader.setInt("material.specular", 1);
    //ourShader.setFloat("material.shininess", 64.0f);
    ////设置灯光位置
    //glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
    //ourShader.setVec3("light.position", lightPos);
    //ourShader.setVec3("viewPos", cameraPos);
    //ourShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
    //ourShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f); // 将光照调暗了一些以搭配场景
    //ourShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

     // be sure to activate shader when setting uniforms/drawing objects
    ourShader.use();
    ourShader.setVec3("viewPos", cameraPos);
    ourShader.setFloat("material.shininess", 32.0f);

    ourShader.setInt("envTexture", 2);
    /*
       Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
       the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
       by defining light types as classes and set their values in there, or by using a more efficient uniform approach
       by using 'Uniform buffer objects', but that is something we'll discuss in the 'Advanced GLSL' tutorial.
    */
    // directional light
    ourShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
    ourShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
    ourShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
    ourShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
    // point light 1
    ourShader.setVec3("pointLights[0].position", pointLightPositions[0]);
    ourShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
    ourShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
    ourShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
    ourShader.setFloat("pointLights[0].constant", 1.0f);
    ourShader.setFloat("pointLights[0].linear", 0.09);
    ourShader.setFloat("pointLights[0].quadratic", 0.032);
    // point light 2
    ourShader.setVec3("pointLights[1].position", pointLightPositions[1]);
    ourShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
    ourShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
    ourShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
    ourShader.setFloat("pointLights[1].constant", 1.0f);
    ourShader.setFloat("pointLights[1].linear", 0.09);
    ourShader.setFloat("pointLights[1].quadratic", 0.032);
    // point light 3
    ourShader.setVec3("pointLights[2].position", pointLightPositions[2]);
    ourShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
    ourShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
    ourShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
    ourShader.setFloat("pointLights[2].constant", 1.0f);
    ourShader.setFloat("pointLights[2].linear", 0.09);
    ourShader.setFloat("pointLights[2].quadratic", 0.032);
    // point light 4
    ourShader.setVec3("pointLights[3].position", pointLightPositions[3]);
    ourShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
    ourShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
    ourShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
    ourShader.setFloat("pointLights[3].constant", 1.0f);
    ourShader.setFloat("pointLights[3].linear", 0.09);
    ourShader.setFloat("pointLights[3].quadratic", 0.032);
    // spotLight
    ourShader.setVec3("spotLight.position", cameraPos);
    ourShader.setVec3("spotLight.direction", cameraFront);
    ourShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
    ourShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
    ourShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
    ourShader.setFloat("spotLight.constant", 1.0f);
    ourShader.setFloat("spotLight.linear", 0.09);
    ourShader.setFloat("spotLight.quadratic", 0.032);
    ourShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
    ourShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
#pragma endregion

#pragma endregion

#pragma region 灯光对象初始化

    std::cout << "灯光对象初始化" << endl;

    //灯光对象
    //--------
    //定义着色器对象
    Shader lightShader("F:/Resources/C++/Hello Under World-Cube/Debug/Shader/lightCube.vert", "F:/Resources/C++/Hello Under World-Cube/Debug/Shader/lightCube.frag");
    //定义顶点数组对象
    unsigned int lightVAO;
    //灯光对象VAO
    //lightVAO_Set(lightingShader, &cubeVBO, &lightVAO);
    {
        glGenVertexArrays(1, &lightVAO);
        glBindVertexArray(lightVAO);
        // 只需要绑定VBO不用再次设置VBO的数据，因为箱子的VBO数据中已经包含了正确的立方体顶点数据
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        // 设置灯立方体的顶点属性（对我们的灯来说仅仅只有位置数据）
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        //法向量属性
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glBindVertexArray(0);
    }
#pragma endregion

#pragma region 原program
    //启用着色器程序
    //glUseProgram(program);
    //glUniform1i():设置uniform采样器的位置值
    //glUniform1i(glGetUniformLocation(program, "ourTexture1"), 0);//设置ourTexture1对应第0个纹理单元
    //glUniform1i(glGetUniformLocation(program, "ourTexture2"), 1);//设置ourTexture2对应第1个纹理单元
#pragma endregion

   ////定义顶点数组对象
   // unsigned int VAO1;
   // {
   //     glGenVertexArrays(1, &VAO1);
   //     glBindVertexArray(VAO1);
   //     // 只需要绑定VBO不用再次设置VBO的数据，因为箱子的VBO数据中已经包含了正确的立方体顶点数据
   //     glBindBuffer(GL_ARRAY_BUFFER, VAO1);
   //     // 设置灯立方体的顶点属性（对我们的灯来说仅仅只有位置数据）
   //     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
   //     glEnableVertexAttribArray(0);
   //     //法向量属性
   //     glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
   //     glEnableVertexAttribArray(1);
   // }

#pragma region 混合

    std::cout << "混合" << endl;

    unsigned int vegetationVAO, vegetationVBO;
    //草的VAO与VBO解析
    {
        float transparentVertices[] = {
            // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
            //位置                //纹理坐标（由于纹理上下颠倒，因此交换了y坐标）
            0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
            0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
            1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

            0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
            1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
            1.0f,  0.5f,  0.0f,  1.0f,  0.0f
        };
        //创建VAO
        glGenVertexArrays(1, &vegetationVAO);
        //创建VBO
        glGenBuffers(1, &vegetationVBO);
        //设置它为当前操作的VAO，之后我们所有关于顶点数据的设置（包括数据使用的VBO对象，顶点的属性设置的信息都会被存储在VAO之中），在设置完成之后一般会解绑VAO，然后在需要绘制的时候启用相应的VAO对象。
        glBindVertexArray(vegetationVAO);
        glBindBuffer(GL_ARRAY_BUFFER, vegetationVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), transparentVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glBindVertexArray(0);
    }

    //草的位置
    vector<glm::vec3> vegetation;
    vegetation.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
    vegetation.push_back(glm::vec3(1.5f, 0.0f, 0.51f));
    vegetation.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
    vegetation.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
    vegetation.push_back(glm::vec3(0.5f, 0.0f, -0.6f));

    //构建且编译着色器对象
    Shader BlendingShader("F:/Resources/C++/Hello Under World-Cube/Debug/Shader/model-loading.vert", "F:/Resources/C++/Hello Under World-Cube/Debug/Shader/model-loading.frag");
    BlendingShader.setInt("texture_diffuse1", 0);


    unsigned int texture_grass;
    wenliduixiang_alpha(&texture_grass, "F:/Resources/C++/Hello Under World-Cube/Debug/Image/grass.png");

#pragma endregion

#pragma region 模型加载

    std::cout << "模型加载" << endl;

    //告诉stb_image.h在y轴上翻转加载的纹理（加载模型之前） tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    //stbi_set_flip_vertically_on_load(true);
    //创建好着色器对象
    // ---------------
    Shader modelShader("F:/Resources/C++/Hello Under World-Cube/Debug/Shader/cube-multiple.vert", "F:/Resources/C++/Hello Under World-Cube/Debug/Shader/cube-multiple.frag");//model-loading//cube-multiple
    //加载模型
    // --------
    Model ourModel("F:/Resources/C++/Hello Under World-Cube/Debug/Mode/nanosuit/nanosuit.obj"); //F:/Resources/C++/Hello Under World-Cube/mode/nanosuit/nanosuit.obj//H:/Downloads-New/QQ/su/wutai2.obj
    //引入线框 draw in wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#pragma endregion

#pragma region 环境映射的模型加载

    std::cout << "环境映射的模型加载" << endl;

    Shader modelShader_env("F:/Resources/C++/Hello Under World-Cube/Debug/Shader/model-environment.vert", "F:/Resources/C++/Hello Under World-Cube/Debug/Shader/model-environment.frag");

    Model modelEnv("F:/Resources/C++/Hello Under World-Cube/Debug/Mode/nanosuit_reflection/nanosuit.obj");
#pragma endregion

#pragma region 帧缓冲

    std::cout << "帧缓冲" << endl;

    ////创建帧缓冲对象
    //unsigned int fbo;
    //glGenFramebuffers(1, &fbo);
    ////设置fbo为当前帧缓冲对象,所有的读取和写入帧缓冲的操作将会影响当前绑定的帧缓冲
    //glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    ////一个完整的帧缓冲需要满足以下的条件：
    ////1.附加至少一个缓冲（颜色、深度或模板缓冲）。
    ////2.至少有一个颜色附件(Attachment)。
    ////3.所有的附件都必须是完整的（保留了内存）。
    ////4.每个缓冲都应该有相同的样本数。
    ////渲染到一个不同的帧缓冲被叫做离屏渲染(Off-screen Rendering)。要保证所有的渲染操作在主窗口中有视觉效果，我们需要再次激活默认帧缓冲，将它绑定到0。
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);
    ////删除帧缓冲对象(完成所有的帧缓冲操作之后)
    //glDeleteFramebuffers(1, &fbo);
    ////纹理附件
    ////--------
    //unsigned int texture;
    //glGenTextures(1, &texture);
    //glBindTexture(GL_TEXTURE_2D, texture);
    ////创建一个数据为空的纹理(宽度和高度一般为屏幕大小)
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    ////附加纹理附件到帧缓冲
    ////target：帧缓冲的目标（绘制、读取或者两者皆有）
    ////attachment：我们想要附加的附件类型。当前我们正在附加一个颜色附件。注意最后的0意味着我们可以附加多个颜色附件。我们将在之后的教程中提到。
    ////textarget：你希望附加的纹理类型
    ////texture：要附加的纹理本身
    ////level：多级渐远纹理的级别。我们将它保留为0。
    //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
    ////也可以将深度缓冲和模板缓冲附加为一个单独的纹理
    ////------------------------------------------------
    ////要附加深度缓冲的话，我们将附件类型设置为GL_DEPTH_ATTACHMENT。注意纹理的格式(Format)和内部格式(Internalformat)类型将变为GL_DEPTH_COMPONENT，来反映深度缓冲的储存格式。要附加模板缓冲的话，你要将第二个参数设置为GL_STENCIL_ATTACHMENT，并将纹理的格式设定为GL_STENCIL_INDEX。
    ////也可以将深度缓冲和模板缓冲附加为一个单独的纹理。纹理的每32位数值将包含24位的深度信息和8位的模板信息。要将深度和模板缓冲附加为一个纹理的话，我们使用GL_DEPTH_STENCIL_ATTACHMENT类型，并配置纹理的格式，让它包含合并的深度和模板值。
    ////将一个深度和模板缓冲附加为一个纹理到帧缓冲的例子可以在下面找到：
    //glTexImage2D(
    //    GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT, 0,
    //    GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL
    //);
    //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
    ////渲染缓冲对象（Renderbuffer Object）
    ////------------
    //// 作为一个可用的帧缓冲附件类型的，所以在过去纹理是唯一可用的附件。
    ////创建一个渲染缓冲对象
    //unsigned int rbo;
    //glGenRenderbuffers(1, &rbo);
    ////绑定这个渲染缓冲对象，让之后所有的渲染缓冲操作影响当前的rbo：
    //glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    ////创建一个深度和模板渲染缓冲对象
    ////创建一个渲染缓冲对象和纹理对象类似，不同的是这个对象是专门被设计作为图像使用的，而不是纹理那样的通用数据缓冲(General Purpose Data Buffer)
    ////GL_DEPTH24_STENCIL8作为内部格式，封装了24位的深度和8位的模板缓冲
    //glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
    ////附加这个渲染缓冲对象
    //glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    ////渲染缓冲对象能为你的帧缓冲对象提供一些优化，但知道什么时候使用渲染缓冲对象，什么时候使用纹理是很重要的。
    ////通常的规则是，如果你不需要从一个缓冲中采样数据，那么对这个缓冲使用渲染缓冲对象会是明智的选择。如果你需要从缓冲中采样颜色或深度值等数据，那么你应该选择纹理附件
    ////性能方面它不会产生非常大的影响的。
    ////检查帧缓冲是否完整
    //if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)


    //首先要创建一个帧缓冲对象，并绑定它
    //----------------------------------
    unsigned int frameb;
    glGenFramebuffers(1, &frameb);
    glBindFramebuffer(GL_FRAMEBUFFER, frameb);
    //创建一个纹理图像，我们将它作为一个颜色附件附加到帧缓冲上。我们将纹理的维度设置为窗口的宽度和高度，并且不初始化它的数据
    //----------------------------------------------------------------------------------------------------------------------
    ////创建颜色附着纹理
    //unsigned int textureColorBuffer;
    //glGenTextures(1, &textureColorBuffer);
    //glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);//GL_RGB/GL_RGB16F --- GL_UNSIGNED_BYTE/GL_FLOAT
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glBindTexture(GL_TEXTURE_2D, 0);
    //// 将它附加到当前绑定的帧缓冲对象
    //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);

    //当使用泛光操作时
    // ---------------
    //通过使用GL_COLOR_ATTACHMENT1，我们可以得到一个附加了两个颜色缓冲的帧缓冲对象：
    GLuint textureColorBuffer[2];
    glGenTextures(2, textureColorBuffer);
    for (GLuint i = 0; i < 2; i++)
    {
        glBindTexture(GL_TEXTURE_2D, textureColorBuffer[i]);
        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL//这里使用FLOAT类型载入浮点缓冲区
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        // attach texture to framebuffer
        glFramebufferTexture2D(
            GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textureColorBuffer[i], 0
        );
    }

    //创建一个深度和模板附件渲染缓冲对象
    //-------------------------------------
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
    //为渲染缓冲对象分配了足够的内存之后，可以解绑这个渲染缓冲
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    //将渲染缓冲对象附加到帧缓冲的深度和模板附件上
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    //检查帧缓冲是否是完整
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    //解绑当前绑定帧缓冲
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

#pragma region 离屏MSAA
    //多重采样纹理附件
    // ----------------
    unsigned int tex_frameb;
    //创建一个支持储存多个采样点的纹理
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureColorBuffer[0]);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, multisampling, GL_RGB, SCR_WIDTH, SCR_HEIGHT, GL_TRUE);//第二个参数设置的是纹理所拥有的样本个数。如果最后一个参数为GL_TRUE，图像将会对每个纹素使用相同的样本位置以及相同数量的子采样点个数
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    //将多重采样纹理附加到帧缓冲上,当前绑定的帧缓冲现在就有了一个纹理图像形式的多重采样颜色缓冲
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureColorBuffer[0], 0);

    //创建一个多重采样渲染缓冲对象,我们所要做的只是在指定（当前绑定的）渲染缓冲的内存存储时，将glRenderbufferStorage的调用改为glRenderbufferStorageMultisample就可以了。
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, multisampling, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);

    ////渲染到多重采样帧缓冲//因为多重采样缓冲有一点特别，我们不能直接将它们的缓冲图像用于其他运算，比如在着色器中对它们进行采样。
    ////glBlitFramebuffer函数会根据、GL_READ_FRAMEBUFFER\GL_DRAW_FRAMEBUFFER、这两个目标，决定哪个是源帧缓冲，哪个是目标帧缓冲
    //glBindFramebuffer(GL_READ_FRAMEBUFFER, frameb);
    //glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    ////缩小或者还原(Resolve)图像,够将一个帧缓冲中的某个区域复制到另一个帧缓冲中，并且将多重采样缓冲还原
    //glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);
#pragma endregion


    //为图像的模糊处理创建两个基本的帧缓冲，每个只有一个颜色缓冲纹理
    GLuint pingpongFBO[2];
    GLuint pingpongBuffer[2];
    glGenFramebuffers(2, pingpongFBO);
    glGenTextures(2, pingpongBuffer);
    for (GLuint i = 0; i < 2; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
        glBindTexture(GL_TEXTURE_2D, pingpongBuffer[i]);
        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(
            GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongBuffer[i], 0
        );
    }

    //记得要解绑帧缓冲，保证我们不会不小心渲染到错误的帧缓冲上
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //现在这个帧缓冲就完整了，我们只需要绑定这个帧缓冲对象，让渲染到帧缓冲的缓冲中而不是默认的帧缓冲中。
    //之后的渲染指令将会影响当前绑定的帧缓冲。所有的深度和模板操作都会从当前绑定的帧缓冲的深度和模板附件中（如果有的话）读取。
    //如果你忽略了深度缓冲，那么所有的深度测试操作将不再工作，因为当前绑定的帧缓冲中不存在深度缓冲。
    //
    //所以，要想绘制场景到一个纹理上，我们需要采取以下的步骤：
    //-将新的帧缓冲绑定为激活的帧缓冲，和往常一样渲染场景
    //-绑定默认的帧缓冲
    //-绘制一个横跨整个屏幕的四边形，将帧缓冲的颜色缓冲作为它的纹理。
    
    //-为屏幕四边形创建并配置一个VAO
    // --------------------------------。。。

    float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
    // positions   // texCoords
    -1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
     1.0f, -1.0f,  1.0f, 0.0f,

    -1.0f,  1.0f,  0.0f, 1.0f,
     1.0f, -1.0f,  1.0f, 0.0f,
     1.0f,  1.0f,  1.0f, 1.0f
    };

    // screen quad VAO
    unsigned int quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    //加载帧缓冲时需要使用的屏幕着色器
    Shader screenShader("F:/Resources/C++/Hello Under World-Cube/Debug/Shader/frame-buffer.vert", "F:/Resources/C++/Hello Under World-Cube/Debug/Shader/frame-buffer.frag");
    //设置着色器对应纹理图像的id
    screenShader.setInt("screenTexture", 0);


    //加载帧缓冲时需要使用的屏幕着色器
    Shader screenShaderFinal("F:/Resources/C++/Hello Under World-Cube/Debug/Shader/frame-buffer.vert", "F:/Resources/C++/Hello Under World-Cube/Debug/Shader/frame-buffer-final.frag");
    //设置着色器对应纹理图像的id
    screenShaderFinal.setInt("screenTexture", 0);
    screenShaderFinal.setInt("bloomBlur", 0);
    screenShaderFinal.setFloat("exposure", 1.0f);//曝光值
#pragma endregion

#pragma region 创建立方体贴图

    std::cout << "创建立方体贴图" << endl;

    ////将合适的纹理路径按照立方体贴图枚举指定的顺序加载到一个vector中
    //vector<std::string> textures_faces
    //{
    //    "right.jpg",
    //    "left.jpg",
    //    "top.jpg",
    //    "bottom.jpg",
    //    "front.jpg",
    //    "back.jpg"
    //};
    ////生成一个纹理，并将其绑定到纹理目标
    //unsigned int textureID;
    //glGenTextures(1, &textureID);
    //glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    ////需要调用glTexImage2D函数6次
    //int width, height, nrChannels;
    //unsigned char* data;
    //for (unsigned int i = 0; i < textures_faces.size(); i++)
    //{
    //    data = stbi_load(textures_faces[i].c_str(), &width, &height, &nrChannels, 0);
    //    glTexImage2D(
    //        //OpenGL给我们提供了6个特殊的纹理目标，专门对应立方体贴图的一个面
    //        //GL_TEXTURE_CUBE_MAP_POSITIVE_X	右
    //        //GL_TEXTURE_CUBE_MAP_NEGATIVE_X	左
    //        //GL_TEXTURE_CUBE_MAP_POSITIVE_Y	上
    //        //GL_TEXTURE_CUBE_MAP_NEGATIVE_Y	下
    //        //GL_TEXTURE_CUBE_MAP_POSITIVE_Z	后
    //        //GL_TEXTURE_CUBE_MAP_NEGATIVE_Z	前
    //        GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
    //        0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
    //    );
    //}
    ////需要设定它的环绕和过滤方式
    //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    ////仅仅是为纹理的R坐标设置了环绕方式，它对应的是纹理的第三个维度（和位置的z一样）
    ////我们将环绕方式设置为GL_CLAMP_TO_EDGE，这是因为正好处于两个面之间的纹理坐标可能不能击中一个面（由于一些硬件限制），所以通过使用GL_CLAMP_TO_EDGE，OpenGL将在我们对两个面之间采样的时候，永远返回它们的边界值。
    //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    //将合适的纹理路径按照立方体贴图枚举指定的顺序加载到一个vector中
    vector<std::string> faces
    {
        "F:/Resources/Image/skybox/right.jpg",
        "F:/Resources/Image/skybox/left.jpg",
        "F:/Resources/Image/skybox/top.jpg",
        "F:/Resources/Image/skybox/bottom.jpg",
        "F:/Resources/Image/skybox/front.jpg",
        "F:/Resources/Image/skybox/back.jpg"
    };


    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    //获得天空盒子纹理对象对应的ID
    unsigned int cubemapTexture = loadCubemap(faces);

    Shader skyboxShader("F:/Resources/C++/Hello Under World-Cube/Debug/Shader/cube-vec3.vert", "F:/Resources/C++/Hello Under World-Cube/Debug/Shader/cube-vec3.frag");
    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);

#pragma endregion

#pragma region 环境映射

    std::cout << "环境映射" << endl;

    float vertices_Environment[] = {
            // positions          // normals
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

    //将合适的纹理路径按照立方体贴图枚举指定的顺序加载到一个vector中
    vector<std::string> faces_EnvironmentMapping
    {
        "F:/Resources/Image/skybox/right.jpg",
        "F:/Resources/Image/skybox/left.jpg",
        "F:/Resources/Image/skybox/top.jpg",
        "F:/Resources/Image/skybox/bottom.jpg",
        "F:/Resources/Image/skybox/front.jpg",
        "F:/Resources/Image/skybox/back.jpg"
    };

    unsigned int environmentVAO, environmentVBO;
    glGenVertexArrays(1, &environmentVAO);
    glGenBuffers(1, &environmentVBO);
    glBindVertexArray(environmentVAO);
    glBindBuffer(GL_ARRAY_BUFFER, environmentVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_Environment), &vertices_Environment, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    //获得环境映射纹理对象对应的ID
    glActiveTexture(GL_TEXTURE2);
    unsigned int skyboxTexture = loadCubemap(faces_EnvironmentMapping);

    Shader environmentShader("F:/Resources/C++/Hello Under World-Cube/Debug/Shader/environment.vert", "F:/Resources/C++/Hello Under World-Cube/Debug/Shader/environment.frag");
    environmentShader.use();
    environmentShader.setInt("skybox", 2);
#pragma endregion

#pragma region 使用uniform缓冲/高级GLSL

    std::cout << "使用uniform缓冲/高级GLSL" << endl;

    ////Uniform缓冲对象
    //// ----------------
    ////创建一个Uniform缓冲对象
    //unsigned int uboExampleBlock;
    //glGenBuffers(1, &uboExampleBlock);
    ////绑定到GL_UNIFORM_BUFFER目标
    //glBindBuffer(GL_UNIFORM_BUFFER, uboExampleBlock);
    ////分配足够的内存
    //glBufferData(GL_UNIFORM_BUFFER, 152, NULL, GL_STATIC_DRAW); // 分配152字节的内存
    //glBindBuffer(GL_UNIFORM_BUFFER, 0);

    ////每当我们需要对缓冲更新或者插入数据，我们都会绑定到uboExampleBlock，并使用glBufferSubData来更新它的内存
    ////我们只需要更新这个Uniform缓冲一次，所有使用这个缓冲的着色器就都使用的是更新后的数据了
    //// 
    ////在OpenGL上下文中，定义了一些绑定点(Binding Point)，我们可以将一个Uniform缓冲链接至它
    ////在创建Uniform缓冲之后，我们将它绑定到其中一个绑定点上，并将着色器中的Uniform块绑定到相同的绑定点，把它们连接到一起
    //// 
    //// Uniform块索引(Uniform Block Index)
    //// ----------------------------------
    ////Uniform块索引(Uniform Block Index)是着色器中已定义Uniform块的位置值索引
    //unsigned int lights_index = glGetUniformBlockIndex(ourShader.ID, "Lights");//获取Uniform块的位置值索引
    ////将uniform块链接到绑定点2
    //glUniformBlockBinding(ourShader.ID, lights_index, 2);//第一个参数是一个程序对象,一个Uniform块索引,链接到的绑定点

    //// 还需要绑定Uniform缓冲对象到相同的绑定点上
    //// -----------------------------------------------------
    ////这可以使用glBindBufferBase或glBindBufferRange来完成。
    //glBindBufferBase(GL_UNIFORM_BUFFER, 2, uboExampleBlock);//一个目标，一个绑定点索引和一个Uniform缓冲对象
    //// 或
    ////让多个不同的Uniform块绑定到同一个Uniform缓冲对象上
    //glBindBufferRange(GL_UNIFORM_BUFFER, 2, uboExampleBlock, 0, 152);//一个目标，一个绑定点索引和一个Uniform缓冲对象,一个附加的偏移量和大小参数

    ////开始向Uniform缓冲中添加数据
    ////---------------------------
    ////用一个字节数组添加所有的数据
    //glBindBuffer(GL_UNIFORM_BUFFER, uboExampleBlock);
    //// GLSL中的bool是4字节的，所以我们将它存为一个integer
    //int b = true; 
    ////或者更新缓冲的一部分
    //glBufferSubData(GL_UNIFORM_BUFFER, 144, 4, &b);
    //glBindBuffer(GL_UNIFORM_BUFFER, 0);
#pragma endregion

#pragma region 几何着色器

    std::cout << "几何着色器" << endl;

#pragma region 绘制二维房子

    std::cout << "绘制二维房子" << endl;

        //在标准化设备坐标的z平面上绘制四个点
    float points[] = {
        -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // top-left
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // top-right
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // bottom-right
        -0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // bottom-left
    };

    //几何着色器也需要编译和链接，但这次在创建着色器时我们将会使用GL_GEOMETRY_SHADER作为着色器类型：
    //unsigned int geometryShader;
    //geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
    //glShaderSource(geometryShader, 1, &gShaderCode, NULL);
    //glCompileShader(geometryShader);

    //glAttachShader(program, geometryShader);
    //glLinkProgram(program);



    Shader point_geom("F:/Resources/C++/Hello Under World-Cube/Debug/Shader/point-geom.vert", "F:/Resources/C++/Hello Under World-Cube/Debug/Shader/point-geom.frag", "F:/Resources/C++/Hello Under World-Cube/Debug/Shader/point-geom.geom");
    point_geom.use();

    unsigned int geomVAO, geomVBO;
    glGenBuffers(1, &geomVBO);
    glGenVertexArrays(1, &geomVAO);
    glBindVertexArray(geomVAO);
    glBindBuffer(GL_ARRAY_BUFFER, geomVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glBindVertexArray(0);
#pragma endregion

#pragma region 爆破物体

    std::cout << "爆破物体" << endl;

    Shader shader_explode("F:/Resources/C++/Hello Under World-Cube/Debug/Shader/explode-model.vert", "F:/Resources/C++/Hello Under World-Cube/Debug/Shader/explode-model.frag", "F:/Resources/C++/Hello Under World-Cube/Debug/Shader/explode-model.geom");
    shader_explode.use();
    Model model_explode("F:/Resources/C++/Hello Under World-Cube/Debug/Mode/nanosuit_reflection/nanosuit.obj");
#pragma endregion

#pragma region 毛发-法向量可视化

    std::cout << "毛发-法向量可视化" << endl;

    Shader shader_fur("F:/Resources/C++/Hello Under World-Cube/Debug/Shader/fur-normal.vert", "F:/Resources/C++/Hello Under World-Cube/Debug/Shader/fur-normal.frag", "F:/Resources/C++/Hello Under World-Cube/Debug/Shader/fur-normal.geom");
    shader_fur.use();
    //设置垂直加翻转
    //stbi_set_flip_vertically_on_load(true);
    //Model model_explode("F:/Resources/C++/Hello Under World-Cube/mode/nanosuit_reflection/nanosuit.obj");
#pragma endregion

#pragma endregion

#pragma region 实例化

    std::cout << "实例化" << endl;

    //渲染调用分别改为glDrawArraysInstanced和glDrawElementsInstanced,需要一个额外的参数，叫做实例数量(Instance Count)

    //设置偏移位置
    //------------
    glm::vec2 translations[100];
    int index = 0;
    float offset = 0.1f;
    for (int y = -10; y < 10; y += 2)
    {
        for (int x = -10; x < 10; x += 2)
        {
            glm::vec2 translation;
            translation.x = (float)x / 10.0f + offset;
            translation.y = (float)y / 10.0f + offset;
            translations[index++] = translation;
        }
    }
    
    //首先将（上一部分的）translations数组存到一个新的缓冲对象中：
    unsigned int instanceVBO;
    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 100, &translations[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    unsigned int grass_instanceVAO, grass_instanceVBO;
    //草实例化的VAO与VBO解析
    {
        float transparentVertices_grass_instance[] = {
            // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
            //位置                //纹理坐标（由于纹理上下颠倒，因此交换了y坐标）
            0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
            0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
            1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

            0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
            1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
            1.0f,  0.5f,  0.0f,  1.0f,  0.0f
        };
        //创建VAO
        glGenVertexArrays(1, &grass_instanceVAO);
        //创建VBO
        glGenBuffers(1, &grass_instanceVBO);
        //设置它为当前操作的VAO，之后我们所有关于顶点数据的设置（包括数据使用的VBO对象，顶点的属性设置的信息都会被存储在VAO之中），在设置完成之后一般会解绑VAO，然后在需要绘制的时候启用相应的VAO对象。
        glBindVertexArray(grass_instanceVAO);
        glBindBuffer(GL_ARRAY_BUFFER, grass_instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices_grass_instance), transparentVertices_grass_instance, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    //设置它的顶点属性指针，并启用顶点属性:
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //告诉了OpenGL该什么时候更新顶点属性的内容至新一组数据
    glVertexAttribDivisor(2, 1);//(需要的顶点属性，属性除数(Attribute Divisor))//glad_glVertexAttribDivisor//glVertexAttribDivisor
    //默认情况下，属性除数是0，告诉OpenGL我们需要在顶点着色器的每次迭代时更新顶点属性。
    //将它设置为1时，告诉OpenGL我们希望在渲染一个新实例的时候更新顶点属性。
    //而设置为2时，我们希望每2个实例更新一次属性，以此类推。
    //将属性除数设置为1，是在告诉OpenGL，处于位置值2的顶点属性是一个实例化数组。
    glBindVertexArray(0);

    Shader shader_instance("F:/Resources/C++/Hello Under World-Cube/Debug/Shader/instance.vert", "F:/Resources/C++/Hello Under World-Cube/Debug/Shader/model-loading.frag");
    shader_instance.use();
    shader_instance.setInt("texture_diffuse1", 0);

    ////将数据转移到着色器中      //使用uniform对象实现实例化
    //shader_instance.use();
    //for (unsigned int i = 0; i < 100; i++)
    //{
    //    stringstream ss;
    //    string index;
    //    //将for循环的计数器i转换为一个string，我们可以用它来动态创建位置值的字符串，用于uniform位置值的索引
    //    ss << i;
    //    index = ss.str();
    //    shader_instance.setVec2(("offsets[" + index + "]").c_str(), translations[i]);
    //}
#pragma endregion

#pragma region 自定义抗锯齿算法

    std::cout << "自定义抗锯齿算法" << endl;

    //将一个多重采样的纹理图像不进行还原直接传入着色器也是可行的。
    //GLSL提供了这样的选项，让我们能够对纹理图像的每个子样本进行采样，所以我们可以创建我们自己的抗锯齿算法。在大型的图形应用中通常都会这么做。

    //要想获取每个子样本的颜色值，你需要将纹理uniform采样器设置为sampler2DMS，而不是平常使用的sampler2D：
    //uniform sampler2DMS screenTextureMS;
    // 
    //使用texelFetch函数就能够获取每个子样本的颜色值了：
    //vec4 colorSample = texelFetch(screenTextureMS, TexCoords, 3);  // 第4个子样本
#pragma endregion

#pragma region Gamma校正

    std::cout << "Gamma校正" << endl;

    //在每个相关像素着色器运行的最后应用gamma校正，所以在发送到帧缓冲前，颜色就被校正了
    //void main()
    //{
    //    // do super fancy lighting 
    //    [...]
    //    // apply gamma correction
    //    float gamma = 2.2;
    //    //将fragColor的每个颜色元素应用有一个1.0/gamma的幂运算，校正像素着色器的颜色输出
    //    fragColor.rgb = pow(fragColor.rgb, vec3(1.0 / gamma));
    //}

    //重校，或把这些sRGB纹理在进行任何颜色值的计算前变回线性空间
    //float gamma = 2.2;
    //vec3 diffuseColor = pow(texture(diffuse, texCoords).rgb, vec3(gamma));

    //如果我们在OpenGL中创建了一个纹理，把它指定为以上两种sRGB纹理格式其中之一(GL_SRGB和GL_SRGB_ALPHA内部纹理格式)，
    //OpenGL将自动把颜色校正到线性空间中，这样我们所使用的所有颜色值都是在线性空间中的了。我们可以这样把一个纹理指定为一个sRGB纹理：
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    // 
    //因为不是所有纹理都是在sRGB空间中的所以当你把纹理指定为sRGB纹理时要格外小心。
    //比如diffuse纹理，这种为物体上色的纹理几乎都是在sRGB空间中的。
    //而为了获取光照参数的纹理，像specular贴图和法线贴图几乎都在线性空间中，所以如果你把它们也配置为sRGB纹理的话，光照就坏掉了。指定sRGB纹理时要当心。


    //光照衰减(Attenuation)
    //float attenuation = 1.0 / (distance * distance);
    //然而，当我们使用这个衰减公式的时候，衰减效果总是过于强烈，光只能照亮一小圈，看起来并不真实。
    // 出于这个原因，我们使用在基本光照教程中所讨论的那种衰减方程，它给了我们更大的控制权，此外我们还可以使用双曲线函数：
    //float attenuation = 1.0 / distance;
    // 
    //双曲线比使用二次函数变体在不用gamma校正的时候看起来更真实，不过但我们开启gamma校正以后线性衰减看起来太弱了，符合物理的二次函数突然出现了更好的效果
#pragma endregion

#pragma region 阴影映射-定向阴影映射

    std::cout << "阴影映射-定向阴影映射" << endl;

    //深度贴图
    //---------
    //加载相应着色器
    Shader simpleDepthShader("F:/Resources/C++/Hello Under World-Cube/Debug/Shader/simpleDepthShader.vert", "F:/Resources/C++/Hello Under World-Cube/Debug/Shader/simpleDepthShader.frag");
    simpleDepthShader.use();

    std::cout << "阴影映射" << endl;

    //为渲染的深度贴图创建一个帧缓冲对象
    GLuint depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
    //创建一个2D纹理，提供给帧缓冲的深度缓冲使用
    const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;//纹理的高宽设置为1024：这是深度贴图的分辨率

    GLuint depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
        SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);//因为只关心深度值,要把纹理格式指定为GL_DEPTH_COMPONENT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //环绕方式设置为GL_REPEAT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);//GL_CLAMP_TO_BORDER//GL_REPEAT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    //我们宁可让所有超出深度贴图的坐标的深度范围是1.0，这样超出的坐标将永远不在阴影之中。我们可以储存一个边框颜色，然后把深度贴图的纹理环绕选项设置为GL_CLAMP_TO_BORDER：
    GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    //把生成的深度纹理作为帧缓冲的深度缓冲
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    //显式告诉OpenGL我们不适用任何颜色数据进行渲染
    glDrawBuffer(GL_NONE);//我们需要的只是在从光的透视图下渲染场景的时候深度信息，所以颜色缓冲没有用。然而，不包含颜色缓冲的帧缓冲对象是不完整的，所以我们需要显式告诉OpenGL我们不适用任何颜色数据进行渲染。我们通过将调用glDrawBuffer和glReadBuffer把读和绘制缓冲设置为GL_NONE来做这件事。
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //合理配置将深度值渲染到纹理的帧缓冲后，我们就可以开始第一步了：生成深度贴图

    ////配置相应位置属性
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);//启用顶点属性0
#pragma endregion

#pragma region 阴影映射-点光阴影(万向阴影映射)

    std::cout << "阴影映射-点光阴影(万向阴影映射)" << endl;

    //创建一个光周围的深度值的立方体贴图，必须渲染场景6次：每次一个面
    //for (int i = 0; i < 6; i++)
    //{
    //    GLuint face = GL_TEXTURE_CUBE_MAP_POSITIVE_X + i;
    //    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, face, depthCubemap, 0);
    //    BindViewMatrix(lightViewMatrices[i]);
    //    RenderScene();
    //}

    //使用几何着色器来生成深度贴图不会一定比每个面渲染场景6次更快。使用几何着色器有它自己的性能局限，在第一个阶段使用它可能获得更好的性能表现。这取决于环境的类型，以及特定的显卡驱动等等，所以如果你很关心性能，就要确保对两种方法有大致了解，然后选择对你场景来说更高效的那个。

    ////几何着色器允许我们使用一次渲染过程来建立深度立方体贴图。
    ////创建一个立方体贴图：
    //GLuint depthCubemap;
    //glGenTextures(1, &depthCubemap);
    ////生成立方体贴图的每个面，将它们作为2D深度值纹理图像：
    ////const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    //glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
    //for (GLuint i = 0; i < 6; ++i)
    //    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
    //        SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    ////设置合适的纹理参数：
    //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    ////使用一个几何着色器，它允许我们把所有面在一个过程渲染，我们可以使用glFramebufferTexture直接把立方体贴图附加成帧缓冲的深度附件：
    //glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    //glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
    ////显式告诉OpenGL这个帧缓冲对象不会渲染到一个颜色缓冲里
    //glDrawBuffer(GL_NONE);
    //glReadBuffer(GL_NONE);
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);

    ///*伪码
    //// 1. first render to depth cubemap//1.首先渲染到深度立方体贴图
    //glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    //glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    //glClear(GL_DEPTH_BUFFER_BIT);
    //ConfigureShaderAndMatrices();
    //RenderScene();
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //// 2. then render scene as normal with shadow mapping (using depth cubemap)//2.然后使用阴影贴图将场景渲染为法线（使用深度立方体贴图）
    //glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //ConfigureShaderAndMatrices();
    //glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
    //RenderScene();
    //*/

    ////每个光空间变换矩阵使用同样的投影矩阵：
    //GLfloat aspect = (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT;
    //GLfloat x_near = 1.0f;
    //GLfloat x_far = 25.0f;
    //glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, x_near, x_far);//视野参数，设置为90度,保证视野足够大到可以合适地填满立方体贴图的一个面，立方体贴图的所有面都能与其他面在边缘对齐。
    ////创建视图矩阵，分别对应：立方体贴图的的一个方向：右、左、上、下、近、远
    //std::vector<glm::mat4> shadowTransforms;
    //shadowTransforms.push_back(shadowProj *
    //    glm::lookAt(pointLightPositions[1], pointLightPositions[1] + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
    //shadowTransforms.push_back(shadowProj *
    //    glm::lookAt(pointLightPositions[1], pointLightPositions[1] + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
    //shadowTransforms.push_back(shadowProj *
    //    glm::lookAt(pointLightPositions[1], pointLightPositions[1] + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
    //shadowTransforms.push_back(shadowProj *
    //    glm::lookAt(pointLightPositions[1], pointLightPositions[1] + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
    //shadowTransforms.push_back(shadowProj *
    //    glm::lookAt(pointLightPositions[1], pointLightPositions[1] + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
    //shadowTransforms.push_back(shadowProj *
    //    glm::lookAt(pointLightPositions[1], pointLightPositions[1] + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));
#pragma endregion

#pragma region 法线贴图

    std::cout << "法线贴图" << endl;

    ////手工计算切线和副切线
    //// -------------------
    ////假设平面使用下面的向量建立起来（1、2、3和1、3、4，它们是两个三角形）：

    //    // positions
    //    glm::vec3 pos1(-1.0, 1.0, 0.0);
    //glm::vec3 pos2(-1.0, -1.0, 0.0);
    //glm::vec3 pos3(1.0, -1.0, 0.0);
    //glm::vec3 pos4(1.0, 1.0, 0.0);
    //// texture coordinates
    //glm::vec2 uv1(0.0, 1.0);
    //glm::vec2 uv2(0.0, 0.0);
    //glm::vec2 uv3(1.0, 0.0);
    //glm::vec2 uv4(1.0, 1.0);
    //// normal vector
    //glm::vec3 nm(0.0, 0.0, 1.0);
    ////我们先计算第一个三角形的边和deltaUV坐标：

    //    glm::vec3 edge1 = pos2 - pos1;
    //glm::vec3 edge2 = pos3 - pos1;
    //glm::vec2 deltaUV1 = uv2 - uv1;
    //glm::vec2 deltaUV2 = uv3 - uv1;
    ////有了计算切线和副切线的必备数据，我们就可以开始写出来自于前面部分中的下列等式：

    //预先计算出等式的分数部分f，然后把它和每个向量的元素进行相应矩阵乘法。如果你把代码和最终的等式对比你会发现，这就是直接套用。最后我们还要进行标准化，来确保切线/副切线向量最后是单位向量。
    //    GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    //tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    //tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    //tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
    //tangent1 = glm::normalize(tangent1);

    //bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    //bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    //bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
    //bitangent1 = glm::normalize(bitangent1);

    ////[...] // 对平面的第二个三角形采用类似步骤计算切线和副切线
#pragma endregion

#pragma region 视差贴图

    std::cout << "视差贴图" << endl;

    //视差贴图属于位移贴图(Displacement Mapping)技术的一种
    //根据储存在纹理中的几何信息对顶点进行位移或偏移。一种实现的方式是比如有1000个顶点，根据纹理中的数据对平面特定区域的顶点的高度进行位移。这样的每个纹理像素包含了高度值纹理叫做高度贴图。
    //整个平面上的每个顶点都根据从高度贴图采样出来的高度值进行位移，根据材质的几何属性平坦的平面变换成凹凸不平的表面
    //置换顶点有一个问题就是平面必须由很多顶点组成才能获得具有真实感的效果，否则看起来效果并不会很好。一个平坦的表面上有1000个顶点计算量太大了。

    //位移贴图技术不需要额外的顶点数据来表达深度，它像法线贴图一样采用一种聪明的手段欺骗用户的眼睛。
    // 
    //反色高度贴图（也叫深度贴图）去模拟深度比模拟高度更容易
#pragma endregion

#pragma region HDR(High Dynamic Range, 高动态范围)

    std::cout << "HDR(High Dynamic Range, 高动态范围)" << endl;

    ////浮点帧缓冲
    //GLuint hdrFBO;
    //GLuint colorBuffer_HDR;
    ////创建一个浮点帧缓冲,只需要改变颜色缓冲的内部格式参数就行了
    //glBindTexture(GL_TEXTURE_2D, colorBuffer_HDR);
    ////帧缓冲的颜色缓冲的内部格式被设定成了GL_RGB16F, GL_RGBA16F, GL_RGB32F 或者GL_RGBA32F时，这些帧缓冲被叫做浮点帧缓冲(Floating Point Framebuffer)
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);//注意GL_FLOAT参数

    ////有了一个带有浮点颜色缓冲的帧缓冲，我们可以放心渲染场景到这个帧缓冲中
    ////先渲染一个光照的场景到浮点帧缓冲中，之后再在一个铺屏四边形(Screen-filling Quad)上应用这个帧缓冲的颜色缓冲
    //glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //// [...] 渲染(光照的)场景
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //// 现在使用一个不同的着色器将HDR颜色缓冲渲染至2D铺屏四边形上//渲染至浮点帧缓冲和渲染至一个普通的帧缓冲是一样的
    //hdrShader.Use();
    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, hdrColorBufferTexture);
    //RenderQuad();
#pragma endregion

#pragma region 泛光

    std::cout << "泛光" << endl;

    //使用一个叫做MRT（Multiple Render Targets，多渲染目标）的小技巧，这样我们就能指定多个像素着色器输出
    //在灯光着色器中添加一个新的输入对象：BrightColor，判断输入哪一个片段着色器
    //在使用MRT之前，要确保有多个颜色缓冲绑定到了帧缓冲对象上
    //通过使用GL_COLOR_ATTACHMENT1，我们可以得到一个附加了两个颜色缓冲的帧缓冲对象：

    Shader shaderBlur("F:/Resources/C++/Hello Under World-Cube/Debug/Shader/frame-buffer.vert", "F:/Resources/C++/Hello Under World-Cube/Debug/Shader/bloom.frag");
    shaderBlur.use();
    shaderBlur.setInt("Image", textureColorBuffer[true]);
#pragma endregion

#pragma region 延迟着色器

    std::cout << "延迟着色器" << endl;

    //首先获得所有物体的G缓冲（包含位置，法线，颜色，镜面强度），存储在显卡内存中（ 几何处理阶段）
    //然后渲染出一个方形图像，在图像上的每一个像素上运行光照片段着色器（着色器中寻找每一个个像素的G缓冲数据，并结合运行光照计算）

    //而为了克服不能进行混合的缺点等，将延迟着色与正向着色相混合（将混合等操作用正向着色渲染出来）
    //但因为，延迟渲染后的图像没有不同的深度值，所以需要将延迟渲染的这一部分图像的深度值复制到一个新的深度缓冲中（这个缓冲用于进行正向着色渲染）

    //为了更多的灯光光源能被引入场景，所以会采用一种概念为：光体积(Light Volumes)，计算光源的半径，知道它所能达到的有效空间范围
    //定义一个认为时黑暗的亮度值：比如0（亮度阈值）但阈值几乎不会等于0，所以需要在接近0 时将它截断
    //使用一个衰减方程来求出光的体积（半径值）
    //GLfloat constant = 1.0;
    //GLfloat linear = 0.7;
    //GLfloat quadratic = 1.8;
    //GLfloat lightMax = std::fmaxf(std::fmaxf(lightColor.r, lightColor.g), lightColor.b);
    //GLfloat radius =
    //    (-linear + std::sqrtf(linear * linear - 4 * quadratic * (constant - (256.0 / 5.0) * lightMax)))
    //    / (2 * quadratic);

    //随后可以将该衰减方程引入片段着色器中，且通过循环计算每一个灯光的体积（当然这在实际中是不行的）

    //由于GPU不擅长使用循环与分支语句运算，所以我们可以使用一个球体来模拟光的体积，这时只需要渲染受球体（球型）影响的物体即可
#pragma endregion

#pragma region SSAO（屏幕空间环境光遮蔽   Screen-Space Ambient Occlusion）

    std::cout << "SSAO（屏幕空间环境光遮蔽   Screen-Space Ambient Occlusion）" << endl;

    //对与铺平的四边形上的每一个片段都会进行一个深度检测，而根据某片段周边的深度值，可以求出一个遮蔽因子(Occlusion Factor)，用来减少或抵消环境光强度分量
    //遮蔽因子是通过采集某片段周围的球形核（kernel）中的各深度值，并与中间片段的深度值做对比得到的，高于当前中间片段的深度值的样本就是遮蔽因子
    //当前片段的核中的遮蔽因子越多，该片段获取的环境光强度就将越低
    //由于遮蔽因子的个数会影响到到程序性能或是环境光计算效果，所以将引入随机性（到）采样核心(Sample Kernel)的采样来限制生成的遮蔽因子个数
    //比如通过随机旋转采样核心，我们能在有限样本数量中得到高质量的结果，但是却也可能产生明显的噪波图案（可以通过模糊来消除噪波）

    //但球形的核心会产生一种灰蒙蒙的感觉，所以将不会使用球形的采样核心，转而使用一种沿物体表面法向量的半球体采样核心
    //这样，就将不会考虑到位于片段底部的几何体，进而减少了遮蔽因子，且消除掉灰蒙蒙的感觉

    //对于每一个片段，需要获得它的逐片段位置向量、逐片段的法线向量、逐片段的反射颜色、采样核心、用来旋转采样核心的随机旋转矢量
    //通过逐片段观察空间位置，我们可以将采样半球的核心对准当前片段的观察空间表面法线，随后采样核心会根据矢量稍微偏转一点，以获取遮蔽因子
    //由于已经有了位置和法线，只需要获取深度信息就好了，这里从glFragColor.z中获取（线性）深度值

    //且因在每个表面法向量生成半球核心很困难，所以会旋转在切线空间中生成半球样本

    //如果为每一个片段都生成一个核心的话，内存可能会很快就被耗尽，所以又会仅仅引入一个小的随机旋转向量纹理，平铺在屏幕上
    //这里创建一个4x4朝向切线空间中平面法线的随机旋转向量的数组，且创建一个包含随机旋转向量的4x4纹理；设定它的封装方法为GL_REPEAT，从而保证它合适地平铺在屏幕上。

    //由于需要储存SSAO的结果，所以创建一个帧缓冲对象用于存放结果，因为环境遮蔽结果的值只有一个（灰度值），所以颜色缓冲的内部格式只需要设置为GL_RED就行了
    //且在着色器中，记得将噪声纹理（一种用于计算随机值的纹理）平铺在屏幕中，平铺参数设置为GL_REPEAT时，随机的值将会在全屏不断重复
    //接下来迭代每一个核心，将样本由切线空间变化为观察空间中，对比深度值
    //然后又变换样本的空间为裁剪空间（），随后又进行透视划分，变换核心样本到屏幕空间，标准化设备坐标之后变换到[0.0, 1.0]范围以便我们使用它们去取样深度纹理：
    //最后比较样品当前深度值是否大于存储的当前片段深度值，如果大于，则添加到贡献因子中

    //而检测一个靠近表面边缘的片段时，核心将会考虑测试表面之下的表面的深度值，所以又需要引入一个范围检测（保证我们只当被测深度值在取样半径内时才会影响到遮蔽因子）

    //最后一步，我们需要将遮蔽贡献根据核心的大小标准化，并输出结果
#pragma endregion

#pragma region PBR
    std::cout << "PBR" << endl;
#pragma endregion

#pragma region 平面渲染

    std::cout << "平面渲染" << endl;

    GLfloat planeVertices[] = {
        // Positions          // Normals         // Texture Coords
        25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 0.0f,
        -25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 25.0f,
        -25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,

        25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 0.0f,
        25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 25.0f,
        -25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 25.0f
    };
    // Setup plane VAO
    GLuint planeVAO,planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glBindVertexArray(0);

    Shader plane_ground("F:/Resources/C++/Hello Under World-Cube/Debug/Shader/cube-multiple.vert", "F:/Resources/C++/Hello Under World-Cube/Debug/Shader/cube-multiple.frag");

    GLuint plane_wood1, plane_wood2;
    wenliduixiang_alpha(&plane_wood1, "F:/Resources/C++/Hello Under World-Cube/Debug/Image/wood.jpg");
    wenliduixiang_alpha(&plane_wood2, "F:/Resources/C++/Hello Under World-Cube/Debug/Image/wood.jpg");

#pragma region shader_use
    plane_ground.use();
    plane_ground.setInt("material.diffuse", 1);
    plane_ground.setInt("material.specular", 2);
    plane_ground.setVec3("viewPos", cameraPos);
    plane_ground.setFloat("material.shininess", 32.0f);

    plane_ground.setInt("envTexture", 3);
    plane_ground.setInt("shadowMap", 4);



    // directional light
    plane_ground.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
    plane_ground.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
    plane_ground.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
    plane_ground.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
    // point light 1
    plane_ground.setVec3("pointLights[0].position", pointLightPositions[0]);
    plane_ground.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
    plane_ground.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
    plane_ground.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
    plane_ground.setFloat("pointLights[0].constant", 1.0f);
    plane_ground.setFloat("pointLights[0].linear", 0.09);
    plane_ground.setFloat("pointLights[0].quadratic", 0.032);
    // point light 2
    plane_ground.setVec3("pointLights[1].position", pointLightPositions[1]);
    plane_ground.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
    plane_ground.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
    plane_ground.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
    plane_ground.setFloat("pointLights[1].constant", 1.0f);
    plane_ground.setFloat("pointLights[1].linear", 0.09);
    plane_ground.setFloat("pointLights[1].quadratic", 0.032);
    // point light 3
    plane_ground.setVec3("pointLights[2].position", pointLightPositions[2]);
    plane_ground.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
    plane_ground.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
    plane_ground.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
    plane_ground.setFloat("pointLights[2].constant", 1.0f);
    plane_ground.setFloat("pointLights[2].linear", 0.09);
    plane_ground.setFloat("pointLights[2].quadratic", 0.032);
    // point light 4
    plane_ground.setVec3("pointLights[3].position", pointLightPositions[3]);
    plane_ground.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
    plane_ground.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
    plane_ground.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
    plane_ground.setFloat("pointLights[3].constant", 1.0f);
    plane_ground.setFloat("pointLights[3].linear", 0.09);
    plane_ground.setFloat("pointLights[3].quadratic", 0.032);
    // spotLight
    plane_ground.setVec3("spotLight.position", cameraPos);
    plane_ground.setVec3("spotLight.direction", cameraFront);
    plane_ground.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
    plane_ground.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
    plane_ground.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
    plane_ground.setFloat("spotLight.constant", 1.0f);
    plane_ground.setFloat("spotLight.linear", 0.09);
    plane_ground.setFloat("spotLight.quadratic", 0.032);
    plane_ground.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
    plane_ground.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
#pragma endregion

    
#pragma endregion


    //渲染循环
    while (!glfwWindowShouldClose(window))
    {

#pragma region 每次循环开始
        //std::cout << "验证是否循环成功" << std::endl;

        // 每帧时间逻辑(per-frame time logic)
        //------------------------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //进程输入
        //--------
        processInput(window);

        //渲染清除
        //-------------
        //设置gl清除屏幕时用到的颜色
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        //清除颜色缓冲
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //别忘了清除模具缓冲区！
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // don't forget to clear the stencil buffer!

        //绑定顶点数组对象
        //glBindVertexArray(VAO1);
#pragma endregion

#pragma region 阴影映射-万向阴影(Omnidirectional Shadow)映射渲染
        /*伪码
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.Use();

        // ... send uniforms to shader (including light's far_plane value)

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);

        // ... bind other textures

        RenderScene();//这里的renderScene函数在一个大立方体房间中渲染一些立方体，它们散落在大立方体各处，光源在场景中央。
        */
#pragma endregion

#pragma region 阴影映射-定向阴影映射渲染
        //为了修复peter偏移，这里使用背面剔除
        glCullFace(GL_FRONT);
        //RenderSceneToDepthMap();
        //glCullFace(GL_BACK); // 不要忘记设回原先的culling face

        // 1. 首选渲染深度贴图
        //--------------------
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);//一定要记得调用glViewport。因为阴影贴图经常和我们原来渲染的场景（通常是窗口分辨率）有着不同的分辨率，我们需要改变视口（viewport）的参数以适应阴影贴图的尺寸
        //绑定相应帧缓冲
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        //清除深度缓冲
        glClear(GL_DEPTH_BUFFER_BIT);
        //ConfigureShaderAndMatrices();
        //将为光源使用正交投影矩阵，透视图将没有任何变形
        GLfloat near_plane = 1.0f, far_plane = 107.5f;//近景，远景
        glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
        //创建一个视图矩阵来变换每个物体，把它们变换到从光源视角可见的空间中         //glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp)//pointLightPositions[]
        glm::mat4 lightView = glm::lookAt(glm::vec3(-1.0f, 3.0f, 0.0f), glm::vec3(-1.0f, 3.0f, 0.0f) + glm::vec3(-1.0, -1.0, 0.0), glm::vec3(0.0f, -1.0f, -1.0)); ;//glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        //二者相结合为我们提供了一个光空间的变换矩阵
        glm::mat4 lightSpaceMatrix = lightProjection * lightView;

        //渲染深度缓冲
        simpleDepthShader.use();
        //设置光空间变换矩阵
        simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

        //RenderScene(simpleDepthShader);//使用深度贴图着色器渲染当前场景
#pragma region 渲染场景深度贴图

        //渲染加载的模型
        //---------------
        glm::mat4 model_shadow = glm::mat4(1.0f);
        model_shadow = glm::translate(model_shadow, glm::vec3(0.0f, 0.5f, 0.0f)); // translate it down so it's at the center of the scene
        model_shadow = glm::scale(model_shadow, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
        simpleDepthShader.setMat4("model", model_shadow);
        ourModel.Draw(simpleDepthShader);

        //渲染正方体
        //----------
        glBindVertexArray(cubeVAO);
        model_shadow = glm::mat4(1.0f);
        for (unsigned int i = 0; i < 10; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing

            model_shadow = glm::translate(model_shadow, cubePositions[i]);
            float angle = 20.0f * i;
            model_shadow = glm::rotate(model_shadow, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            simpleDepthShader.setMat4("model", model_shadow);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
#pragma endregion
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        //渲染完深度值后再使用正面剔除
        glCullFace(GL_BACK);

        // 2. 像往常一样渲染场景，但这次使用深度贴图
        //------------------------------------------
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        //ConfigureShaderAndMatrices();
        //记得绑定深度贴图
        //glBindTexture(GL_TEXTURE_2D, depthMap);
        //RenderScene();
#pragma endregion

#pragma region 帧缓冲第一处理阶段
        // 第一处理阶段(Pass)
        //-------------------
        glBindFramebuffer(GL_FRAMEBUFFER, frameb);//绑定自定义的帧缓冲对象
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);//绑定自定义的渲染缓冲对象
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 我们现在不使用模板缓冲
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
#pragma endregion

#pragma region 模型渲染

        //将投影矩阵传递给着色器（注意，在这种情况下，它可能会更改每一帧）// pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //设置着色器中对象的值
        modelShader.setMat4("projection", projection);
        // 摄影机/视图转换(camera/view transformation)
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        // don't forget to enable shader before setting uniforms
        modelShader.use();
        //视口矩阵/投影矩阵及设置
        modelShader.setMat4("projection", projection);
        modelShader.setMat4("view", view);

        modelShader.setInt("envTexture", 2);
        modelShader.setInt("shadowMap", 3);
        modelShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
#pragma region modelShader
        modelShader.setVec3("viewPos", cameraPos);
        modelShader.setFloat("material.shininess", 32.0f);
        // spotLight
        modelShader.setVec3("spotLight.position", cameraPos);
        modelShader.setVec3("spotLight.direction", cameraFront);
        modelShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        modelShader.setVec3("spotLight.diffuse", 0.5f, 1.0f, 1.0f);
        modelShader.setVec3("spotLight.specular", 0.5f, 1.0f, 1.0f);
        modelShader.setFloat("spotLight.constant", 1.0f);
        modelShader.setFloat("spotLight.linear", 0.5);
        modelShader.setFloat("spotLight.quadratic", 0.032);
        modelShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        modelShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

        // directional light
        modelShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        modelShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        modelShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        modelShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
        // point light 1
        modelShader.setVec3("pointLights[0].position", pointLightPositions[0]);
        modelShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        modelShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        modelShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        modelShader.setFloat("pointLights[0].constant", 1.0f);
        modelShader.setFloat("pointLights[0].linear", 0.09);
        modelShader.setFloat("pointLights[0].quadratic", 0.032);
        // point light 2
        modelShader.setVec3("pointLights[1].position", pointLightPositions[1]);
        modelShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        modelShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        modelShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        modelShader.setFloat("pointLights[1].constant", 1.0f);
        modelShader.setFloat("pointLights[1].linear", 0.09);
        modelShader.setFloat("pointLights[1].quadratic", 0.032);
        // point light 3
        modelShader.setVec3("pointLights[2].position", pointLightPositions[2]);
        modelShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        modelShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        modelShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        modelShader.setFloat("pointLights[2].constant", 1.0f);
        modelShader.setFloat("pointLights[2].linear", 0.09);
        modelShader.setFloat("pointLights[2].quadratic", 0.032);
        // point light 4
        modelShader.setVec3("pointLights[3].position", pointLightPositions[3]);
        modelShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        modelShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
        modelShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        modelShader.setFloat("pointLights[3].constant", 1.0f);
        modelShader.setFloat("pointLights[3].linear", 0.09);
        modelShader.setFloat("pointLights[3].quadratic", 0.032);
#pragma endregion

        //渲染加载的模型
        //---------------
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
        modelShader.setMat4("model", model);
        ourModel.Draw(modelShader);

        //在相应的纹理单元上绑定纹理
        //--------------------------
        //在绑定纹理之前先激活纹理单元
        glActiveTexture(GL_TEXTURE0);
        //绑定纹理单元
        glBindTexture(GL_TEXTURE_2D, texture1);//纹理单元GL_TEXTURE0默认总是被激活，所以我们在前面的例子里当我们使用glBindTexture的时候，无需激活任何纹理单元
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);

#pragma endregion

#pragma region 天空盒子渲染
        //通过取4x4矩阵左上角的3x3矩阵来移除变换矩阵的位移部分
        //将观察矩阵转换为3x3矩阵（移除位移），再将其转换回4x4矩阵，来达到类似的效果。
        glm::mat4 view_skybox = glm::mat4(glm::mat3(glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp)));
        //投影矩阵
        glm::mat4 projection_skybox = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        ////关闭深度写入
        //glDepthMask(GL_FALSE);
        //skyboxShader.use();
        //skyboxShader.setInt("skybox", cubemapTexture);
        //skyboxShader.setMat4("view", view_skybox);
        //skyboxShader.setMat4("projection", projection_skybox);
        //// ... 设置观察和投影矩阵
        //glBindVertexArray(skyboxVAO);
        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        //glDrawArrays(GL_TRIANGLES, 0, 36);
        //glDepthMask(GL_TRUE);
        // ... 绘制剩下的场景

#pragma endregion

#pragma region 环境映射渲染
        glEnable(GL_TEXTURE_2D);      // 启用纹理映射
        //glShadeModel(GL_SMOOTH);      // 启用阴影*滑

        glm::mat4 model_skybox = glm::mat4(1.0f); // 确保首先将矩阵初始化为单位矩阵(make sure to initialize matrix to identity matrix first)
        //设置模型矩阵的值
        //model_skybox = glm::translate(model_skybox, glm::vec3(-3.0f, 1.0f, 3.0f));
        //model_skybox = glm::scale(model_skybox, glm::vec3(0.5f, 0.5f, 0.5f));
        //设置旋转角度
        //float angle_skybox = 20.0f * 1;
        //将旋转动作加入模型矩阵中
        //model_skybox = glm::rotate(model_skybox, glm::radians(angle_skybox), glm::vec3(1.0f, 0.3f, 0.5f));
        //glm::mat4 view_skybox = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        //glm::mat4 projection_skybox = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        environmentShader.use();
        environmentShader.setInt("skybox", 2);
        environmentShader.setVec3("cameraPos", cameraPos);
        environmentShader.setMat4("model", model_skybox);
        environmentShader.setMat4("view", view);
        environmentShader.setMat4("projection", projection);

        glBindVertexArray(environmentVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);//GL_TEXTURE_CUBE_MAP//cubemapTexture
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
#pragma endregion

#pragma region 环境映射的模型加载
        // 获取Reflection 注意: AssImp对Reflection支持不好 所以这里采用ambient_map
        // 除了这里的代码 还需要修改对应的obj文件
        //std::vector<Texture> reflectionTexture;
        //this->processMaterial(materialPtr, sceneObjPtr, aiTextureType_AMBIENT, reflectionTexture);
        //textures.insert(textures.end(), reflectionTexture.begin(), reflectionTexture.end());

        //同时在加载模型，使用reflection map时，天空包围盒使用的纹理单元，要更新为3， 因为前面加载了其他的specular map、diffuse map、reflection map :
        //glActiveTexture(GL_TEXTURE3);
        //glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.getTextId());
        //glUniform1i(glGetUniformLocation(shader.programId, "envText"), 3);


        //片元着色器


        glm::mat4 projection_env = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view_env = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glm::mat4 model_env = glm::mat4(1.0f);
        model_env = glm::translate(model_env, glm::vec3(1.0f, 0.5f, 0.0f)); // translate it down so it's at the center of the scene
        model_env = glm::scale(model_env, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down

        modelShader_env.use();
        //视口矩阵/投影矩阵及设置
        modelShader_env.setMat4("projection", projection_env);
        modelShader_env.setMat4("view", view_env);
        modelShader_env.setMat4("model", model_env);
        
        modelShader_env.setVec3("cameraPos", cameraPos);

        //modelShader_env.setInt("texture_diffuse", cubemapTexture);
        //modelShader_env.setInt("texture_specular", cubemapTexture);
        //modelShader_env.setInt("texture_normal", cubemapTexture);

        //modelShader_env.setInt("materail.diffuse", texture1);
        //modelShader_env.setInt("materail.specular", texture2);

        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
        //glUniform1i(glGetUniformLocation(cubemapTexture, "envText"), 3);
        modelShader_env.setInt("envText", 2);


#pragma region modelEnv_Shader
        ////modelShader_env.setVec3("viewPos", cameraPos);
        ////modelShader_env.setFloat("material.shininess", 32.0f);
        //// spotLight
        //modelShader_env.setVec3("spotLight.position", cameraPos);
        //modelShader_env.setVec3("spotLight.direction", cameraFront);
        //modelShader_env.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        //modelShader_env.setVec3("spotLight.diffuse", 0.5f, 1.0f, 1.0f);
        //modelShader_env.setVec3("spotLight.specular", 0.5f, 1.0f, 1.0f);
        //modelShader_env.setFloat("spotLight.constant", 1.0f);
        //modelShader_env.setFloat("spotLight.linear", 0.5);
        //modelShader_env.setFloat("spotLight.quadratic", 0.032);
        //modelShader_env.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        //modelShader_env.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

        //// directional light
        //modelShader_env.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        //modelShader_env.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        //modelShader_env.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        //modelShader_env.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
        //// point light 1
        //modelShader_env.setVec3("pointLights[0].position", pointLightPositions[0]);
        //modelShader_env.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        //modelShader_env.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        //modelShader_env.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        //modelShader_env.setFloat("pointLights[0].constant", 1.0f);
        //modelShader_env.setFloat("pointLights[0].linear", 0.09);
        //modelShader_env.setFloat("pointLights[0].quadratic", 0.032);
        //// point light 2
        //modelShader_env.setVec3("pointLights[1].position", pointLightPositions[1]);
        //modelShader_env.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        //modelShader_env.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        //modelShader_env.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        //modelShader_env.setFloat("pointLights[1].constant", 1.0f);
        //modelShader_env.setFloat("pointLights[1].linear", 0.09);
        //modelShader_env.setFloat("pointLights[1].quadratic", 0.032);
        //// point light 3
        //modelShader_env.setVec3("pointLights[2].position", pointLightPositions[2]);
        //modelShader_env.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        //modelShader_env.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        //modelShader_env.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        //modelShader_env.setFloat("pointLights[2].constant", 1.0f);
        //modelShader_env.setFloat("pointLights[2].linear", 0.09);
        //modelShader_env.setFloat("pointLights[2].quadratic", 0.032);
        //// point light 4
        //modelShader_env.setVec3("pointLights[3].position", pointLightPositions[3]);
        //modelShader_env.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        //modelShader_env.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
        //modelShader_env.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        //modelShader_env.setFloat("pointLights[3].constant", 1.0f);
        //modelShader_env.setFloat("pointLights[3].linear", 0.09);
        //modelShader_env.setFloat("pointLights[3].quadratic", 0.032);
#pragma endregion

        modelEnv.Draw(modelShader_env);
#pragma endregion

#pragma region ourShader着色器设置

        //启用相应的着色器程序
        ourShader.use();
        //将投影矩阵传递给着色器（注意，在这种情况下，它可能会更改每一帧）// pass projection matrix to shader (note that in this case it could change every frame)
        //glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //设置着色器中对象的值
        ourShader.setMat4("projection", projection);
        // 摄影机/视图转换(camera/view transformation)
        //glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        ourShader.setMat4("view", view);
        //设置物体颜色
        //ourShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        //设置灯光位置
        glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
        //ourShader.setVec3("lightPos", lightPos);
        //设置摄像机位置
        //ourShader.setVec3("viewPos", cameraPos);

        ourShader.setInt("shadowMap", 2);
        ourShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        //ourShader.setInt("material.diffuse", texture1);
        //ourShader.setInt("material.specular", texture2);


        // spotLight
        ourShader.setVec3("spotLight.position", cameraPos);
        ourShader.setVec3("spotLight.direction", cameraFront);
        ourShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        ourShader.setVec3("spotLight.diffuse", 0.5f, 1.0f, 1.0f);
        ourShader.setVec3("spotLight.specular", 0.5f, 1.0f, 1.0f);
        ourShader.setFloat("spotLight.constant", 1.0f);
        ourShader.setFloat("spotLight.linear", 0.5);
        ourShader.setFloat("spotLight.quadratic", 0.032);
        ourShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        ourShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

        // directional light
        ourShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        ourShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        ourShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        ourShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
        // point light 1
        ourShader.setVec3("pointLights[0].position", pointLightPositions[0]);
        ourShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        ourShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        ourShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        ourShader.setFloat("pointLights[0].constant", 1.0f);
        ourShader.setFloat("pointLights[0].linear", 0.09);
        ourShader.setFloat("pointLights[0].quadratic", 0.032);
        // point light 2
        ourShader.setVec3("pointLights[1].position", pointLightPositions[1]);
        ourShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        ourShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        ourShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        ourShader.setFloat("pointLights[1].constant", 1.0f);
        ourShader.setFloat("pointLights[1].linear", 0.09);
        ourShader.setFloat("pointLights[1].quadratic", 0.032);
        // point light 3
        ourShader.setVec3("pointLights[2].position", pointLightPositions[2]);
        ourShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        ourShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        ourShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        ourShader.setFloat("pointLights[2].constant", 1.0f);
        ourShader.setFloat("pointLights[2].linear", 0.09);
        ourShader.setFloat("pointLights[2].quadratic", 0.032);
        // point light 4
        ourShader.setVec3("pointLights[3].position", pointLightPositions[3]);
        ourShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        ourShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
        ourShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        ourShader.setFloat("pointLights[3].constant", 1.0f);
        ourShader.setFloat("pointLights[3].linear", 0.09);
        ourShader.setFloat("pointLights[3].quadratic", 0.032);
#pragma endregion

#pragma region 随时间改变物体颜色
        //glm::vec3 lightColor;
        //lightColor.x = sin(glfwGetTime() * 2.0f);
        //lightColor.y = sin(glfwGetTime() * 0.7f);
        //lightColor.z = sin(glfwGetTime() * 1.3f);

        //glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // 降低影响
        //glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // 很低的影响

        //ourShader.setVec3("light.ambient", ambientColor);
        //ourShader.setVec3("light.diffuse", diffuseColor);
#pragma endregion

#pragma region 原program

        ////启用相应的着色器程序
        //glUseProgram(program);

        ////将投影矩阵传递给着色器（注意，在这种情况下，它可能会更改每一帧）// pass projection matrix to shader (note that in this case it could change every frame)
        //glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //zhaoseqizhi(&program, "projection", projection);

        //// 摄影机/视图转换(camera/view transformation)
        //glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);//LookAt矩阵
        //zhaoseqizhi(&program, "view", view);

#pragma endregion

#pragma region 第一次渲染模具
        glm::mat4 modelx = glm::mat4(1.0f);
        //view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);;
        //projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);


        //ourShader.setMat4("view", view);
        //ourShader.setMat4("projection", projection);

        // draw floor as normal, but don't write the floor to the stencil buffer, we only care about the containers. We set its mask to 0x00 to not write to the stencil buffer.
        //按常规绘制地板，但不要将地板写入模具缓冲区，我们只关心容器。我们将其掩码设置为0x00，以不写入模具缓冲区。
        //控制模板平面中各个位的写入(指定位掩码,以启用和禁用模板平面中各个位的写入。最初，mask全是1,为启用写入)
        //glStencilMask(0x00);
        // floor
        //glBindVertexArray(planeVAO);
        //glBindTexture(GL_TEXTURE_2D, floorTexture);
        //shader.setMat4("model", glm::mat4(1.0f));
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        //glBindVertexArray(0);

        // 1st. render pass, draw objects as normal, writing to the stencil buffer
        // 第一。渲染过程，按常规绘制对象，写入模具缓冲区
        // --------------------------------------------------------------------
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
        // cubes
        glBindVertexArray(cubeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        modelx = glm::translate(modelx, glm::vec3(-1.0f, 0.0f, -1.0f));
        ourShader.setMat4("model", modelx);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        modelx = glm::mat4(1.0f);
        modelx = glm::translate(modelx, glm::vec3(2.0f, 0.0f, 0.0f));
        ourShader.setMat4("model", modelx);
        glDrawArrays(GL_TRIANGLES, 0, 36);
#pragma endregion

#pragma region 盒子渲染

        //渲染所有的盒子
        //--------------
        //绑定顶点数组对象
        glBindVertexArray(cubeVAO);
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        //循环10次
        for (unsigned int i = 0; i < 10; i++)
        {
            glBindVertexArray(cubeVAO);
            glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
            //计算每个对象的模型矩阵，并在绘制之前将其传递给着色器(calculate the model matrix for each object and pass it to shader before drawing)
            //-------------------------------------------------------------------------------------------------------------------------------------
            //创建一个模型矩阵
            glm::mat4 model = glm::mat4(1.0f); // 确保首先将矩阵初始化为单位矩阵(make sure to initialize matrix to identity matrix first)
            //设置模型矩阵的值
            model = glm::translate(model, cubePositions[i]);
            //设置旋转角度
            float angle = 20.0f * i;
            //将旋转动作加入模型矩阵中
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            //传递值到着色器中
            ourShader.setMat4("model", model);
#pragma region 原program
            ////传递值到着色器中
            //zhaoseqizhi(&program, "model", model);
#pragma endregion
            //以三角形的形式绘制图形,共有36个顶点


            ourShader.setInt("envTexture", skyboxTexture);

            glDrawArrays(GL_TRIANGLES, 0, 36);//图元类型，顶点数组起始索引，需要绘制的顶点的个数
        }

        //glm::mat4 modelxxx = glm::mat4(1.0f);
        //for (unsigned int i = 0; i < 10; i++)
        //{
        //    // calculate the model matrix for each object and pass it to shader before drawing

        //    modelxxx = glm::translate(modelxxx, cubePositions[i]);
        //    float angle = 20.0f * i;
        //    modelxxx = glm::rotate(modelxxx, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        //    ourShader.setMat4("model", modelxxx);

        //    glDrawArrays(GL_TRIANGLES, 0, 36);
        //}

        glBindVertexArray(0);
#pragma endregion

#pragma region 平台渲染
        glBindVertexArray(planeVAO);

        glm::mat4 model_plane = glm::mat4(1.0f);
        model_plane = glm::translate(model, glm::vec3(0.0, -5.0, 20.0));

        //在相应的纹理单元上绑定纹理
        //--------------------------
        //在绑定纹理之前先激活纹理单元
        glActiveTexture(GL_TEXTURE0);
        //绑定纹理单元
        glBindTexture(GL_TEXTURE_2D, plane_wood1);//纹理单元GL_TEXTURE0默认总是被激活，所以我们在前面的例子里当我们使用glBindTexture的时候，无需激活任何纹理单元
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, plane_wood2);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, depthMap);

        plane_ground.use();
        plane_ground.setMat4("projection", projection);
        plane_ground.setMat4("view", view);
        plane_ground.setMat4("model", model_plane);

        plane_ground.setMat4("lightSpaceMatrix", lightSpaceMatrix);

        plane_ground.setInt("material.diffuse", 0);
        plane_ground.setInt("material.specular", 1);
        plane_ground.setInt("envTexture", 2);
        plane_ground.setInt("depthMap", 3);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        glBindVertexArray(0);
#pragma endregion


#pragma region 混合-渲染草
        // transparent window locations
        // 透明窗口的位置
        // --------------------------------
        vector<glm::vec3> windows
        {
            glm::vec3(-1.5f, 0.0f, -0.48f),
            glm::vec3(1.5f, 0.0f, 0.51f),
            glm::vec3(0.0f, 0.0f, 0.7f),
            glm::vec3(-0.3f, 0.0f, -2.3f),
            glm::vec3(0.5f, 0.0f, -0.6f)
        };

        // sort the transparent windows before rendering
        // 渲染前对透明窗口进行排序
        // ---------------------------------------------
        std::map<float, glm::vec3> sorted;
        for (unsigned int i = 0; i < windows.size(); i++)
        {
            float distance = glm::length(cameraPos - windows[i]);
            sorted[distance] = windows[i];
        }

        BlendingShader.use();
        BlendingShader.setMat4("view", view);
        BlendingShader.setMat4("projection", projection);

        //在相应的纹理单元上绑定纹理
        //--------------------------
        //在绑定纹理之前先激活纹理单元
        glActiveTexture(GL_TEXTURE0);
        //绑定纹理单元
        //纹理单元GL_TEXTURE0默认总是被激活，所以我们在前面的例子里当我们使用glBindTexture的时候，无需激活任何纹理单元

        //绑定相应的顶点对象
        glBindVertexArray(vegetationVAO);
        glBindTexture(GL_TEXTURE_2D, texture_grass);
        //窗口（从最远到最近）
        for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
        {
            glm::mat4 modelb = glm::mat4(1.0f);
            modelb = glm::translate(modelb, it->second);
            BlendingShader.setMat4("model", modelb);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
#pragma endregion

#pragma region 实例化
        glBindVertexArray(grass_instanceVAO);
        shader_instance.use();
        shader_instance.setMat4("view", view);
        shader_instance.setMat4("projection", projection);
        glm::mat4 modelb = glm::mat4(1.0f);
        modelb = glm::translate(modelb, windows[1]);
        shader_instance.setMat4("model", modelb);
        //渲染实例化
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100);
#pragma endregion

#pragma region 第二次渲染模具

        // set uniforms
        lightShader.use();
        lightShader.setMat4("view", view);
        lightShader.setMat4("projection", projection);

        // 2nd. render pass: now draw slightly scaled versions of the objects, this time disabling stencil writing.
        // Because the stencil buffer is now filled with several 1s. The parts of the buffer that are 1 are not drawn, thus only drawing 
        // the objects' size differences, making it look like borders.
        // //第二。渲染过程：现在绘制稍微缩放的对象版本，这次禁用模具写入。因为模具缓冲区现在填充了几个1。缓冲区中为1的部分未绘制，因此仅绘制对象的大小不同，使其看起来像边框。
        // -----------------------------------------------------------------------------------------------------------------------------
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);
        glDisable(GL_DEPTH_TEST);
        lightShader.use();
        float scale = 1.1;
        // cubes
        glBindVertexArray(cubeVAO);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glm::mat4 model1 = glm::mat4(1.0f);
        model1 = glm::translate(model1, glm::vec3(-1.0f, 0.0f, -1.0f));
        model1 = glm::scale(model1, glm::vec3(scale, scale, scale));//对模型进行缩放
        lightShader.setMat4("model", model1);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model1 = glm::mat4(1.0f);
        model1 = glm::translate(model1, glm::vec3(2.0f, 0.0f, 0.0f));
        model1 = glm::scale(model1, glm::vec3(scale, scale, scale));
        lightShader.setMat4("model", model1);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glStencilMask(0xFF);
        glStencilFunc(GL_ALWAYS, 0, 0xFF);
        glEnable(GL_DEPTH_TEST);

#pragma endregion

#pragma region 灯光渲染
        //渲染所有的灯光
        //--------------
        //使用相应的着色器程序
        lightShader.use();
        //将投影矩阵传递给着色器（注意，在这种情况下，它可能会更改每一帧）// pass projection matrix to shader (note that in this case it could change every frame)
        //glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //设置着色器中对象的值
        lightShader.setMat4("projection", projection);
        // 摄影机/视图转换(camera/view transformation)
        //glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        lightShader.setMat4("view", view);
        //绑定相应顶点数组对象
        glBindVertexArray(lightVAO);
        //glBindBuffer(GL_ARRAY_BUFFER,cubeVBO);
        if (true) {
            //灯光位置
            //glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
            //创建模型矩阵
            glm::mat4 model = glm::mat4(1.0);
            //灯光位置等属性变换
            model = glm::translate(model, lightPos);
            model = glm::scale(model, glm::vec3(0.2f));
            // 设置模型、视图和投影矩阵uniform
            //
            lightShader.setMat4("model", model);
            // 绘制灯立方体对象
            glDrawArrays(GL_TRIANGLES, 0, 36);

            // we now draw as many light bulbs as we have point lights.
            glBindVertexArray(lightVAO);
            for (unsigned int i = 0; i < 4; i++)
            {
                model = glm::mat4(1.0f);
                model = glm::translate(model, pointLightPositions[i]);
                model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
                lightShader.setMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }
#pragma endregion

#pragma region 天空盒子_优化-后渲染（欺骗深度缓冲）
        // draw skybox as last
        //改变一下深度函数，将它从默认的GL_LESS改为GL_LEQUAL。深度缓冲将会填充上天空盒的1.0值，所以我们需要保证天空盒在值小于或等于深度缓冲而不是小于时通过深度测试
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        skyboxShader.use();
        view_skybox = glm::mat4(glm::mat3(glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp)));// remove translation from the view matrix
        skyboxShader.setMat4("view", view_skybox);
        skyboxShader.setMat4("projection", projection_skybox);
        // skybox cube
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default
#pragma endregion

#pragma region 几何着色器

#pragma region 绘制二维房子

        //// draw points
        //point_geom.use();
        //glBindVertexArray(geomVAO);
        //glDrawArrays(GL_POINTS, 0, 4);
#pragma endregion

#pragma region 爆破物体
        // configure transformation matrices
        glm::mat4 projection_explode = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 1.0f, 100.0f);
        glm::mat4 view_explode = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glm::mat4 modelx_explode = glm::mat4(1.0f);
        modelx_explode = glm::translate(model, glm::vec3(-3.0f, 0.5f, 0.0f));//设置模型位置
        modelx_explode = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));//设置模型的缩放
        shader_explode.use();
        shader_explode.setMat4("projection", projection_explode);
        shader_explode.setMat4("view", view_explode);
        shader_explode.setMat4("model", modelx_explode);

        // add time component to geometry shader in the form of a uniform
        shader_explode.setFloat("time", glfwGetTime());

        // draw model
        model_explode.Draw(shader_explode);
#pragma endregion

#pragma region 毛发-法向量可视化
        shader_fur.use();
        shader_fur.setMat4("projection", projection_explode);
        shader_fur.setMat4("view", view_explode);
        shader_fur.setMat4("model", modelx_explode);

        model_explode.Draw(shader_explode);
#pragma endregion

#pragma endregion

#pragma region 帧缓冲第二处理阶段
#pragma region 离屏MSAA
        // 将多重采样缓冲还原到中介FBO上
        glBindFramebuffer(GL_READ_FRAMEBUFFER, frameb);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameb);//中介fbo
        ////缩小或者还原(Resolve)图像,够将一个帧缓冲中的某个区域复制到另一个帧缓冲(当前绑定帧缓冲)中，并且将多重采样缓冲还原
        glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);
#pragma endregion
        // 第二处理阶段
        //---------------
        glBindFramebuffer(GL_FRAMEBUFFER, 0); // 返回默认
        //解绑这个渲染缓冲
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        //禁用深度测试，这样屏幕空间四元不会因为深度测试而被丢弃。
        glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
#pragma endregion

#pragma region 泛光
        //使用屏幕图像着色器
        screenShader.use();
        //指定相应使用的泛光图片
        screenShader.setInt("bloomBlur", textureColorBuffer[false]);
        glBindVertexArray(quadVAO);
        glDisable(GL_DEPTH_TEST);
        //glBindTexture(GL_TEXTURE_2D, textureColorBuffer[0]);

        //使用泛光操作时
        // --------------
        //显示的告诉OpenGL，正在通过glDrawBuffers渲染到多个颜色缓冲
        GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
        //第一次渲染为正常画布，第二次渲染为非正常画布(进行泛光操作)
        glDrawBuffers(2, attachments);

        glBindTexture(GL_TEXTURE_2D, textureColorBuffer[0]);
        glDrawArrays(GL_TRIANGLES, 0, 6);



        ////高斯模糊渲染
        //// ------------2.使用两次高斯模糊对明亮碎片进行模糊
        ////得到一个HDR纹理后，我们用提取出来的亮区纹理填充一个帧缓冲，然后对其模糊处理10次（5次垂直5次水平）：
        ////是否水平或垂直
        //GLboolean horizontal = true, first_iteration = true;//第一次迭代
        //GLuint amount = 10;
        //shaderBlur.use();
        //for (GLuint i = 0; i < amount; i++)
        //{
        //    //绑定好相应的颜色缓冲模式
        //    glBindFramebuffer(GL_FRAMEBUFFER, pingpongBuffer[horizontal]);
        //    shaderBlur.setBool("horizontal", horizontal);
        //    glBindTexture(
        //        GL_TEXTURE_2D, first_iteration ? textureColorBuffer[1] : pingpongBuffer[!horizontal]
        //    );


        //    //RenderQuad();
        //    glBindTexture(GL_TEXTURE_2D, textureColorBuffer[0]);
        //    glDrawArrays(GL_TRIANGLES, 0, 6);

        //    //每次循环交换水平或垂直选项
        //    horizontal = !horizontal;
        //    //判断是否为第一次迭代
        //    if (first_iteration)
        //        first_iteration = false;
        //}
        //glBindFramebuffer(GL_FRAMEBUFFER, 0);
#pragma endregion

#pragma region 帧缓冲后显示最终结果
        ////3.现在将浮点颜色缓冲区渲染为2D四边形，并将色调映射HDR颜色渲染为默认帧缓冲区（钳制）的颜色范围
        //glClear(GL_COLOR_BUFFER_BIT);// | GL_DEPTH_BUFFER_BIT


        ////使用屏幕图像着色器
        //screenShaderFinal.use();
        ////第一个图片
        //screenShaderFinal.setInt("screenTexture", textureColorBuffer[0]);//textureColorBuffer[0]
        //screenShaderFinal.setInt("bloomBlur", pingpongBuffer[0]);//textureColorBuffer[1]

        ////绑定相应图片
        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, textureColorBuffer[0]);
        //glActiveTexture(GL_TEXTURE1);
        //glBindTexture(GL_TEXTURE_2D, pingpongBuffer[0]);
        //glBindVertexArray(quadVAO);
        //glDisable(GL_DEPTH_TEST);
        //glBindTexture(GL_TEXTURE_2D, textureColorBuffer[0]);
        //glDrawArrays(GL_TRIANGLES, 0, 6);

#pragma endregion

        //glfw：交换缓冲区和轮询IO事件（按键按下/释放、鼠标移动等）// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // ------------------------------------------------------------------------------------------------------------------------------------------
        //交换颜色缓冲
        glfwSwapBuffers(window);
        //SwapBuffers(hdc);
        //检查有没有出发什么事件（比如键盘输入，鼠标移动等）
        glfwPollEvents();
    }
    // 可选：一旦资源超出其用途，则取消分配所有资源：//optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &cubeVBO);

    // glfw：终止，清除以前分配的所有glfw资源。//glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------------------------------------------------
    glfwTerminate();
    char* a = NULL;
    try {
        std::cin >> a;
    }
    catch(std::ifstream::failure& e){
        std::cout << "好了，你结束了你罪恶的一身";
    }

#pragma region 销毁OpenGL上下文
    //// 销毁OpenGL上下文
    //if (hglrc)     // Do We Have A Rendering Context?  
    //{
    //    // Are We Able To Release The DC And RC Contexts?  
    //    if (!wglMakeCurrent(NULL, NULL))
    //    {
    //        std::cout << "Release HGLRC failed." << endl;
    //    }

    //    if (!wglDeleteContext(hglrc))  // Are We Able To Delete The HGLRC?  
    //    {
    //        std::cout << "Release Rendering Context Failed." << endl;
    //    }
    //    hglrc = NULL;     // Set RC To NULL  
    //}
#pragma endregion

    return 0;
}

//------
//处理所有输入：查询GLFW是否在此帧中按下/释放相关键并做出相应反应//process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
//------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = 2.5 * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

//------
//glfw：每当窗口大小改变（由操作系统或用户调整大小）时，都会执行此回调函数//glfw: whenever the window size changed (by OS or user resize) this callback function executes
//------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

//------
//glfw：只要鼠标移动，就会调用此回调//glfw: whenever the mouse moves, this callback is called
//------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

//------
//glfw：每当鼠标滚轮滚动时，调用此回调//glfw: whenever the mouse scroll wheel scrolls, this callback is called
//------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}

//------
//加载天空盒子（立方体贴图）
//------
unsigned int loadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrComponents;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrComponents, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

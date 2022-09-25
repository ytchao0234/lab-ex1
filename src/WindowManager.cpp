#include <WindowManager.h>

WindowManager::WindowManager(string title, int width, int height)
{
    mTitle = title;
    mWidth = width;
    mHeight = height;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    mWindow = glfwCreateWindow(mWidth, mHeight, mTitle.c_str(), NULL, NULL);
    if (mWindow == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(mWindow);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
        glfwTerminate();
    }

    glViewport(0, 0, mWidth, mHeight);
}

WindowManager::~WindowManager()
{
    glfwDestroyWindow(mWindow);
}

void WindowManager::initCallbacks()
{
    glfwSetWindowUserPointer(mWindow, this);

    auto resizeCb = [](GLFWwindow* window, int width, int height){
        static_cast<WindowManager*>(glfwGetWindowUserPointer(window))->resizeCallback(window, width, height);
    };

    auto keyCb = [](GLFWwindow * window, int key, int scancode, int action, int mods){
        static_cast<WindowManager*>(glfwGetWindowUserPointer(window))->keyCallback(window, key, scancode, action, mods);
    };

    auto mouseMoveCb = [](GLFWwindow* window, double xpos, double ypos){
        static_cast<WindowManager*>(glfwGetWindowUserPointer(window))->mouseMoveCallback(window, xpos, ypos);
    };

    auto mousePressCb = [](GLFWwindow* window, int button, int action, int mods){
        static_cast<WindowManager*>(glfwGetWindowUserPointer(window))->mousePressCallback(window, button, action, mods);
    };

    auto scrollCb = [](GLFWwindow* window, double xoffset, double yoffset){
        static_cast<WindowManager*>(glfwGetWindowUserPointer(window))->scrollCallback(window, xoffset, yoffset);
    };

    glfwSetFramebufferSizeCallback(mWindow, resizeCb);
    glfwSetKeyCallback(mWindow, keyCb);
    glfwSetMouseButtonCallback(mWindow, mousePressCb);
    glfwSetCursorPosCallback(mWindow, mouseMoveCb);
    glfwSetScrollCallback(mWindow, scrollCb);
}

void WindowManager::initObjects()
{
    mCamera = new Camera(glm::vec3(0.0f, 0.0f, 10.0f), -90.0f, 0.0f);
    mShader = new Shader("src/Shaders/sample.vert", "src/Shaders/sample.frag");
    mCloth = new Cloth();
    mCloth->makeVertices();
}

void WindowManager::resizeCallback(GLFWwindow* window, int width, int height)
{
    mWidth = width;
    mHeight = height;

    glViewport(0, 0, width, height);
}

void WindowManager::keyCallback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
    if(action == GLFW_PRESS)
    {
        switch(key)
        {
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(mWindow, GLFW_TRUE);
                break;

            default:
                break;
        }
    }
}

void  WindowManager::mousePressCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        mLeftButtonIsPressing = true;
    else if( button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        mLeftButtonIsPressing = false;
}

void WindowManager::mouseMoveCallback(GLFWwindow* window, double posX, double posY)
{
    static bool firstPress = true;
    static float lastX, lastY;
    float offsetX, offsetY;

    if( mLeftButtonIsPressing )
    {
        if( firstPress )
        {
            lastX = posX;
            lastY = posY;
            firstPress = false;
            return;
        }

        offsetX = posX - lastX;
        offsetY = lastY - posY;

        lastX = posX;
        lastY = posY;

        const float sensitivity = 0.5f;
        offsetX *= sensitivity;
        offsetY *= sensitivity;

        mCamera->move(offsetX, offsetY);
    }

    firstPress = true;
}

void WindowManager::scrollCallback(GLFWwindow* window, double offsetX, double offsetY)
{
    mCamera->zoom(offsetY); 
}

void WindowManager::drawSample() const
{
    unsigned int VAO[1];
    unsigned int VBO[1];
    vector<float> vertices = 
    {
        0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    };

    glGenVertexArrays(1, VAO);
    glBindVertexArray(VAO[0]);

    glGenBuffers(1, VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    mShader->use();

    mShader->setMatrix4("projection", (float*)glm::value_ptr(mCamera->getProjection(mWidth, mHeight)));
    mShader->setMatrix4("view", (float*)glm::value_ptr(mCamera->getView()));

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
    mShader->setMatrix4("model", (float*)glm::value_ptr(model));

    glDisable(GL_CULL_FACE);

    glBindVertexArray(VAO[0]);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawArrays(GL_LINES, 0, vertices.size() / 6);
}
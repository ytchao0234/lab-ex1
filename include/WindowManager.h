#pragma once
#include <iostream>
#include <algorithm>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <Camera.h>
#include <Shader.h>
#include <Cloth.h>

using namespace std;

class WindowManager
{
private:
    string mTitle;
    int mWidth;
    int mHeight;
    bool mLeftButtonIsPressing = false;
    GLFWwindow* mWindow;
    Camera* mCamera;
    Shader* mShader;
    Cloth* mCloth;

public:
    WindowManager(string, int, int);
    ~WindowManager();

    void initCallbacks();
    void initObjects();
    void initImportList();

    void resizeCallback(GLFWwindow*, int, int);
    void keyCallback(GLFWwindow*, int, int, int, int);
    void mousePressCallback(GLFWwindow*, int, int, int);
    void mouseMoveCallback(GLFWwindow*, double, double);
    void scrollCallback(GLFWwindow*, double, double);

    void drawSample() const;

    int getWidth() { return mWidth; }
    int getHeight() { return mHeight; }
    GLFWwindow* getWindow() { return mWindow; }
    Camera* getCamera() { return mCamera; }
    Cloth* getCloth() { return mCloth; }
};
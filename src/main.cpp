#include <WindowManager.h>

int main()
{
    WindowManager wm("Rigid Cloth Behavior", 1200, 900);
    wm.initObjects();
    wm.initCallbacks();

    while(!glfwWindowShouldClose(wm.getWindow()))
    {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        wm.drawSample();
        wm.getCloth()->update();
        wm.getCloth()->render(wm.getCamera()->getProjection(wm.getWidth(), wm.getHeight()), wm.getCamera()->getView(), wm.getCamera()->getPosition());

        glfwPollEvents();
        glfwSwapBuffers(wm.getWindow());
    }

    glfwTerminate();
    return 0;
}
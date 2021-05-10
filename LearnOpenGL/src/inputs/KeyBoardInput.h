#pragma once

#include "GLFW/glfw3.h"

class KeyboardInput
{
public:
	static bool Keys[1024];
	inline static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
    {
        // when a user presses the escape key, we set the WindowShouldClose property to true, closing the application
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        if (key >= 0 && key < 1024)
        {
            if (action == GLFW_PRESS)
            {
                KeyboardInput::Keys[key] = true;
            }
            else if (action == GLFW_RELEASE)
            {
                KeyboardInput::Keys[key] = false;
            }
        }
    }
};



#include "../../include/hkui/inputHelpers/InputHelper.hpp"

#include <assert.h>

namespace inputHelpers
{

InputHelper& inputHelpers::InputHelper::get()
{
    static InputHelper instance;
    return instance;
}


/**
 * @brief Observe window and fire events accordingly.
 *
 * It sets internal callback listeners for events such as mouse movement/window resize/etc
 * that appear on the observed window. User has to **register** callbacks for when
 * those events fire.
 *
 * @param window GLFW window to be observed.
 */
void InputHelper::observe(GLFWwindow* window)
{
    gObservedWindow = window;

    /* Set key callback */
    glfwSetKeyCallback(window, keyCallback);

    /* Window resize callback */
    glfwSetFramebufferSizeCallback(window, windowResizeCallback);

    /* Window mouse button callback */
    glfwSetMouseButtonCallback(window, mouseButtonCallback);

    /* Mouse move callback */
    glfwSetCursorPosCallback(window, mouseMoveCallback);

    /* Mouse dropped items callback */
    glfwSetDropCallback(window, dropCallback);

}


// ------ STATIC CALLBACKS ------
/**
 * @brief Static gateway needed to access key input data from the OS.
 */
void InputHelper::keyCallback(GLFWwindow*, int key, int scanCode, int action, int mods)
{
    get().invokeOnKeyAction(key, scanCode, action, mods);
}


/**
 * @brief Static gateway needed to access window resize data from the OS.
 */
void InputHelper::windowResizeCallback(GLFWwindow*, int width, int height)
{
    get().invokeOnWindowResizeAction(width, height);
}


/**
 * @brief Static gateway needed to access window mouse button data from the OS.
 */
void InputHelper::mouseButtonCallback(GLFWwindow*, int button, int action, int mods)
{
    get().invokeOnMouseButtonAction(button, action, mods);
}


/**
 * @brief Static gateway needed to access window mouse position data from the OS.
 */
void InputHelper::mouseMoveCallback(GLFWwindow*, double xPos, double yPos)
{
    get().invokeOnMouseMoveAction(xPos, yPos);
}


/**
 * @brief Static gateway needed to access window mouse dropped items from the OS.
 */
void InputHelper::dropCallback(GLFWwindow*, int dropCount, const char** paths)
{
    get().invokeOnMouseDropAction(dropCount, paths);
}


// ------ INVOKE ----------
/**
 * @brief Invoke the key action event.
 *
 * This will be called when any key events happen on the observed window.
 *
 * @param key      Pressed key itself.
 * @param scanCode Scan code of the pressed key.
 * @param action   Action such as GLFW_PRESS/RELEASE/REPEAT.
 * @param mods     Mods such as if capslock is pressed, or shift, etc.
 */
void InputHelper::invokeOnKeyAction(int key, int scanCode, int action, int mods)
{
    if (gOnKeyActionCallback)
        gOnKeyActionCallback(key, scanCode, action, mods);
}


/**
 * @brief Invoke the window resize action event.
 *
 * @param width  New window width.
 * @param height New window height.
 */
void InputHelper::invokeOnWindowResizeAction(int width, int height)
{
    if (gOnWindowResizeActionCallback)
        gOnWindowResizeActionCallback(width, height);
}


/**
 * @brief Invoke the window resize action event.
 *
 * @param width  New window width.
 * @param height New window height.
 */
void InputHelper::invokeOnMouseButtonAction(int button, int action, int mods)
{
    if (gOnMouseButtonActionCallback)
        gOnMouseButtonActionCallback(button, action, mods);
}


/**
 * @brief Invoke the mouse move action event.
 *
 * @param xPos  New cursor x position.
 * @param yPos  New cursor y position.
 */
void InputHelper::invokeOnMouseMoveAction(double xPos, double yPos)
{
    if (gOnMouseMoveActionCallback)
        gOnMouseMoveActionCallback(xPos, yPos);
}


/**
 * @brief Invoke the mouse drop action event.
 *
 * @param dropCount  Number of paths dropped.
 * @param paths      Vector of string paths.
 */
void InputHelper::invokeOnMouseDropAction(int dropCount, const char** paths)
{
    if (gOnMouseDropActionCallback)
        gOnMouseDropActionCallback(dropCount, paths);
}


// ------ REGISTER ----------
/**
 * @brief Register callback to be called on window key action.
 *
 * @param callback Callback to be called.
 */
void InputHelper::registerOnKeyAction(const std::function<void(int key, int scanCode, int action, int mods)> callback)
{
    gOnKeyActionCallback = callback;
}


/**
 * @brief Register callback to be called on window resize action.
 *
 * @param callback Callback to be called.
 */
void InputHelper::registerOnWindowResizeAction(
    const std::function<void(int width, int height)> callback)
{
    gOnWindowResizeActionCallback = callback;
}


/**
 * @brief Register callback to be called on mouse button action.
 *
 * @param callback Callback to be called.
 */
void InputHelper::registerOnMouseButtonAction(const std::function<void(int button, int action, int mods)> callback)
{
    gOnMouseButtonActionCallback = callback;
}


/**
 * @brief Register callback to be called on mouse move action.
 *
 * @param callback Callback to be called.
 */
void InputHelper::registerOnMouseMoveAction(const std::function<void(double xPos, double yPos)> callback)
{
    gOnMouseMoveActionCallback = callback;
}


/**
 * @brief Register callback to be called on mouse drop action.
 *
 * @param callback Callback to be called.
 */
void InputHelper::registerOnMouseDropAction(const std::function<void(double dropCount, const char** paths)> callback)
{
    gOnMouseDropActionCallback = callback;
}

}
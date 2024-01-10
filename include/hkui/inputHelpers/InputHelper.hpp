#pragma once

#include <functional>

#include "../../hkui/vendor/GL/Glfw.hpp"

namespace inputHelpers
{
/**
 * @brief Singleton class dealing with basic user input on a given window
 */
class InputHelper
{
public:
    static InputHelper& get();

    void observe(GLFWwindow* window);

    void registerOnKeyAction(const std::function<void(int key, int scanCode, int action, int mods)> callback);
    void registerOnWindowResizeAction(const std::function<void(int width, int height)> callback);
    void registerOnMouseButtonAction(const std::function<void(int button, int action, int mods)> callback);
    void registerOnMouseMoveAction(const std::function<void(double xPos, double yPos)> callback);
    void registerOnMouseDropAction(const std::function<void(double xPos, double yPos)> callback);
    void registerOnMouseDropAction(const std::function<void(double dropCount, const char** paths)> callback);

private:
    InputHelper() = default;
    InputHelper(const InputHelper&) = delete;
    InputHelper& operator=(const InputHelper&) = delete;

    static void keyCallback(GLFWwindow* window, int key, int scanCode, int action, int mods);
    static void windowResizeCallback(GLFWwindow* window, int width, int height);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void mouseMoveCallback(GLFWwindow* window, double xPos, double yPos);
    static void dropCallback(GLFWwindow* window, int dropCount, const char** paths);

    void invokeOnKeyAction(int key, int scanCode, int action, int mods);
    void invokeOnWindowResizeAction(int width, int height);
    void invokeOnMouseButtonAction(int button, int action, int mods);
    void invokeOnMouseMoveAction(double xPos, double yPos);
    void invokeOnMouseDropAction(int dropCount, const char** paths);

    GLFWwindow* gObservedWindow{ nullptr };

    std::function<void(int key, int scanCode, int action, int mods)> gOnKeyActionCallback;
    std::function<void(int width, int height)> gOnWindowResizeActionCallback;
    std::function<void(int button, int action, int mods)> gOnMouseButtonActionCallback;
    std::function<void(double xPos, double yPos)> gOnMouseMoveActionCallback;
    std::function<void(int dropCount, const char** paths)> gOnMouseDropActionCallback;
};
}
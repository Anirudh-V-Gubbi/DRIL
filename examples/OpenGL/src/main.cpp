#include <glad/glad.h>
#include <GLFW/glfw3.h>
//#include <glm/vec2.hpp>
#include "..\dep\glm\glm\vec2.hpp"
#include "Shaders/shader.h"
#include "Textures/texture.h"
#include "Logger/logger.h"
#include "Shaders/shader_manager.h"
#include "application.h"
#include <Interface.h>
#include <PubSub/subscriber.h>
#include <renderer.h>
#include <Event/event_handler.h>

#include <iostream>
#include <memory>
#include <vector>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// screen dimensions and view and projection matrices
const glm::ivec2 SCREEN_DIMENSIONS(1000, 675);
const glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -1.0f));
const glm::mat4 projectionMatrix = glm::ortho<float>(0.0f, SCREEN_DIMENSIONS.x, SCREEN_DIMENSIONS.y, 0.0f, -0.1f, 100.0f);

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Initialise the logger
    new Logger("Logger");

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCREEN_DIMENSIONS.x, SCREEN_DIMENSIONS.y, "DRIL - OpenGL Example", NULL, NULL);
    if (window == NULL)
    {
        Logger::GetInstance()->error("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        Logger::GetInstance()->error("Failed to initialize GLEW");
        return -1;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Renderer
    Renderer *renderer = new Renderer();
    renderer->SetBackgroundColor({0, 0, 0, 1});
    
    // uncomment this call to draw in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Dynamically reloadable interface setup
    // --------------------------------------
    DRIL applicationInterface(
    R"(C:\Users\DRIL\examples\OpenGL\bin\debug)",
    R"(C:\Users\DRIL\examples\OpenGL\application)",
    R"(C:\Users\DRIL\examples\OpenGL\build)");
    applicationInterface.LoadILibrary();

    // Event listener
    // --------------
    Subscriber<Event*> m_eventSubscriber;
    m_eventSubscriber.SetOnNotifyCallback([&applicationInterface, &window, &renderer](Event* event) {
        applicationInterface.Execute<void>("OnEvent", event, window, renderer);
    });
    EventHandler::GetInstance()->Subscribe(std::make_shared<Subscriber<Event*>>(m_eventSubscriber));
    
    double m_timestamp;
    double m_deltaTime;

    while (!glfwWindowShouldClose(window))
    {   
        double currentTimestamp = glfwGetTime();
        m_deltaTime = currentTimestamp - m_timestamp;
        m_timestamp = currentTimestamp;
        
        // render
        // ------
        renderer->Draw(viewMatrix, projectionMatrix);

        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    delete renderer;
    return 0;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// process key inputs
// ------------------
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(action == GLFW_PRESS || action == GLFW_REPEAT) {
        KeyPressedEvent event(key, 1);
        EventHandler::GetInstance()->DispatchEvent(event);
    }
    else if(action == GLFW_RELEASE) {
        KeyReleasedEvent event(key);
        EventHandler::GetInstance()->DispatchEvent(event);
    }
}

// process mouse movements
// -----------------------
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    MouseMovedEvent event(xpos, ypos);
    EventHandler::GetInstance()->DispatchEvent(event);
}

// process mouse button clicks
// ---------------------------
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    double xPos, yPos;
    glfwGetCursorPos(window, &xPos, &yPos);
    
    if(action == GLFW_PRESS) {
        MouseButtonPressedEvent event(button, xPos, yPos);
        EventHandler::GetInstance()->DispatchEvent(event);
    }
    else if(action == GLFW_RELEASE) {
        MouseButtonReleasedEvent event(button, xPos, yPos);
        EventHandler::GetInstance()->DispatchEvent(event);
    }
}

// process mouse scrolls
// ---------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    MouseScrolledEvent event(xoffset, yoffset);
    EventHandler::GetInstance()->DispatchEvent(event);
}
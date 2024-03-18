#define RESOURCE_PATH R"(G:\DRIL\examples\OpenGL\resources\)"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
//#include <glm/vec2.hpp>
#include "..\dep\glm\glm\vec2.hpp"
#include "Shaders/shader.h"
#include "Textures/texture.h"
#include "Logger/logger.h"
#include "Shaders/shader_manager.h"
#include "../application/application.h"
#include "../../../lib/Interface.h"
#include "PubSub/subscriber.h"
#include "renderer.h"
#include "Event/event_handler.h"

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
glm::mat4 trans = glm::mat4(1.0f);
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
    std::string s(RESOURCE_PATH);
    std::cout << s << std::endl;

    // Initialise the logger
    // new Logger("Logger");

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCREEN_DIMENSIONS.x, SCREEN_DIMENSIONS.y, "DRIL - OpenGL Example", NULL, NULL);
    if (window == NULL)
    {
        // Logger::GetInstance()->error("Failed to create GLFW window");
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
        // Logger::GetInstance()->error("Failed to initialize GLEW");
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
    R"(G:\DRIL\examples\OpenGL\bin\debug\OpenGLApplication.dll)",
    R"(G:\DRIL\examples\OpenGL\application)",
    R"(G:\DRIL\examples\OpenGL\build)");
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


void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

////////////////////////////////////////////////////////
// #include "../dep/Glad/include/glad/glad.h"
// #include "../dep/glfw/include/GLFW/glfw3.h"
// #include "../dep/stb_image/stb_image.h"
// #include "../dep/glm/glm/glm.hpp"
// #include "../dep/glm/glm/gtc/matrix_transform.hpp"
// #include "../dep/glm/glm/gtc/type_ptr.hpp"
// #include "../../../lib/Interface.h"
// #include "Event/event.h"
// #include "PubSub/subscriber.h"
// #include "renderer.h"
// #include "../application/application.h"
// #include "Event/event_handler.h"

// #include <iostream>


//  void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//  void processInput(GLFWwindow *window);
// __declspec(dllexport) void click(GLFWwindow* window, int button, int action, int mods);
// __declspec(dllexport) void giveOutput(GLFWwindow *window, int texture, int shaderProgram, int VAO);

// const unsigned int SCR_WIDTH = 800;
// const unsigned int SCR_HEIGHT = 600;
// float kih=0;
// const char *vertexShaderSource = "#version 330 core\n"
//     "layout (location = 0) in vec3 aPos;\n"
//     "layout (location = 1) in vec3 aColor;\n"
//     "layout (location = 2) in vec2 aTexCoord;\n"
//     "out vec3 ourColor;\n"
//     "out vec2 texCoord;\n"
//     "uniform mat4 transform;\n"
//     "void main()\n"
//     "{\n"
//     "   gl_Position = transform * vec4(aPos, 1.0);\n"
//     "   ourColor = aColor;\n"
//     "   texCoord = aTexCoord;\n"
//     "}\n\0";

// const char *fragmentShaderSource = "#version 330 core\n"
//     "out vec4 FragColor;\n"
//     "in vec3 ourColor;\n"
//     "in vec2 texCoord;\n"
//     "uniform sampler2D ourTexture;\n"
//     "void main()\n"
//     "{\n"
//     "   FragColor = texture(ourTexture, texCoord);\n"
//     "}\n\0";

// glm::mat4 trans = glm::mat4(1.0f);

// int main()
// {
//     glfwInit();
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

// #ifdef __APPLE__
//     glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
// #endif

//     GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
//     if (window == NULL)
//     {
//         std::cout << "Failed to create GLFW window" << std::endl;
//         glfwTerminate();
//         return -1;
//     }
//     glfwMakeContextCurrent(window);
//     glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
//     glfwSetMouseButtonCallback(window, click);

//     if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//     {
//         std::cout << "Failed to initialize GLAD" << std::endl;
//         return -1;
//     }



//     unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
//     glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
//     glCompileShader(vertexShader);

//     unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//     glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
//     glCompileShader(fragmentShader);

//     unsigned int shaderProgram = glCreateProgram();
//     glAttachShader(shaderProgram, vertexShader);
//     glAttachShader(shaderProgram, fragmentShader);
//     glLinkProgram(shaderProgram);

//     glDeleteShader(vertexShader);
//     glDeleteShader(fragmentShader);

//     float vertices[] = {
//         0.5f+kih,  0.5f+kih, 0.0f,   1.0f+kih, 0.0f+kih, 0.0f,   1.0f, 1.0f,
//         0.5f+kih, -0.5f+kih, 0.0f,   0.0f+kih, 1.0f+kih, 0.0f,   1.0f, 0.0f,
//         -0.5f+kih, -0.5f+kih, 0.0f,   0.0f+kih, 0.0f+kih, 1.0f,   0.0f, 0.0f,
//         -0.5f+kih,  0.5f+kih, 0.0f,   1.0f+kih, 1.0f+kih, 0.0f,   0.0f, 1.0f
//     };
//     unsigned int indices[] = {
//         0, 1, 3, // first triangle
//         1, 2, 3  // second triangle
//     };

//     unsigned int VBO, VAO, EBO;
//     glGenVertexArrays(1, &VAO);
//     glGenBuffers(1, &VBO);
//     glGenBuffers(1, &EBO);

//     glBindVertexArray(VAO);
//     glBindBuffer(GL_ARRAY_BUFFER, VBO);
//     glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//     glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
//     glEnableVertexAttribArray(0);
//     glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
//     glEnableVertexAttribArray(1);
//     glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
//     glEnableVertexAttribArray(2);

//     unsigned int texture;
//     glGenTextures(1, &texture);
//     glBindTexture(GL_TEXTURE_2D, texture);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//     int width, height, nrChannels;
//     unsigned char *data = stbi_load("G:\\DRIL\\examples\\OpenGL\\resources\\textures\\tex2.jpg", &width, &height, &nrChannels, 0);
//     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
//     glGenerateMipmap(GL_TEXTURE_2D);
//     stbi_image_free(data);

//     glEnable(GL_DEPTH_TEST);

//     Renderer *renderer = new Renderer();
//     renderer->SetBackgroundColor({1, 0, 0, 1});

//     DRIL applicationInterface(
//     R"(G:\DRIL\examples\OpenGL\bin\debug\OpenGLApplication.dll)",
//     R"(G:\DRIL\examples\OpenGL\aplication)",
//     R"(G:\DRIL\examples\OpenGL\build)");
//     applicationInterface.LoadILibrary();

//     Subscriber<Event*> m_eventSubscriber;
//     m_eventSubscriber.SetOnNotifyCallback([&applicationInterface, &window, &renderer](Event* event) {
//         applicationInterface.Execute<void>("giveOutput", event, window, renderer);
//     });
//     EventHandler::GetInstance()->Subscribe(std::make_shared<Subscriber<Event*>>(m_eventSubscriber));

//     while (!glfwWindowShouldClose(window))
//     {
//         giveOutput(window,texture,shaderProgram,VAO);
//     }

//     glDeleteVertexArrays(1, &VAO);
//     glDeleteBuffers(1, &VBO);
//     glDeleteBuffers(1, &EBO);
//     glDeleteProgram(shaderProgram);

//     glfwTerminate();
//     return 0;

// }


// void processInput(GLFWwindow *window)
// {
//     if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//         glfwSetWindowShouldClose(window, true);
// }

// void framebuffer_size_callback(GLFWwindow* window, int width, int height)
// {
//     glViewport(0, 0, width, height);
// }

//////////////////////////////////////////////


// #define RESOURCE_PATH R"(G:\DRIL\examples\OpenGL\resources\)"

// #include <glad/glad.h>
// #include <GLFW/glfw3.h>
// //#include <glm/vec2.hpp>
// #include "..\dep\glm\glm\vec2.hpp"
// #include "Shaders/shader.h"
// #include "Textures/texture.h"
// #include "Logger/logger.h"
// #include "Shaders/shader_manager.h"
// #include "../application/application.h"
// #include "../../../lib/Interface.h"
// #include <PubSub/subscriber.h>
// #include <renderer.h>
// #include <Event/event_handler.h>

// #include <iostream>
// #include <memory>
// #include <vector>



// void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
// void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
// void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
// void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// // screen dimensions and view and projection matrices
// const glm::ivec2 SCREEN_DIMENSIONS(1000, 675);
// const glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -1.0f));
// const glm::mat4 projectionMatrix = glm::ortho<float>(0.0f, SCREEN_DIMENSIONS.x, SCREEN_DIMENSIONS.y, 0.0f, -0.1f, 100.0f);

// int main()
// {
//     // glfw: initialize and configure
//     // ------------------------------
//     glfwInit();
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

// #ifdef __APPLE__
//     glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
// #endif
//     glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
//     std::string s(RESOURCE_PATH);
//     std::cout << s << std::endl;

//     // Initialise the logger
//     // new Logger("Logger");

//     // glfw window creation
//     // --------------------
//     GLFWwindow* window = glfwCreateWindow(SCREEN_DIMENSIONS.x, SCREEN_DIMENSIONS.y, "DRIL - OpenGL Example", NULL, NULL);
//     if (window == NULL)
//     {
//         // Logger::GetInstance()->error("Failed to create GLFW window");
//         glfwTerminate();
//         return -1;
//     }
//     glfwMakeContextCurrent(window);
//     glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
//     glfwSetKeyCallback(window, key_callback);
//     glfwSetCursorPosCallback(window, cursor_position_callback);
//     glfwSetMouseButtonCallback(window, mouse_button_callback);
//     glfwSetScrollCallback(window, scroll_callback);
    
//     // glad: load all OpenGL function pointers
//     // ---------------------------------------
//     if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//     {
//         // Logger::GetInstance()->error("Failed to initialize GLEW");
//         return -1;
//     }

//     glEnable(GL_BLEND);
//     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//     // Renderer
//     Renderer *renderer = new Renderer();
//     renderer->SetBackgroundColor({0, 0, 0, 1});
    
//     // uncomment this call to draw in wireframe polygons.
//     // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

//     // Dynamically reloadable interface setup
//     // --------------------------------------
//     DRIL applicationInterface(
//     R"(G:\DRIL\examples\OpenGL\bin\debug\OpenGLApplication.dll)",
//     R"(G:\DRIL\examples\OpenGL\application)",
//     R"(G:\DRIL\examples\OpenGL\build)");
//     applicationInterface.LoadILibrary();

//     // Event listener
//     // --------------
//     Subscriber<Event*> m_eventSubscriber;
//     m_eventSubscriber.SetOnNotifyCallback([&applicationInterface, &window, &renderer](Event* event) {
//         applicationInterface.Execute<void>("OnEvent", event, window, renderer);
//     });
//     EventHandler::GetInstance()->Subscribe(std::make_shared<Subscriber<Event*>>(m_eventSubscriber));
    
//     double m_timestamp;
//     double m_deltaTime;

//     while (!glfwWindowShouldClose(window))
//     {   
//         double currentTimestamp = glfwGetTime();
//         m_deltaTime = currentTimestamp - m_timestamp;
//         m_timestamp = currentTimestamp;
        
//         // render
//         // ------
//         renderer->Draw(viewMatrix, projectionMatrix);

        
//         glfwSwapBuffers(window);
//         glfwPollEvents();
//     }

//     // glfw: terminate, clearing all previously allocated GLFW resources.
//     // ------------------------------------------------------------------
//     glfwTerminate();
//     delete renderer;
//     return 0;
// }

// // glfw: whenever the window size changed (by OS or user resize) this callback function executes
// // ---------------------------------------------------------------------------------------------
// void framebuffer_size_callback(GLFWwindow* window, int width, int height)
// {
//     // make sure the viewport matches the new window dimensions; note that width and
//     // height will be significantly larger than specified on retina displays.
//     glViewport(0, 0, width, height);
// }

// // process key inputs
// // ------------------
// void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
// {
//     if(action == GLFW_PRESS || action == GLFW_REPEAT) {
//         KeyPressedEvent event(key, 1);
//         EventHandler::GetInstance()->DispatchEvent(event);
//     }
//     else if(action == GLFW_RELEASE) {
//         KeyReleasedEvent event(key);
//         EventHandler::GetInstance()->DispatchEvent(event);
//     }
// }

// // process mouse movements
// // -----------------------
// void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
// {
//     MouseMovedEvent event(xpos, ypos);
//     EventHandler::GetInstance()->DispatchEvent(event);
// }

// // process mouse button clicks
// // ---------------------------
// void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
// {
//     double xPos, yPos;
//     glfwGetCursorPos(window, &xPos, &yPos);
    
//     if(action == GLFW_PRESS) {
//         MouseButtonPressedEvent event(button, xPos, yPos);
//         EventHandler::GetInstance()->DispatchEvent(event);
//     }
//     else if(action == GLFW_RELEASE) {
//         MouseButtonReleasedEvent event(button, xPos, yPos);
//         EventHandler::GetInstance()->DispatchEvent(event);
//     }
// }

// // process mouse scrolls
// // ---------------------
// void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
// {
//     MouseScrolledEvent event(xoffset, yoffset);
//     EventHandler::GetInstance()->DispatchEvent(event);
// }

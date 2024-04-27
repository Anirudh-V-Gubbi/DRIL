#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <PubSub/subscriber.h>
#include "renderer.h"
#include <Event/event_handler.h>
#include "../../../lib/Interface.h"
#include "../application/application.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

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

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
float kih=0;
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "layout (location = 2) in vec2 aTexCoord;\n"
    "out vec3 ourColor;\n"
    "out vec2 texCoord;\n"
    "uniform mat4 transform;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = transform * vec4(aPos, 1.0);\n"
    "   ourColor = aColor;\n"
    "   texCoord = aTexCoord;\n"
    "}\n\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor;\n"
    "in vec2 texCoord;\n"
    "uniform sampler2D ourTexture;\n"
    "void main()\n"
    "{\n"
    "   FragColor = texture(ourTexture, texCoord);\n"
    "}\n\0";


int main()
{
    glm::mat4 trans = glm::mat4(1.0f);

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }



    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    float vertices[] = {
        0.5f+kih,  0.5f+kih, 0.0f,   1.0f+kih, 0.0f+kih, 0.0f,   1.0f, 1.0f,
        0.5f+kih, -0.5f+kih, 0.0f,   0.0f+kih, 1.0f+kih, 0.0f,   1.0f, 0.0f,
        -0.5f+kih, -0.5f+kih, 0.0f,   0.0f+kih, 0.0f+kih, 1.0f,   0.0f, 0.0f,
        -0.5f+kih,  0.5f+kih, 0.0f,   1.0f+kih, 1.0f+kih, 0.0f,   0.0f, 1.0f
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    Texture texture("tex2.jpg", TextureFormats::JPG);

    glEnable(GL_DEPTH_TEST); // Enable depth testing

    Renderer *renderer = new Renderer();
    

    float xx =0.0003;//((float)glfwGetTime())/1000> 0.005 ? 0:(float)glfwGetTime()/1000;
    float yy =-0.00;
    float rr=0.0;
    float gg=0.0;
    float bb=0.0;

    DRIL applicationInterface(DLL_PATH "/OpenGLApplication.dll");
    applicationInterface.Initialise(APPLICATION_PATH, OUTPUT_PATH, [&applicationInterface, &xx, &yy, &rr, &gg, &bb]() {
        xx = applicationInterface.Execute<float>("GetX");
        yy = applicationInterface.Execute<float>("GetY");
        rr = applicationInterface.Execute<float>("GetR");
        gg = applicationInterface.Execute<float>("GetG");
        bb = applicationInterface.Execute<float>("GetB");
    });
    renderer->SetBackgroundColor({rr, gg, bb, 1});
    applicationInterface.LoadILibrary();

    xx = applicationInterface.Execute<float>("GetX");
    yy = applicationInterface.Execute<float>("GetY");

    Subscriber<Event*> m_eventSubscriber;
    m_eventSubscriber.SetOnNotifyCallback([&applicationInterface, &window, &renderer, &xx, &yy, &texture](Event* event) {
        texture = Texture(applicationInterface.Execute<std::string>("GetTex"), TextureFormats::JPG);
    });
    EventHandler::GetInstance()->Subscribe(std::make_shared<Subscriber<Event*>>(m_eventSubscriber));

    while (!glfwWindowShouldClose(window))
    {

        //((((float)glfwGetTime())/1000> 0.005) ? 0:(float)glfwGetTime()/1000);
        processInput(window);
        glm::vec3 currentPosition = glm::vec3(trans[3][0], trans[3][1], trans[3][2]);
    glm::vec3 nextPosition = currentPosition + glm::vec3(xx, yy, 0.0f);
    
    if (nextPosition.x >= 0.5 || nextPosition.x <= -0.5)
    {
//trans = glm::scale(trans, glm::vec3(-1.0f, 1.0f, 1.0f)); 
xx=-xx;   
    }
    if (nextPosition.y >= 0.5 || nextPosition.y <= -0.5)
    {  yy=-yy; 
//trans = glm::scale(trans, glm::vec3(1.0f, -1.0f, 1.0f)); 
    }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        texture.Bind();

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        trans = glm::translate(trans, glm::vec3(xx,yy, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "transform"), 1, GL_FALSE, glm::value_ptr(trans));
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
#include "application.h"

#include <Entities/entity.h>
#include <Shaders/shader_manager.h>
#include <Textures/texture.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

extern "C"
{
    void OnEvent(Event *event, GLFWwindow *window, Renderer *renderer, glm::mat4* trans)
    {
        
        gladLoadGL();
        EventType type = event->GetEventType();
        int category = event->GetCategoryFlags();

        switch (type)
        {
        case EventType::None:
            break;
        case EventType::WindowClose:
            break;
        case EventType::WindowResize:
            break;
        case EventType::KeyPressed:
        {
            KeyPressedEvent *kEvent = dynamic_cast<KeyPressedEvent *>(event);
            if (kEvent->GetKeyCode() == GLFW_KEY_ESCAPE)
            {
                glfwSetWindowShouldClose(window, 1);
            }

            if (kEvent->GetKeyCode() == GLFW_KEY_W)
            {
                glfwSetWindowShouldClose(window, 1);
                 renderer->SetBackgroundColor({0.0, 0.0, 1.0, 1.0});
            }

            break;
        }
        case EventType::KeyReleased:
            break;
        case EventType::KeyTyped:
            break;
        case EventType::MouseButtonPressed:
        {
            // #include <glm/gtc/matrix_transform.hpp> // Include the missing header file

            MouseButtonPressedEvent *mEvent = dynamic_cast<MouseButtonPressedEvent *>(event);
             renderer->SetBackgroundColor({0.0,1.0, 0.0, 1.0});
            Shader shader("rect_vertex.vs", "rect_fragment.fs");
            Entity entity(shader, Texture("tex2.jpg", TextureFormats::JPG), glm::vec3(mEvent->GetX(),mEvent->GetY(), 0.0));
            renderer->AddEntityToRender(entity);
            break;

            // if (mEvent->GetMouseButton() == GLFW_MOUSE_BUTTON_LEFT)
            // {
            //     *trans = glm::rotate(*trans, glm::radians(-6.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            // }
            // else if (mEvent->GetMouseButton() == GLFW_MOUSE_BUTTON_RIGHT)
            // {
            //     double xpos, ypos;
            //     glfwGetCursorPos(window, &xpos, &ypos);
            //     std::cout << xpos << " " << ypos << std::endl;
            //     float normalizedX = (float)xpos / 800 * 2 - 1;
            //     float normalizedY = (float)ypos / 600 * 2 - 1;
            //     *trans = glm::translate(*trans, glm::vec3(normalizedX / 1000, -normalizedY / 1000, 0.0f));
            // }
        }
        case EventType::MouseButtonReleased:
            break;
        case EventType::MouseMoved:
            break;
        case EventType::MouseScrolled:
            break;
        }
    }
} 



// extern "C"
// {
//     void click(GLFWwindow *window, int button, int action, int mods)
//     {
//         if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
//         {
//             trans = glm::rotate(trans, glm::radians(6.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//         }
//         else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
//         {
//             double xpos, ypos;
//             glfwGetCursorPos(window, &xpos, &ypos);
//             std::cout << xpos << " " << ypos << std::endl;
//             float normalizedX = (float)xpos / 800 * 2 - 1;
//             float normalizedY = (float)ypos / 600 * 2 - 1;
//             trans = glm::translate(trans, glm::vec3(normalizedX / 1000, -normalizedY / 1000, 0.0f));
//         }
//     }

//     void giveOutput(GLFWwindow *window, int texture, int shaderProgram, int VAO, float xx, float yy)
//     {
//          xx = 0.00003;   //((float)glfwGetTime())/1000> 0.005 ? 0:(float)glfwGetTime()/1000;
//          yy = -0.000002; //((((float)glfwGetTime())/1000> 0.005) ? 0:(float)glfwGetTime()/1000);
//     }
// }
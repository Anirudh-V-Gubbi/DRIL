#include "application.h"

#include <Entities/entity.h>
#include <Shaders/shader_manager.h>
#include <Textures/texture.h>

extern "C" {
    void OnEvent(Event* event, GLFWwindow* window, Renderer* renderer) {
        // std::cout << " Event";
        EventType type = event->GetEventType();
        int category = event->GetCategoryFlags();

        switch(type) {
            case EventType::None:
                break;
            case EventType::WindowClose:
                break;
            case EventType::WindowResize:
                break;
            case EventType::KeyPressed:
            {
                KeyPressedEvent* kEvent = dynamic_cast<KeyPressedEvent*>(event);
                if(kEvent->GetKeyCode() == GLFW_KEY_ESCAPE) {
                    glfwSetWindowShouldClose(window, 1);
                }

                if(kEvent->GetKeyCode() == GLFW_KEY_W) {
                    glfwSetWindowShouldClose(window, 1);
                    // renderer->SetBackgroundColor({1.0, 0.0, 1.0, 1.0});
                }

                break;
            }
            case EventType::KeyReleased:
                break;
            case EventType::KeyTyped:
                break;
            case EventType::MouseButtonPressed:
            {
                MouseButtonPressedEvent* mEvent = dynamic_cast<MouseButtonPressedEvent*>(event);
                std::cout << mEvent << std::endl;
                // Shader shader("rect_vertex.vs", "rect_fragment.fs");
                // Entity rect(shader, Texture("tex2.jpg", TextureFormats::JPG), glm::vec3(mEvent->GetX(), mEvent->GetY(), 0.0));
                // renderer->AddEntityToRender(rect);
                renderer->SetBackgroundColor({0.0, 1.0, 1.0, 1.0});
                break;
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

// #include "application.h"
// #include "../src/main.cpp"
// #include "../src/Entities/entity.h"
// #include <Shaders/shader_manager.h>
// #include <Textures/texture.h>

// extern "C" {
//     void click(GLFWwindow* window, int button, int action, int mods)
// {
//     if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
//     {
//         trans = glm::rotate(trans, glm::radians(-6.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//     }else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
//     {
//         double xpos, ypos;
//         glfwGetCursorPos(window, &xpos, &ypos);
//         std::cout<<xpos<<" "<<ypos<<std::endl;
//         float normalizedX = (float)xpos/800*2-1;
//         float normalizedY = (float)ypos/600*2-1;
//         trans = glm::translate(trans, glm::vec3(normalizedX/1000, -normalizedY/1000, 0.0f));
//     }
// }

// void giveOutput(GLFWwindow *window, int texture, int shaderProgram, int VAO){
//     float xx = 0.00003;  //((float)glfwGetTime())/1000> 0.005 ? 0:(float)glfwGetTime()/1000;
//         float yy = -0.000002; //((((float)glfwGetTime())/1000> 0.005) ? 0:(float)glfwGetTime()/1000);
//         processInput(window);
//         glm::vec3 currentPosition = glm::vec3(trans[3][0], trans[3][1], trans[3][2]);
//         glm::vec3 nextPosition = currentPosition + glm::vec3(xx, yy, 0.0f);

//         if (nextPosition.x >= 0.5 || nextPosition.x <= -0.5)
//         {
//             trans = glm::scale(trans, glm::vec3(-1.0f, 1.0f, 1.0f));
//         }
//         if (nextPosition.y >= 0.5 || nextPosition.y <= -0.5)
//         {
//             trans = glm::scale(trans, glm::vec3(1.0f, -1.0f, 1.0f));
//         }

//         glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//         glActiveTexture(GL_TEXTURE0);
//         glBindTexture(GL_TEXTURE_2D, texture);

//         glUseProgram(shaderProgram);
//         glBindVertexArray(VAO);
//         trans = glm::translate(trans, glm::vec3(xx, yy, 0.0f));
//         glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "transform"), 1, GL_FALSE, glm::value_ptr(trans));
//         glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

//         glfwSwapBuffers(window);
//         glfwPollEvents();


// }




// }
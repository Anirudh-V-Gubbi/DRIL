#include "application.h"

#include <Entities/entity.h>
#include <Shaders/shader_manager.h>
#include <Textures/texture.h>

extern "C" {
    void OnEvent(Event* event, GLFWwindow* window, Renderer* renderer) {
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
                // std::cout << mEvent->GetX() << "," << mEvent->GetY() << std::endl;
                Entity rect(Shader("rect_vertex.vs", "rect_fragment.fs"), Texture("tex2.jpg", TextureFormats::JPG), glm::vec3(mEvent->GetX(), mEvent->GetY(), 0.0));
                renderer->AddEntityToRender(rect);
                renderer->SetBackgroundColor({1.0, 1.0, 1.0, 1.0});
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

#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <Logger/logger.h>
#include <PubSub/publisher.h>
#include <map>
#include "..\dep\glm\glm\vec2.hpp"
#include "application_event.h"
#include "key_event.h"
#include "mouse_event.h"

enum class ButtonState { RELEASED, PRESSED };

class EventHandler : public Publisher<Event*> {
public:
  ~EventHandler() {}

  inline static std::map<GLint, ButtonState> MouseButtonsState = {
      {GLFW_MOUSE_BUTTON_LEFT, ButtonState::RELEASED},
      {GLFW_MOUSE_BUTTON_RIGHT, ButtonState::RELEASED},
      {GLFW_MOUSE_BUTTON_MIDDLE, ButtonState::RELEASED},
  };
  inline static 
  glm::vec2 clickPosition;

  static std::shared_ptr<EventHandler> GetInstance() {
    if (m_instance == nullptr) {
      m_instance = std::make_shared<EventHandler>(EventHandler());
    }

    return m_instance;
  }

  void DispatchEvent(Event &event) {
    EventType type = event.GetEventType();

    switch (type) {
    case EventType::None:
      break;
    case EventType::WindowClose:
      break;
    case EventType::WindowResize:
      break;
    case EventType::KeyPressed:
      break;
    case EventType::KeyReleased:
      break;
    case EventType::KeyTyped:
      break;
    case EventType::MouseButtonPressed: {
      MouseButtonPressedEvent *mEvent =
          dynamic_cast<MouseButtonPressedEvent *>(&event);
      MouseButtonsState[mEvent->GetMouseButton()] = ButtonState::PRESSED;
      clickPosition.x = mEvent->GetX();
      clickPosition.y = mEvent->GetY();
      break;
    }
    case EventType::MouseButtonReleased: {
      MouseButtonReleasedEvent *mEvent =
          dynamic_cast<MouseButtonReleasedEvent *>(&event);
      MouseButtonsState[mEvent->GetMouseButton()] = ButtonState::RELEASED;
      break;
    }
    case EventType::MouseMoved:
      break;
    case EventType::MouseScrolled:
      break;
    }

    Notify(&event);
  }

private:
  inline static std::shared_ptr<EventHandler> m_instance;

  EventHandler() {}
};

#endif

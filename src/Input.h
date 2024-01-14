#pragma once
#include <map>
#include <functional>
#include <set>

namespace im {
  template <typename ...T>
  class Event {
  public:
    class Handler {
      static uint32_t nextId;

    public:
      std::function<void(T ...)> onEvent;
      uint32_t                   id;

      Handler(const std::function<void(T ...)>& f): id{nextId++}, onEvent{f} {}
    };

    struct Comparator {
      bool operator()(const Handler& a, const Handler& b) const { return a.id > b.id; }
    };
  };

  typedef Event<>               KeyEvent;
  typedef Event<double, double> MouseEvent;

  class Module {
    typedef std::set<KeyEvent::Handler, KeyEvent::Comparator>     KeySet;
    typedef std::set<MouseEvent::Handler, MouseEvent::Comparator> MouseSet;

    std::map<std::pair<int, int>, std::pair<std::map<int, KeySet>, KeySet>> keyMap         = {};
    MouseSet                                                                mouseListeners = {};

  public:
    static Module& getInstance();

    void sendKeyEvent(int key, int action, int mods, const char* keyName = nullptr, const char* actionName = nullptr, const char* modName = nullptr);
    void sendMouseEvent(double x, double y);
    void mapKey(int key, int action, int mod, const std::function<void()>& f);
    void addMouseListener(const std::function<void(double, double)>& f);
  };
}

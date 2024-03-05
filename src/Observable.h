#pragma once
#include <vector>

template <typename T>
class Observable {
  std::vector<T> observers;

public:
  void addObserver(T observer) {
    observers.push_back(observer);
  }

  void removeObserver(T observer) {
    observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
  }
};

#pragma once
template <typename T>
class DoubleList {
public:
  T data;
  DoubleList<T>* prev;
  DoubleList<T>* next;

  // Constructor
  DoubleList(const T& value) : data(value), prev(nullptr), next(nullptr) {}
};

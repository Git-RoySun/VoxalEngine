#pragma once
#include <vector>

class Updatable {
  static std::vector<Updatable*> updateQueue;
  //todo: should be a linked list implementation (iterators that don't invalidate)

  bool queued = false;

protected:
  void         queueUpdate();
  void         unqueuUpdate();
  virtual void update(float delta) = 0;

public:
  static void updateAll(float delta);
  Updatable() {};
  ~Updatable() { unqueuUpdate(); };
};

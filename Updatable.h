#pragma once
#include <vector>

class Updatable{
	//static members
	static std::vector<Updatable*> updateQueue;
protected:
	static std::vector<Updatable*>::iterator add(Updatable* update);
	static void remove(std::vector<Updatable*>::iterator it) { updateQueue.erase(it); };
public:
	static void updateAll();

	//class members
private:
	std::vector<Updatable*>::iterator queuePosition;
	bool queued = false;
protected:
	void queueUpdate();
	void unqueuUpdate();
	virtual void update() = 0;
public:
	Updatable();
	~Updatable() { unqueuUpdate(); };
};

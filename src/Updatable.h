#pragma once
#include <vector>

class Updatable{
	//static members
	static std::vector<Updatable*> updateQueue;//should be DoubleList when it is implemented
public:
	static void updateAll(float delta);
	//class members
private:
	bool queued = false;
protected:
	void queueUpdate();
	void unqueuUpdate();
	virtual void update(float delta) = 0;
public:
	Updatable(){};
	~Updatable() { unqueuUpdate(); };
};

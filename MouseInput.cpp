#include "MouseInput.h"
namespace ic{
	void MouseInput::move(double xpos, double ypos){
		callback(xpos, ypos);
	}

}
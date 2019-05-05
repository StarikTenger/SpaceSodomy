#include "Robot.h"
#include "Random.h"

Robot::Robot(){
	characteristic = random::intRandom(0, 16000);
}

Robot::~Robot(){
}

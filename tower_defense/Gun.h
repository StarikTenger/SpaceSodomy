#pragma once
class Gun {
public:
	double cooldownTime = 0;
	double timeToCooldown = 0;
	double bulletVelocity = 0;
	Gun();
	Gun(double _cooldownTime, double _bulletVelocity);
	~Gun();
};


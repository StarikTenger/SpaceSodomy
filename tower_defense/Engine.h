#pragma once
class Engine{
public:
	double mainForce = 0;
	double backForce = 0;
	double turnForce = 0;
	Engine();
	Engine(double _mainForce, double _backForce, double _turnForce);
	~Engine();
};


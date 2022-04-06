#include "../ecs/Component.h"
#pragma once
struct Generations : public ecs::Component
{
public:
	__CMPID_DECL__(ecs::_GENERATIONS)
	Generations();
	~Generations();
	void setGenerations(int newGens);
	int getGenerations() const;
	void decreaseGenerations();
protected:
	int gens_;
};


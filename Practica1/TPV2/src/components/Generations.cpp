#include "Generations.h"

Generations::Generations(): gens_()
{
}

Generations::~Generations()
{
}

void Generations::setGenerations(int newGens)
{
	gens_ = newGens;
}

int Generations::getGenerations() const
{
	return gens_;
}

void Generations::decreaseGenerations()
{
	gens_--;
}

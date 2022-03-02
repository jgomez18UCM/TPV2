// This file is part of the course TPV2@UCM - Samir Genaim

#include "Manager.h"

namespace ecs {

Manager::Manager() :
		ents_(), //
		hdlrs_(), //
		entsByGroup_() //
{
}

Manager::~Manager() {

	// destroy all entities
	for (auto e : ents_) {
		delete e;
	}
}

void Manager::refresh() {

	// remove dead entities from the groups lists
	for (auto &groupEntities : entsByGroup_) {
		groupEntities.remove_if([](Entity *e) {
			return !e->isAlive();
		});
	}

	// remove all dead
	ents_.remove_if([](Entity *e) {
		// we are using the predicate to free
		// the memory if 'e' is not alive as well
		if (!e->isAlive())
			delete e;

		return !e->isAlive();
	});

}

} // end of namespace

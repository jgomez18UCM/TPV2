// This file is part of the course TPV2@UCM - Samir Genaim

#include "Manager.h"

namespace ecs {

Manager::Manager() :
		ents_(), //
		hdlrs_(), //
		entsByGroup_() //
{
	// we reserve space for 100 entities, just to avoid resizing. You
	// can adjust this number.
	//
	ents_.reserve(100);

	// for each group we reserve space for 100 entities,
	// just to avoid copies
	//
	for (auto &groupEntities : entsByGroup_) {
		groupEntities.reserve(100);
	}
}

Manager::~Manager() {

	// delete all entities
	//
	for (auto e : ents_)
		delete e;
}

void Manager::refresh() {

	// remove dead entities from the groups lists, and also those
	// do not belong to the group anymore
	for (ecs::grpId_type gId = 0; gId < ecs::maxGroupId; gId++) {
			auto &groupEntities = entsByGroup_[gId];
		groupEntities.erase(
				remove_if(groupEntities.begin(), groupEntities.end(),
						[gId](Entity *e) {
							return !e->isAlive() || !e->hasGroup(gId);
						}), groupEntities.end());
	}

	// remove all dead
	ents_.erase( //
			remove_if(ents_.begin(), ents_.end(), [](Entity *e) {
				if (e->isAlive()) {
					return false;
				} else {
					delete e;
					return true;
				}
			}), //
			ents_.end());

}

} // end of namespace

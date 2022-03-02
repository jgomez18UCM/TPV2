// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include <iostream>
#include <list>
#include <array>
#include <cassert>

#include "Component.h"
#include "ecs.h"
#include "Entity.h"

namespace ecs {

class Manager {

public:
	Manager();
	virtual ~Manager();

	// Adding an entity simply creates an instance of Entity, adds
	// it to the list of entities and returns it to the caller.
	//
	inline Entity* addEntity() {

		// create and initialise the entity
		auto e = new Entity();
		e->setAlive(true);
		e->resetGroups();
		e->setContext(this);

		// add the entity 'e' to list of entities
		//
		// IMPORTANT NOTE:
		//
		// Currently we immediately add the entity to the list of entities 'ents_',
		// so we will actually see them in this 'frame' if we traverse the list of
		// entities afterwards!
		//
		// A better solution would be to add them to an auxiliary list, and
		// define a method 'flush()' that moves them from the auxiliary list
		// to 'ents_'. We will have to call 'flush()' in each iteration of the
		// main loop. This way we guarantee that entities that are added in one
		// 'frame' they will appear only in the next 'frame' -- I leave it as an
		// exercise for you ...
		//
		ents_.push_back(e);

		// return it to the caller
		//
		return e;
	}

	// returns the vector of all entities
	//
	inline const auto& getEntities() {
		return ents_;
	}

	// returns the vector of all entities for group 'gId'
	//
	inline const auto& getEntitiesByGroup(grpId_type gId) {
		assert(gId < ecs::maxGroupId);
		return entsByGroup_[gId];
	}

	// associates the entity 'e' to the handler 'hId'
	//
	inline void setHandler(hdlrId_type hId, Entity *e) {
		assert(hId < ecs::maxHdlrId);
		hdlrs_[hId] = e;
	}

	// returns the entity associated to the handler 'hId'
	//
	inline Entity* getHandler(hdlrId_type hId) {
		assert(hId < ecs::maxHdlrId);
		return hdlrs_[hId];
	}

	// call update of all entities
	//
	void update() {
		for (auto e : ents_)
			e->update();
	}

	// call render of all entities
	//
	void render() {
		for (auto e : ents_)
			e->render();
	}

	// eliminate dead entities (the implementation of this method
	// is in Manager.cpp, but we could also defined it here).
	//
	void refresh();

private:

	// class Entity is a friend so it can access addToGroup and removeFromGroup
	//
	friend Entity;

	// ** addToGroup and removeFromGroup do not check if 'e' is
	//    there for efficiency, etc. We suppose that they are
	//    called correctly. This is actually checked in the corresponding
	//    methods of Entity.

	// add the entity to the group 'gId'
	//
	void addToGroup(grpId_type gId, Entity *e) {
		entsByGroup_[gId].push_back(e);
	}

	// remove the entity from the group 'gId'
	//
	void removeFromGroup(grpId_type gId, Entity *e) {

		// find the element that is equal to e (returns an iterator)
		auto iter = std::find(	//
				entsByGroup_[gId].begin(),	//
				entsByGroup_[gId].end(), //
				e);

		// remove the element specified by 'iter'
		entsByGroup_[gId].erase(iter);
	}

	std::list<Entity*> ents_;
	std::array<Entity*, maxHdlrId> hdlrs_;
	std::array<std::list<Entity*>, maxGroupId> entsByGroup_;
};

} // end of namespace

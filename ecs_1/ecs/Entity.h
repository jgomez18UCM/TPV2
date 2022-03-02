// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include <array>
#include <bitset>
#include <cassert>
#include <list>

#include "Component.h"
#include "ecs.h"

namespace ecs {

class Entity {
public:
	Entity() :
			mngr_(nullptr), //
			cmps_(), //
			currCmps_(), //
			alive_() //
	{
	}

	// we delete the copy constructor/assignment because it is
	// not clear how to copy the components
	//
	Entity(const Entity&) = delete;
	Entity& operator=(const Entity&) = delete;

	// Exercise: define move constructor/assignment for class Entity

	// Destroys the entity
	//
	virtual ~Entity() {

		// we delete all available components
		//
		for (auto c : currCmps_) {
			delete c;
		}
	}

	// Each entity knows to which manager it belongs, we use
	// this method to set the context
	//
	inline void setContext(Manager *mngr) {
		mngr_ = mngr;
	}

	// Setting the state of the entity (alive or dead)
	//
	inline void setAlive(bool alive) {
		alive_ = alive;
	}

	// Returns the state of the entity (alive o dead)
	//
	inline bool isAlive() {
		return alive_;
	}

	// Updating  an entity simply calls the update of all
	// components
	//
	void update() {
		for (auto c : currCmps_)
			c->update();
	}

	// Rendering an entity simply calls the render of all
	// components
	//
	void render() {
		for (auto c : currCmps_)
			c->render();
	}

	// Adds a component. It receives the type T (to be created), and the
	// list of arguments (if any) to be passed to the constructor.
	// The component identifier 'cId' is taken from T::id.
	//
	template<typename T, typename ...Ts>
	inline T* addComponet(Ts &&... args) {

		constexpr cmpId_type cId = T::id;
		assert(cId < ecs::maxComponentId);

		// delete the current component, if any
		//
		removeComponent<T>();

		// create, initialise and install the new component
		//
		auto c = new T(std::forward<Ts>(args)...);
		c->setContext(this, mngr_);
		c->initComponent();
		cmps_[cId] = c;
		currCmps_.push_back(c);

		// return it to the user so i can be initialised if needed
		return c;
	}

	// Removes the components at position T::id.
	//
	template<typename T>
	inline void removeComponent() {

		constexpr cmpId_type cId = T::id;
		assert(cId < ecs::maxComponentId);

		if (cmps_[cId] != nullptr) {

			// remove it from the list of current components
			//
			currCmps_.remove(cmps_[cId]);

			// destroy it
			//
			delete cmps_[cId];

			// remove the pointer
			//
			cmps_[cId] = nullptr;
		}
	}

	// Returns the component that corresponds to position T::id, casting it
	// to T*. The casting is done just for ease of use, to avoid casting
	// outside.
	//
	template<typename T>
	inline T* getComponent() {

		constexpr cmpId_type cId = T::id;
		assert(cId < ecs::maxComponentId);

		return static_cast<T*>(cmps_[cId]);
	}

	// return true if there is a component with identifier T::id
	//
	template<typename T>
	inline bool hasComponent() {

		constexpr cmpId_type cId = T::id;
		assert(cId < ecs::maxComponentId);

		return cmps_[cId] != nullptr;
	}

	// these methods are in .cpp because they need access to
	// the Manager (and thus we would have circular include if
	// we put them in the .h)
	//
	void addToGroupInTheManager(grpId_type gId);
	void removeFromGroupInTheManager(grpId_type gId);

	// adds the entity to the group 'gId'
	//
	inline void addToGroup(grpId_type gId) {
		assert(gId < ecs::maxGroupId);

		if (!groups_[gId]) {
			groups_[gId] = true;
			addToGroupInTheManager(gId);
		}
	}

	// removes the entity from the group 'gId'
	//
	inline void removeFromGroup(grpId_type gId) {
		assert(gId < ecs::maxGroupId);

		if (groups_[gId]) {
			groups_[gId] = false;
			removeFromGroupInTheManager(gId);
		}
	}

	// returns true/false depending on if the entity
	// does/doesn't belong to the group 'gId'
	//
	inline bool hasGroup(grpId_type gId) {
		assert(gId < ecs::maxGroupId);

		return groups_[gId];
	}

	// removes the entity from all groups
	//
	inline void resetGroups() {
		groups_.reset();
	}

private:

	Manager *mngr_;
	std::array<Component*, maxComponentId> cmps_;
	std::list<Component*> currCmps_;
	std::bitset<maxGroupId> groups_;
	bool alive_;
};

} // end of name space

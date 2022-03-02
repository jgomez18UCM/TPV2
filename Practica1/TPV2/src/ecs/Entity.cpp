// This file is part of the course TPV2@UCM - Samir Genaim

#include "Entity.h"
#include "Manager.h"

namespace ecs {

void ecs::Entity::addToGroupInTheManager(grpId_type gId) {
	mngr_->addToGroup(gId, this);
}


} // end of namespace

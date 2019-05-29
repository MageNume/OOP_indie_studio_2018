/*
** EPITECH PROJECT, 2019
** test
** File description:
** IrrEntity
*/

#include "IrrEntity.hpp"

IndieStudio::IrrEntity::~IrrEntity()
{
}

IndieStudio::Pos IndieStudio::IrrEntity::getPosition(void) const noexcept
{
	const irr::core::vector3df &entityPos = this->_entity->getPosition();
	IndieStudio::Pos pos(entityPos.X, entityPos.Y, entityPos.Z);
	return (pos);
}

void IndieStudio::IrrEntity::setPosition(IndieStudio::Pos position) noexcept
{
	irr::core::vector3df entityPos(position._x, position._y, position._z);
	this->_entity->setPosition(entityPos);
}

void IndieStudio::IrrEntity::setRotation(IndieStudio::Pos position) noexcept
{
	this->_entity->setRotation(irr::core::vector3df(position._x, position._y, position._z));
}

void IndieStudio::IrrEntity::setAnimation(int animation) noexcept
{
	this->_entity->setMD2Animation(static_cast<irr::scene::EMD2_ANIMATION_TYPE>(animation));
}

void IndieStudio::IrrEntity::setScale(IndieStudio::Pos scale) noexcept
{
	irr::core::vector3df newScale(scale._x, scale._y, scale._z);
	this->_entity->setScale(newScale);
}

irr::scene::IAnimatedMeshSceneNode *IndieStudio::IrrEntity::getMesh() const noexcept
{
	return (this->_entity);
}

irr::scene::IParticleSystemSceneNode *IndieStudio::IrrEntity::getParticle() const noexcept
{
	return (this->_particle);
}


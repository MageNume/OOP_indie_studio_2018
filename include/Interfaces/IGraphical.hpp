/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** IGraphical
*/

#ifndef IGRAPHICAL_HPP_
#define IGRAPHICAL_HPP_

enum Render {
	GAME,
	MAIN_MENU,
	PAUSE_MENU
};

#include <iostream>
#include <memory>
#include "IEntity.hpp"
#include "IEvent.hpp"

namespace IndieStudio {
	class IGraphical {
		public:
			virtual ~IGraphical() = default;
			virtual IndieStudio::IEntity *createMesh(std::string meshPath) const noexcept = 0;
			virtual IndieStudio::IEntity *createAnimatedMesh(std::string meshPath, std::string texturePath) const noexcept = 0;
			virtual IndieStudio::IEntity *createCube(float size, std::string texturePath, IndieStudio::Pos pos = IndieStudio::Pos(0, 0, 0), IndieStudio::Pos scale = IndieStudio::Pos(1, 1, 1)) const noexcept = 0;
			virtual IndieStudio::IEntity *createParticle(IndieStudio::Pos boxPos, IndieStudio::Pos dirPos, int min, int max, IndieStudio::Pos colorMin, IndieStudio::Pos colorMax) const noexcept = 0;
			virtual void createColision(IndieStudio::IEntity *cube, IndieStudio::IEntity *entity) const noexcept = 0;
			virtual void setCursorVisible(bool b) noexcept = 0;
			virtual bool run(void) const noexcept = 0;
			virtual void startRender(void) const noexcept = 0;
			virtual void endRender(void) const noexcept = 0;
			virtual void drawScene(void) const noexcept = 0;
			virtual void drop(void) const noexcept = 0;
			virtual void setCameraTarget(IndieStudio::Pos pos) const noexcept = 0;
			virtual IndieStudio::IEvent getEvent(void) const noexcept = 0;
			virtual void deleteEntity(IndieStudio::IEntity *) const noexcept = 0;
			virtual IndieStudio::IEntity *createImage(std::string texturePath, std::pair<int, int> pos) const noexcept = 0;
			virtual void drawImage(IndieStudio::IEntity *) const noexcept = 0;
	};
};

#endif
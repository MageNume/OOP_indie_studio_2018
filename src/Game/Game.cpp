/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** Game
*/

#include "Game.hpp"

IndieStudio::Game::Game(IndieStudio::IGraphical &graphical) : _graphical(graphical)
{
	this->createCharacters();
	// this->set_Map_Collision();
	this->createCubes();
}

IndieStudio::Game::~Game()
{
}

// void IndieStudio::Game::set_Map_Collision()
// {
// 	std::map<std::string, std::vector<irr::scene::IMeshSceneNode *>> cube;
// 	cube = _map->get_All_Cube();
// 	for (auto i = cube["Wall"].begin(); i != cube["Wall"].end(); i++)
// 		createCubeColision(*i);
// 	for (auto i = cube["Floor"].begin(); i != cube["Floor"].end(); i++)
// 		createCubeColision(*i);

// }


// void IndieStudio::Game::createCubeColision(irr::scene::IMeshSceneNode *cube) noexcept
// {
// 	/* CREATE TRIANGLE SELECTOR FOR THE CREATED CUBE */
// 	irr::scene::IMetaTriangleSelector* metaSelector = this->_sceneManager->createMetaTriangleSelector();
// 	irr::scene::ITriangleSelector* selector = 0;
// 	selector = this->_sceneManager->createTriangleSelectorFromBoundingBox(cube);
// 	cube->setTriangleSelector(selector);
// 	metaSelector->addTriangleSelector(selector);

// 	/* CREATE ANIMATOR RESPONSE FOR EACH OF THE CHARACTERS */
// 	for (auto & character_it : this->_characterVec) {
// 		irr::scene::ISceneNode *node = character_it.getMesh();
// 		irr::scene::ISceneNodeAnimatorCollisionResponse* anim = this->_sceneManager->createCollisionResponseAnimator(
// 			metaSelector,
// 			node,
// 			irr::core::vector3df(20, 20, 20),
// 			irr::core::vector3df(0, 0, 0)
// 		);
// 		node->addAnimator(anim);
// 		anim->drop();
// 	}
// }

#define CUBE_SIDE 30.f
void IndieStudio::Game::createCubes() noexcept
{
	for (int i = 0; i != 10; i++) {
		IndieStudio::IEntity *cube = this->_graphical.createCube(
			CUBE_SIDE,
			"./assets/map/bricks.png",
			IndieStudio::Pos(
				0, 0, (i * CUBE_SIDE) + 100
			)
		);
		// this->createCubeColision(cube);
		this->_cubeVec.push_back(cube);
	}
}

void IndieStudio::Game::createCharacters() noexcept
{
	this->_characterVec.push_back(
		IndieStudio::Character(this->_graphical, "assets/characters/yoshi/tris.md2", "assets/characters/yoshi/yoshi.pcx", "assets/characters/yoshi/death.wav", false, 'i', 'j', 'k', 'l', 'o')
	);
	this->_characterVec.push_back(
		IndieStudio::Character(this->_graphical, "assets/characters/spongebob/tris.md2", "assets/characters/spongebob/bob.pcx", "assets/characters/spongebob/death.wav", false, 'w', 'x', 'c', 'v', 'b')
	);
	this->_characterVec.push_back(
		IndieStudio::Character(this->_graphical, "assets/characters/eric_c/tris.md2", "assets/characters/eric_c/eric.pcx", "assets/characters/eric_c/death.wav", false, 't', 'f', 'g', 'h', 'y')
	);
	this->_characterVec.push_back(
		IndieStudio::Character(this->_graphical, "assets/characters/starfox/tris.md2", "assets/characters/starfox/starfox.pcx", "assets/characters/starfox/death.wav", false, 'z', 'q', 's', 'd', 'e')
	);
}

void IndieStudio::Game::render() noexcept
{
	// this->_sceneManager->getActiveCamera()->setTarget(irr::core::vector3df(this->_rot_x, this->_rot_y, this->_rot_z));
	this->_rot_x = cos(this->_counter) * 3;
	this->_rot_z = sin(this->_counter) * 3;
	// this->moveCharacter();
	// this->_sceneManager->drawAll();
	this->_counter += 0.05;
}

int IndieStudio::Game::getRenderStatus(void) const noexcept
{
	return this->_renderStatus;
}

void IndieStudio::Game::setRenderStatus(int status) noexcept
{
	this->_renderStatus = status;
}

#define UP_ROT 0
#define DOWN_ROT 180
#define RIGHT_ROT 90
#define LEFT_ROT 270

void checkMove(std::vector<IndieStudio::Character>::iterator character_it, bool isMoving,irr::f32 &coordinate, int rotation, bool sign)
{
	if (isMoving == true) {
		if (sign)
			coordinate += character_it->getSpeed();
		else
			coordinate -= character_it->getSpeed();
		character_it->getMesh()->setRotation(IndieStudio::Pos(0, rotation, 0));
		character_it->setIsMoving(true);
	}
}

#include "Bomb.hpp"

void IndieStudio::Game::moveCharacter() noexcept
{
	bool isMoving = false;

	for (auto character_it = this->_characterVec.begin(); character_it != this->_characterVec.end(); character_it++) {
		IndieStudio::Pos v = character_it->getMesh()->getPosition();
		isMoving = character_it->getIsMoving();
		checkMove(character_it, character_it->getMovingUp(),v._x, UP_ROT, true);
		checkMove(character_it, character_it->getMovingDown(),v._x, DOWN_ROT, false);
		checkMove(character_it, character_it->getMovingLeft(),v._z, LEFT_ROT, true);
		checkMove(character_it, character_it->getMovingRight(),v._z, RIGHT_ROT, false);
		if (character_it->getMesh()->getPosition() != v && isMoving == false)
			character_it->getMesh()->setAnimation(irr::scene::EMAT_RUN);
		else if (character_it->getMesh()->getPosition() == v && isMoving == true) {
			character_it->setIsMoving(false);
			character_it->getMesh()->setAnimation(irr::scene::EMAT_STAND);
		}
		character_it->getMesh()->setPosition(v);
		if (character_it->getDoingAction() == true) {
			character_it->getDeathSound()->playSound();
		}
		if (character_it->getDoingAction() == true && this->_keyPressed == false) {
			this->_keyPressed = true;
			character_it->checkDeleteBomb();
			if ((unsigned)character_it->getBombNb() > character_it->getLaidBomb()) {
				IndieStudio::Bomb *bomb = new IndieStudio::Bomb(this->_graphical, character_it->getMesh()->getPosition(), character_it->getBombSize());
				character_it->addBomb(bomb);
			}
		}
	}
}

bool isKeyPress2(const irr::SEvent &event, irr::EKEY_CODE key) noexcept
{
	if (event.KeyInput.Key == key)
		return true;
	else
		return false;
}

void IndieStudio::Game::checkEvent(void) noexcept
{
	IndieStudio::IEvent event = this->_graphical.getEvent();

	if (event._key[IndieStudio::Key::ESC] == true)
		this->_renderStatus = PAUSE_MENU;
	for (auto character_it = this->_characterVec.begin(); character_it != this->_characterVec.end(); character_it++) {
		character_it->setMovingUp(event._key[static_cast<IndieStudio::Key>(character_it->getUpKey())]);
		character_it->setMovingLeft(event._key[static_cast<IndieStudio::Key>(character_it->getLeftKey())]);
		character_it->setMovingDown(event._key[static_cast<IndieStudio::Key>(character_it->getDownKey())]);
		character_it->setMovingRight(event._key[static_cast<IndieStudio::Key>(character_it->getRightKey())]);
		character_it->setDoingAction(event._key[static_cast<IndieStudio::Key>(character_it->getActionKey())]);
		if (event._key[static_cast<IndieStudio::Key>(character_it->getActionKey())] == false)
			this->_keyPressed = false;
	}
}
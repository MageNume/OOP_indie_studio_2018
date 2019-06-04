/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** Bomb
*/

#include "Bomb.hpp"
#include "Audio.hpp"
#include <chrono>
#include <iostream>
#include <thread>
#include <unistd.h>

#define WALL_SIZE 40
#define EXPLOSION_DURATION 1

void setMiddle(float &vec)
{
	if ((int)vec % WALL_SIZE != 0) {
		float f = (int)vec % WALL_SIZE;
		if (f >= 0)
			vec += (WALL_SIZE / 2 - f);
		else
			vec -= (WALL_SIZE / 2 + f);
	}
	vec = static_cast<int>(vec);
}

int getMiddle(float vec)
{
	if ((int)vec % WALL_SIZE != 0) {
		float f = (int)vec % WALL_SIZE;
		if (f > 0)
			vec += (WALL_SIZE / 2 - f);
		else
			vec -= (WALL_SIZE / 2 + f);
	}
	vec = static_cast<int>(vec);
	return (vec);
}

IndieStudio::Bomb::Bomb(IndieStudio::IGraphical &graphical, IndieStudio::Pos vector, int bombSize, IndieStudio::Map &map, std::vector<std::shared_ptr<IndieStudio::Bomb>> &bombVec, std::vector<IndieStudio::Character> &characterVec) :
	_graphical(graphical),
	_map(map),
	_sound(IndieStudio::Audio("assets/bomb/bomb.wav")),
	_bombSize(bombSize),
	_bombVec(bombVec),
	_bomb(graphical.createMesh("assets/bomb/dinamite.obj")),
	_characterVec(characterVec)
{
	setMiddle(vector._x);
	setMiddle(vector._z);
	this->_bomb->setScale(IndieStudio::Pos(20, 20, 20));
	this->_bomb->setPosition(vector);
	this->createParticule(vector);
}

IndieStudio::Pos IndieStudio::Bomb::getPosition() const noexcept
{
	return (this->_bomb->getPosition());
}

IndieStudio::Bomb::~Bomb()
{
}

void IndieStudio::Bomb::createParticule(IndieStudio::Pos vector) noexcept
{
	this->_particle = this->_graphical.createParticle(
		IndieStudio::Pos(vector._x, vector._y, vector._z),
		IndieStudio::Pos(0.0f, 0.05f, 0.0f),
		3, 10,
		0,
		IndieStudio::Pos(0, 0, 0),
		IndieStudio::Pos(30, 30, 30), 0);
}

std::vector<IndieStudio::Pos> IndieStudio::Bomb::explosionDir(std::vector<IndieStudio::Pos> vec)
{
	float size = 0.05 * _bombSize;
	IndieStudio::Pos up(size, 0.0f, 0.0f);
	IndieStudio::Pos down(size * -1, 0.0f, 0.0f);
	IndieStudio::Pos right(0.0f, 0.0f, size);
	IndieStudio::Pos left(0.0f, 0.0f, size * -1);
	vec.push_back(up);
	vec.push_back(down);
	vec.push_back(right);
	vec.push_back(left);
	return (vec);
}

void IndieStudio::Bomb::createAutoParticle(IndieStudio::Pos position, int lifeTime)
{
	this->_graphical.createParticle(
		IndieStudio::Pos(
			position._x,
			position._y,
			position._z),
		IndieStudio::Pos(
			0,
			0.01,
			0),
		10, 20,
		2000,
		IndieStudio::Pos(
			255,
			1,
			1),
		IndieStudio::Pos(
			255,
			255,
			255),
		lifeTime);
}

#include <chrono>

void IndieStudio::Bomb::checkHit(IndieStudio::Pos position, std::vector<bool> boolVec)
{
	std::vector<IndieStudio::Pos> posVec;
	for (int i = 1; i != _bombSize + 1; i++) {
		posVec.push_back(IndieStudio::Pos(position._x, position._y - 10, position._z + (i * WALL_SIZE)));
		posVec.push_back(IndieStudio::Pos(position._x, position._y - 10, position._z - (i * WALL_SIZE)));
		posVec.push_back(IndieStudio::Pos(position._x + (i * WALL_SIZE), position._y - 10, position._z));
		posVec.push_back(IndieStudio::Pos(position._x - (i * WALL_SIZE), position._y - 10, position._z));
		auto bool_it = boolVec.begin();
		for (auto pos_it = posVec.begin(); pos_it != posVec.end(); pos_it++, bool_it++) {
			auto cube = this->_map.get_Cube_By_Position(*pos_it);
			if (cube != nullptr && *bool_it == false) {
				this->_map.delete_Cube(cube);
				*bool_it = true;
			}
		}
		bool_it = boolVec.begin();
		for (auto pos_it = posVec.begin(); pos_it != posVec.end(); pos_it++, bool_it++)
			for (auto bomb_it = this->_bombVec.begin(); bomb_it != this->_bombVec.end(); bomb_it++)
				if (bomb_it->get()->getPosition()._x == pos_it->_x && bomb_it->get()->getPosition()._z == pos_it->_z && bomb_it->get()->getAlive() == true && *bool_it == false)
					bomb_it->get()->explosion();
		std::thread([this, posVec, boolVec]() {
			std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
			std::chrono::time_point<std::chrono::system_clock> end;
			while (std::chrono::duration_cast<std::chrono::seconds>(end-start).count() < EXPLOSION_DURATION + 1) {
				end = std::chrono::system_clock::now();
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				auto bool_it = boolVec.begin();
				for (auto pos_it = posVec.begin(); pos_it != posVec.end(); pos_it++, bool_it++)
					for (auto character_it = this->_characterVec.begin(); character_it != this->_characterVec.end(); character_it++)
						if (pos_it->_x == getMiddle(character_it->getPosition()._x) && pos_it->_z == getMiddle(character_it->getPosition()._z) && *bool_it == false) {
							character_it->playDeathSound();
							character_it->setPosition(character_it->getSpawnPos());
						}
			}
		}).detach();
		bool_it = boolVec.begin();
		for (auto pos_it = posVec.begin(); pos_it != posVec.end(); pos_it++, bool_it++)
			if (*bool_it == false)
				this->createAutoParticle(*pos_it, EXPLOSION_DURATION);
		posVec.clear();
	}
}

void IndieStudio::Bomb::explosion()
{
	this->_alive = false;
	IndieStudio::Pos position = this->_bomb->getPosition();
	this->createAutoParticle(position, EXPLOSION_DURATION);
	this->checkHit(position, std::vector<bool>{false, false, false, false});
	this->playExplosionSound();
	this->_graphical.deleteEntity(this->_bomb);
	this->_graphical.deleteEntity(this->_particle);
}

void IndieStudio::Bomb::destroyExplosionParticle()
{
	for (std::vector<IndieStudio::IEntity *>::iterator it = this->_explosionParticule.begin();
		 it != this->_explosionParticule.end(); ++it) {
		this->_graphical.deleteEntity(*it);
	}
}

void IndieStudio::Bomb::playExplosionSound(void) noexcept
{
	this->_sound.playSound(true);
}

void IndieStudio::Bomb::startCountdown(void)
{
	for (int i = 0, j = 22; i != 4; i++) {
		std::this_thread::sleep_for(std::chrono::seconds(1));
		this->_bomb->setScale(IndieStudio::Pos(j, j, j));
		j += 2;
	}
	if (this->_alive)
		this->explosion();
}

bool IndieStudio::Bomb::getAlive() const noexcept
{
	return (this->_alive);
}
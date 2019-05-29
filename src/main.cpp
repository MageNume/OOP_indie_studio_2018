/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** main
*/

#include <irrlicht.h>
#include "Game.hpp"
#include "MenuManager.hpp"
#include "Volume.hpp"
#include "IrrGraphical.hpp"

int main()
{
	IndieStudio::IrrGraphical graphical;
	Render render = Render::MAIN_MENU;

	IndieStudio::Volume *volume = new IndieStudio::Volume();
	// IndieStudio::MenuManager menu(graphical, volume);
	IndieStudio::Game game(graphical, render);

	render = Render::GAME;
	while (graphical.run()) {
		graphical.startRender();
		if (render == Render::GAME)
			game.render();
		graphical.endRender();
	}
	graphical.drop();
	return 0;
}
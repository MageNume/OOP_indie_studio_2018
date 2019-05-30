/*
** EPITECH PROJECT, 2019
** 2018
** File description:
** MenuManager
*/

#ifndef MENUMANAGER_HPP_
#define MENUMANAGER_HPP_

#include <irrlicht.h>
#include <iostream>
#include <unistd.h>

#include "Audio.hpp"
#include "IRender.hpp"
#include "MenuMain.hpp"
#include "MenuPause.hpp"
#include "IGraphical.hpp"

namespace IndieStudio
{
class MenuManager
{
public:
	MenuManager(Render &, IGraphical &, Volume *);
	~MenuManager();

	void render(void) noexcept;
	void checkEvent(void) noexcept;
private:
	void checkActions(void) noexcept;
	void returnActionManager(void) noexcept;
	void escapeActionManager(void) noexcept;
	void leftActionManager(void) noexcept;
	void rightActionManager(void) noexcept;
	void upActionManager(void) noexcept;
	void downActionManager(void) noexcept;

private:
	Render &_renderStatus;

	MenuMain *_menuMain;
	MenuPause *_menuPause;

	Volume *_volume;
	MenuSounds *_sounds;
	IGraphical &_graphical;
	IndieStudio::IEvent _event;
};
}; // namespace IndieStudio

#endif /* !MENUMANAGER_HPP_ */
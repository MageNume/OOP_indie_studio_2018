/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** IrrGraphical
*/

#include "IrrGraphical.hpp"

IndieStudio::IrrGraphical::IrrGraphical()
{
	this->_device = irr::createDevice(
		irr::video::EDT_OPENGL,
		irr::core::dimension2d<irr::u32>(2000, 2000),
		32,
		false,
		false,
		true);
	this->_sceneManager = this->_device->getSceneManager();
	this->_driver = this->_device->getVideoDriver();
	this->_sceneManager->addCameraSceneNode(
		0,
		irr::core::vector3df(-200, 500, 0),
		irr::core::vector3df(0, 0, 0));
	this->setCursorVisible(false);
	this->_device->setEventReceiver(this);
}

IndieStudio::IrrGraphical::~IrrGraphical()
{
}

void IndieStudio::IrrGraphical::setCursorVisible(bool b) noexcept
{
	this->_device->getCursorControl()->setVisible(b);
}

IndieStudio::IEntity *IndieStudio::IrrGraphical::createMesh(std::string meshPath) const noexcept
{
	irr::scene::IMeshSceneNode *model = this->_sceneManager->addMeshSceneNode(this->_sceneManager->getMesh(meshPath.c_str()));

	if (model == NULL)
		exit(84);
	model->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	IndieStudio::IrrEntity *obj = new IndieStudio::IrrEntity(model);
	return (obj);
}

IndieStudio::IEntity *IndieStudio::IrrGraphical::createAnimatedMesh(std::string meshPath, std::string texturePath) const noexcept
{
	irr::video::ITexture *texture = this->_driver->getTexture(texturePath.c_str());
	irr::scene::IAnimatedMeshSceneNode *model = this->_sceneManager->addAnimatedMeshSceneNode(this->_sceneManager->getMesh(meshPath.c_str()));

	if (model == nullptr || texture == nullptr)
		exit(84);
	model->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	model->setMaterialTexture(0, texture);
	model->setMD2Animation(irr::scene::EMAT_STAND);
	IndieStudio::IrrEntity *obj = new IndieStudio::IrrEntity(model);
	return (obj);
}

IndieStudio::IEntity *IndieStudio::IrrGraphical::createCube(float size, std::string texturePath, IndieStudio::Pos pos, IndieStudio::Pos scale) const noexcept
{
	irr::video::ITexture *texture = this->_driver->getTexture(texturePath.c_str());
	irr::scene::IMeshSceneNode *cube = this->_sceneManager->addCubeSceneNode(
		size,
		0,
		-1,
		irr::core::vector3df(pos._x, pos._y, pos._z),
		irr::core::vector3df(0, 0, 0),
		irr::core::vector3df(scale._x, scale._y, scale._z));

	if (texture == nullptr || cube == nullptr)
		exit(84);
	cube->setMaterialTexture(0, texture);
	cube->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	IndieStudio::IEntity *obj = new IndieStudio::IrrEntity(cube);
	return (obj);
}

IndieStudio::IEntity *IndieStudio::IrrGraphical::createParticle(IndieStudio::Pos boxPos, IndieStudio::Pos dirPos, int min, int max, IndieStudio::Pos colorMin, IndieStudio::Pos colorMax) const noexcept
{
	irr::scene::IParticleSystemSceneNode *particle = this->_sceneManager->addParticleSystemSceneNode(false);
	irr::scene::IParticleEmitter *emitter = particle->createBoxEmitter(
		irr::core::aabbox3d<irr::f32>(boxPos._x, boxPos._y, boxPos._z, boxPos._x, boxPos._y, boxPos._z),
		irr::core::vector3df(dirPos._x, dirPos._y, dirPos._z),
		min, max,
		irr::video::SColor(1, colorMin._x, colorMin._y, colorMin._z),
		irr::video::SColor(1, colorMax._x, colorMax._y, colorMax._z),
		1000, 1000,
		0,
		irr::core::dimension2df(13.0f, 13.0f),
		irr::core::dimension2df(13.0f, 13.0f));
	particle->setEmitter(emitter);
	emitter->drop();
	particle->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	IndieStudio::IEntity *obj = new IndieStudio::IrrEntity(particle);
	return (obj);
}

bool IndieStudio::IrrGraphical::run(void) const noexcept
{
	return (this->_device->run());
}

void IndieStudio::IrrGraphical::startRender(void) const noexcept
{
	this->_driver->beginScene(true, true, irr::video::SColor(255, 0, 0, 0));
}

void IndieStudio::IrrGraphical::endRender(void) const noexcept
{
	this->_driver->endScene();
}

void IndieStudio::IrrGraphical::drawScene(void) const noexcept
{
	this->_sceneManager->drawAll();
}

void IndieStudio::IrrGraphical::drop(void) const noexcept
{
	this->_device->drop();
}

void IndieStudio::IrrGraphical::setCameraTarget(IndieStudio::Pos pos) const noexcept
{
	this->_sceneManager->getActiveCamera()->setTarget(irr::core::vector3df(pos._x, pos._y, pos._z));
}

bool IndieStudio::IrrGraphical::OnEvent(const irr::SEvent &event)
{
	this->_event.update(event);
	return (false);
}

IndieStudio::IEvent IndieStudio::IrrGraphical::getEvent(void) const noexcept
{
	return (this->_event);
}

void IndieStudio::IrrGraphical::deleteEntity(IndieStudio::IEntity *entity) const noexcept
{
	IndieStudio::IrrEntity *irrEntity = static_cast<IndieStudio::IrrEntity *>(entity);

	if (irrEntity->getAnimatedMesh() != nullptr)
		this->_sceneManager->addToDeletionQueue(irrEntity->getAnimatedMesh());
	if (irrEntity->getMesh() != nullptr)
		this->_sceneManager->addToDeletionQueue(irrEntity->getMesh());
	if (irrEntity->getParticle() != nullptr)
		this->_sceneManager->addToDeletionQueue(irrEntity->getParticle());
}

void IndieStudio::IrrGraphical::createColision(IndieStudio::IEntity *cube, IndieStudio::IEntity *entity) const noexcept
{
	IndieStudio::IrrEntity *irrCube = dynamic_cast<IndieStudio::IrrEntity *>(cube);
	IndieStudio::IrrEntity *irrEntity = dynamic_cast<IndieStudio::IrrEntity *>(entity);

	if (irrCube == nullptr)
		std::cout << "failed to cast cubeNode" << std::endl;
	if (irrEntity == nullptr)
		std::cout << "failed to cast entityNode" << std::endl;
	irr::scene::IMetaTriangleSelector *metaSelector = this->_sceneManager->createMetaTriangleSelector();
	irr::scene::ITriangleSelector *selector = 0;
	selector = this->_sceneManager->createTriangleSelectorFromBoundingBox(irrCube->getMesh());
	irrCube->getMesh()->setTriangleSelector(selector);
	metaSelector->addTriangleSelector(selector);

	irr::scene::ISceneNode *node = irrEntity->getAnimatedMesh();
	irr::scene::ISceneNodeAnimatorCollisionResponse *anim = this->_sceneManager->createCollisionResponseAnimator(
		metaSelector,
		node,
		irr::core::vector3df(20, 20, 20),
		irr::core::vector3df(0, 0, 0));
	node->addAnimator(anim);
	anim->drop();
}

IndieStudio::IEntity *IndieStudio::IrrGraphical::createImage(std::string texturePath, std::pair<int, int> pos) const noexcept
{
	irr::video::ITexture *image;
	image = this->_driver->getTexture(texturePath.c_str());
	irr::core::dimension2d<irr::u32> taille = image->getSize();
	irr::core::position2d<irr::s32> position0;
	position0.X = 0;
	position0.Y = 0;
	irr::core::position2d<irr::s32> position1;
	position1.X = taille.Width;
	position1.Y = taille.Height;
	irr::core::rect<irr::s32> rectangle;
	rectangle.UpperLeftCorner = position0;
	rectangle.LowerRightCorner = position1;
	if (pos.first == -1)
	{
		irr::core::dimension2d<irr::u32> screen = _driver->getScreenSize();
		pos.first = screen.Width / 2 - rectangle.LowerRightCorner.X / 2;
	}
	IndieStudio::IrrEntity *obj = new IndieStudio::IrrEntity(image, pos, rectangle);
	return (obj);
}

void IndieStudio::IrrGraphical::drawImage(IndieStudio::IEntity *image) const noexcept
{
	IndieStudio::IrrEntity *irrImage = dynamic_cast<IndieStudio::IrrEntity *>(image);
	this->_driver->draw2DImage(irrImage->getImage(), irrImage->get2DPos(), irrImage->getRectangle(), 0, irr::video::SColor(255, 255, 255, 255), true);
}

void IndieStudio::IrrGraphical::setSkin(IndieStudio::IEntity *image, std::string skin) noexcept
{
	IndieStudio::IrrEntity *irrImage = dynamic_cast<IndieStudio::IrrEntity *>(image);
	irrImage->setImage(this->_driver->getTexture(skin.c_str()));
}

void IndieStudio::IrrGraphical::setCustomRectangle(IndieStudio::IEntity *image, int p0X, int p0Y, int p1X, int p1Y) noexcept
{
	IndieStudio::IrrEntity *irrImage = dynamic_cast<IndieStudio::IrrEntity *>(image);
	irr::core::position2d<irr::s32> position0;
	position0.X = p0X;
	position0.Y = p0Y;
	irr::core::position2d<irr::s32> position1;
	position1.X = p1X;
	position1.Y = p1Y;
	irr::core::rect<irr::s32> rectangle;
	rectangle.UpperLeftCorner = position0;
	rectangle.LowerRightCorner = position1;
	irrImage->setRectangle(rectangle);
}
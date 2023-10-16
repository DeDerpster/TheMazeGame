#include "GUILayer.h"

#include "Button.h"
#include "GUILayer.h"
#include "MenuBackgroundObject.h"
#include "MenuItemHolderManager.h"
#include "StatBar.h"
#include "TransferObject.h"

#include "Utils.h"

#include "Application.h"

GUILayer::GUILayer()
	: m_ConnectedLevel(nullptr)
{
}

GUILayer::GUILayer(Level *connectedLevel)
	: m_ConnectedLevel(connectedLevel)
{
}

GUILayer::GUILayer(GUILayer::Type genType, Level *connectedLevel)
	: m_ConnectedLevel(connectedLevel)
{
	switch(genType)
	{
	case GUILayer::Type::GameOverlay:
	{
		auto clickedFunc = [this](int index, Level *level) {
			m_ConnectedLevel->getPlayer()->setCurrentWeapon(index);
		};
		addMenuObject(new MIHManager(0, 0, 300, 100, 100, this, m_ConnectedLevel->getPlayer()->getWeapons(), clickedFunc, m_ConnectedLevel->getPlayer()->getCurrentWeaponPointer()));

		auto posFunc = [](float *x, float *y, float *width, float *height) {
			*x      = Application::getWidth() / 2;
			*y      = 20;
			*width  = Application::getWidth() / 3;
			*height = 10;
		};
		addMenuObject(new StatBar(posFunc, this, m_ConnectedLevel->getPlayer()->getHealthPointer(), m_ConnectedLevel->getPlayer()->getMaxHealthPointer()));

		break;
	}
	case GUILayer::Type::PlayerInventory:
	{
		TransferObject *transfer = new TransferObject(TILE_SIZE, TILE_SIZE, this);
		addMenuObject(transfer);

		auto backgroundPosFunc = [](float *x, float *y, float *width, float *height) {
			*width  = 550;
			*height = 575;
			*x      = Application::getWidth() / 2;
			*y      = Application::getHeight() / 2 + 75.0f / 2.0f;
		};

		auto exitFunc = []() {
			Event::ExitGUIMenuEvent e;
			Application::callEvent(e, true);
		};

		addMenuObject(new MenuBackground(backgroundPosFunc, this, {0.3f, 0.3f, 0.3f, 0.9f}, exitFunc));

		auto clickedFunc = [this](int index, Level *level) {   // TODO: remove the level *
			m_ConnectedLevel->getPlayer()->useItemInInventory(index);
		};

		auto posFunc = [](float *x, float *y, float *width, float *height) {
			*width  = 500;
			*height = 400;
			*x      = Application::getWidth() / 2 - *width / 2;
			*y      = Application::getHeight() / 2 + *height / 2 - 112.5f;
		};
		addMenuObject(new MIHManager(posFunc, 100, this, m_ConnectedLevel->getPlayer()->getInventory(), clickedFunc));

		auto clickedWeaponFunc = [this](int index, Level *level) {
			m_ConnectedLevel->getPlayer()->setCurrentWeapon(index);
		};
		auto posWeaponFunc = [](float *x, float *y, float *width, float *height) {
			*width  = 300;
			*height = 100;
			*x      = Application::getWidth() / 2 - *width / 2;
			*y      = Application::getHeight() / 2 + 200;
		};
		addMenuObject(new MIHManager(posWeaponFunc, 100, this, m_ConnectedLevel->getPlayer()->getWeapons(), clickedWeaponFunc, m_ConnectedLevel->getPlayer()->getCurrentWeaponPointer()));

		break;
	}
	case GUILayer::Type::ChestInventory:
	{
		TransferObject *transfer = new TransferObject(TILE_SIZE, TILE_SIZE, this);
		addMenuObject(transfer);

		auto backgroundPosFunc = [](float *x, float *y, float *width, float *height) {
			*width  = 900;
			*height = 675;
			*x      = Application::getWidth() / 2;
			*y      = Application::getHeight() / 2 + 75.0f / 2.0f;
		};
		auto exitFunc = []() {
			Event::ExitGUIMenuEvent e;
			Application::callEvent(e, true);
		};
		addMenuObject(new MenuBackground(backgroundPosFunc, this, {0.3f, 0.3f, 0.3f, 0.9f}, exitFunc));

		auto clickedFunc = [](int index, Level *level) {   // TODO: remove the level *
														   // this->m_ConnectedLevel->getPlayer().useItemInInventory(index);
		};

		auto posFuncForPlayerInventory = [](float *x, float *y, float *width, float *height) {
			*width  = 400;
			*height = 500;
			*x      = Application::getWidth() / 2 - *width - 10.0f;
			*y      = Application::getHeight() / 2 + *height / 2 - 112.5f;
		};
		addMenuObject(new MIHManager(posFuncForPlayerInventory, 100, this, m_ConnectedLevel->getPlayer()->getInventory(), clickedFunc));

		auto posFuncForChestInventory = [](float *x, float *y, float *width, float *height) {
			*width  = 400;
			*height = 500;
			*x      = Application::getWidth() / 2 + 10.0f;
			*y      = Application::getHeight() / 2 + *height / 2 - 112.5f;
		};
		MIHManager *chestSection = new MIHManager(posFuncForChestInventory, 100, this, nullptr, clickedFunc);
		addMenuObject(chestSection);

		auto setChestToMenu = [chestSection](ItemContainer &items) {
			chestSection->setInventory(&items);
		};

		m_ConnectedLevel->setChestMenuFunc(setChestToMenu);

		auto clickedWeaponFunc = [this](int index, Level *level) {
			m_ConnectedLevel->getPlayer()->setCurrentWeapon(index);
		};
		auto posWeaponFunc = [](float *x, float *y, float *width, float *height) {
			*width  = 300;
			*height = 100;
			*x      = Application::getWidth() / 2 - *width / 2;
			*y      = Application::getHeight() / 2 + 250;
		};
		addMenuObject(new MIHManager(posWeaponFunc, 100, this, m_ConnectedLevel->getPlayer()->getWeapons(), clickedWeaponFunc, m_ConnectedLevel->getPlayer()->getCurrentWeaponPointer()));

		break;
	}
	default:
		Log::warning("Unknown GUI type");
		break;
	}
}

GUILayer::~GUILayer()
{
}

void GUILayer::render()
{
	for(MenuObject *obj : m_Objects)
		obj->render();
}

void GUILayer::update()
{
	for(MenuObject *obj : m_Objects)
		obj->update();
}

bool GUILayer::eventCallback(const Event::Event &e)
{
	for(MenuObject *obj : m_Objects)
	{
		if(obj->eventCallback(e))
			return true;
	}
	return false;
}

void GUILayer::addMenuObject(MenuObject *object)
{
	m_Objects.push_back(object);
}

void GUILayer::setConnectedLevel(Level *level)
{
	m_ConnectedLevel = level;
}

void GUILayer::transferObject(TransferObject *transfer)
{
	for(MenuObject *obj : m_Objects)
	{
		MIHManager *manager = dynamic_cast<MIHManager *>(obj);
		if(manager)
		{
			int hoverBlock = manager->getIndexMouseAt();
			if(hoverBlock != -1)
			{
				manager->transferItem(transfer);
				break;
			}
		}
	}
}
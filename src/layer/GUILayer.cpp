#include "GUILayer.h"

#include "gui/object/Button.h"
#include "gui/object/Image.h"
#include "gui/object/MenuBackgroundObject.h"
#include "gui/object/MenuItemHolderManager.h"
#include "gui/object/StatBar.h"
#include "gui/object/TextMenuObject.h"
#include "gui/object/TransferObject.h"

#include "rendering/sprite/Sprite.h"

#include "Utils.h"

#include "Application.h"

#include "event/game/PlayerResponse.h"
#include "event/menu/ChangeGUIMenu.h"

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
	case GUILayer::Type::MainMenu:   // TODO: Add scale
	{
		auto backgroundPosFunc = [](float *x, float *y, float *width, float *height) {
			*width  = Application::getWidth();
			*height = Application::getHeight();
			*x      = Application::getWidth() / 2;
			*y      = Application::getHeight() / 2;
		};

		addMenuObject(new MenuBackground(backgroundPosFunc, this, {1.0f, 1.0f, 1.0f, 1.0f}, []() {}));

		auto posFunc = [](float *x, float *y, float *width, float *height) {
			*width  = Application::getWidth() / 4;
			*height = 1080.0f * (*width) / 1920.0f;
			*x      = Application::getWidth() / 2;
			*y      = 3 * Application::getHeight() / 4;
		};

		addMenuObject(new Image(posFunc, Sprite::ID::menuTitle, this));

		auto exitFunc = []() {
			Application::closeApplication();
		};

		auto exitPos = [](float *x, float *y, float *width, float *height) {
			*width  = 400;
			*height = 100;
			*x      = Application::getWidth() / 2;
			*y      = Application::getHeight() / 2 - (*height);
		};

		Text exitText("Exit");

		addMenuObject(new Button(exitText, exitPos, this, exitFunc));

		auto startFunc = []() {
			Application::startGame();
		};

		auto startPos = [](float *x, float *y, float *width, float *height) {
			*width  = 400;
			*height = 100;
			*x      = Application::getWidth() / 2;
			*y      = Application::getHeight() / 2 + (*height);
		};

		Text startText("Start");

		addMenuObject(new Button(startText, startPos, this, startFunc));

		break;
	}

	case GUILayer::Type::GameOverlay:   // TODO: Turn these into functions?
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

		auto exitFunc = []() {
			Event::ChangeGUIActiveLayerEvent e(InGameGUILayer::exitMenu);
			Application::callEvent(e, Event::CallType::Overlay);
		};

		auto exitPos = [](float *x, float *y, float *width, float *height) {
			*width  = 200;
			*height = 50;
			*x      = Application::getWidth() - (*width) / 2;
			*y      = Application::getHeight() - (*height) / 2;
		};

		Text exitText("Exit");

		addMenuObject(new Button(exitText, exitPos, this, exitFunc));

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
			Event::ChangeGUIActiveLayerEvent e(InGameGUILayer::overlay);
			Application::callEvent(e, Event::CallType::Overlay);
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
			Event::ChangeGUIActiveLayerEvent e(InGameGUILayer::overlay);
			Application::callEvent(e, Event::CallType::Overlay);
		};
		addMenuObject(new MenuBackground(backgroundPosFunc, this, {0.3f, 0.3f, 0.3f, 0.9f}, exitFunc));

		auto clickedFunc = [](int index, Level *level) {   // TODO: remove the level * and do something here
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
		MIHManager *chestSection = new MIHManager(posFuncForChestInventory, 100, this, nullptr, clickedFunc, nullptr, GUIInventoryIDCode::inventory);
		addMenuObject(chestSection);

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

	case GUILayer::Type::NPCInventory:
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
			Event::ChangeGUIActiveLayerEvent e(InGameGUILayer::overlay);
			Application::callEvent(e, Event::CallType::Overlay);
		};
		addMenuObject(new MenuBackground(backgroundPosFunc, this, {0.3f, 0.3f, 0.3f, 0.9f}, exitFunc));

		auto clickedFunc = [](int index, Level *level) {   // TODO: remove the level * and do something here
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
		MIHManager *chestSection = new MIHManager(posFuncForChestInventory, 100, this, nullptr, clickedFunc, nullptr, GUIInventoryIDCode::inventory);
		addMenuObject(chestSection);

		auto playerClickedWeaponFunc = [this](int index, Level *level) {
			m_ConnectedLevel->getPlayer()->setCurrentWeapon(index);
		};
		auto playerPosWeaponFunc = [](float *x, float *y, float *width, float *height) {
			*width  = 300;
			*height = 100;
			*x      = Application::getWidth() / 2 - *width / 2 - 210.0f;
			*y      = Application::getHeight() / 2 + 250;
		};
		addMenuObject(new MIHManager(playerPosWeaponFunc, 100, this, m_ConnectedLevel->getPlayer()->getWeapons(), playerClickedWeaponFunc, m_ConnectedLevel->getPlayer()->getCurrentWeaponPointer()));

		auto npcClickedWeaponFunc = [this](int index, Level *level) {
		};
		auto npcPosWeaponFunc = [](float *x, float *y, float *width, float *height) {
			*width  = 300;
			*height = 100;
			*x      = Application::getWidth() / 2 - *width / 2 + 210.0f;
			*y      = Application::getHeight() / 2 + 250;
		};
		addMenuObject(new MIHManager(npcPosWeaponFunc, 100, this, nullptr, npcClickedWeaponFunc, nullptr, GUIInventoryIDCode::weapons));

		break;
	}

	case GUILayer::Type::NPCInteraction:
	{
		auto backgroundPosFunc = [](float *x, float *y, float *width, float *height) {
			*width  = 470;
			*height = 100;
			*x      = Application::getWidth() / 2;
			*y      = (*height) / 2 + 50.0f;
		};

		auto exitFunc = []() {
			Event::PlayerResponseEvent e(Event::PlayerResponseEvent::Response::reject);
			Application::callEvent(e);
		};

		addMenuObject(new MenuBackground(backgroundPosFunc, this, {0.3f, 0.3f, 0.3f, 0.9f}, exitFunc));

		auto rejectFunc = []() {
			Event::PlayerResponseEvent e(Event::PlayerResponseEvent::Response::reject);
			Application::callEvent(e);
		};

		auto rejectPos = [](float *x, float *y, float *width, float *height) {
			*width  = 200;
			*height = 50;
			*x      = Application::getWidth() / 2 - (*width) / 2 - 10.0f;
			*y      = 100.0f;
		};

		Text rejectText("Reject");

		addMenuObject(new Button(rejectText, rejectPos, this, rejectFunc));

		auto acceptFunc = []() {
			Event::PlayerResponseEvent e(Event::PlayerResponseEvent::Response::accept);
			Application::callEvent(e);
		};

		auto acceptPos = [](float *x, float *y, float *width, float *height) {
			*width  = 200;
			*height = 50;
			*x      = Application::getWidth() / 2 + (*width) / 2 + 10.0f;
			*y      = 100.0f;
		};

		Text acceptText("Accept");

		addMenuObject(new Button(acceptText, acceptPos, this, acceptFunc));

		break;
	}

	case GUILayer::Type::ExitMenu:
	{
		auto backgroundPosFunc = [](float *x, float *y, float *width, float *height) {
			*width  = 250;
			*height = 175;
			*x      = Application::getWidth() / 2;
			*y      = Application::getHeight() / 2;
		};

		auto exitFunc = []() {
			Event::ChangeGUIActiveLayerEvent e(InGameGUILayer::overlay);
			Application::callEvent(e, Event::CallType::Overlay);
		};

		addMenuObject(new MenuBackground(backgroundPosFunc, this, {0.3f, 0.3f, 0.3f, 0.9f}, exitFunc));

		auto exitGameFunc = []() {
			Application::exitGame();
		};

		auto exitPos = [](float *x, float *y, float *width, float *height) {
			*width  = 200;
			*height = 50;
			*x      = Application::getWidth() / 2;
			*y      = Application::getHeight() / 2 - (*height) / 2 - 12.5f;
		};

		Text exitText("Main Menu");

		addMenuObject(new Button(exitText, exitPos, this, exitGameFunc));

		auto continuePos = [](float *x, float *y, float *width, float *height) {
			*width  = 200;
			*height = 50;
			*x      = Application::getWidth() / 2;
			*y      = Application::getHeight() / 2 + (*height) / 2 + 12.5f;
		};

		Text continueText("Continue");

		addMenuObject(new Button(continueText, continuePos, this, exitFunc));

		break;
	}

	case GUILayer::Type::PlayerDeath:
	{
		auto backgroundPosFunc = [](float *x, float *y, float *width, float *height) {
			*width  = Application::getWidth();
			*height = Application::getHeight();
			*x      = Application::getWidth() / 2;
			*y      = Application::getHeight() / 2;
		};

		auto exitFunc = []() {
			Application::exitGame();
		};

		addMenuObject(new MenuBackground(backgroundPosFunc, this, {0.0f, 0.0f, 0.0f, 1.0f}, exitFunc));

		auto exitPos = [](float *x, float *y, float *width, float *height) {
			*width  = 600;
			*height = 200;
			*x      = Application::getWidth() / 2;
			*y      = (*height) / 2 + 200.0f;
		};

		Text exitText("Main Menu", 0.0f, 0.0f, 150.0f, {1.0f, 1.0f, 1.0f, 1.0f});

		addMenuObject(new Button(exitText, exitPos, this, exitFunc));

		auto textPos = [](float *x, float *y) {
			*x = Application::getWidth() / 2;
			*y = Application::getHeight() / 2 + 150.0f;
		};

		Text text("YOU DIED...", 0.0f, 0.0f, 800.0f, {1.0f, 0.0f, 0.0f, 1.0f});

		addMenuObject(new TextMenuObject(text, textPos, this));

		break;
	}

	case GUILayer::Type::PlayerWin:
	{
		auto backgroundPosFunc = [](float *x, float *y, float *width, float *height) {
			*width  = Application::getWidth();
			*height = Application::getHeight();
			*x      = Application::getWidth() / 2;
			*y      = Application::getHeight() / 2;
		};

		auto exitFunc = []() {
			Application::exitGame();
		};

		addMenuObject(new MenuBackground(backgroundPosFunc, this, {0.0f, 0.0f, 0.0f, 1.0f}, exitFunc));

		auto exitPos = [](float *x, float *y, float *width, float *height) {
			*width  = 600;
			*height = 200;
			*x      = Application::getWidth() / 2;
			*y      = (*height) / 2 + 200.0f;
		};

		Text exitText("Main Menu", 0.0f, 0.0f, 150.0f, {1.0f, 1.0f, 1.0f, 1.0f});

		addMenuObject(new Button(exitText, exitPos, this, exitFunc));

		auto textPos = [](float *x, float *y) {
			*x = Application::getWidth() / 2;
			*y = Application::getHeight() / 2 + 150.0f;
		};

		Text text("YOU WIN!", 0.0f, 0.0f, 800.0f, {1.0f, 0.843f, 0.0f, 1.0f});

		addMenuObject(new TextMenuObject(text, textPos, this));

		break;
	}

	default:
		Log::warning("Unknown GUI type");
		break;
	}
}

GUILayer::~GUILayer()
{
	for(MenuObject *obj : m_Objects)
		delete obj;
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

	return Layer::eventCallback(e);
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
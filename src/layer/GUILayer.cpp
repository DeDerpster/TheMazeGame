#include "GUILayer.h"

#include "gui/object/Button.h"
#include "gui/object/Image.h"
#include "gui/object/MenuBackgroundObject.h"
#include "gui/object/MenuItemHolderManager.h"
#include "gui/object/Slider.h"
#include "gui/object/StatBar.h"
#include "gui/object/TextMenuObject.h"
#include "gui/object/TransferObject.h"

#include "Application.h"
#include "rendering/sprite/Sprite.h"
#include "layer/level/Level.h"

#include "entity/movableEntity/mob/Player.h"
#include "entity/object/WorldItem.h"

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
	// Goes through all the different layers and builds the setup
	switch(genType)
	{
	case GUILayer::Type::MainMenu:
	{
		auto backgroundPosFunc = [](float *x, float *y, float *width, float *height) {
			*width  = Application::getWidth();
			*height = Application::getHeight();
			*x      = Application::getWidth() / 2;
			*y      = Application::getHeight() / 2;
		};

		addMenuObject(new MenuBackground(backgroundPosFunc, this, {1.0f, 1.0f, 1.0f, 1.0f}, []() {}));

		auto posFunc = [](float *x, float *y, float *width, float *height) {
			if(Application::getWidth < Application::getHeight)
			{
				*width  = Application::getWidth() / 4;
				*height = 1080.0f * (*width) / 1920.0f;
			}
			else
			{
				*height = Application::getHeight() / 3;
				*width  = 1920.0f * (*height) / 1080.0f;
			}

			*x      = Application::getWidth() / 2;
			*y      = 3 * Application::getHeight() / 4;
		};

		addMenuObject(new Image(posFunc, Sprite::ID::menuTitle, this));

		auto exitFunc = []() {
			Application::closeApplication();
		};

		auto exitPos = [](float *x, float *y, float *width, float *height) {
			*width  = 400 * Application::getGUIScale();
			*height = 100 * Application::getGUIScale();
			*x      = Application::getWidth() / 2;
			*y      = Application::getHeight() / 3 - (*height) / 2 - (*height) * Application::getHeight() / 2160.0f;
		};

		Text exitText("Exit");

		addMenuObject(new Button(exitText, exitPos, this, exitFunc));

		auto startFunc = []() {
			Application::startGame();
		};

		auto startPos = [](float *x, float *y, float *width, float *height) {
			*width  = 400 * Application::getGUIScale();
			*height = 100 * Application::getGUIScale();
			*x      = Application::getWidth() / 2;
			*y      = Application::getHeight() / 3 + (*height) / 2 + (*height) * Application::getHeight() / 2160.0f;
		};

		Text startText("Start");

		addMenuObject(new Button(startText, startPos, this, startFunc));

		break;
	}

	case GUILayer::Type::GameOverlay:
	{
		auto clickedFunc = [this](int index) {
			m_ConnectedLevel->getPlayer()->setCurrentWeapon(index);
		};
		auto weaponPosFunc = [](float *x, float *y, float *width, float *height) {
			*x      = 0.0f;
			*y      = 0.0f;
			*width  = 300 * Application::getGUIScale();
			*height = 100 * Application::getGUIScale();
		};
		addMenuObject(new MIHManager(weaponPosFunc, 100, this, clickedFunc, m_ConnectedLevel->getPlayer()->getWeapons(), m_ConnectedLevel->getPlayer()->getCurrentWeaponPointer()));

		auto posFunc = [](float *x, float *y, float *width, float *height) {
			*x      = Application::getWidth() / 2;
			*y      = 20;
			*width  = Application::getWidth() / 3;
			*height = 10 * Application::getGUIScale();
		};
		std::string name = "Health";
		addMenuObject(new StatBar(name, posFunc, this, m_ConnectedLevel->getPlayer()->getStats()->getHealthPointer(), m_ConnectedLevel->getPlayer()->getStats()->getMaxHealthPointer()));

		auto exitFunc = []() {
			Event::ChangeGUIMenuEvent e(Event::ChangeGUIMenuEvent::Menu::ExitMenu);
			Application::callEvent(e, Event::CallType::Overlay);
		};

		auto exitPos = [](float *x, float *y, float *width, float *height) {
			*width  = 200 * Application::getGUIScale();
			*height = 50 * Application::getGUIScale();
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
			*width  = 550 * Application::getGUIScale();
			*height = 575 * Application::getGUIScale();
			*x      = Application::getWidth() / 2;
			*y      = Application::getHeight() / 2 + 75.0 * Application::getGUIScale() / 2.0f;
		};

		auto exitFunc = []() {
			Event::ChangeGUIMenuEvent e(Event::ChangeGUIMenuEvent::Menu::Overlay);
			Application::callEvent(e, Event::CallType::Overlay);
		};

		addMenuObject(new MenuBackground(backgroundPosFunc, this, {0.3f, 0.3f, 0.3f, 0.9f}, exitFunc));

		auto clickedFunc = [this](int index) {
			m_ConnectedLevel->getPlayer()->useItemInInventory(index);
		};

		auto posFunc = [](float *x, float *y, float *width, float *height) {
			*width  = 500 * Application::getGUIScale();
			*height = 400 * Application::getGUIScale();
			*x      = Application::getWidth() / 2 - *width / 2;
			*y      = Application::getHeight() / 2 + *height / 2 - 112.5f * Application::getGUIScale();
		};
		addMenuObject(new MIHManager(posFunc, 100, this, clickedFunc, m_ConnectedLevel->getPlayer()->getInventory()));

		auto clickedWeaponFunc = [this](int index) {
			m_ConnectedLevel->getPlayer()->setCurrentWeapon(index);
		};
		auto posWeaponFunc = [](float *x, float *y, float *width, float *height) {
			*width  = 300 * Application::getGUIScale();
			*height = 100 * Application::getGUIScale();
			*x      = Application::getWidth() / 2 - *width / 2;
			*y      = Application::getHeight() / 2 + 200 * Application::getGUIScale();
		};
		addMenuObject(new MIHManager(posWeaponFunc, 100, this, clickedWeaponFunc, m_ConnectedLevel->getPlayer()->getWeapons(), m_ConnectedLevel->getPlayer()->getCurrentWeaponPointer()));

		break;
	}

	case GUILayer::Type::ChestInventory:
	{
		TransferObject *transfer = new TransferObject(TILE_SIZE, TILE_SIZE, this);
		addMenuObject(transfer);

		auto backgroundPosFunc = [](float *x, float *y, float *width, float *height) {
			*width  = 900 * Application::getGUIScale();
			*height = 675 * Application::getGUIScale();
			*x      = Application::getWidth() / 2;
			*y      = Application::getHeight() / 2 + 75.0f * Application::getGUIScale() / 2.0f;
		};
		auto exitFunc = []() {
			Event::ChangeGUIMenuEvent e(Event::ChangeGUIMenuEvent::Menu::Overlay);
			Application::callEvent(e, Event::CallType::Overlay);
		};
		addMenuObject(new MenuBackground(backgroundPosFunc, this, {0.3f, 0.3f, 0.3f, 0.9f}, exitFunc));

		auto clickedFunc = [](int index) {
		};

		auto posFuncForPlayerInventory = [](float *x, float *y, float *width, float *height) {
			*width  = 400 * Application::getGUIScale();
			*height = 500 * Application::getGUIScale();
			*x      = Application::getWidth() / 2 - *width - 10.0f * Application::getGUIScale();
			*y      = Application::getHeight() / 2 + *height / 2 - 112.5f * Application::getGUIScale();
		};
		addMenuObject(new MIHManager(posFuncForPlayerInventory, 100, this, clickedFunc, m_ConnectedLevel->getPlayer()->getInventory()));

		auto posFuncForChestInventory = [](float *x, float *y, float *width, float *height) {
			*width  = 400 * Application::getGUIScale();
			*height = 500 * Application::getGUIScale();
			*x      = Application::getWidth() / 2 + 10.0f * Application::getGUIScale();
			*y      = Application::getHeight() / 2 + *height / 2 - 112.5f * Application::getGUIScale();
		};
		MIHManager *chestSection = new MIHManager(posFuncForChestInventory, 100, this, clickedFunc, IContainer::Type::Item);
		addMenuObject(chestSection);

		auto clickedWeaponFunc = [this](int index) {
			m_ConnectedLevel->getPlayer()->setCurrentWeapon(index);
		};
		auto posWeaponFunc = [](float *x, float *y, float *width, float *height) {
			*width  = 300 * Application::getGUIScale();
			*height = 100 * Application::getGUIScale();
			*x      = Application::getWidth() / 2 - *width / 2;
			*y      = Application::getHeight() / 2 + 250.0f * Application::getGUIScale();
		};
		addMenuObject(new MIHManager(posWeaponFunc, 100, this, clickedWeaponFunc, m_ConnectedLevel->getPlayer()->getWeapons(), m_ConnectedLevel->getPlayer()->getCurrentWeaponPointer()));

		break;
	}

	case GUILayer::Type::NPCInventory:
	{
		TransferObject *transfer = new TransferObject(TILE_SIZE, TILE_SIZE, this);
		addMenuObject(transfer);

		auto backgroundPosFunc = [](float *x, float *y, float *width, float *height) {
			*width  = 900 * Application::getGUIScale();
			*height = 675 * Application::getGUIScale();
			*x      = Application::getWidth() / 2;
			*y      = Application::getHeight() / 2 + 75.0f * Application::getGUIScale() / 2.0f;
		};
		auto exitFunc = []() {
			Event::ChangeGUIMenuEvent e(Event::ChangeGUIMenuEvent::Menu::Overlay);
			Application::callEvent(e, Event::CallType::Overlay);
		};
		addMenuObject(new MenuBackground(backgroundPosFunc, this, {0.3f, 0.3f, 0.3f, 0.9f}, exitFunc));

		auto clickedFunc = [](int index) {
		};

		auto posFuncForPlayerInventory = [](float *x, float *y, float *width, float *height) {
			*width  = 400 * Application::getGUIScale();
			*height = 500 * Application::getGUIScale();
			*x      = Application::getWidth() / 2 - *width - 10.0f * Application::getGUIScale();
			*y      = Application::getHeight() / 2 + *height / 2 - 112.5f * Application::getGUIScale();
		};
		addMenuObject(new MIHManager(posFuncForPlayerInventory, 100, this, clickedFunc, m_ConnectedLevel->getPlayer()->getInventory()));

		auto posFuncForNPCInventory = [](float *x, float *y, float *width, float *height) {
			*width  = 400 * Application::getGUIScale();
			*height = 500 * Application::getGUIScale();
			*x      = Application::getWidth() / 2 + 10.0f * Application::getGUIScale();
			*y      = Application::getHeight() / 2 + *height / 2 - 112.5f * Application::getGUIScale();
		};
		MIHManager *npcSection = new MIHManager(posFuncForNPCInventory, 100, this, clickedFunc, IContainer::Type::Item);
		addMenuObject(npcSection);

		auto playerClickedWeaponFunc = [this](int index) {
			m_ConnectedLevel->getPlayer()->setCurrentWeapon(index);
		};
		auto playerPosWeaponFunc = [](float *x, float *y, float *width, float *height) {
			*width  = 300 * Application::getGUIScale();
			*height = 100 * Application::getGUIScale();
			*x      = Application::getWidth() / 2 - *width / 2 - 210.0f * Application::getGUIScale();
			*y      = Application::getHeight() / 2 + 250.0f * Application::getGUIScale();
		};
		addMenuObject(new MIHManager(playerPosWeaponFunc, 100, this, playerClickedWeaponFunc, m_ConnectedLevel->getPlayer()->getWeapons(), m_ConnectedLevel->getPlayer()->getCurrentWeaponPointer()));

		auto npcClickedWeaponFunc = [this](int index) {
		};
		auto npcPosWeaponFunc = [](float *x, float *y, float *width, float *height) {
			*width  = 300 * Application::getGUIScale();
			*height = 100 * Application::getGUIScale();
			*x      = Application::getWidth() / 2 - *width / 2 + 210.0f * Application::getGUIScale();
			*y      = Application::getHeight() / 2 + 250.0f * Application::getGUIScale();
		};
		addMenuObject(new MIHManager(npcPosWeaponFunc, 100, this, npcClickedWeaponFunc, IContainer::Type::Weapon));

		break;
	}

	case GUILayer::Type::NPCInteraction:
	{
		auto backgroundPosFunc = [](float *x, float *y, float *width, float *height) {
			*width  = 470 * Application::getGUIScale();
			*height = 100 * Application::getGUIScale();
			*x      = Application::getWidth() / 2;
			*y      = (*height) / 2 + 50.0f * Application::getGUIScale();
		};

		auto exitFunc = []() {
			Event::PlayerResponseEvent e(Event::PlayerResponseEvent::Response::Reject);
			Application::callEvent(e);
		};

		addMenuObject(new MenuBackground(backgroundPosFunc, this, {0.3f, 0.3f, 0.3f, 0.9f}, exitFunc));

		auto rejectFunc = []() {
			Event::PlayerResponseEvent e(Event::PlayerResponseEvent::Response::Reject);
			Application::callEvent(e);
		};

		auto rejectPos = [](float *x, float *y, float *width, float *height) {
			*width  = 200 * Application::getGUIScale();
			*height = 50 * Application::getGUIScale();
			*x      = Application::getWidth() / 2 - (*width) / 2 - 10.0f * Application::getGUIScale();
			*y      = 100.0f * Application::getGUIScale();
		};

		Text rejectText("Reject");

		addMenuObject(new Button(rejectText, rejectPos, this, rejectFunc));

		auto acceptFunc = []() {
			Event::PlayerResponseEvent e(Event::PlayerResponseEvent::Response::Accept);
			Application::callEvent(e);
		};

		auto acceptPos = [](float *x, float *y, float *width, float *height) {
			*width  = 200 * Application::getGUIScale();
			*height = 50 * Application::getGUIScale();
			*x      = Application::getWidth() / 2 + (*width) / 2 + 10.0f * Application::getGUIScale();
			*y      = 100.0f * Application::getGUIScale();
		};

		Text acceptText("Accept");

		addMenuObject(new Button(acceptText, acceptPos, this, acceptFunc));

		break;
	}

	case GUILayer::Type::ExitMenu:
	{
		auto backgroundPosFunc = [](float *x, float *y, float *width, float *height) {
			*width  = 250 * Application::getGUIScale();
			*height = 175 * Application::getGUIScale();
			*x      = Application::getWidth() / 2;
			*y      = Application::getHeight() / 2;
		};

		auto exitFunc = []() {
			Event::ChangeGUIMenuEvent e(Event::ChangeGUIMenuEvent::Menu::Overlay);
			Application::callEvent(e, Event::CallType::Overlay);
		};

		addMenuObject(new MenuBackground(backgroundPosFunc, this, {0.3f, 0.3f, 0.3f, 0.9f}, exitFunc));

		auto sliderPos = [](float *x, float *y, float *width, float *height) {
			*width  = 200 * Application::getGUIScale();
			*height = 20 * Application::getGUIScale();
			*x      = Application::getWidth() / 2;
			*y      = Application::getHeight() / 2;
		};

		addMenuObject(new Slider("GUI Scale", sliderPos, this, 0.1f, 3.0f, Application::getGUIScale(), 0.1f, Application::changeGUIScale));

		auto exitPos = [](float *x, float *y, float *width, float *height) {
			*width  = 200 * Application::getGUIScale();
			*height = 50 * Application::getGUIScale();
			*x      = Application::getWidth() / 2;
			*y      = Application::getHeight() / 2 - (*height) / 2 - 22.5f * Application::getGUIScale();
		};

		auto exitGameFunc = []() {
			Application::exitGame();
		};

		Text exitText("Main Menu");

		addMenuObject(new Button(exitText, exitPos, this, exitGameFunc));

		auto continuePos = [](float *x, float *y, float *width, float *height) {
			*width  = 200 * Application::getGUIScale();
			*height = 50 * Application::getGUIScale();
			*x      = Application::getWidth() / 2;
			*y      = Application::getHeight() / 2 + (*height) / 2 + 22.5f * Application::getGUIScale();
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
			*width  = 600 * Application::getGUIScale();
			*height = 200 * Application::getGUIScale();
			*x      = Application::getWidth() / 2;
			*y      = (*height) / 2 + 200.0f * Application::getGUIScale();
		};

		Text exitText("Main Menu", 0.0f, 0.0f, 150.0f, {1.0f, 1.0f, 1.0f, 1.0f});

		addMenuObject(new Button(exitText, exitPos, this, exitFunc));

		auto textPos = [](float *x, float *y) {
			*x = Application::getWidth() / 2;
			*y = Application::getHeight() / 2 + 150.0f * Application::getGUIScale();
		};

		Text text("YOU DIED...", 0.0f, 0.0f, 800.0f, {1.0f, 0.0f, 0.0f, 1.0f});

		addMenuObject(new TextMenuObject(text, textPos, this));

		break;
	}

	case GUILayer::Type::PlayerWin:
	{
		auto backgroundPosFunc = [](float *x, float *y, float *width, float *height) {
			*width  = Application::getWidth() * Application::getGUIScale();
			*height = Application::getHeight() * Application::getGUIScale();
			*x      = Application::getWidth() / 2;
			*y      = Application::getHeight() / 2;
		};

		auto exitFunc = []() {
			Application::exitGame();
		};

		addMenuObject(new MenuBackground(backgroundPosFunc, this, {0.0f, 0.0f, 0.0f, 1.0f}, exitFunc));

		auto exitPos = [](float *x, float *y, float *width, float *height) {
			*width  = 600 * Application::getGUIScale();
			*height = 200 * Application::getGUIScale();
			*x      = Application::getWidth() / 2;
			*y      = (*height) / 2 + 200.0f * Application::getGUIScale();
		};

		Text exitText("Main Menu", 0.0f, 0.0f, 150.0f, {1.0f, 1.0f, 1.0f, 1.0f});

		addMenuObject(new Button(exitText, exitPos, this, exitFunc));

		auto textPos = [](float *x, float *y) {
			*x = Application::getWidth() / 2;
			*y = Application::getHeight() / 2 + 150.0f * Application::getGUIScale();
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
	// Deletes all the objects
	for(MenuObject *obj : m_Objects)
		delete obj;
}

void GUILayer::render()
{
	// Renders all the objects
	for(MenuObject *obj : m_Objects)
		obj->render();
}

void GUILayer::update()
{
	// Updates the objects
	for(MenuObject *obj : m_Objects)
		obj->update();
}

bool GUILayer::eventCallback(const Event::Event &e)
{
	// Calls the event on all the objects
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
	// Sets the connected level
	m_ConnectedLevel = level;
}

void GUILayer::transferObject(TransferObject *transfer)
{
	// Goes through all the objects and checks if it is a MIHM and the mouse is over it and then sends the transfer object to it.
	for(MenuObject *obj : m_Objects)
	{
		// Checks if the mouse is outside of the menu (if it is it will move the item to the current room the player is in as a world item)
		MenuBackground *background = dynamic_cast<MenuBackground *>(obj);
		if(background && !background->isMouseOver())
		{
			Item *item = transfer->getItem();
			transfer->removeItem();

			if(m_ConnectedLevel)
			{
				Player *const player    = m_ConnectedLevel->getPlayer();
				WorldItem *   worldItem = new WorldItem(player->getX(), player->getY(), m_ConnectedLevel, item);
				m_ConnectedLevel->addEntityToRoom(worldItem);
			}
			else
				delete item;

			break;
		}
		MIHManager *manager = dynamic_cast<MIHManager *>(obj);
		if(manager)
		{
			int hoverBlock = manager->getIndexMouseAt();
			if(hoverBlock != -1)   // If the mouse is over a specific block of the MIHM it will tell it to transfer an item
			{
				manager->transferItem(transfer);
				break;
			}
		}
	}
}
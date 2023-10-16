#include "MenuItemHolderManager.h"

// #include <algorithm>

#include "Application.h"
#include "Log.h"
#include "event/Event.h"
#include "layer/GUILayer.h"
#include "rendering/Renderer.h"

#include "ItemContainer.h"
#include "WeaponContainer.h"

#include "layer/MessageManager.h"

#include "event/game/ChestOpened.h"
#include "event/input/Mouse.h"
#include "event/menu/Transfer.h"
#include "event/menu/WindowResize.h"

MIHManager::MIHManager(float x, float y, float width, float height, float blockSize, Layer *layer, std::function<void(int)> clickedFunc, IContainer *items, int *activeItem)
	: MenuObject(x, y, width, height, layer),
	  m_BlockSize(blockSize),
	  m_Items(items),
	  m_ActiveItem(activeItem),
	  m_ClickedFunc(clickedFunc),
	  m_BackgroundColour({0.6f, 0.6f, 0.6f, 1.0f}),
	  m_BorderColour({0.0f, 0.0f, 0.0f, 1.0f}),
	  m_HoverBorderColour({0.0f, 1.0f, 0.0f, 1.0f}),
	  m_ActiveBorderColour({1.0f, 0.0f, 0.0f, 1.0f}),
	  m_ListenType(IContainer::Type::None)
{
}

MIHManager::MIHManager(std::function<void(float *, float *, float *, float *)> posFunc, float blockSize, Layer *layer, std::function<void(int)> clickedFunc, IContainer *items, int *activeItem)
	: MenuObject(posFunc, layer),
	  m_BlockSize(blockSize),
	  m_Items(items),
	  m_ActiveItem(activeItem),
	  m_ClickedFunc(clickedFunc),
	  m_BackgroundColour({0.6f, 0.6f, 0.6f, 1.0f}),
	  m_BorderColour({0.0f, 0.0f, 0.0f, 1.0f}),
	  m_HoverBorderColour({0.0f, 1.0f, 0.0f, 1.0f}),
	  m_ActiveBorderColour({1.0f, 0.0f, 0.0f, 1.0f}),
	  m_ListenType(IContainer::Type::None)
{
}

MIHManager::MIHManager(std::function<void(float *, float *, float *, float *)> posFunc, float blockSize, Layer *layer, std::function<void(int)> clickedFunc, IContainer::Type type)
	: MenuObject(posFunc, layer),
	  m_BlockSize(blockSize),
	  m_Items(nullptr),
	  m_ActiveItem(nullptr),
	  m_ClickedFunc(clickedFunc),
	  m_BackgroundColour({0.6f, 0.6f, 0.6f, 1.0f}),
	  m_BorderColour({0.0f, 0.0f, 0.0f, 1.0f}),
	  m_HoverBorderColour({0.0f, 1.0f, 0.0f, 1.0f}),
	  m_ActiveBorderColour({1.0f, 0.0f, 0.0f, 1.0f}),
	  m_ListenType(type)
{
}

MIHManager::MIHManager(float x, float y, float width, float height, float blockSize, Layer *layer, glm::vec4 backgroundColour, glm::vec4 borderColour, glm::vec4 hoverColour, glm::vec4 activeColour, std::function<void(int)> clickedFunc, IContainer *items, int *activeItem)
	: MenuObject(x, y, width, height, layer),
	  m_BlockSize(blockSize),
	  m_Items(items),
	  m_ActiveItem(activeItem),
	  m_ClickedFunc(clickedFunc),
	  m_BackgroundColour(backgroundColour),
	  m_BorderColour(borderColour),
	  m_HoverBorderColour(hoverColour),
	  m_ActiveBorderColour(activeColour),
	  m_ListenType(IContainer::Type::None)
{
}

MIHManager::MIHManager(std::function<void(float *, float *, float *, float *)> posFunc, float blockSize, Layer *layer, glm::vec4 backgroundColour, glm::vec4 borderColour, glm::vec4 hoverColour, glm::vec4 activeColour, std::function<void(int)> clickedFunc, IContainer *items, int *activeItem)
	: MenuObject(posFunc, layer),
	  m_BlockSize(blockSize),
	  m_Items(items),
	  m_ActiveItem(activeItem),
	  m_ClickedFunc(clickedFunc),
	  m_BackgroundColour(backgroundColour),
	  m_BorderColour(borderColour),
	  m_HoverBorderColour(hoverColour),
	  m_ActiveBorderColour(activeColour),
	  m_ListenType(IContainer::Type::None)
{
}

MIHManager::~MIHManager()
{
	// NOTE: m_Items is a reference to something stored elsewhere, same with m_ActiveItem so they shouldn't be deleted here
}

void MIHManager::render()
{
	// If the container is not a nullptr it will render
	if(m_Items)
	{
		// Gets the width and height of the grid created
		int gridWidth  = (int) round(width / getScaledBlockSize());
		int gridHeight = (int) round(height / getScaledBlockSize());

		uint8_t layer = 7;

		// If the mouse is hovering over the MIHM then it will get the index of the block it is hovering over
		int mouseHoverBlock = -1;
		if(m_State == Button::State::Hover)
			mouseHoverBlock = getIndexMouseAt();

		// Goes through the grid width and height and renders each block
		for(int posY = 0; posY < gridHeight; posY++)
		{
			for(int posX = 0; posX < gridWidth; posX++)
			{
				int   i     = posX + posY * gridWidth;
				// Gets the postion of the block
				float nextX = getScaledBlockSize() / 2 + x + posX * getScaledBlockSize();
				float nextY = getScaledBlockSize() / 2 + y - posY * getScaledBlockSize();

				float borderWidth = 2.0f;
				// Checks what type of block this is
				if(i < m_Items->size() && i == mouseHoverBlock)
				{
					// If it is the block the mouse is hovering over and has an item in it it will render a square with the hover border colour
					borderWidth += 1.0f;
					Render::rectangle(nextX, nextY, getScaledBlockSize(), getScaledBlockSize(), m_BackgroundColour, borderWidth, m_HoverBorderColour, layer, true, true);

					// It will also render text over the block with the item name
					float scale    = 35.0f * Application::getGUIScale();
					Vec2f mousePos = Event::getMousePos();
					Render::hoverText(*m_Items->getItem(i)->getName(), mousePos.x, mousePos.y, scale, {1.0f, 1.0f, 1.0f, 1.0f}, {0.3f, 0.3f, 0.3f, 0.7f}, layer + 2, true);
				}
				else if(m_ActiveItem && i == *m_ActiveItem)
				{
					// If it is the active block then it will render a square with current active border colour
					borderWidth += 3.0f;
					Render::rectangle(nextX, nextY, getScaledBlockSize(), getScaledBlockSize(), m_BackgroundColour, borderWidth, m_ActiveBorderColour, layer, true, true);
				}
				else
					Render::rectangle(nextX, nextY, getScaledBlockSize(), getScaledBlockSize(), m_BackgroundColour, borderWidth, m_BorderColour, layer, true, true);

				// If there is an item stored in the block then it will render the item inside (NOTE: It will be rendered at a slightly smaller size)
				if(i < m_Items->size())
					m_Items->getItem(i)->render(nextX, nextY, 0.0f, getScaledBlockSize() - 10.0f * Application::getGUIScale(), layer + 1, true);
			}
		}
	}
}

void MIHManager::update()
{
	// If the active item is greater than the size of items then it will be updated
	if(m_ActiveItem && (*m_ActiveItem) >= m_Items->size())
		(*m_ActiveItem)--;

	// Updates the state
	Vec2f mousePos = Event::getMousePos();
	if(mousePos.x > x && mousePos.x < x + width && mousePos.y < y + getScaledBlockSize() && mousePos.y > y + getScaledBlockSize() - height)
		m_State = Button::State::Hover;
	else
		m_State = Button::State::None;
}

bool MIHManager::eventCallback(const Event::Event &e)
{
	switch(e.getType())
	{
	case Event::EventType::MouseClicked:
	{
		if(!m_Items)
			return false;

		// If there is a mouse clicked event it will find the block clicked
		const Event::MouseClickedEvent &ne = static_cast<const Event::MouseClickedEvent &>(e);

		Vec2f mousePos   = ne.pos;
		int   hoverBlock = getIndexMouseAt();
		if(hoverBlock != -1 && ne.action == Event::Action::Press)
		{
			switch(ne.button)
			{
			case Event::MouseButton::RightButton:
			{
				// If there was a right click it will call the clicked function
				m_State         = Button::State::Press;
				GUILayer *layer = dynamic_cast<GUILayer *>(m_Layer);
				if(layer && hoverBlock < m_Items->size())
					m_ClickedFunc(hoverBlock);

				return true;
			}

			case Event::MouseButton::LeftButton:
			{
				// If a left click has occurred it will call an item transfer event
				if(hoverBlock < m_Items->size())
				{
					Event::ItemTransferEvent e(hoverBlock, m_Items);
					Application::callEvent(e, Event::CallType::Overlay);
				}

				return true;
			}

			default:
				break;
			}
		}

		return false;
	}

	case Event::EventType::ChestOpened:
	{
		// If a chest has been opened it will check its listening type is equal to the container type
		const Event::ChestOpenedEvent &ne = static_cast<const Event::ChestOpenedEvent &>(e);

		if(m_ListenType == ne.container->getType())
		{
			// Updates the items and active item
			m_Items      = ne.container;
			m_ActiveItem = ne.activeItem;

			return true;
		}

		return false;
	}

	default:
		return MenuObject::eventCallback(e);
	}
}

int MIHManager::getIndexMouseAt()
{

	// Checks the mouse is hovering over the MIHM
	if(m_State == Button::State::Hover)
	{
		Vec2f mousePos = Event::getMousePos();

		// Gets the grid position the mouse is at
		int mouseGridX = (int) (mousePos.x - x) / getScaledBlockSize();
		int mouseGridY = (int) -(mousePos.y - (y + getScaledBlockSize())) / getScaledBlockSize();

		int gridWidth  = (int) round(width / getScaledBlockSize());
		int gridHeight = (int) round(height / getScaledBlockSize());

		// Checks the hover block is in the range
		int mouseHoverBlock = mouseGridX + mouseGridY * gridWidth;
		if(mouseHoverBlock < 0 || mouseHoverBlock >= gridWidth * gridHeight)
			return -1;

		return mouseHoverBlock;
	}

	return -1;
}

void MIHManager::transferItem(TransferObject *o)
{
	// Gets the block the mouse is hovering over
	int hoverBox = getIndexMouseAt();
	if(hoverBox != -1)
	{
		// Determines if it needs to swap an item
		bool swap = hoverBox < m_Items->size();

		IContainer *oContainer = o->getContainer();
		int         oIndex     = o->getIndex();

		// Determines whether it is possible to move the item (taking into account the types of the containers and items)
		bool cancel = false;

		{
			Weapon *oWeapon = dynamic_cast<Weapon *>(oContainer->getItem(oIndex));
			cancel          = m_Items->getType() == IContainer::Type::Weapon && !oWeapon;
		}

		// If it is wapping it checks the type of its container and item
		if(swap)
		{
			Weapon *mWeapon = dynamic_cast<Weapon *>(m_Items->getItem(hoverBox));
			cancel          = cancel || (oContainer->getType() == IContainer::Type::Weapon && !mWeapon);
		}

		// If an item cannot be stored it will send a message to the user saying it cannot be stored there
		if(cancel)
		{
			MessageManager::sendMessage("Item cannot be stored there!", MessageManager::Priority::High);
			return;
		}

		// If the containers are different or it is swapping it will transfer
		if(oContainer != m_Items || swap)
		{
			// Function for inserting an item to the correct position correctly
			auto insertItem = [swap](IContainer *container, Item *item, int index) {
				switch(container->getType())
				{
				case IContainer::Type::Item:
				{
					ItemContainer *itemContainer = static_cast<ItemContainer *>(container);

					if(item == nullptr)
					{
						// If the item is a nullptr then it will just erase the item at that index
						itemContainer->erase(itemContainer->begin() + index);
						break;
					}

					if(swap)
					{
						// Will erase the item in the index
						itemContainer->erase(itemContainer->begin() + index);
						// Will erase insert the item at that index
						itemContainer->insert(itemContainer->begin() + index, item);
					}
					else
						itemContainer->push_back(item);
					break;
				}
				case IContainer::Type::Weapon:
				{
					// Same as above just with a weapon container instead
					WeaponContainer *weaponContainer = static_cast<WeaponContainer *>(container);

					if(item == nullptr)
					{
						weaponContainer->erase(weaponContainer->begin() + index);
						break;
					}

					Weapon *weapon = static_cast<Weapon *>(item);
					if(swap)
					{
						weaponContainer->erase(weaponContainer->begin() + index);
						weaponContainer->insert(weaponContainer->begin() + index, weapon);
					}
					else
						weaponContainer->push_back(weapon);
					break;
				}
				default:
					Log::warning("Unknown container type!");
					break;
				}
			};

			o->hasTransferred();   // Tells the transfer object that it has transfer so it can render the items again

			// Gets the other item
			Item *oItem = oContainer->getItem(oIndex);
			Item *mItem = nullptr;   // Will be nullptr unless it is swapping

			if(swap)
				mItem = m_Items->getItem(hoverBox);

			// Inserts both the items at the given position
			insertItem(m_Items, oItem, hoverBox);
			insertItem(oContainer, mItem, oIndex);

			// Will update the active item if it is incorrect
			if(m_ActiveItem && (*m_ActiveItem) == -1 && m_Items->size() > 0)
				(*m_ActiveItem) = 0;
		}
	}
}

float const MIHManager::getScaledBlockSize() const
{
	// Returns the scaled version of the block size
	return m_BlockSize * Application::getGUIScale();
}

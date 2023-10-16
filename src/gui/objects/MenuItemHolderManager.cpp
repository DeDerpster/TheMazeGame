#include "MenuItemHolderManager.h"

#include "Application.h"
#include "Event.h"
#include "GUILayer.h"
#include "Log.h"
#include "Renderer.h"

#include "ItemContainer.h"
#include "WeaponContainer.h"

#include "MessageManager.h"

MIHManager::MIHManager(float x, float y, float width, float height, float blockSize, Layer *layer, IContainer *items, std::function<void(int, Level *)> clickedFunc, int *activeItem, GUIInventoryIDCode listenID)
	: MenuObject(x, y, width, height, layer),
	  m_BlockSize(blockSize),
	  m_Items(items),
	  m_ActiveItem(activeItem),
	  m_ClickedFunc(clickedFunc),
	  m_BackgroundColour({0.6f, 0.6f, 0.6f, 1.0f}),
	  m_BorderColour({0.0f, 0.0f, 0.0f, 1.0f}),
	  m_HoverBorderColour({0.0f, 1.0f, 0.0f, 1.0f}),
	  m_ActiveBorderColour({1.0f, 0.0f, 0.0f, 1.0f}),
	  m_ListenID(listenID)
{
}
MIHManager::MIHManager(std::function<void(float *, float *, float *, float *)> posFunc, float blockSize, Layer *layer, IContainer *items, std::function<void(int, Level *)> clickedFunc, int *activeItem, GUIInventoryIDCode listenID)
	: MenuObject(posFunc, layer),
	  m_BlockSize(blockSize),
	  m_Items(items),
	  m_ActiveItem(activeItem),
	  m_ClickedFunc(clickedFunc),
	  m_BackgroundColour({0.6f, 0.6f, 0.6f, 1.0f}),
	  m_BorderColour({0.0f, 0.0f, 0.0f, 1.0f}),
	  m_HoverBorderColour({0.0f, 1.0f, 0.0f, 1.0f}),
	  m_ActiveBorderColour({1.0f, 0.0f, 0.0f, 1.0f}),
	  m_ListenID(listenID)

{

}
// TODO: Clean up the parameter order
MIHManager::MIHManager(float x, float y, float width, float height, float blockSize, Layer *layer, IContainer *items, glm::vec4 backgroundColour, glm::vec4 borderColour, glm::vec4 hoverColour, glm::vec4 activeColour, std::function<void(int, Level *)> clickedFunc, int *activeItem, GUIInventoryIDCode listenID)
	: MenuObject(x, y, width, height, layer),
	  m_BlockSize(blockSize),
	  m_Items(items),
	  m_ActiveItem(activeItem),
	  m_ClickedFunc(clickedFunc),
	  m_BackgroundColour(backgroundColour),
	  m_BorderColour(borderColour),
	  m_HoverBorderColour(hoverColour),
	  m_ActiveBorderColour(activeColour),
	  m_ListenID(listenID)

{
}
MIHManager::MIHManager(std::function<void(float *, float *, float *, float *)> posFunc, float blockSize, Layer *layer, IContainer *items, glm::vec4 backgroundColour, glm::vec4 borderColour, glm::vec4 hoverColour, glm::vec4 activeColour, std::function<void(int, Level *)> clickedFunc, int *activeItem, GUIInventoryIDCode listenID)
	: MenuObject(posFunc, layer),
	  m_BlockSize(blockSize),
	  m_Items(items),
	  m_ActiveItem(activeItem),
	  m_ClickedFunc(clickedFunc),
	  m_BackgroundColour(backgroundColour),
	  m_BorderColour(borderColour),
	  m_HoverBorderColour(hoverColour),
	  m_ActiveBorderColour(activeColour),
	  m_ListenID(listenID)
{
}


MIHManager::~MIHManager()
{
	// NOTE: m_Items is a reference to something stored elsewhere, same with m_ActiveItem so they shouldn't be deleted here
}

void MIHManager::render()
{
	if(m_Items)
	{
		int xOffset    = 0;
		int yOffset    = 0;
		int gridWidth  = (int) width / m_BlockSize;
		int gridHeight = (int) height / m_BlockSize;

		uint8_t layer = 7;

		int mouseHoverBlock = -1;
		if(m_State == Button::State::Hover)
		{
			mouseHoverBlock = getIndexMouseAt();
		}
		for(int posY = 0; posY < gridHeight; posY++)
		{
			for(int posX = 0; posX < gridWidth; posX++)
			{
				int   i     = posX + posY * gridWidth;
				float nextX = m_BlockSize / 2 + x + xOffset * m_BlockSize;
				float nextY = m_BlockSize / 2 + y - yOffset * m_BlockSize;

				float borderWidth = 2.0f;
				if(i < m_Items->size() && i == mouseHoverBlock)
				{
					borderWidth += 1.0f;
					Render::rectangle(nextX, nextY, m_BlockSize, m_BlockSize, m_BackgroundColour, borderWidth, m_HoverBorderColour, layer, true, true);

					float        scale    = 35.0f;   // TODO: Increase this probably - with scale
					Vec2f        mousePos = Event::getMousePos();
					Render::hoverText(*m_Items->getItem(i)->getName(), mousePos.x, mousePos.y, scale, {1.0f, 1.0f, 1.0f, 1.0f}, {0.3f, 0.3f, 0.3f, 0.7f}, layer + 2, true);
				}
				else if(m_ActiveItem && i == *m_ActiveItem)
				{
					borderWidth += 3.0f;
					Render::rectangle(nextX, nextY, m_BlockSize, m_BlockSize, m_BackgroundColour, borderWidth, m_ActiveBorderColour, layer, true, true);
				}
				else
					Render::rectangle(nextX, nextY, m_BlockSize, m_BlockSize, m_BackgroundColour, borderWidth, m_BorderColour, layer, true, true);

				if(i < m_Items->size())
					m_Items->getItem(i)->render(nextX, nextY, 0.0f, m_BlockSize - 10.0f, layer + 1, true);

				xOffset++;
				if(xOffset == gridWidth)
				{
					xOffset = 0;
					yOffset++;
				}
				if(yOffset == height)
					break;
			}
		}
	}
}

void MIHManager::update()
{
	if(m_ActiveItem && (*m_ActiveItem) >= m_Items->size())
		(*m_ActiveItem)--;

	Vec2f mousePos = Event::getMousePos();

	if(mousePos.x > x && mousePos.x < x + width && mousePos.y < y + m_BlockSize && mousePos.y > y + m_BlockSize - height)
		m_State = Button::State::Hover;
	else
		m_State = Button::State::None;
}

bool MIHManager::eventCallback(const Event::Event &e)
{
	if(m_Items)
	{
		if(e.getType() == Event::EventType::mouseClicked)
		{
			const Event::MouseClickedEvent &ne = static_cast<const Event::MouseClickedEvent &>(e);

			Vec2f mousePos = ne.pos;
			int   hoverBlock = getIndexMouseAt();
			if(ne.button == Event::leftButton && hoverBlock != -1)
			{
				m_State         = Button::State::Press;
				GUILayer *layer = dynamic_cast<GUILayer *>(m_Layer);
				if(layer && hoverBlock < m_Items->size())
					m_ClickedFunc(hoverBlock, layer->getConnectedLevel());
				return true;
			}
			else if(ne.button == Event::rightButton && hoverBlock != -1)
			{
				Event::ItemTransfer e(hoverBlock, m_Items);
				Application::callEvent(e, true);

				return true;
			}
		}
	}

	if(e.getType() == Event::EventType::chestOpened)
	{
		const Event::ChestOpenedEvent &ne = static_cast<const Event::ChestOpenedEvent &>(e);

		if(m_ListenID == ne.id)
		{
			m_Items = ne.container;
			m_ActiveItem = ne.activeItem;
			return true;
		}

		return false;
	}

	return MenuObject::eventCallback(e);
}

int MIHManager::getIndexMouseAt()
{
	Vec2f mousePos = Event::getMousePos();
	if(mousePos.x > x && mousePos.x < x + width && mousePos.y < y + m_BlockSize && mousePos.y > y + m_BlockSize - height)
	{
		int   mouseGridX    = -1;
		int   mouseGridY    = -1;
		Vec2f mousePos      = Event::getMousePos();
		mouseGridX          = (mousePos.x - x) / m_BlockSize;
		mouseGridY          = -(mousePos.y - (y + m_BlockSize)) / m_BlockSize;
		int mouseHoverBlock = mouseGridX + mouseGridY * ((int) width / m_BlockSize);
		if(mouseHoverBlock < 0 || mouseHoverBlock >= ((int) width / m_BlockSize) * ((int) height / m_BlockSize))
			return -1;
		return mouseHoverBlock;
	}
	return -1;
}

void MIHManager::transferItem(TransferObject *o)
{
	int hoverBox = getIndexMouseAt();
	if(hoverBox != -1)
	{
		bool swap = hoverBox < m_Items->size();

		IContainer *oContainer = o->getContainer();
		int         oIndex     = o->getIndex();

		bool cancel = false;

		{
			Weapon *oWeapon = dynamic_cast<Weapon *>(oContainer->getItem(oIndex));
			cancel          = m_Items->getType() == IContainer::Type::weapon && !oWeapon;
		}

		if(swap)
		{
			Weapon *mWeapon = dynamic_cast<Weapon *>(m_Items->getItem(hoverBox));
			cancel          = oContainer->getType() == IContainer::Type::weapon && !mWeapon;
		}

		if(cancel)
		{
			MessageManager::sendMessage("Item cannot be stored there!", MessageManager::Priority::High);
			return;
		}

		if(!(oContainer == m_Items && !swap))
		{
			auto insertItem = [swap](IContainer *container, Item *item, int index) {
				switch(container->getType())
				{
				case IContainer::Type::item:
				{
					ItemContainer *itemContainer = static_cast<ItemContainer *>(container);

					if(item == nullptr)
					{
						itemContainer->erase(itemContainer->begin() + index);
						break;
					}

					if(swap)
					{
						itemContainer->erase(itemContainer->begin() + index);
						itemContainer->insert(itemContainer->begin() + index, item);
					}
					else
						itemContainer->push_back(item);
					break;
				}
				case IContainer::Type::weapon:
				{
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

			o->hasTransferred();
			Item *oItem = oContainer->getItem(oIndex);
			Item *mItem = nullptr;
			if(swap)
				mItem = m_Items->getItem(hoverBox);

			insertItem(m_Items, oItem, hoverBox);
			insertItem(oContainer, mItem, oIndex);

			if(m_ActiveItem && (*m_ActiveItem) == -1 && m_Items->size() > 0)
				(*m_ActiveItem) = 0;
		}
	}
}
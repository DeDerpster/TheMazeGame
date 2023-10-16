#include "MenuItemHolderManager.h"

#include "Event.h"
#include "Log.h"
#include "Renderer.h"

MIHManager::MIHManager(float x, float y, uint32_t width, uint32_t height, uint32_t blockSize, const std::vector<Item *> &items, int *activeItem)
	: MenuObject(x, y, (float) width, (float) height),
	  m_Items(items),
	  m_ActiveItem(activeItem),
	  m_BlockSize(blockSize),
	  m_BackgroundColour({0.6f, 0.6f, 0.6f, 1.0f}),
	  m_BorderColour({0.0f, 0.0f, 0.0f, 1.0f}),
	  m_HoverBorderColour({0.0f, 1.0f, 0.0f, 1.0f}),
	  m_ActiveBorderColour({1.0f, 0.0f, 0.0f, 1.0f})
{
}
MIHManager::MIHManager(float x, float y, uint32_t width, uint32_t height, uint32_t blockSize, const std::vector<Item *> &items, int *activeItem, glm::vec4 backgroundColour, glm::vec4 borderColour, glm::vec4 hoverColour, glm::vec4 activeColour)
	: MenuObject(x, y, (float) width, (float) height),
	  m_Items(items),
	  m_ActiveItem(activeItem),
	  m_BlockSize(blockSize),
	  m_BackgroundColour(backgroundColour),
	  m_BorderColour(borderColour),
	  m_HoverBorderColour(hoverColour),
	  m_ActiveBorderColour(activeColour)
{
}
MIHManager::~MIHManager()
{
	// NOTE: m_Items is a reference to something stored elsewhere, same with m_ActiveItem so they shouldn't be deleted here
}

void MIHManager::render()
{
	int xOffset = 0;
	int yOffset = 0;

	int mouseHoverBlock = -1;
	if(m_State == Button::State::Hover)
	{
		int   mouseGridX = -1;
		int   mouseGridY = -1;
		Vec2f mousePos   = Event::getMousePos();
		mouseGridX       = (mousePos.x - x) / m_BlockSize;
		mouseGridY       = (mousePos.y - y) / m_BlockSize;
		mouseHoverBlock  = mouseGridX + mouseGridY * width;
	}
	for(int i = 0; i < m_Items.size(); i++)
	{
		float nextX = m_BlockSize / 2 + x + xOffset * m_BlockSize;
		float nextY = m_BlockSize / 2 + y + yOffset * m_BlockSize;

		float borderWidth = 2.0f;
		if(i == *m_ActiveItem)
		{
			borderWidth += 3.0f;
			Render::rectangle(nextX, nextY, m_BlockSize, m_BlockSize, m_BackgroundColour, borderWidth, m_ActiveBorderColour, true, true, true);
		}
		else if(i == mouseHoverBlock)
		{
			borderWidth += 1.0f;
			Render::rectangle(nextX, nextY, m_BlockSize, m_BlockSize, m_BackgroundColour, borderWidth, m_HoverBorderColour, true, true, true);
			// TODO: Make a function in the renderer for this
			float        scale    = 35.0f;
			Vec2f        mousePos = Event::getMousePos();
			CollisionBox box      = Render::getTextCollisionBox(*m_Items[i]->getName(), scale);
			Render::rectangle(mousePos.x, mousePos.y + 4.0f + box.upperBound.y / 2, 0.0f, box.upperBound.x + 2.0f, box.upperBound.y + 4.0f, {0.3f, 0.3f, 0.3f, 0.7f}, true, true);
			Render::text(*m_Items[i]->getName(), mousePos.x, mousePos.y + 5.0f + box.upperBound.y / 2, scale, {1.0f, 1.0f, 1.0f, 1.0f}, true, true);
		}
		else
			Render::rectangle(nextX, nextY, m_BlockSize, m_BlockSize, m_BackgroundColour, borderWidth, m_BorderColour, true, true, true);

		m_Items[i]->render(nextX, nextY, 0.0f, m_BlockSize - 10.0f, true);

		xOffset++;
		if(xOffset == width)
		{
			xOffset = 0;
			yOffset++;
		}
		if(yOffset == height)
			break;
	}   // TODO:: make it show empty blocks
}

void MIHManager::update()
{
	Vec2f mousePos = Event::getMousePos();

	if(mousePos.x > x && mousePos.x < x + width * m_BlockSize && mousePos.y > y && mousePos.y < y + height * m_BlockSize)
		m_State = Button::State::Hover;
	else
		m_State = Button::State::None;
}

bool MIHManager::eventCallback(const Event::Event &e)
{
	if(e.getType() == Event::EventType::mouseClicked)
	{
		const Event::MouseClickedEvent &ne = static_cast<const Event::MouseClickedEvent &>(e);

		Vec2f mousePos = ne.pos;
		if(ne.button == Event::leftButton && mousePos.x > x && mousePos.x < x + width * m_BlockSize && mousePos.y > y && mousePos.y < y + height * m_BlockSize)
		{
			m_State             = Button::State::Press;
			int   mouseGridX    = -1;
			int   mouseGridY    = -1;
			Vec2f mousePos      = Event::getMousePos();
			mouseGridX          = (mousePos.x - x) / m_BlockSize;
			mouseGridY          = (mousePos.y - y) / m_BlockSize;
			int mouseHoverBlock = mouseGridX + mouseGridY * width;
			if(mouseHoverBlock > -1 && mouseHoverBlock < m_Items.size())
				(*m_ActiveItem) = mouseHoverBlock;
		}
		return true;
	}
	return MenuObject::eventCallback(e);
}

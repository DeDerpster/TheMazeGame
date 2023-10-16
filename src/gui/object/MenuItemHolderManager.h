#pragma once

#include "MenuObject.h"

#include <GLM.h>
#include <functional>

#include "Button.h"
#include "Container.h"
#include "TransferObject.h"
#include "Utils.h"
#include "item/Item.h"
#include "level/Level.h"

class MIHManager : public MenuObject
{
  private:
	uint32_t                          m_BlockSize;
	IContainer *                      m_Items;
	int *                             m_ActiveItem;
	std::function<void(int, Level *)> m_ClickedFunc;
	glm::vec4                         m_BackgroundColour, m_BorderColour, m_HoverBorderColour, m_ActiveBorderColour;
	Button::State                     m_State;

	GUIInventoryIDCode m_ListenID;

  public:   // TODO: Make the function be able to handle it without the level input (cos you can just give it a reference straight to the player)
	MIHManager(float x, float y, float width, float height, float blockSize, Layer *layer, IContainer *items, std::function<void(int, Level *)> clickedFunc, int *activeItem = nullptr, GUIInventoryIDCode listenID = GUIInventoryIDCode::none);
	MIHManager(std::function<void(float *, float *, float *, float *)> posFunc, float blockSize, Layer *layer, IContainer *items, std::function<void(int, Level *)> clickedFunc, int *activeItem = nullptr, GUIInventoryIDCode listenID = GUIInventoryIDCode::none);
	MIHManager(float x, float y, float width, float height, float blockSize, Layer *layer, IContainer *items, glm::vec4 backgroundColour, glm::vec4 borderColour, glm::vec4 hoverColour, glm::vec4 activeColour, std::function<void(int, Level *)> clickedFunc, int *activeItem = nullptr, GUIInventoryIDCode listenID = GUIInventoryIDCode::none);
	MIHManager(std::function<void(float *, float *, float *, float *)> posFunc, float blockSize, Layer *layer, IContainer *items, glm::vec4 backgroundColour, glm::vec4 borderColour, glm::vec4 hoverColour, glm::vec4 activeColour, std::function<void(int, Level *)> clickedFunc, int *activeItem = nullptr, GUIInventoryIDCode listenID = GUIInventoryIDCode::none);
	virtual ~MIHManager() override;

	virtual void render() override;
	virtual void update() override;
	virtual bool eventCallback(const Event::Event &e) override;

	int getIndexMouseAt();

	void transferItem(TransferObject *o);

	void setInventory(IContainer *inventory) { m_Items = inventory; }
};
#pragma once

#include "MenuObject.h"

#include <GLM.h>
#include <functional>

#include "Button.h"
#include "Container.h"
#include "TransferObject.h"
#include "layer/level/Level.h"

class MIHManager : public MenuObject
{
  private:
	float                    m_BlockSize;     // Stores the size of each item block
	IContainer *             m_Items;         // Stores a pointer to a container of items
	int *                    m_ActiveItem;    // Stores a pointer to the integer that determines what is the active item (can be nullptr)
	std::function<void(int)> m_ClickedFunc;   // Stores function that is called when a block is right clicked

	// Stores all the colour information of the MIHM
	glm::vec4 m_BackgroundColour, m_BorderColour, m_HoverBorderColour, m_ActiveBorderColour;

	// Stores the state of the MIHM
	Button::State m_State;

	// Stores whether it is listening for events to change its container if so what.
	IContainer::Type m_ListenType;

	float const getScaledBlockSize() const;

  public:
	MIHManager(float x, float y, float width, float height, float blockSize, Layer *layer, std::function<void(int)> clickedFunc, IContainer *items, int *activeItem = nullptr);
	MIHManager(std::function<void(float *, float *, float *, float *)> posFunc, float blockSize, Layer *layer, std::function<void(int)> clickedFunc, IContainer *items, int *activeItem = nullptr);
	MIHManager(std::function<void(float *, float *, float *, float *)> posFunc, float blockSize, Layer *layer, std::function<void(int)> clickedFunc, IContainer::Type type = IContainer::Type::Item);
	MIHManager(float x, float y, float width, float height, float blockSize, Layer *layer, glm::vec4 backgroundColour, glm::vec4 borderColour, glm::vec4 hoverColour, glm::vec4 activeColour, std::function<void(int)> clickedFunc, IContainer *items, int *activeItem = nullptr);
	MIHManager(std::function<void(float *, float *, float *, float *)> posFunc, float blockSize, Layer *layer, glm::vec4 backgroundColour, glm::vec4 borderColour, glm::vec4 hoverColour, glm::vec4 activeColour, std::function<void(int)> clickedFunc, IContainer *items, int *activeItem = nullptr);
	virtual ~MIHManager() override;

	virtual void render() override;
	virtual void update() override;
	virtual bool eventCallback(const Event::Event &e) override;

	// Gets the index which the mouse is currently at
	int getIndexMouseAt();

	// Transfers an item between two different MIHMs
	void transferItem(TransferObject *o);

	// Changes the inventory of the MIHM
	void setInventory(IContainer *inventory) { m_Items = inventory; }
};
#pragma once

#include "MenuObject.h"

#include <GLM.h>

#include "Button.h"
#include "Item.h"

class MIHManager : public MenuObject
{
  private:
	const std::vector<Item *> &m_Items;
	int *                      m_ActiveItem;
	uint32_t                   m_BlockSize;
	glm::vec4                  m_BackgroundColour, m_BorderColour, m_HoverBorderColour, m_ActiveBorderColour;
	Button::State              m_State;

  public:
	MIHManager(float x, float y, uint32_t width, uint32_t height, uint32_t blockSize, const std::vector<Item *> &items, int *activeItem = nullptr);
	MIHManager(float x, float y, uint32_t width, uint32_t height, uint32_t blockSize, const std::vector<Item *> &items, int *activeItem, glm::vec4 backgroundColour, glm::vec4 borderColour, glm::vec4 hoverColour, glm::vec4 activeColour);
	virtual ~MIHManager();

	virtual void render() override;
	virtual void update() override;
	virtual bool eventCallback(const Event::Event &e) override;
};
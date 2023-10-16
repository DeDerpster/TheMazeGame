#pragma once

#include "MenuObject.h"
#include "Text.h"

class TextMenuObject : public MenuObject
{
  private:
	Text m_Text;

  public:
	TextMenuObject(Text text, float x, float y, Layer *layer);
	TextMenuObject(Text text, std::function<void(float *, float *)> posFunc, Layer *layer);

	virtual void render() override;
	virtual void update() override;
};
#pragma once

#include "MenuObject.h"
#include "Text.h"

class Title : public MenuObject
{
  private:
	Text m_Text;

  public:
	Title(Text text, float x, float y, std::function<void(float *, float *)> posFunc);

	~Title();

	virtual void update() override;
	virtual void render() override;
	virtual bool eventCallback(const Event::Event &e) override;
};
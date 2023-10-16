#include "Title.h"

Title::Title(Text text, float x, float y, std::function<void(float *, float *)> posFunc)
	: MenuObject(x, y, [posFunc](float *x, float *y, float *width, float *height) { posFunc(x, y); }),
	  m_Text(text)
{
}

Title::~Title()
{
}

void Title::update()
{
}

void Title::render()
{
	m_Text.render(x, y);
}

bool Title::eventCallback(const Event::Event &e)
{
	return MenuObject::eventCallback(e);
}

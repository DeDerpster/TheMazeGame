#include "TextMenuObject.h"

TextMenuObject::TextMenuObject(Text text, float x, float y, Layer *layer)
	: MenuObject(x, y, 0.0f, 0.0f, layer), m_Text(text)
{
}

TextMenuObject::TextMenuObject(Text text, std::function<void(float *, float *)> posFunc, Layer *layer)
	: MenuObject([posFunc](float *x, float *y, float *width, float *height) { posFunc(x, y); (*width) = 0.0f; (*height) = 0.0f; }, layer), m_Text(text)
{
}

void TextMenuObject::render()
{
	m_Text.render(x, y, 8);
}

void TextMenuObject::update()
{
}

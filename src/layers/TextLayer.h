#include "Layer.h"

class TextLayer : public Layer
{
  public:
	TextLayer();

	~TextLayer() {}

	virtual void render() override;

	virtual void update() override {}

	virtual bool eventCallback(const Application::Event &e) override { return false; }

#ifdef DEBUG
	virtual void imGuiRender() override
	{
	}
#endif
};
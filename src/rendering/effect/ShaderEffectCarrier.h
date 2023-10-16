#include "Effect.h"

namespace Effect
{
	// These are for all the different shader effects that could be applied
	class ShaderEffectCarrier : public Effect
	{
	  protected:
		uint16_t m_ID;

	  public:
		ShaderEffectCarrier(uint16_t id)
			: m_ID(id)
		{
		}
		virtual ~ShaderEffectCarrier() override {}

		uint16_t             getID() { return m_ID; }
		virtual Effect::Type getType() const override { return Effect::Type::shaderEffect; }
	};

	// This is for removing an effect from a layer
	class RemoveShaderEffect : public ShaderEffectCarrier
	{
	  public:
		RemoveShaderEffect(uint16_t id)
			: ShaderEffectCarrier(id)
		{
		}
		virtual ~RemoveShaderEffect() override {}

		virtual Effect::Type getType() const override { return Effect::Type::removeShaderEffect; }
	};
}   // namespace Effect

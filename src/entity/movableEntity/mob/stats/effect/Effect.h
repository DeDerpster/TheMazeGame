#pragma once

#include <functional>

class StatsEffect
{
  private:
	std::function<void()> m_Function;
	uint16_t              m_Lifetime;

  public:
	StatsEffect(std::function<void()> effect, uint16_t lifetime)
		: m_Function(effect), m_Lifetime(lifetime) {}
	~StatsEffect() {}

	void update()
	{
		if(m_Lifetime != 0)
		{
			m_Function();
			m_Lifetime--;
		}
	}

	bool deleteMe() { return m_Lifetime == 0; }
};
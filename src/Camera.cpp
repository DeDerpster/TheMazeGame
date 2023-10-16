#include "Camera.h"

#include <GLFW/glfw3.h>
#include "ImGui.h"

#include "Application.h"
#include "KeyDefinitions.h"
#include "Log.h"
#include "ShaderEffect.h"

Camera::Camera()
	: x(0.0f), y(0.0f), zoomPercentage(1.0f), moveSpeed(10.0f), moveLock(false), updateView(true), lockOnAnchor(false), m_Anchor(nullptr), m_ZoomEffectID(0), m_PositionEffectID(0)
{
	Log::info("Initialised Camera");
}

Camera::Camera(float x, float y)
	: x(x), y(y), zoomPercentage(1.0f), moveSpeed(10.0f), moveLock(false), updateView(true), lockOnAnchor(false), m_Anchor(nullptr), m_ZoomEffectID(0), m_PositionEffectID(0)
{
}

void Camera::update()
{
	if(m_Anchor && lockOnAnchor)
	{
		if(m_Anchor->getX() != x || m_Anchor->getY() != y || updateView)
		{
			updateView = true;
			x          = m_Anchor->getX();
			y          = m_Anchor->getY();
		}
	}
	else
	{
		if(Event::isKeyPressed(GLFW_KEY_W) || Event::isKeyPressed(GLFW_KEY_UP))
		{
			updateView = true;
			y += moveSpeed;
		}
		if(Event::isKeyPressed(GLFW_KEY_S) || Event::isKeyPressed(GLFW_KEY_DOWN))
		{
			updateView = true;
			y -= moveSpeed;
		}
		if(Event::isKeyPressed(GLFW_KEY_A) || Event::isKeyPressed(GLFW_KEY_LEFT))
		{
			updateView = true;
			x -= moveSpeed;
		}
		if(Event::isKeyPressed(GLFW_KEY_D) || Event::isKeyPressed(GLFW_KEY_RIGHT))
		{
			updateView = true;
			x += moveSpeed;
		}
	}
}

void Camera::render()
{
	if(updateView)
	{
		updatePositionEffect();
		updateView = false;
	}
}

#ifdef DEBUG
void Camera::imGuiRender()
{
	ImGui::SliderFloat("Speed", &moveSpeed, 0.0f, 20.0f);
	ImGui::SliderFloat("X", &x, -300.0f, 7700.0f);
	ImGui::SliderFloat("Y", &y, -300.0f, 7700.0f);
	int before = zoomPercentage;
	ImGui::SliderFloat("Zoom", &zoomPercentage, 0.05f, 2.0f);
	if(before != zoomPercentage)
	{
		updateZoomEffect();
	}
	if(ImGui::Checkbox("Camera Lock", &lockOnAnchor))
	{
		m_Anchor->setIsInControl(lockOnAnchor);
		updateView = true;
	}
}
#endif

bool Camera::eventCallback(const Event::Event &e)
{
	if(e.getType() == Event::EventType::scroll)
	{
		const Event::ScrollEvent &ne = static_cast<const Event::ScrollEvent &>(e);

		if(zoomPercentage == 0.25f && ne.yoffset < 0.0f)
			return false;

		float oldZoom = zoomPercentage;
		zoomPercentage += (ne.yoffset * 0.02f) * (zoomPercentage / 0.20f);

		if(zoomPercentage < 0.25f)
			zoomPercentage = 0.25f;

		updateZoomEffect();

		updateView = true;

		return false;
	}
	else if(e.getType() == Event::EventType::windowResize)
	{
		const Event::WindowResizeEvent &ne = static_cast<const Event::WindowResizeEvent &>(e);

		updateView = true;
		return false;
	}
	else if(e.getType() == Event::EventType::mazeMovedEvent)
	{
		changeUpdateView();
	}
	return false;
}

bool Camera::setEffect(const Effect::RenderEffect &e)
{
	return false;
}

uint16_t Camera::getPositionEffectID()
{
	if(m_PositionEffectID == 0)
		setShaderEffects();
	return m_PositionEffectID;
}
uint16_t Camera::getZoomEffectID()
{
	if(m_ZoomEffectID == 0)
		setShaderEffects();
	return m_ZoomEffectID;
}
void Camera::setShaderEffects()
{
	{
		std::string name   = "u_MVP";
		m_PositionEffectID = Effect::ShaderEffects::sendShaderEffect(name, Application::getProj() * glm::translate(glm::mat4(1.0f), glm::vec3(Application::getWidth() / 2 - x * zoomPercentage, Application::getHeight() / 2 - y * zoomPercentage, 0.0f)));
	}

	{
		std::string name = "u_Zoom";
		m_ZoomEffectID   = Effect::ShaderEffects::sendShaderEffect(name, glm::vec4(zoomPercentage, zoomPercentage, 1.0f, 1.0f));
	}
}

void Camera::updatePositionEffect()
{
	if(m_PositionEffectID == 0)
		setShaderEffects();

	Effect::UniformMat4 *effect = static_cast<Effect::UniformMat4 *>(Effect::ShaderEffects::getShaderEffect(m_PositionEffectID));
	effect->setMat(Application::getProj() * glm::translate(glm::mat4(1.0f), glm::vec3(Application::getWidth() / 2 - x * zoomPercentage, Application::getHeight() / 2 - y * zoomPercentage, 0.0f)));
}
void Camera::updateZoomEffect()
{
	if(m_ZoomEffectID == 0)
		setShaderEffects();

	Effect::UniformVec4 *effect = static_cast<Effect::UniformVec4 *>(Effect::ShaderEffects::getShaderEffect(m_ZoomEffectID));   // TODO: Make this a dynamic cast
	effect->setVec(glm::vec4(zoomPercentage, zoomPercentage, 1.0f, 1.0f));
}

bool Camera::isInFrame(float objX, float objY, CollisionBox &box)
{   // TODO: Try and make the buffer of 1 tiles slightly smaller :D
	return objX + box.upperBound.x + TILE_SIZE > x - Application::getWidth() / (zoomPercentage * 2) && objX + box.lowerBound.x - TILE_SIZE <= x + Application::getWidth() / (zoomPercentage * 2) && objY + box.upperBound.y + TILE_SIZE > y - Application::getHeight() / (zoomPercentage * 2) && objY + box.lowerBound.y - TILE_SIZE <= y + Application::getHeight() / (zoomPercentage * 2);
}

void Camera::setLock(bool locked)
{
	moveLock = locked;
}

void Camera::setX(float newX)
{
	x = newX;
}

void Camera::setY(float newY)
{
	y = newY;
}

void Camera::setAnchor(Mob *e)
{
	lockOnAnchor = true;
	m_Anchor     = e;
	m_Anchor->setIsInControl(true);
}

float Camera::getZoom()
{
	return zoomPercentage;
}

void Camera::changeUpdateView()
{
	updateView = true;
}

Vec2f Camera::convertWindowToLevel(Vec2f inp)
{
	float nx = x + (2 * inp.x - Application::getWidth()) / (2 * zoomPercentage);
	float ny = y + (2 * inp.y - Application::getHeight()) / (2 * zoomPercentage);
	return {nx, ny};
}
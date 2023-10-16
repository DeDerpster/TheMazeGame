#include "Camera.h"

#include "ImGui.h"

#include "Application.h"
#include "KeyDefinitions.h"
#include "ShaderEffectsManager.h"
#include "utils/Log.h"

#include "event/game/MazeMoved.h"
#include "event/input/Keyboard.h"
#include "event/input/Mouse.h"
#include "event/menu/WindowResize.h"

Camera::Camera()
	: x(0.0f), y(0.0f), zoomPercentage(1.0f), moveSpeed(10.0f), moveLock(false), updateView(true), lockOnAnchor(false), m_Anchor(nullptr), m_ZoomEffectID(0), m_PositionEffectID(0)
{
	Log::info("Initialised Camera");
}

Camera::Camera(float x, float y)
	: x(x), y(y), zoomPercentage(1.0f), moveSpeed(10.0f), moveLock(false), updateView(true), lockOnAnchor(false), m_Anchor(nullptr), m_ZoomEffectID(0), m_PositionEffectID(0)
{
	Log::info("Initialised Camera");
}

void Camera::update()
{
	// If it is meant to be following a mob it will update its x and y coordinates
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
		// Otherwise it will check the pressed keys to see where the user wants it to move (mainly used for testing and debugging)
		if(Event::isKeyPressed(Event::KeyboardKey::W) || Event::isKeyPressed(Event::KeyboardKey::Up))
		{
			updateView = true;
			y += moveSpeed;
		}
		if(Event::isKeyPressed(Event::KeyboardKey::S) || Event::isKeyPressed(Event::KeyboardKey::Down))
		{
			updateView = true;
			y -= moveSpeed;
		}
		if(Event::isKeyPressed(Event::KeyboardKey::A) || Event::isKeyPressed(Event::KeyboardKey::Left))
		{
			updateView = true;
			x -= moveSpeed;
		}
		if(Event::isKeyPressed(Event::KeyboardKey::D) || Event::isKeyPressed(Event::KeyboardKey::Right))
		{
			updateView = true;
			x += moveSpeed;
		}
	}
}

void Camera::render()
{
	// If an update is needed it will update the effect
	if(updateView)
	{
		updatePositionEffect();
		updateView = false;
	}
}

#ifdef DEBUG
void Camera::imGuiRender()
{
	// This is for debug information and to allow quick alterations
	ImGui::SliderFloat("Speed", &moveSpeed, 0.0f, 20.0f);
	ImGui::SliderFloat("X", &x, -300.0f, 7700.0f);
	ImGui::SliderFloat("Y", &y, -300.0f, 7700.0f);
	int before = zoomPercentage;
	ImGui::SliderFloat("Zoom", &zoomPercentage, 0.05f, 2.0f);
	if(before != zoomPercentage)
		updateZoomEffect();
	if(ImGui::Checkbox("Camera Lock", &lockOnAnchor))
		m_Anchor->setIsInControl(lockOnAnchor);

	updateView = true;
}
#endif

bool Camera::eventCallback(const Event::Event &e)
{
	switch(e.getType())
	{
	case Event::EventType::Scroll:
	{
		// This allows for the user to Scroll to zoom into the map
		const Event::ScrollEvent &ne = static_cast<const Event::ScrollEvent &>(e);

		if(zoomPercentage == 0.25f && ne.yoffset < 0.0f)
			return false;

		float oldZoom = zoomPercentage;
		// This equation allows for a smooth zoom as it alters depending out far out you are
		zoomPercentage += (ne.yoffset * 0.02f) * (zoomPercentage / 0.20f);

		if(zoomPercentage < 0.25f)   // Sets limit to the scrolling out
			zoomPercentage = 0.25f;

		updateZoomEffect();

		updateView = true;

		return false;
	}

	case Event::EventType::WindowResize:
		// This just updates the view if the window has been changed - will update the view on the next line

	case Event::EventType::MazeMoved:
		changeUpdateView();

	default:
		return false;
	}
}

// These functions check if the effects have been initialised, if they have not they create a new one
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

// This creates the effects needed for the cameras and sends them to the shaderEffectsManager for distribution
void Camera::setShaderEffects()
{
	{
		std::string name   = "u_MVP";
		m_PositionEffectID = Effect::ShaderEffectsManager::sendEffect(name, Application::getProj() * glm::translate(glm::mat4(1.0f), glm::vec3(Application::getWidth() / 2 - x * zoomPercentage, Application::getHeight() / 2 - y * zoomPercentage, 0.0f)));
	}

	{
		std::string name = "u_Zoom";
		m_ZoomEffectID   = Effect::ShaderEffectsManager::sendEffect(name, glm::vec4(zoomPercentage, zoomPercentage, 1.0f, 1.0f));
	}
}

// This updates the position effect stored by the shader effects manager
void Camera::updatePositionEffect()
{
	if(m_PositionEffectID == 0)
		setShaderEffects();

	Effect::UniformMat4 *effect = static_cast<Effect::UniformMat4 *>(Effect::ShaderEffectsManager::getShaderEffect(m_PositionEffectID));
	effect->setMat(Application::getProj() * glm::translate(glm::mat4(1.0f), glm::vec3(Application::getWidth() / 2 - x * zoomPercentage, Application::getHeight() / 2 - y * zoomPercentage, 0.0f)));
}

// This updates the zoom effect stored by the shader effects manager
void Camera::updateZoomEffect()
{
	if(m_ZoomEffectID == 0)
		setShaderEffects();

	Effect::UniformVec4 *effect = dynamic_cast<Effect::UniformVec4 *>(Effect::ShaderEffectsManager::getShaderEffect(m_ZoomEffectID));
	if(effect)
		effect->setVec(glm::vec4(zoomPercentage, zoomPercentage, 1.0f, 1.0f));
	else
		Log::error("Incorrect effect ID given!", LOGINFO);
}

// Returns whether an object is in the frame or not (this is to decrease render buffers for when things are not in frame)
bool Camera::isInFrame(float objX, float objY, CollisionBox &box)
{
	return objX + box.upperBound.x > x - Application::getWidth() / (zoomPercentage * 2) && objX + box.lowerBound.x <= x + Application::getWidth() / (zoomPercentage * 2) && objY + box.upperBound.y > y - Application::getHeight() / (zoomPercentage * 2) && objY + box.lowerBound.y <= y + Application::getHeight() / (zoomPercentage * 2);
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

// Clears the anchor
void Camera::clearAnchor()
{
	lockOnAnchor = false;
	m_Anchor     = nullptr;
}

float Camera::getZoom()
{
	return zoomPercentage;
}

void Camera::changeUpdateView()
{
	updateView = true;
}

// This is for converting window coordinates to ones inside the level
Vec2f Camera::convertWindowToLevel(Vec2f inp)
{
	float nx = x + (2 * inp.x - Application::getWidth()) / (2 * zoomPercentage);
	float ny = y + (2 * inp.y - Application::getHeight()) / (2 * zoomPercentage);
	return {nx, ny};
}
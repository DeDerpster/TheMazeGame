#include "Camera.h"

#include <GLFW/glfw3.h>

#include "Log.h"

#include "Tile.h"

#include "Application.h"

#include "ImGui.h"

Camera::Camera()
	: x(0.0f), y(0.0f), zoomPercentage(1.0f), moveSpeed(10.0f), moveLock(false), updateView(true), lockOnAnchor(false), m_Anchor(nullptr)
{
	Log::info("Initialised Camera");
}

Camera::Camera(float x, float y)
	: x(x), y(y), zoomPercentage(1.0f), moveSpeed(10.0f), moveLock(false), updateView(true), lockOnAnchor(false), m_Anchor(nullptr)
{
}

void Camera::update()
{
	if(m_Anchor && lockOnAnchor)
	{
		if(m_Anchor->getIsMoving() || updateView)
		{
			updateView = true;
			x          = m_Anchor->getX();
			y          = m_Anchor->getY();
		}
	}
	else
	{
		if(Application::isKeyPressed(GLFW_KEY_W) || Application::isKeyPressed(GLFW_KEY_UP))
		{
			updateView = true;
			y += moveSpeed;
		}
		if(Application::isKeyPressed(GLFW_KEY_S) || Application::isKeyPressed(GLFW_KEY_DOWN))
		{
			updateView = true;
			y -= moveSpeed;
		}
		if(Application::isKeyPressed(GLFW_KEY_A) || Application::isKeyPressed(GLFW_KEY_LEFT))
		{
			updateView = true;
			x -= moveSpeed;
		}
		if(Application::isKeyPressed(GLFW_KEY_D) || Application::isKeyPressed(GLFW_KEY_RIGHT))
		{
			updateView = true;
			x += moveSpeed;
		}
	}
	if(updateView)
	{
		std::string         name = "u_MVP";
		Effect::UniformMat4 effect(name, Application::getProj() * getView());
		Application::setEffect(effect, false);
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
		std::string         name = "u_Zoom";
		float               zoom = zoomPercentage;
		Effect::UniformVec4 zoomEffect(name, glm::vec4(zoom, zoom, 1.0f, 1.0f));
		Application::setEffect(zoomEffect);
	}
	if(ImGui::Checkbox("Camera Lock", &lockOnAnchor))
	{
		m_Anchor->setIsInControl(lockOnAnchor);
		updateView = true;
	}
}
#endif

bool Camera::eventCallback(const Application::Event &e)
{
	if(e.getType() == Application::EventType::scroll)
	{
		const Application::ScrollEvent &ne = static_cast<const Application::ScrollEvent &>(e);

		if(zoomPercentage == 0.25f && ne.yoffset < 0.0f)
			return false;

		float oldZoom = zoomPercentage;
		zoomPercentage += (ne.yoffset * 0.02f) * (zoomPercentage / 0.20f);

		if(zoomPercentage < 0.25f)
			zoomPercentage = 0.25f;

		std::string         name = "u_Zoom";
		Effect::UniformVec4 zoomEffect(name, glm::vec4(zoomPercentage, zoomPercentage, 1.0f, 1.0f));
		Application::setEffect(zoomEffect);

		updateView = true;

		return false;
	}
	else if(e.getType() == Application::EventType::windowResize)
	{
		const Application::WindowResizeEvent &ne = static_cast<const Application::WindowResizeEvent &>(e);

		//int widthChange  = ne.oWidth - ne.width;
		//int heightChange = ne.oHeight - ne.height;
		//x += ((float) widthChange) / 2;
		//y += ((float) heightChange) / 2;
		updateView = true;
		return true;
	}
	return false;
}

bool Camera::setEffect(const Effect::RenderEffect &e)
{
	return false;
}

glm::mat4 Camera::getView()
{
	return glm::translate(glm::mat4(1.0f), glm::vec3(Application::getWidth() / 2 - x * zoomPercentage, Application::getHeight() / 2 - y * zoomPercentage, 0.0f));
}

bool Camera::isInFrame(float objX, float objY)
{
	// TODO: Fix this so it accounts for zoom!
	return objX + 100 > x - Application::getWidth() / (zoomPercentage * 2) && objX - 100 <= x + Application::getWidth() / (zoomPercentage * 2) && objY + 100 > y - Application::getHeight() / (zoomPercentage * 2) && objY - 100 <= y + Application::getHeight() / (zoomPercentage * 2);
	return true;
}

void Camera::setLock(bool locked)
{
	if(locked)
	{
		moveUp    = false;
		moveDown  = false;
		moveLeft  = false;
		moveRight = false;
	}
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

void Camera::setAnchor(Entity *e)
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
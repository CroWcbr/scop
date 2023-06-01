#include "Window.hpp"
#include "Keys.hpp"
#include "Input.hpp"
#include "Vector.hpp"

#include <GLFW/glfw3.h>
#include <iostream>

namespace Scop
{

void	glViewportUpdate(const unsigned int width, const unsigned int height)
{
	if (width > height)
	{
		glViewport((width - height) / 2, 0, height, height);
	}
	else
	{
		glViewport(0, (height - width) / 2, width, width);
	}
}

unsigned int	updateResolution(const unsigned int width, const unsigned int height)
{
	return width > height \
			? height / 100 * 100 == 0 ? 100 : height / 100 * 100 \
			: width / 100 * 100 == 0 ? 100 :  width / 100 * 100;
}

Window::Window(std::string title, const unsigned int width, const unsigned int height)
	: m_title(std::move(title))
	, m_data({ width, height })
{
	m_data.resolution = updateResolution(width, height);
	m_resultCode = init();
}

Window::~Window()
{
	glfwTerminate();
}

int Window::init()
{
	if (!glfwInit())
	{
		std::cerr << "Can't initialize GLFW!" << std::endl;
		return -1;
	}

	m_pWindow = glfwCreateWindow(m_data.width, m_data.height, m_title.c_str(), nullptr, nullptr);
	if (!m_pWindow)
	{
		std::cerr << "Can't initialize window [" << m_title << "] with size " << m_data.width << "x" << m_data.height << std::endl;
		glfwTerminate();
		return -2;
	}

	glfwMakeContextCurrent(m_pWindow);
	glViewportUpdate(m_data.width, m_data.height);
	std::cout << "Vendor: " << reinterpret_cast<const char*>(glGetString(GL_VENDOR)) << std::endl;
	std::cout << "Renderer: " << reinterpret_cast<const char*>(glGetString(GL_RENDERER)) << std::endl;
	std::cout << "OpenGL version supported: " << reinterpret_cast<const char*>(glGetString(GL_VERSION)) << std::endl;

	init_callback();

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return 0;
}

void	Window::init_callback()
{
	glfwSetWindowUserPointer(m_pWindow, &m_data);
	glfwSetWindowSizeCallback(m_pWindow, 
		[](GLFWwindow* pWindow, int width, int height)
		{
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
			std::cout << "[WindowSizeCallback] Changed size to " << width << "x" << height << std::endl;
			data.width = width;
			data.height = height;
			data.resolution = updateResolution(width, height);
			glViewportUpdate(width, height);
		}
	);

	glfwSetWindowCloseCallback(m_pWindow,
		[](GLFWwindow* pWindow)
		{
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
			std::cout << "[WindowCloseCallback]" << std::endl;
			data.isClosed = true;
		}
	);

	glfwSetKeyCallback(m_pWindow,
		[](GLFWwindow* pWindow, int key, int scancode, int action, int mods)
		{
			if (key < 0)
			{
				std::cout << "unknown key " << key << std::endl;
				return;
			}
			switch (action)
			{
				case GLFW_PRESS:
				{
					std::cout << "[KeyCallback GLFW_PRESS] " << key << std::endl;
					Input::PressKey(key);
					break;
				}
				case GLFW_RELEASE:
				{
					std::cout << "[KeyCallback GLFW_RELEASE] " << key << std::endl;
					Input::ReleaseKey(key);
					break;
				}
				case GLFW_REPEAT:
				{
					std::cout << "[KeyCallback GLFW_REPEAT] " << key << std::endl;
					Input::PressKey(key);
					break;
				}
			}
		}
	);
}

const void	Window::on_update(const unsigned char* image, const unsigned int image_resolution) const
{
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_resolution, image_resolution, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, 1.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glfwSwapBuffers(m_pWindow);
	glfwPollEvents();
}

const void	Window::on_update_test(const unsigned char* image, const unsigned int width, const unsigned int height) const
{
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT);

	const unsigned char *ttt = image;
	for (int i = 0; i < height ; i++)
	{
		for (int j = 0; j < width; j++)
		{
			glColor3ub(ttt[0],ttt[1], ttt[2]);
			glBegin(GL_POINTS);
			glVertex2f(j / (width / 2.f) - 1.0f, i / (height / 2.f)  - 1.0f);
			glEnd();	
			ttt += 3;
		}
	}
	glfwSwapBuffers(m_pWindow);
	glfwPollEvents();
}

}

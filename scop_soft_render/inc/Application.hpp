#pragma once

#include "Model.hpp"
#include "TGAimage.hpp"
#include "Camera.hpp"
#include "Vector.hpp"

namespace Scop
{

	class Application
	{
	protected:
		class Window*		m_pWindow = nullptr;
		Model*				m_pModel = nullptr;
		TGAimage*			m_pTga_image = nullptr;
		struct IShader*		m_pShader = nullptr;
		unsigned char*		m_pImage = nullptr;
		int*				m_pZbuffer = nullptr;
		unsigned int		m_image_size = 0;
		unsigned int		m_image_resolution = 0;	//Texture a multiple of 100
		unsigned int		m_bytespp = 3;			//RGB
		bool				m_redraw = true;
		void				(Application::*m_pDrawFunction)() = nullptr;

		Camera				m_camera{ Vec3f({0, 0, 0}) };

	public:
		void				draw_model_in_point();
		void				draw_model_in_line();
		void				draw_model_in_simple_triangle();

	protected:
		void				point(int x, int y, const unsigned char* color);
		void				line(int x0, int y0, int x1, int y1, const unsigned char* color);

		virtual void		on_update() {};



	protected:

		const unsigned char	m_white[3] = { 255, 255, 255 };

		Vec3f				camera_position = { 0.f, 0.f, 1.f };
		Vec3f				camera_rotation = { 0.f, 0.f, 0.f };
		


	protected:
		// void			draw_point_triangle(Vec4f* world_coords);
		// void			draw_line_triange(Vec4f* world_coords);




		// void			draw_line_triange_test(Vec4i* world_coords);

		// void			draw_fill_triange_test(Vec4i* screen_coords, unsigned char color[3]);
		// void			draw_fill_triange_texture(Vec4i* screen_coords, Vec2i* uv, float intensivity);
		// void			draw_fill_triange_texture_test(Vec3i* screen_coords, Vec2i* uv, float intensivity);
		// void			draw_fill_triange_texture_test_inten(Vec3i*  screen_coords_test, float* intensity);

		// void			triangle_shader(Vec4f* screen_coords, Vec3f& varying_intensity);
		// void			triangle();


	public:
		Application();
		~Application();

		Application(const Application&) = delete;
		Application(Application&&) = delete;
		Application&	operator=(const Application&) = delete;
		Application&	operator=(const Application&&) = delete;

		int				start(unsigned int window_width, \
								unsigned int window_height, \
								const char* title, \
								const char* path_model_obj, \
								const char* path_texture_tga);

	};

}

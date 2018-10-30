
#ifndef __mainApp_h_
#define __mainApp_h_

#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cmath>
#include <fstream>

#include "algebra.h"

class sombrero_c {
	public:
		sombrero_c();
		sombrero_c(const float from, const float to, const int res);
		~sombrero_c();
		
		void foo();
		
		float *argument_xy;
		float *value_z;
		
		float interval_from;
		float interval_to;
		
		int resolution;
};

class plot_c {
	private:
		vec3_c *geometry;
		int vcount;
	
	public:
		plot_c(): geometry(nullptr), vcount(0) {};
		plot_c(const plot_c &c);
		~plot_c();
		
		plot_c & operator = (const plot_c &c);
 
		void from_fnc(const sombrero_c &in);
		void upd();
		void show();
};

class mainApp_c {
	private:
		int wnd_height, wnd_width;
		int wnd_posx, wnd_posy;
		
		SDL_Window *window;
		
		bool is_run;
		
		plot_c plot;
		
	public:
		~mainApp_c();
		mainApp_c();

		void init_app();
		void looper();
};

#endif

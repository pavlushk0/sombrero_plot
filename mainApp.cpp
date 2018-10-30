
#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cmath>
#include <cstring>

#include "mainApp.h"
#include "algebra.h"

sombrero_c::sombrero_c(const float from, const float to, const int res) {
	float step;
	
	if (res == 0) {
		std::cout << "Error! sombrero_c constructor - resolution = 0" << std::endl;
		exit(1);
	}
	
	if (to == from) {
		std::cout << "Error! sombrero_c constructor - boundries can't be equal!" << std::endl;
		exit(1);
	}
	
	interval_from = from;
	interval_to = to;
	
	if (from > to) swap_float(interval_from,interval_to);
	
	resolution = abs(res);
	
	step = (interval_to - interval_from) / (resolution - 1);
	
	argument_xy = new float [resolution];
	value_z = new float [resolution*resolution];
	
	argument_xy[0] = interval_from;
	
	for (int i = 1; i < resolution; i++) {
		argument_xy[i] = argument_xy[i-1] + step;
	}
}

sombrero_c::~sombrero_c() {
	delete [] argument_xy;
	delete [] value_z;
}

void sombrero_c::foo() {
	float p;
	int g = 0;
	
	for (int i = 0; i < resolution; i++) {
		for (int j = 0; j < resolution; j++) {
			p = sqrt(argument_xy[i]*argument_xy[i] + argument_xy[j]*argument_xy[j]);
			value_z[g] = sin(2.5*p)/p; /* ROOT of ALL*/
			g++;
		}
	}
}

/*----------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------*/

plot_c::plot_c(const plot_c &c) {
	vcount = c.vcount;
	geometry = new vec3_c[vcount];

	std::memcpy(geometry, c.geometry, (std::size_t)sizeof(vec3_c)*vcount);
}

plot_c::~plot_c() {
	if (geometry != nullptr) delete [] geometry;
}

/*--------------------------------------------------------------------------------
	plot_c & operator = (const plot_c &c) {....} и 
	plot_c(const plot_c &c) {....}  - это калька из Праты, глава про конструктор 
	копирования и перегрузку оператора присваивания, которые нужно использовать
	в случае если класс содержит указатель на динамически выделяемую область памяти
----------------------------------------------------------------------------------*/

plot_c & plot_c::operator = (const plot_c &c) {
	if (this == &c) {
		return *this;
	}
	delete [] geometry;
	vcount = c.vcount;
	geometry = new vec3_c[vcount];
	
	std::memcpy(geometry, c.geometry, (std::size_t)sizeof(vec3_c)*vcount);
	return *this;
}

void plot_c::from_fnc(const sombrero_c &in) {
	vcount = in.resolution*in.resolution;
	geometry = new vec3_c[vcount];
	
	int g = 0;
	
	for (int i = 0; i < in.resolution; i++) {
		for (int j = 0; j < in.resolution; j++) {
			geometry[g] = vec3_c(in.argument_xy[i],in.value_z[g],in.argument_xy[j]);
			g++;
		}
	}
}

void plot_c::show() {
	
	glBegin(GL_POINTS);
	for (int i = 0; i < vcount; i++) {
		glVertex3fv(geometry[i].vec);
	}
	glEnd();
	
}

void plot_c::upd() {
	//mtrx3_c rtn;
	
	//rtn.from_euler(0.0,0.01,0.01);
	//rtn.from_axis_angl(vec3_c(0.0,1.0,0.0),0.002);
	
	qtnn_c rtn;
	
	rtn.from_axis_angl(vec3_c(0.0,1.0,0.0),0.002);
	
	for (int i = 0; i < vcount; i++) {
		geometry[i] = rtn.transform_vec3(geometry[i]);
	}
}

/*----------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------*/

mainApp_c::mainApp_c() {
	wnd_height = 640;
	wnd_width = 480;

	wnd_posx = SDL_WINDOWPOS_CENTERED;
	wnd_posy = SDL_WINDOWPOS_CENTERED;
	
	is_run = true;
}

mainApp_c::~mainApp_c() {

}

void mainApp_c::init_app() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "Error: Unable to init SDL; " << SDL_GetError() << std::endl;
		exit(1);
	}

	window = SDL_CreateWindow("Somrero plot", wnd_posx, wnd_posy, 
							wnd_height, wnd_width, 
							SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

	if(window == nullptr) {
		std::cout << "Error: Unable to creaate window!" << std::endl;
		exit(1);
	}

	sombrero_c sombrero(-7.0, 7.0, 50);
	sombrero.foo();
	
	plot_c tmp;
	tmp.from_fnc(sombrero);
	
	plot = plot_c(tmp);
	
	SDL_GLContext glcontext = SDL_GL_CreateContext(window); 

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 6);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); 
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST); 
	glShadeModel(GL_SMOOTH);
}

void mainApp_c::looper() {
	while(is_run) {
		SDL_Event event; 
		
		while (SDL_PollEvent(&event)) {
			switch(event.type){ 
				case SDL_QUIT: 
					is_run = false;
					break;

				case SDL_KEYDOWN: 
					switch(event.key.keysym.sym) {
						case SDLK_ESCAPE: 
						is_run = false; 
						break;
					}
				break;
			}
		}
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0f, (float) wnd_height / (float) wnd_width, 0.1f, 100.0f); 
		glRotatef(20.0f, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.0f, -10.0f, -20.0f);
		
		glMatrixMode(GL_MODELVIEW); 
		glLoadIdentity();
		
		plot.show();
		plot.upd();
		
		/*
		glBegin(GL_QUADS);

		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f( 1.0, 1.0, 0.0);
		glVertex3f(-1.0, 1.0, 0.0);
		glVertex3f(-1.0,-1.0, 0.0);
		glVertex3f( 1.0,-1.0, 0.0);
		
		glEnd();
		*/
		
		glFlush();
		SDL_GL_SwapWindow(window);
	}
	
	SDL_Quit(); 
}

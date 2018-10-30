
#ifndef ALGEBRA_H
#define ALGEBRA_H

#define IX (i,j) ((i) + (N+2)*(j))

const int _XC = 0;
const int _YC = 1;
const int _ZC = 2;
const int _WC = 3;

inline int INDXn(int i, int j, int n);
float lerp(float a, float b, float t);
void swap_float(float &a, float &b);

class vec3_c {
	public:
		float vec[3];
		
		vec3_c(): vec{0.0, 0.0, 0.0} {};
		vec3_c(const float x, const float y, const float z): vec{x, y, z} {};
		~vec3_c();
		
		void	set(const int index, const float x);
		void	set(const vec3_c &vec);
		float	get(const int index) const;
		float & get(const int index);

		float	operator[](const int index) const;
		const	float & operator[](const int index);

		float	dot(const vec3_c &b);
		vec3_c	cross(const vec3_c &b);
		float	lenght();
		void	normalize_self();
		
		vec3_c	scale(const float &t);
		vec3_c	add(const vec3_c &b);
};

class mtrx3_c {
	private:
		float mtrx[9];

	public:
		mtrx3_c(): mtrx{1.0, 0.0, 0.0,
						0.0, 1.0, 0.0,
						0.0, 0.0, 1.0} {};
		~mtrx3_c();

		void	from_euler(const float yaw, const float pitch, const float roll);
		void	from_axis_angl(const vec3_c &ax, const float phi);

		void	set_idtt(); 
		float	det();
		mtrx3_c	inverse();
		void	tranpose();
		mtrx3_c	mult(const mtrx3_c &b);
		vec3_c	mult_vec3(const vec3_c &b);
};

class mtrx4_c {
	private:
		float mtrx[16];
	
	public:
		mtrx4_c(): mtrx{1.0, 0.0, 0.0, 0.0,
						0.0, 1.0, 0.0, 0.0,
						0.0, 0.0, 1.0, 0.0,
						0.0, 0.0, 0.0, 1.0} {};
		~mtrx4_c();

		void	from_euler(const float yaw, const float pitch, const float roll);
		void	from_axis_angl(const vec3_c &ax, const float phi);
		
		void	set_idtt(); 
		float	det();
		mtrx3_c	inverse();
		void	tranpose();
		mtrx3_c	mult(const mtrx3_c &b);
		vec3_c	mult_vec3(const vec3_c &b);
};

class qtnn_c {
	public:
		float qtnn[4];

		qtnn_c(): qtnn{0.0, 0.0, 0.0, 1.0} {};
		qtnn_c(const float x, const float y, const float z, const float w):
		qtnn{x, y, z, w} {};
		qtnn_c(const vec3_c &vq);
		~qtnn_c();
		
		void 	set(const qtnn_c &q);
		
		float	lenght();
		void	normalize();
		qtnn_c	invert();
		
		qtnn_c	from_mtrx3(const mtrx3_c &b);
		void	from_euler(const float yaw, const float pitch, const float roll);
		void	from_axis_angl(const vec3_c &ax, const float phi);
		qtnn_c	scale(const float t);
		qtnn_c	add(const qtnn_c &q);
		qtnn_c	mult(const qtnn_c &b);
		qtnn_c	mult_vec3(const vec3_c &b);
		vec3_c	transform_vec3(const vec3_c &b);
		
		vec3_c 	to_vec3();
};

class plane_c {
	public:
		float plane[4];
		
		plane_c();
		~plane_c();
};

class line_c {
	public:
		float line[3];
		
		line_c();
		~line_c();
};

class mtrxN_c
{
	private:
		float *mtrx;
		int range;

	public:
		mtrxN_c(const int n);
		~mtrxN_c();
		
		int			get_range() const;
		mtrxN_c		mult(const mtrxN_c &b);
		void		tranpose_self();
};

#endif 

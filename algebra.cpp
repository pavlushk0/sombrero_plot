#include <cmath>
#include "algebra.h"

float lerp(float a, float b, float t) {
	return a + t*(b - a);
}

void swap_float(float &a, float &b) {
	float c;
	
	c = b;
	b = a;
	a = c;
}

inline int INDXn(int i, int j, int n) {
	return (j*n + i); 
}

//#define INDXn(i,j,n) (j*n + i)

/*------------------------------------------------------------------------*/
/*																		  */
/* Three dimensional vector definition									  */
/*																		  */
/*------------------------------------------------------------------------*/

vec3_c::~vec3_c() {

}

void vec3_c::set(const int index, const float x) {
	if ((index < 0) && (index > 3)) return;
	else vec[index] = x;
}

void vec3_c::set(const vec3_c & vec) {
	this->vec[_XC] = vec[_XC];
	this->vec[_YC] = vec[_YC];
	this->vec[_ZC] = vec[_ZC];
}

float vec3_c::get(const int index) const {
	if ((index < 0) && (index > 3)) return 0.0;
	else return vec[index];
}

float & vec3_c::get(const int index) {
	if ((index < 0) && (index > 3)) return vec[index];
	else return vec[index];
}

float vec3_c::operator[](const int index) const {
	return get(index);
}

const float & vec3_c::operator[](const int index) {
	return get(index);
}

float vec3_c::dot(const vec3_c &b) {
	return vec[_XC]*b.vec[_XC] + vec[_YC]*b.vec[_YC] + vec[_ZC]*b.vec[_ZC];
}

vec3_c vec3_c::cross(const vec3_c &b) {
	vec3_c result;

	result.vec[_XC] = vec[_YC]*b.vec[_ZC] - vec[_ZC]*b.vec[_YC];
	result.vec[_YC] = vec[_ZC]*b.vec[_XC] - vec[_XC]*b.vec[_ZC];
	result.vec[_ZC] = vec[_XC]*b.vec[_YC] - vec[_YC]*b.vec[_XC];

	return result;
}

float vec3_c::lenght() {
	return sqrt(vec[_XC]*vec[_XC] + vec[_YC]*vec[_YC] + vec[_ZC]*vec[_ZC]);
}

void vec3_c::normalize_self() {
	float l = this->lenght();
	
	if (l != 0.0) {
		vec[_XC] = vec[_XC] / l;
		vec[_YC] = vec[_YC] / l;
		vec[_ZC] = vec[_ZC] / l;
	} else return;
}

vec3_c vec3_c::scale(const float &t) {
	return vec3_c(vec[_XC] * t, vec[_YC] * t, vec[_ZC] * t);
}

vec3_c vec3_c::add(const vec3_c &b) {
	return vec3_c(vec[_XC] + b.vec[_XC], vec[_YC] + b.vec[_YC], vec[_ZC] + b.vec[_ZC]);
}
		
/*------------------------------------------------------------------------*/
/*																		  */
/* 3x3 matrix definition		     									  */
/*																		  */
/* 0   1   2															  */
/* 3   4   5															  */
/* 6   7   8															  */
/*------------------------------------------------------------------------*/

mtrx3_c::~mtrx3_c() {
	
}

void mtrx3_c::from_euler(const float yaw, const float pitch, const float roll) {
	float cosy = cos(yaw);
	float siny = sin(yaw);
	float cosp = cos(pitch);
	float sinp = sin(pitch);
	float cosr = cos(roll);
	float sinr = sin(roll);

	mtrx[0] =  cosy*cosr - siny*cosp*sinr;
	mtrx[1] = -cosy*sinr - siny*cosp*cosr;
	mtrx[2] =  siny*sinp;

	mtrx[3] =  siny*cosr + cosy*cosp*sinr;
	mtrx[4] = -siny*sinr + cosy*cosp*cosr;
	mtrx[5] = -cosy*sinp;

	mtrx[6] = sinp*sinr;
	mtrx[7] = sinp*cosr;
	mtrx[8] = cosp;
}

void mtrx3_c::from_axis_angl(const vec3_c &ax, const float phi) {
	float cosphi = cos(phi);
	float sinphi = sin(phi);
	float vxvy = ax[_XC]*ax[_YC];
	float vxvz = ax[_XC]*ax[_ZC];
	float vyvz = ax[_YC]*ax[_ZC];
	float vx = ax[_XC];
	float vy = ax[_YC];
	float vz = ax[_ZC];

	mtrx[0] = cosphi + (1.0 - cosphi)*vx*vx;
	mtrx[1] = (1.0 - cosphi)*vxvy - sinphi*vz;
	mtrx[2] = (1.0 - cosphi)*vxvz + sinphi*vy;

	mtrx[3] = (1.0 - cosphi)*vxvy + sinphi*vz;
	mtrx[4] = cosphi + (1.0 - cosphi)*vy*vy;
	mtrx[5] = (1.0 - cosphi)*vyvz - sinphi*vz;

	mtrx[6] = (1.0 - cosphi)*vxvz - sinphi*vy;
	mtrx[7] = (1.0 - cosphi)*vyvz + sinphi*vx;
	mtrx[8] = cosphi + (1.0 - cosphi)*vz*vz;
}

float mtrx3_c::det() {
	return mtrx[0]*mtrx[4]*mtrx[8] +
		   mtrx[6]*mtrx[1]*mtrx[5] +
		   mtrx[2]*mtrx[3]*mtrx[7] -
		   mtrx[0]*mtrx[7]*mtrx[5] -
		   mtrx[8]*mtrx[3]*mtrx[1];
}

mtrx3_c mtrx3_c::inverse() {
	return mtrx3_c();
}

void mtrx3_c::tranpose() {

}

mtrx3_c mtrx3_c::mult(const mtrx3_c &b) {
	const int _N = 3;
	mtrx3_c result;

	for (int j = 0; j < _N; j++)
		for (int i = 0; i < _N; i++)
			result.mtrx[INDXn(i,j,_N)] = mtrx[INDXn(0,j,_N)]*b.mtrx[INDXn(i,0,_N)] + 
								   		 mtrx[INDXn(1,j,_N)]*b.mtrx[INDXn(i,1,_N)] +
								    	 mtrx[INDXn(2,j,_N)]*b.mtrx[INDXn(i,2,_N)];

	return result;
}

vec3_c mtrx3_c::mult_vec3(const vec3_c &b) {
	vec3_c result(mtrx[0]*b[_XC] + mtrx[1]*b[_YC] + mtrx[2]*b[_ZC],
				  mtrx[3]*b[_XC] + mtrx[4]*b[_YC] + mtrx[5]*b[_ZC],
				  mtrx[6]*b[_XC] + mtrx[7]*b[_YC] + mtrx[8]*b[_ZC]);
	
	return result;
}

/*------------------------------------------------------------------------*/
/*																		  */
/* Quaternion definition												  */
/*																		  */
/*------------------------------------------------------------------------*/

qtnn_c::qtnn_c(const vec3_c &vq) {
	qtnn[_WC] = 0.0;
	qtnn[_XC] = vq.vec[_XC];
	qtnn[_YC] = vq.vec[_YC];
	qtnn[_ZC] = vq.vec[_ZC];
}

qtnn_c::~qtnn_c() {

}

void qtnn_c::set(const qtnn_c &q) {
	this->qtnn[_WC] = q.qtnn[_WC];
	this->qtnn[_XC] = q.qtnn[_XC];
	this->qtnn[_YC] = q.qtnn[_YC];
	this->qtnn[_ZC] = q.qtnn[_ZC];
}

float qtnn_c::lenght() {
	return sqrt(qtnn[_XC]*qtnn[_XC] + 
				qtnn[_YC]*qtnn[_YC] + 
				qtnn[_ZC]*qtnn[_ZC] +
				qtnn[_WC]*qtnn[_WC]);
}

void qtnn_c::normalize() {
	float l = this->lenght();
	
	if (l != 0.0) {
		qtnn[_WC] = qtnn[_WC] / l;
		qtnn[_XC] = qtnn[_XC] / l;
		qtnn[_YC] = qtnn[_YC] / l;
		qtnn[_ZC] = qtnn[_ZC] / l;
	}
}

qtnn_c qtnn_c::invert() {
	qtnn_c res;
	
	res.qtnn[_WC] = qtnn[_WC];
	res.qtnn[_XC] = -qtnn[_XC];
	res.qtnn[_YC] = -qtnn[_YC];
	res.qtnn[_ZC] = -qtnn[_ZC];
	
	res.normalize();
	
	return res;
}

qtnn_c qtnn_c::from_mtrx3(const mtrx3_c &b) {
	return qtnn_c();
}

void qtnn_c::from_euler(const float yaw, const float pitch, const float roll) {
	qtnn_c qyaw, qpitch, qroll, tmp;
	
	qyaw.from_axis_angl(vec3_c(1.0, 0.0, 0.0), yaw);
	qpitch.from_axis_angl(vec3_c(0.0, 1.0, 0.0), pitch);
	qroll.from_axis_angl(vec3_c(0.0, 0.0, 1.0), roll);
	
	tmp = qyaw.mult(qpitch);
	
	this->set(tmp.mult(qroll));
}

void qtnn_c::from_axis_angl(const vec3_c &ax, const float phi) {
	float sinhalfphi = sin(phi * 0.5);  
	qtnn[_WC] = cos(phi * 0.5);
	qtnn[_XC] = ax.vec[_XC] * sinhalfphi;
	qtnn[_YC] = ax.vec[_YC] * sinhalfphi;
	qtnn[_ZC] = ax.vec[_ZC] * sinhalfphi;
}

qtnn_c qtnn_c::scale(const float t) {
	qtnn_c res;
	res.qtnn[_WC] = qtnn[_WC] * t;
	res.qtnn[_XC] = qtnn[_XC] * t;
	res.qtnn[_YC] = qtnn[_YC] * t;
	res.qtnn[_ZC] = qtnn[_ZC] * t;
	return res;
}

qtnn_c qtnn_c::add(const qtnn_c &q) {
	return qtnn_c(qtnn[_WC] + q.qtnn[_WC], qtnn[_XC] + q.qtnn[_XC], qtnn[_YC] + q.qtnn[_YC], qtnn[_ZC] + q.qtnn[_ZC]);
}

qtnn_c qtnn_c::mult(const qtnn_c &b) {
	qtnn_c res;
	res.qtnn[_WC] = qtnn[_WC] * b.qtnn[_WC] - qtnn[_XC] * b.qtnn[_XC] - qtnn[_YC] * b.qtnn[_YC] - qtnn[_ZC] * b.qtnn[_ZC];
    res.qtnn[_XC] = qtnn[_WC] * b.qtnn[_XC] + qtnn[_XC] * b.qtnn[_WC] + qtnn[_YC] * b.qtnn[_ZC] - qtnn[_ZC] * b.qtnn[_YC];
    res.qtnn[_YC] = qtnn[_WC] * b.qtnn[_YC] - qtnn[_XC] * b.qtnn[_ZC] + qtnn[_YC] * b.qtnn[_WC] + qtnn[_ZC] * b.qtnn[_XC];
    res.qtnn[_ZC] = qtnn[_WC] * b.qtnn[_ZC] + qtnn[_XC] * b.qtnn[_YC] - qtnn[_YC] * b.qtnn[_XC] + qtnn[_ZC] * b.qtnn[_WC];
	return res;
}

/* function is broken */
qtnn_c qtnn_c::mult_vec3(const vec3_c &b) {
	qtnn_c res;
	res.qtnn[_WC] = -qtnn[_WC] * b.vec[_XC] - qtnn[_YC] * b.vec[_YC] - qtnn[_ZC] * b.vec[_ZC];
    res.qtnn[_XC] =  qtnn[_WC] * b.vec[_XC] + qtnn[_YC] * b.vec[_ZC] - qtnn[_ZC] * b.vec[_YC];
    res.qtnn[_YC] =  qtnn[_WC] * b.vec[_YC] - qtnn[_XC] * b.vec[_ZC] + qtnn[_ZC] * b.vec[_XC];
    res.qtnn[_ZC] =  qtnn[_WC] * b.vec[_ZC] + qtnn[_XC] * b.vec[_YC] - qtnn[_YC] * b.vec[_XC];
	return res;
}

vec3_c qtnn_c::transform_vec3(const vec3_c &b) {
	qtnn_c vq(b);
	qtnn_c tmp;// = this->mult_vec3(b);
	
	tmp = this->mult(vq);
	tmp = tmp.mult(this->invert());
	
	return tmp.to_vec3();
}

vec3_c qtnn_c::to_vec3() {
	return vec3_c(qtnn[_XC], qtnn[_YC], qtnn[_ZC]);
}

/*------------------------------------------------------------------------*/
/*																		  */
/* N*N square matrix definition											  */
/*																		  */
/*------------------------------------------------------------------------*/

mtrxN_c::mtrxN_c(const int n) {
	int i;

	range = n;

	mtrx = new float[range*range];

	for (i = 0; i < range*range; i++)
		mtrx[i] = 0.0;

	for (i = 0; i < range; i++)
		mtrx[range*i] = 1.0;

}

mtrxN_c::~mtrxN_c() {
	delete mtrx;
}

int mtrxN_c::get_range() const {
	return range;
}
/*
mtrxN_c mtrxN_c::mult(const mtrxN_c &b) {
	mtrxN_c ret(range)

	if (range != b.range) return ret;

	for (int j = 0; j < range; j++)
		for (int i = 0; i < range; i++) {
			ret[INDXn(i,j,range)] = 0.0;
			for (int s = 0; s < range; s++)
				ret[INDXn(i,j,range)] = ret[INDXn(i,j,range)] + mtrx[INDXn(s,j,range)]*b.mtrx[INDXn(i,s,range)];
		}
}

void mtrxN_C::tranpose_self() {
	int i, j;
	float tmp;

	for (i = 0; i < range; i++) 
		for 
}
*/
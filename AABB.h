#pragma once
#ifndef AABB_H
#define AABB_H

struct AABB{
	inline AABB::AABB():x(0),y(0),w(0),h(0){}
	int x, y, w, h;
	inline void setX(int x){this->x = x;}
	inline void setY(int y){this->y = y;}
	inline void setW(int w){this->w = w;}
	inline void setH(int h){this->h = h;}
};
#endif
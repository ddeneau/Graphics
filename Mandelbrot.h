#pragma once	
#include<math.h>
class Mandelbrot {
public:
	Mandelbrot() = default;
	int MandelbrotIteration(double x0, double y0, int max_iter);
	double get_x();
	double get_y();

	double x;
	double y;
	int iter;
};
#include<math.h>

class Mandelbrot {
	public:
		double x;
		double y;
		int iter;
		Mandelbrot() {
			x = 0;
			y = 0;
			iter = 0;
		}
	 int MandelbrotIteration(double x0, double y0, int max_iter) {
		if (x*x + y*y >= 4.0 || iter >= max_iter) {
			return 0;
		} else {
			double xtemp = x*x - y*y + x0;
			y = 2*x*y + y0;
			x = xtemp;
			iter++;
		}
		return iter;
	}

	double get_x() {
		return x;
	}

	double get_y() {
		return x;
	}
};
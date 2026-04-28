class Mandelbrot {
public:
	double x;
	double y;
	int iter;
	Mandelbrot();

	int MandelbrotIteration(double x0, double y0, int max_iter);
};
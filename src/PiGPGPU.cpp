/*

// PiGPGPU.cpp : Defines the entry point for the application.
//
*/

#include "PiGPGPU.h"

#define MAX_FRACTAL_ITERATIONS 64


void gcd(Ptr<Int> p, Ptr<Int> q, Ptr<Int> r)
{
	Int a = *p;
	Int b = *q;
	While(any(a != b))
		Where(a > b)
		a = a - b;
	EndBlock
		Where(a < b)
		b = b - a;
	EndBlock
		EndBlock
		* r = a;
}

//Implementation of pseudocode from https://en.wikipedia.org/wiki/Mandelbrot_set#Escape_time_algorithm
void mandelbrot(Ptr<Float> x, Ptr<Float> y, Ptr<Int> iterations) {
	//Temporary variables
	Float a = 0.0f;
	Float b = 0.0f;
	//The location to render for each pixel
	Float p = *x;
	Float q = *y;
	//The number of iterations it has taken to prove a point is
	// or is not in the Mandelbrot set
	Int it = *iterations;
	Int mit = MAX_FRACTAL_ITERATIONS;

	//Create a temporary value
	Float xtemp = 0.0;
	BoolExpr keepGoing = it < mit && a * a + b * b <= 4.f;

	While(any(keepGoing))
		Where(keepGoing)
				xtemp = (a * a) - (b * b) + p;
				b = 2 * a * b + q;
				a = xtemp;
				it++;
			EndBlock
		EndBlock

		* iterations = it;
}


//The same mandelbrot algorithm, only enhanced further for the Pi's GPU
void mandelbrot2(Float upper_x, Float upper_y, Float image_width, Float image_height, Ptr<Int> iterations) {
	//Temporary variables
	Float a = 0.0f;
	Float b = 0.0f;
	//The location in memory of the array of pixels
	Ptr<Int> it_ptr = iterations;
	//The location to render for each pixel
	Float p = 0.f;//TODO
	Float q = 0.f;
	//The number of iterations it has taken to prove a point is
	// or is not in the Mandelbrot set
	Int it = *iterations;
	Int mit = MAX_FRACTAL_ITERATIONS;

	//Create a temporary value
	Float xtemp = 0.0;
	BoolExpr keepGoing = it < mit && a * a + b * b <= 4.f;



	While(any(keepGoing))
		Where(keepGoing)
		For(Int i = 0, i < 8, i++)
			xtemp = (a * a) - (b * b) + p;
			b = 2 * a * b + q;
			a = xtemp;
			it++;
		EndBlock
		EndBlock
		EndBlock

		* iterations = it;
}


/*
int main()
{
	// Construct kernel
	auto k = compile(gcd);

	// Allocate and initialise arrays shared between ARM and GPU
	SharedArray<int> a(16), b(16), r(16);
	srand(0);
	for (int i = 0; i < 16; i++) {
		a[i] = 100 + (rand() % 100);
		b[i] = 100 + (rand() % 100);
	}

	// Invoke the kernel and display the result
	k(&a, &b, &r);
	for (int i = 0; i < 16; i++)
		printf("gcd(%i, %i) = %i\n", a[i], b[i], r[i]);

	return 0;
}
*/



float random_float() {
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

int main()
{
	//Graphics
	sf::RenderWindow window(sf::VideoMode(800, 480), "SFML works!");
	int width = window.getSize().x, height = window.getSize().y;
	int dim = width * height;

	//Initialize the image
	initializeImage(window.getSize().x, window.getSize().y);

	// Construct kernel
	auto k = compile(mandelbrot);

	// Allocate and initialise arrays shared between ARM and GPU
	SharedArray<float> x(16), y(16);
	SharedArray<int> iterations(16);


	int p = 0;
	int q = 0;
	int i = 0;
	bool is_processing = true;
	while (is_processing) {

		//Load up the inputs
		int ptemp = p;
		int qtemp = q;
		for (i = 0; i < 16; i++) {
			x[i] = ((static_cast<float>(ptemp) / static_cast<float>(width)) - 0.7f) * 3.f;
			y[i] = ((static_cast<float>(qtemp) / static_cast<float>(height)) - 0.5f) * 3.f;
			iterations[i] = 0;

			ptemp++;
			if (ptemp >= width) {
				ptemp = 0;
				qtemp++;
			}
		}

		// Invoke the kernel
		k(&x, &y, &iterations);

		// Use the output to change the pixels
		for (i = 0; i < 16; i++) {
			int val = iterations[i] * 255 / MAX_FRACTAL_ITERATIONS;
			sf::Color c = sf::Color(val/3, val, val);
			paintPixel(p, q, c);
			p++;
			if (p >= width) {
				p = 0;
				q++;
				if (q >= height) {
					is_processing = false;
					break;
				}
			}
		}
	}

	printf("Done!\n");

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		drawImage(&window);
		window.display();
	}

	return 0;
}

void initializeImage(int width, int height)
{
	img = new sf::Image();
	img->create(width, height);

	tex = new sf::Texture();
	tex->create(width, height);

	sprite = new sf::Sprite();
	sprite->setTexture(*tex);
}

void paintPixel(int x, int y, sf::Color color)
{
	img->setPixel(x, y, color);
}

void drawImage(sf::RenderTarget* renderTarget)
{
	//Update the texture to the image
	tex->update(*img);
	renderTarget->draw(*sprite);
}

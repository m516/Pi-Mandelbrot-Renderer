/*

// PiGPGPU.cpp : Defines the entry point for the application.
//
*/

#include "PiGPGPU.h"

#include "QPULib.h"
#include <time.h>
#include <SFML/Graphics.hpp>


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
	Float a = 0.0f;
	Float b = 0.0f;
	Float p = *x;
	Float q = *y;
	Int it = *iterations;
	Int mit = 10000;

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

	// Construct kernel
	auto k = compile(mandelbrot);

	// Allocate and initialise arrays shared between ARM and GPU
	SharedArray<float> x(16), y(16);
	SharedArray<int> iterations(16);



	for (int i = 0; i < 16; i++) {
		x[i] = -1.0f + (random_float() * 2.f);
		y[i] = -1.0f + (random_float() * 2.f);
		iterations[i] = 0;
	}

	// Invoke the kernel and display the result
	k(&x, &y, &iterations);
	for (int i = 0; i < 16; i++)
		printf("mandelbrot(%2.3f, %2.3f) = %i\n", x[i], y[i], iterations[i]);



	//Graphics
	sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		window.draw(shape);
		window.display();
	}

	return 0;
}


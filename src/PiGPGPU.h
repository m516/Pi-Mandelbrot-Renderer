// PiGPGPU.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>

#include "QPULib.h"
#include <time.h>
#include <SFML/Graphics.hpp>

sf::Image* img;
sf::Texture* tex;
sf::Sprite* sprite;

void initializeImage(int width, int height);

void paintPixel(int x, int y, sf::Color color);

void drawImage(sf::RenderTarget* renderTarget);
// OpenGlMinecraft.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#pragma once

#include "MainGL.h"

int main()
{
	MainGL::initMainGL(1000, 600, "Hello");
	
	MainGL::loadScene("some folder");

	MainGL::startGame();
}




/* BasicSnake.cpp : Defines the entry point for the console application.
This project uses sfml for the graphical interface.

A Queue is used to create the snake body by extending the tail body and deleting the snake tail

A Set is utilized to store all available open spots for generating randomized dot for snake to eat

This is before I read the book "Clean Code".
*/

#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <queue>
#include <iostream>
#include <set>

int random(std::set <int> A) {	//randomly generates location for new fruit
	return int(rand() % A.size());
}

int main() {
	const int xWindow = 400, yWindow = 400;		//originally 600 and 400
	const int gridSize = 10;
	const int tileRow = xWindow / gridSize, tileCol = yWindow / gridSize;

	const int winCond = tileRow * tileCol;
	int menu = -2;//determines screen: -2 for start, -1 for losing (and restarting), -3 for winning, and 0 for gameplay
	sf::RenderWindow window(sf::VideoMode(xWindow, yWindow), "Snake Game");
	int level[tileRow][tileCol] = { {0} };
	sf::RectangleShape rect(sf::Vector2f(gridSize, gridSize));
	level[0][tileCol / 2] = 1;
	int rPos = 0, cPos = tileCol / 2;	//initializing snake head
	bool up = false, down = false, left = false, right = true;	//keeping track of input
	std::queue<int> rQ;	//queue for tracking row coordinates
	std::queue<int> cQ;	//queue for tracking col coordinates

	window.setFramerateLimit(15);	//sets the frame rate for the game

	int t;//used to store random
	//for calculating random generation
	std::set<int> randGen;
	for (int i = 0; i < winCond; i++) {
		randGen.insert(i);
	}
	randGen.erase(randGen.find((rPos * tileCol) + cPos));

	//fruit location tracker
	int rfPos = tileRow / 2, cfPos = tileCol / 2;
	level[rfPos][cfPos] = 2;

	//to control when to add to the snake's body
	bool popping = false;
	rQ.push(rPos);
	cQ.push(cPos);
	//text for game
	sf::Text text;
	sf::Font MyFont;
	// Load from a font file on disk
	if (!MyFont.loadFromFile("arial.ttf"))
	{
		std::cout << "font error" << std::endl;
	}

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

		//clears the window
		window.clear();

		if (menu == -2) {
			text.setString("Snake Game\n Press Spacebar to Start");
			text.setCharacterSize(30);
			text.setFont(MyFont);
			text.setFillColor(sf::Color::Red);
			text.setStyle(sf::Text::Bold);
			window.draw(text);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {//starting the game
				menu = 0;
			}
		}
		else if (menu == -1) {
			text.setString("You Lose\n Press Spacebar to try again");
			text.setCharacterSize(30);
			text.setFillColor(sf::Color::Red);
			text.setFont(MyFont);
			text.setStyle(sf::Text::Bold);
			window.draw(text);
			//reinitializing
			//reset map
			for (int i = 0; i < tileRow; i++) {
				for (int j = 0; j < tileCol; j++) {
					level[i][j] = 0;
				}
			}
			for (int i = 0; i < winCond; i++) {
				randGen.insert(i);
			}
			randGen.erase(randGen.find((rPos * tileCol) + cPos));
			level[0][tileCol / 2] = 1;
			rPos = 0, cPos = tileCol / 2;	//reinitializing snake head

			//reset fruit location tracker
			rfPos = tileRow / 2, cfPos = tileCol / 2;
			level[rfPos][cfPos] = 2;

			//to control when to add to the snake's body--reinitializing it
			while (!rQ.empty() && !cQ.empty()) {
				rQ.pop();
				cQ.pop();
			}
			bool popping = false;
			rQ.push(rPos);
			cQ.push(cPos);

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {//starting the game
				menu = 0;
			}

			//reset controls
			right = true;
			left = false;
			up = false;
			down = false;
		}
		else if (menu == -3) {
			text.setString("Winner");
			text.setCharacterSize(30);
			text.setFillColor(sf::Color::Red);
			text.setFont(MyFont);
			text.setStyle(sf::Text::Bold);
			window.draw(text);
		}
		else
		{
			//updating the game and determining frames per second
			popping = true;

			//input
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && !right) {//moving to the left
				left = true;
				down = false;
				up = false;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && !up) {//moving down
				down = true;
				right = false;
				left = false;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !down) {//moving up
				up = true;
				left = false;
				right = false;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && !left) {//moving right
				right = true;
				up = false;
				down = false;
			}

			if (menu == 0) {

				//continued movement
				if (left && rPos > 0 && level[rPos - 1][cPos] != 1) {	//moving left
					if (level[rPos - 1][cPos] == 2) {
						popping = false;
					}
					randGen.erase(randGen.find(((rPos - 1) * tileCol) + cPos));
					rQ.push(rPos - 1);
					cQ.push(cPos);
					level[rPos - 1][cPos] = 1;
					rPos--;
				}
				else if (down && cPos < tileCol - 1 && level[rPos][cPos + 1] != 1) {	//moving down
					if (level[rPos][cPos + 1] == 2) {
						popping = false;
					}
					randGen.erase(randGen.find((rPos * tileCol) + (cPos + 1)));
					rQ.push(rPos);
					cQ.push(cPos + 1);
					level[rPos][cPos + 1] = 1;
					cPos++;
				}
				else if (up && cPos > 0 && level[rPos][cPos - 1] != 1) {	//moving up
					if (level[rPos][cPos - 1] == 2) {
						popping = false;
					}
					randGen.erase(randGen.find((rPos * tileCol) + cPos - 1));
					rQ.push(rPos);
					cQ.push(cPos - 1);
					level[rPos][cPos - 1] = 1;
					cPos--;
				}
				else if (right && rPos < tileRow - 1 && level[rPos + 1][cPos] != 1) {	//moving right
					if (level[rPos + 1][cPos] == 2) {
						popping = false;
					}
					randGen.erase(randGen.find(((rPos + 1) * tileCol) + cPos));
					rQ.push(rPos + 1);
					cQ.push(cPos);
					level[rPos + 1][cPos] = 1;
					rPos++;
				}
				else {
					menu = -1;
				}

				//delete tail of snake
				if (!rQ.empty() && popping && !randGen.empty()) {
					randGen.insert((rQ.front()* tileCol) + cQ.front());
					level[rQ.front()][cQ.front()] = 0;
					rQ.pop();
					cQ.pop();
				}

				//Winning condition
				if (rQ.size() == winCond) {
					menu = -3;
					popping = true;
				}
			}
			//checks if game needs to generate another fruit
			if (!popping) {
				t = random(randGen);
				std::set<int>::iterator it = randGen.begin();
				std::advance(it, t);
				level[*it / tileRow][*it%tileRow] = 2;
			}

			//sets the color
			for (int i = 0; i < tileRow; i++) {
				for (int j = 0; j < tileCol; j++) {
					rect.setPosition(gridSize*i, gridSize*j);
					if (level[i][j] == 0) {
						rect.setFillColor(sf::Color::Black);
					}
					else if (level[i][j] == 1) {
						rect.setFillColor(sf::Color::White);
					}
					else {
						rect.setFillColor(sf::Color::Green);
					}
					window.draw(rect);
				}
			}
		}

		window.display();
	}
    return 0;
}
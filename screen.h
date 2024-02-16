#pragma once
#define ROWS 17
#define COLS 40
#define NODE_SIZE 40
#define HEADER_HEIGHT 200


#include <SFML/Graphics.hpp>
#include "grid.h"
#include "menu.h"

// DONT forget to set destroyers

using namespace std;
using namespace sf;

class Screen {
private:
	RenderWindow window;
	bool help_window = false;
	bool selected_alg = false;
public:

	Screen(){}

	void execute() {

		window.create({ NODE_SIZE * COLS
						, NODE_SIZE * ROWS + HEADER_HEIGHT }
					, "Project");

		window.setFramerateLimit(60);

		Grid grid(ROWS, COLS, window);

		Menu menu(HEADER_HEIGHT
					, NODE_SIZE * COLS
					, NODE_SIZE * ROWS
					, window
			);


		while (window.isOpen()) {

			window.clear(Color::White);
			for (auto event = Event{}; window.pollEvent(event); ) {
				if (event.type == Event::Closed)
					window.close();

				if (help_window) {
					if (event.type == Event::KeyPressed) {
						help_window = false;
						menu.activate_help_window(help_window);
					}
				}

				else {

					if (event.type == Event::MouseButtonPressed) {

						Tuple current_pos = getTuple(); // x -> [0,39]  y -> [0,16]

						if (event.mouseButton.button == Mouse::Left) {

							if (selected_alg) grid.partial_clean(); // no se limpia inicio ni fin
							if (grid.getEnd() == current_pos)
								grid.clearEnd(); // sus valores se ponen a -1,-1
							grid.setBegin(current_pos);
						} else if (event.mouseButton.button == Mouse::Right) {
							
							if (selected_alg) grid.partial_clean();
							if (grid.getBegin() == current_pos)
								grid.clearBegin();
							grid.setEnd(current_pos);
						}

					}

					if (event.type == Event::KeyPressed) {

						if (event.key.code == Keyboard::H) {
							help_window = true;
							menu.activate_help_window(help_window);
						}

						if (event.key.code == Keyboard::C) {
							grid.total_clean(); // comienzo y fin se borran
						}

						if (event.key.code >= Keyboard::Num1 && event.key.code <= Keyboard::Num3) {
							
							if (grid.isBegin() == false || grid.isEnd() == false) {
								menu.changeDynamicText(1); // 1: No begin or end
							}
							
							else {

								if (event.key.code == Keyboard::Num1) {
									menu.updateSelector(1);
									menu.changeDynamicText(2); // 2. executing dijkstra
									grid.setAlgorithm(1);
								}
								else if (event.key.code == Keyboard::Num2) {
									menu.updateSelector(2);
									menu.changeDynamicText(3); // 3. executing dfs
									grid.setAlgorithm(2);
								}
								else if (event.key.code == Keyboard::Num3) {
									menu.updateSelector(3);
									menu.changeDynamicText(4); // 4. executing bfs
									grid.setAlgorithm(3);
								}
							
							}

						}
						
						// Ahora falta que el usuario no pueda settear el inicio o
						// fin en una pared
						if (event.key.code == Keyboard::Space) {
							
							if (grid.getAlgorithm() != 0 && (grid.isBegin() == true && grid.isEnd() == true)) {
								
								switch (grid.getAlgorithm()) {
								case 1: // DIJKSTRA
									// cuando ya se ejecutó y se muestra en pantalla
									grid.dijkstra();
									selected_alg = true;
									break;
								case 2: // DFS
									// falta
									selected_alg = true;
									break;
								case 3: // BFS
									// falta
									selected_alg = true;
									break;
								default:
									break;
								}
							
							} else menu.changeDynamicText(1);
							
						}

					}

				}

			}

			grid.draw();
			menu.draw();

		}	

	}


	Tuple getTuple() {

		Vector2i mouse = Mouse::getPosition(window);
		
		int x = mouse.x / NODE_SIZE;
		int y = mouse.y / NODE_SIZE - (HEADER_HEIGHT / NODE_SIZE);

		return Tuple(x, y);

	}

};


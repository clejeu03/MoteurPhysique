#include <iostream>
#include <cstdlib>

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>

#include "renderer/WindowManager.hpp"
#include "renderer/ParticleRenderer2D.hpp"

#include "ParticleManager.hpp"
#include "ConstantForce.hpp"
#include "LeapFrogSolver.hpp"

#include "Polygon.hpp"
#include "PolygonForce.hpp"
#include "HookForce.hpp"
#include "BrakeForce.hpp"
#include "GraphHookForce.hpp"
#include "GraphBrakeForce.hpp"

#include <vector>

static const Uint32 WINDOW_WIDTH = 1024;
static const Uint32 WINDOW_HEIGHT = 1024;


int main() {
    imac3::WindowManager wm(WINDOW_WIDTH, WINDOW_HEIGHT, "Newton Pendulum");
    wm.setFramerate(30);

    imac3::ParticleRenderer2D renderer;

    // Création des particules
    imac3::ParticleManager pm(16, 16);
    
	// Création des forces
	imac3::ConstantForce gravity(glm::vec2(0, -0.01f));

    // Création du solver physique
	imac3::LeapFrogSolver solver;

    // Attributs de la chute
    float debit = 1000;
    float widthWaterfall = 1.6;
    float heightWater = 0.2; // Hauteur d'eau en amont de la chute

    // Création des obstacles
    std::vector<imac3::Polygon> polygons;
    std::vector<imac3::PolygonForce> polygonForces;

    polygons.push_back(imac3::Polygon::buildCircle(glm::vec3(1, 0, 0), glm::vec2(-0.5, 0.5), 0.1, 9));
    polygons.push_back(imac3::Polygon::buildCircle(glm::vec3(0, 1, 0), glm::vec2(0.5, 0.5), 0.3, 9));
    polygons.push_back(imac3::Polygon::buildCircle(glm::vec3(0, 0, 1), glm::vec2(0.5, -0.5), 0.05, 9));
    polygons.push_back(imac3::Polygon::buildCircle(glm::vec3(1, 0.5, 0), glm::vec2(-0.5, -0.5), 0.1, 9));
    polygons.push_back(imac3::Polygon::buildCircle(glm::vec3(1, 0, 1), glm::vec2(0., 0.), 0.2, 9));

    for(size_t i = 0; i < polygons.size(); i++)
    {
        polygonForces.push_back(imac3::PolygonForce(polygons.at(i), 1, debit, widthWaterfall, heightWater, solver));
        polygonForces.at(i).setDt(0.01f);
    }

    // Temps s'écoulant entre chaque frame
    float dt = 0.f;
	bool done = false;

    float lastX, lastY;
    bool leftKeyPressed = false;

    while(!done)
    {
        wm.startMainLoop();

        pm.createParticlesFromSource(50, glm::vec2(1, -1), glm::vec2(-0.3, 0.5), 1, glm::vec3(1.f, 1.f, 1.f));
        pm.createParticlesFromSource(50, glm::vec2(-1, 0.5), glm::vec2(0.1, 0.1), 1, glm::vec3(1.f, 1.f, 1.f));

        // Rendu
        renderer.clear();
        pm.drawParticles(renderer);
        
        for(size_t i = 0; i < polygons.size(); i++)
        {
            polygons.at(i).draw(renderer);
        }

        // Application des forces
        gravity.apply(pm);
        for(size_t i = 0; i < polygonForces.size(); i++)
        {
            polygonForces.at(i).apply(pm);
        }

        // Solve
		solver.solve(pm, dt);

        // Gestion des evenements
		SDL_Event e;
        while(wm.pollEvent(e))
        {
			switch(e.type)
            {
				default:
					break;
				case SDL_QUIT:
					done = true;
					break;
                case SDL_MOUSEBUTTONDOWN:
                    if(e.button.button == SDL_BUTTON_LEFT)
                    {
                        leftKeyPressed = true;
                        lastX = -1 + 2 * (float)e.button.x / WINDOW_WIDTH;
                        lastY = 1 - 2 * (float)e.button.y / WINDOW_HEIGHT;
                        for(size_t t = 0; t < polygons.size(); t++)
                        {
                            if(polygons.at(t).isMouseOn(lastX, lastY))
                            {
                                polygons.at(t).select();
                            }
                        }
                    }
                    break;

                case SDL_MOUSEBUTTONUP:
                    if(e.button.button == SDL_BUTTON_LEFT)
                    {
                        leftKeyPressed = false;
                        for(size_t t = 0; t < polygons.size(); t++)
                        {
                            polygons.at(t).unselect();
                        }
                    }
                    break;

                case SDL_MOUSEMOTION:
                    float x = -1 + 2 * (float)e.button.x / WINDOW_WIDTH;
                    float y = 1 - 2 * (float)e.button.y / WINDOW_HEIGHT;
                    for(size_t t = 0; t < polygons.size(); t++)
                    {
                        if(leftKeyPressed && polygons.at(t).isSelected())
                        {
                            polygons.at(t).translate(glm::vec2(x - lastX, y - lastY));
                            lastX = x;
                            lastY = y;
                        }
                        if(polygons.at(t).isMouseOn(x, y))
                        {
                            polygons.at(t).highlight(true);
                        }
                        else
                        {
                            polygons.at(t).highlight(false);
                        }
                    }            
                    break;
			}
		}

        // Mise à jour de la fenêtre
        dt = wm.update();

        // Mise à jour des forces
        for(size_t i = 0; i < polygonForces.size(); i++)
        {
            polygonForces.at(i).setDt(dt);
        }

	}

	return EXIT_SUCCESS;
}

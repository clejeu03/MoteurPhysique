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

#include <vector>

static const Uint32 WINDOW_WIDTH = 512;
static const Uint32 WINDOW_HEIGHT = 512;


int main() {
    imac3::WindowManager wm(WINDOW_WIDTH, WINDOW_HEIGHT, "Newton was a Geek");
    wm.setFramerate(30);

    imac3::ParticleRenderer2D renderer;

    // Création des particules
    imac3::ParticleManager pm;
	pm.addRandomParticles(1);
	
	glm::vec2 W = pm.getPosition(0);
	std::cout << "particle is in " << W.x << ", " << W.y << std::endl;
	
	// CRéation des forces
	imac3::ConstantForce gravity(glm::vec2(0, -0.01f));
	
	size_t count = pm.getNumberParticles();
	
	imac3::LeapFrogSolver solver;

	// Création des polygones
	imac3::Polygon box = imac3::Polygon::buildBox(glm::vec3(1.f, 0.f, 0.f), glm::vec2(-0.9f, -0.9f), 1.8, 1.8);
	imac3::Polygon circle = imac3::Polygon::buildCircle(glm::vec3(0.f, 1.f, 0.f), glm::vec2(0.f, 0.2f), 0.2, 32);

	// Création des forces correspondantes
	imac3::PolygonForce boxForce(box, 1.5, solver);
	boxForce.setDt(0.1f);
	
    // Temps s'écoulant entre chaque frame
    float dt = 0.f;

	bool done = false;
	
    while(!done) {
        wm.startMainLoop();

        // Rendu
        renderer.clear();
        pm.drawParticles(renderer);
		box.draw(renderer);
		circle.draw(renderer);

        // Simulation
        gravity.apply(pm);
        //std::cout << "force buffer " << pm.getForceBuffer(0).x << ", " << pm.getForceBuffer(0).y << std::endl;
        boxForce.apply(pm);
        //std::cout << "force buffer " << pm.getForceBuffer(0).x << ", " << pm.getForceBuffer(0).y << std::endl;
         
		solver.solve(pm, dt);

		
        // Gestion des evenements
		SDL_Event e;
        while(wm.pollEvent(e)) {
			switch(e.type) {
				default:
					break;
				case SDL_QUIT:
					done = true;
					break;
			}
		}

        // Mise à jour de la fenêtre
        dt = wm.update();
        boxForce.setDt(dt);
	}

	return EXIT_SUCCESS;
}

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

#include <vector>

static const Uint32 WINDOW_WIDTH = 1024;
static const Uint32 WINDOW_HEIGHT = 1024;


int main() {
    imac3::WindowManager wm(WINDOW_WIDTH, WINDOW_HEIGHT, "Newton was a Geek");
    wm.setFramerate(30);

    imac3::ParticleRenderer2D renderer;

    // Création des particules
    imac3::ParticleManager particleManager;
    particleManager.addParticle(glm::vec2(0, 0), 1.f, glm::vec2(0, 0), glm::vec3(1, 1, 1));
	particleManager.addRandomParticles(20);
	
	// CRéation des forces
	imac3::ConstantForce gravity(glm::vec2(0, -0.01f));
	
	size_t count = particleManager.getNumberParticles();
	
	imac3::LeapFrogSolver solver;

	// Création des polygones
	imac3::Polygon box = imac3::Polygon::buildBox(glm::vec3(1.f, 0.f, 0.f), glm::vec2(-0.9f, -0.9f), 1.8, 1.8);
	imac3::Polygon circle = imac3::Polygon::buildCircle(glm::vec3(0.f, 1.f, 0.f), glm::vec2(0.f, 0.2f), 0.2, 32);

	std::cout << imac3::Polygon::intersect(glm::vec2(0, 1), glm::vec2(0, -1), glm::vec2(0, 1), glm::vec2(0, -1), NULL, NULL) << std::endl;


    // Temps s'écoulant entre chaque frame
    float dt = 0.f;

	bool done = false;
    while(!done) {
        wm.startMainLoop();

        // Rendu
        renderer.clear();
        particleManager.drawParticles(renderer);
		box.draw(renderer);
		circle.draw(renderer);

        // Simulation
        gravity.apply(particleManager);
		solver.solve(particleManager, dt);

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
	}

	return EXIT_SUCCESS;
}

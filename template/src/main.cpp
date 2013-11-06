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

static const Uint32 WINDOW_WIDTH = 512;
static const Uint32 WINDOW_HEIGHT = 512;


int main() {
    imac3::WindowManager wm(WINDOW_WIDTH, WINDOW_HEIGHT, "Newton was a Geek");
    wm.setFramerate(30);

    imac3::ParticleRenderer2D renderer;

    // Création des particules
    imac3::ParticleManager pm;
    /*pm.addParticle(glm::vec2(-0.3, 0), 1.f, glm::vec2(0.0, 0.0), glm::vec3(1, 1, 1));
    pm.addParticle(glm::vec2(0.3, 0), 1.f, glm::vec2(0.0, 0.0), glm::vec3(1, 1, 1));
    pm.addParticle(glm::vec2(0.0, 0.3), 1.f, glm::vec2(0.0, 0.0), glm::vec3(1, 1, 1));*/
	//pm.addRandomParticles(100);
	
	//Creation d'un graph
	imac3::ParticleGraph graph = createString(glm::vec2(-5.0, 0.0), glm::vec2(5.0, 2.0), glm::vec3(1, 1, 1), 4, pm);

	// Création des forces
	imac3::ConstantForce gravity(glm::vec2(0, -0.01f));
	
	imac3::LeapFrogSolver solver;

	// SANS GRAPHE
	/*
	// Création des polygones
	imac3::Polygon box = imac3::Polygon::buildBox(glm::vec3(1.f, 0.f, 0.f), glm::vec2(-0.9f, -0.9f), 1.8, 1.8, true);
	imac3::Polygon circle = imac3::Polygon::buildCircle(glm::vec3(0.f, 1.f, 0.f), glm::vec2(0.f, 0.0f), 0.2, 4);

	// Création des forces correspondantes
	imac3::PolygonForce boxForce(box, 2, solver);
	boxForce.setDt(0.01f);

	imac3::PolygonForce circleForce(circle, 2, solver);
	circleForce.setDt(0.01f);

	//Systeme stable pour 100 particules : 
	// hooke K = 0.05 / L = 1.0
	// frein cinetique v = 0.01 / dt = 0.6
	imac3::HookForce hookForce(0.05, 1.0);
	imac3::BrakeForce brakeForce(0.01, 0.6);
	*/

	// AVEC GRAPHE
	imac3::GraphHookForce graphHook(0.5f, 1.0f, &graph);
	imac3::GraphBrakeForce graphBrake(0.01f, 0.6f, &graph);


    // Temps s'écoulant entre chaque frame
    float dt = 0.f;

	bool done = false;
	

    while(!done) {
        wm.startMainLoop();

        // Rendu
        renderer.clear();
        pm.drawParticles(renderer);
		
		//box.draw(renderer);
		//circle.draw(renderer);

        // Simulation
        gravity.apply(pm);

        // SANS GRAPHE
		/*
        hookForce.apply(pm);
        brakeForce.apply(pm);

        boxForce.apply(pm);
        //circleForce.apply(pm);

		solver.solve(pm, dt);
		*/

		// AVEC GRAPH
		graphHook.apply(pm);
		graphBrake.apply(pm);

		float KLstep = 0.1;
        // Gestion des evenements
		SDL_Event e;
        while(wm.pollEvent(e)) {
			switch(e.type) {
				default:
					break;
				case SDL_QUIT:
					done = true;
					break;
				/*case SDL_KEYDOWN:
	                if(e.key.keysym.sym == SDLK_UP)
	                    hookForce.setK(hookForce.getK() + KLstep);
	                if(e.key.keysym.sym == SDLK_RIGHT)
	                    hookForce.setL(hookForce.getL() + KLstep);
	                if(e.key.keysym.sym == SDLK_DOWN)
	                    hookForce.setK(hookForce.getK() - KLstep);
	                if(e.key.keysym.sym == SDLK_LEFT)
	                    hookForce.setL(hookForce.getL() - KLstep);
                break;*/
			}
		}

		//std::cout << "\rK : " << hookForce.getK() << " | L = " << hookForce.getL();

        // Mise à jour de la fenêtre
        dt = wm.update();
        //boxForce.setDt(dt);
        //circleForce.setDt(dt);

        //brakeForce.setDt(dt);
        graphBrake.setDt(dt);
	}

	return EXIT_SUCCESS;
}

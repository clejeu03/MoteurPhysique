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

static const Uint32 CIRCLE_SEGMENTS_NUMBER = 20;


int main() {
    imac3::WindowManager wm(WINDOW_WIDTH, WINDOW_HEIGHT, "Newton Pendulum");
    wm.setFramerate(30);

    imac3::ParticleRenderer2D renderer;

    // Création des particules
    imac3::ParticleManager pm;
    
	// Création des forces
	imac3::ConstantForce gravity(glm::vec2(0, -0.01f));

	imac3::LeapFrogSolver solver;

	//Systeme stable pour 100 particules :
    // hooke K = 0.05 / L = 1.0
    // frein cinetique v = 0.01 / dt = 0.6
    //imac3::HookForce hookForce(0.05, 1.0);
    //imac3::BrakeForce brakeForce(0.01, 0.6);

    // Temps s'écoulant entre chaque frame
    float dt = 0.f;
	bool done = false;

	float debit = 1000; // Number of particle generated by second.

    while(!done) {
        wm.startMainLoop();

        pm.createWaterfallParticles(100);

        // Rendu
        renderer.clear();
        pm.drawParticles(renderer);

        // Application des forces
        gravity.apply(pm);
        //hookForce.applyToPendulum(pendulum, pm);
        //hookForce.apply(pm);
        //brakeForce.apply(pm);

        // Solve
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

        // Mise à jour de la fenêtre
        dt = wm.update();

        //brakeForce.setDt(dt);
	}

	return EXIT_SUCCESS;
}

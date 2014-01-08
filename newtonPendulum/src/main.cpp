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
    imac3::WindowManager wm(WINDOW_WIDTH, WINDOW_HEIGHT, "Newton Pendulum");
    wm.setFramerate(30);

    imac3::ParticleRenderer2D renderer;

    // Création des particules
    imac3::ParticleManager pm;

	// Création des forces
	imac3::ConstantForce gravity(glm::vec2(0, -0.01f));

	imac3::LeapFrogSolver solver;
	
	imac3::ParticleGraph circleGraph1 = imac3::createCircleGraph(glm::vec2(-0.6,0), 0.15f, glm::vec3(1.f, 0.f, 0.f), 20, pm); 
	imac3::ParticleGraph circleGraph2 = imac3::createCircleGraph(glm::vec2(-0.3,0), 0.15f, glm::vec3(0.f, 1.f, 0.f), 20, pm); 
	imac3::ParticleGraph circleGraph3 = imac3::createCircleGraph(glm::vec2(0,0), 0.15f, glm::vec3(0.f, 0.f, 1.f), 20, pm); 
	imac3::ParticleGraph circleGraph4 = imac3::createCircleGraph(glm::vec2(0.3,0), 0.15f, glm::vec3(1.f, 1.f, 0.f), 20, pm); 
	imac3::ParticleGraph circleGraph5 = imac3::createCircleGraph(glm::vec2(0.6,0), 0.15f, glm::vec3(1.f, 0.f, 1.f), 20, pm);

	imac3::ParticleGraph string1 = imac3::createStringGraph(glm::vec2(-0.6, 1), glm::vec2(-0.6, 0), glm::vec3(1.f, 1.f, 1.f), pm);
	imac3::ParticleGraph string2 = imac3::createStringGraph(glm::vec2(-0.3, 1), glm::vec2(-0.3, 0), glm::vec3(1.f, 1.f, 1.f), pm);
	imac3::ParticleGraph string3 = imac3::createStringGraph(glm::vec2(0, 1), glm::vec2(0, 0), glm::vec3(1.f, 1.f, 1.f), pm);
	imac3::ParticleGraph string4 = imac3::createStringGraph(glm::vec2(0.3, 1), glm::vec2(0.3, 0), glm::vec3(1.f, 1.f, 1.f), pm);
	imac3::ParticleGraph string5 = imac3::createStringGraph(glm::vec2(0.6, 1), glm::vec2(0.6, 0), glm::vec3(1.f, 1.f, 1.f), pm);
 
	//Systeme stable pour 100 particules :
    // hooke K = 0.05 / L = 1.0
    // frein cinetique v = 0.01 / dt = 0.6
    imac3::HookForce hookForce(0.05, 1.0);
    //imac3::BrakeForce brakeForce(0.01, 0.6);

    // Temps s'écoulant entre chaque frame
    float dt = 0.f;
	bool done = false;
    while(!done) {
        wm.startMainLoop();

        // Rendu
        renderer.clear();
        pm.drawParticles(renderer);

		pm.drawParticleGraph(circleGraph1, renderer);
		pm.drawParticleGraph(circleGraph2, renderer);
		pm.drawParticleGraph(circleGraph3, renderer);
		pm.drawParticleGraph(circleGraph4, renderer);
		pm.drawParticleGraph(circleGraph5, renderer);

		pm.drawParticleGraph(string1, renderer);
		pm.drawParticleGraph(string2, renderer);
		pm.drawParticleGraph(string3, renderer);
		pm.drawParticleGraph(string4, renderer);
		pm.drawParticleGraph(string5, renderer);

        // Application des forces
        gravity.apply(pm);
        hookForce.apply(pm);
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

		//std::cout << "\rK : " << hookForce.getK() << " | L = " << hookForce.getL();

        // Mise à jour de la fenêtre
        dt = wm.update();

        //brakeForce.setDt(dt);
	}

	return EXIT_SUCCESS;
}

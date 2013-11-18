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

#include "FlagGraph.hpp"

#include <vector>

static const Uint32 WINDOW_WIDTH = 512;
static const Uint32 WINDOW_HEIGHT = 512;


int main() {
    imac3::WindowManager wm(WINDOW_WIDTH, WINDOW_HEIGHT, "Newton was a Geek");
    wm.setFramerate(30);

    imac3::ParticleRenderer2D renderer;

    // Création des particules
    imac3::ParticleManager pm;
	
	//Creation d'un graph
	imac3::ParticleGraph graph = createString(glm::vec2(-0.7, 0.3), glm::vec2(0.3, -0.55), glm::vec3(1, 1, 1), 3, pm);
    /*imac3::FlagGraph flag;
    flag.createFlag(glm::vec2(0.f, 0.f), 0.3f, 0.3f, 5, 5, pm);*/

	// Création des forces
	imac3::ConstantForce gravity(glm::vec2(0, -0.01f));

	imac3::LeapFrogSolver solver;
	
	// Création des polygones et forces correspondantes
	imac3::Polygon box = imac3::Polygon::buildBox(glm::vec3(1.f, 0.f, 0.f), glm::vec2(-0.9f, -0.9f), 1.8, 1.8, true);
	imac3::PolygonForce boxForce(box, 2, solver);
	boxForce.setDt(0.01f);

	imac3::Polygon circle = imac3::Polygon::buildCircle(glm::vec3(0.f, 1.f, 0.f), glm::vec2(0.f, 0.0f), 0.2, 4);
	imac3::PolygonForce circleForce(circle, 2, solver);
	circleForce.setDt(0.01f);
	
	// Forces cinétiques
	imac3::GraphHookForce graphHook(0.1f, 0.6f, &graph);
	imac3::GraphBrakeForce graphBrake(0.01f, 0.6f, &graph);

    // Temps s'écoulant entre chaque frame
    float dt = 0.f;

	bool done = false;
	

    while(!done) {
        wm.startMainLoop();

        // Rendu
        renderer.clear();
        pm.drawParticles(renderer);
        //pm.drawParticleGraph(graph, renderer);
        //pm.drawFlag(flag, renderer);
		//box.draw(renderer);
		//circle.draw(renderer);

        // Application des forces
        //gravity.apply(pm);
        //boxForce.apply(pm);
        //circleForce.apply(pm);
		//graphHook.apply(pm);
		//graphBrake.apply(pm);

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
        //boxForce.setDt(dt);
        //circleForce.setDt(dt);
        //graphBrake.setDt(dt);
	}

	return EXIT_SUCCESS;
}

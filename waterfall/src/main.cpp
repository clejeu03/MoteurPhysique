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
    imac3::ParticleManager pm(32);
    
	// Création des forces
	imac3::ConstantForce gravity(glm::vec2(0, -0.01f));

    // Création du solver physique
	imac3::LeapFrogSolver solver;

    // Attributs de la chute
    float debit = 1000;
    float widthWaterfall = 1.6;
    float heightWater = 0.2; // Hauteur d'eau en amont de la chute

    // Création d'une box
    imac3::Polygon box = imac3::Polygon::buildBox(glm::vec3(1, 0, 0), glm::vec2(0, 0), 0.5, 0.2);
    imac3::PolygonForce boxForce(box, 1, debit, widthWaterfall, heightWater, solver);
    boxForce.setDt(0.01f);

    // Création d'un polygon
    imac3::Polygon circle = imac3::Polygon::buildCircle(glm::vec3(1, 0, 0), glm::vec2(-0.3, 0.3), 0.3, 6);
    imac3::PolygonForce circleForce(circle, 1, debit, widthWaterfall, heightWater, solver);
    circleForce.setDt(0.01f);

    imac3::Polygon circle2 = imac3::Polygon::buildCircle(glm::vec3(1, 1, 0), glm::vec2(0.5, -0.5), 0.4, 6);
    imac3::PolygonForce circleForce2(circle2, 1, debit, widthWaterfall, heightWater, solver);
    circleForce2.setDt(0.01f);

    // Temps s'écoulant entre chaque frame
    float dt = 0.f;
	bool done = false;

    while(!done) {
        wm.startMainLoop();

        pm.createWaterfallParticles(100, widthWaterfall, heightWater);

        // Rendu
        renderer.clear();
        pm.drawParticles(renderer);
        //box.draw(renderer);
        circle.draw(renderer);
        circle2.draw(renderer);

        // Application des forces
        gravity.apply(pm);
        //boxForce.apply(pm);
        //boxForce.applyRealPhysicFormula(pm);
        circleForce.apply(pm);
        circleForce2.apply(pm);
        //circleForce.applyRealPhysicFormula(pm);
        //circleForce2.applyRealPhysicFormula(pm);

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

        // Mise à jour des forces
        //boxForce.setDt(dt);
        circleForce.setDt(dt);
        circleForce2.setDt(dt);
	}

	return EXIT_SUCCESS;
}

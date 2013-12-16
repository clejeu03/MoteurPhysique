#include <iostream>
#include <cstdlib>

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "renderer/WindowManager.hpp"
#include "renderer/FlagRenderer3D.hpp"
#include "renderer/TrackballCamera.hpp"

#include "Flag.hpp"

#include "imgui.h"
#include "imguiRenderGL3.h"

#include <vector>
#include <ostream>

static const Uint32 WINDOW_WIDTH = 1024;
static const Uint32 WINDOW_HEIGHT = 768;

using namespace imac3;

int main()
{

    WindowManager wm(WINDOW_WIDTH, WINDOW_HEIGHT, "Newton was a Geek");
    wm.setFramerate(30);

    Flag flag(4096.f, 2, 1.5, 32, 16); // Création d'un drapeau
    glm::vec3 G(0.f, -0.01f, 0.f); // Gravité

    FlagRenderer3D renderer(flag.gridWidth, flag.gridHeight);
    renderer.setProjMatrix(glm::perspective(70.f, float(WINDOW_WIDTH) / WINDOW_HEIGHT, 0.1f, 100.f));

    TrackballCamera camera;
    int mouseLastX, mouseLastY;

    // Temps s'écoulant entre chaque frame
    float dt = 0.f;
    
    if (!imguiRenderGLInit("../fonts/DroidSans.ttf"))
        {
            fprintf(stderr, "Could not init GUI renderer.\n");
            exit(EXIT_FAILURE);
        }

    int scrollarea1 = 0;

	bool done = false;
    bool wireframe = true;

    while(!done)
    {

        wm.startMainLoop();

        // Rendu
        renderer.clear();
        renderer.setViewMatrix(camera.getViewMatrix());
        renderer.drawGrid(flag.positionArray.data(), wireframe);

        glm::vec3 windForce(0.01, glm::linearRand(0.0f, 0.01f), glm::linearRand(0.0f, 0.01f));
        //windForce.rotate

        // Simulation
        if(dt > 0.f)
        {
            flag.applyExternalForce(G); // Applique la gravité
            flag.applyExternalForce(windForce);
            //flag.applyExternalForce(glm::sphericalRand(0.05f)); // Applique un "vent" de direction aléatoire et de force 0.1 Newtons
            flag.applyInternalForces(dt); // Applique les forces internes
            flag.update(dt); // Mise à jour du système à partir des forces appliquées
        }

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
                case SDL_KEYDOWN:
                    switch(e.key.keysym.sym) 
                    {
                        case SDLK_0:

                            break;
                        case SDLK_1:

                            break;
                        case SDLK_2:

                            break;
                        case SDLK_SPACE:
                            wireframe = !wireframe;
                            break;
                        default:
                            break;
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if(e.button.button == SDL_BUTTON_WHEELUP)
                    {
                        camera.moveFront(0.1f);
                    }
                    else if(e.button.button == SDL_BUTTON_WHEELDOWN)
                    {
                        camera.moveFront(-0.1f);
                    }
                    else if(e.button.button == SDL_BUTTON_LEFT)
                    {
                        mouseLastX = e.button.x;
                        mouseLastY = e.button.y;
                    }

			}
		}

        int mouseX, mouseY;
        if(SDL_GetMouseState(&mouseX, &mouseY) & SDL_BUTTON(SDL_BUTTON_LEFT))
        {
            int dX = mouseX - mouseLastX, dY = mouseY - mouseLastY;
            camera.rotateLeft(dX);
            camera.rotateUp(dY);
            mouseLastX = mouseX;
            mouseLastY = mouseY;
        }

        // GUI

        

        glDisable(GL_DEPTH_TEST);

        int scrollarea1 = 0;

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


        imguiBeginFrame(mouseX, mouseY, 0, 0);
            imguiBeginScrollArea("Scroll area", 10, 10, WINDOW_WIDTH / 5, WINDOW_HEIGHT - 20, &scrollarea1);
                imguiButton("Button");
            imguiEndScrollArea();
        imguiEndFrame();

        imguiRenderGLDraw(WINDOW_WIDTH, WINDOW_HEIGHT); 

        glDisable(GL_BLEND);

        // Mise à jour de la fenêtre
        dt = wm.update();
	}

    imguiRenderGLDestroy();

	return EXIT_SUCCESS;
}

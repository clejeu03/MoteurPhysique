#include "Flag.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>


Flag::Flag(float mass, float width, float height, int gridWidth, int gridHeight):
        gridWidth(gridWidth), gridHeight(gridHeight),
        positionArray(gridWidth * gridHeight),
        velocityArray(gridWidth * gridHeight, glm::vec3(0.f)),
        massArray(gridWidth * gridHeight, mass / (gridWidth * gridHeight)),
        forceArray(gridWidth * gridHeight, glm::vec3(0.f))
{
	glm::vec3 origin(-0.5f * width, -0.5f * height, 0.f);
	glm::vec3 scale(width / (gridWidth - 1), height / (gridHeight - 1), 1.f);

	for(int j = 0; j < gridHeight; ++j) {
	    for(int i = 0; i < gridWidth; ++i) {
	        positionArray[i + j * gridWidth] = origin + glm::vec3(i, j, origin.z) * scale;
	    }
	}

	// Les longueurs à vide sont calculés à partir de la position initiale
	// des points sur le drapeau
	L0.x = scale.x;
	L0.y = scale.y;
	L1 = glm::length(L0);
	L2 = 2.f * L0;

	// Ces paramètres sont à fixer pour avoir un système stable: HAVE FUN !
	K0 = 2.0f;
	K1 = 1.0f;
	K2 = 1.0f;

	V0 = 0.1f;
	V1 = 0.1f;
	V2 = 0.1f;
}

// Applique les forces internes sur chaque point du drapeau SAUF les points fixes
void Flag::applyInternalForces(float dt) {
    for (size_t k = 0; k < gridWidth*gridHeight; ++k){
        
        size_t i = k%gridWidth;
        size_t j = k/gridWidth;

        const bool top_border = (j==0)?true:false;
        const bool right_border = ((i+1)%gridWidth == 0)?true:false;
        const bool bottom_border = ((j+1)==gridHeight)?true:false;
        const bool horizontal_penultimate = ((i+2)%gridWidth == 0)?true:false;
        const bool vertical_penultimate = ((j+2)==gridHeight)?true:false;
        const bool horizontal_second = ((i-1)%gridWidth== 0)?true:false;
        const bool vertical_second = ((j-1)%gridHeight== 0)?true:false;

        // On applique les force que si la particule n'est pas fixe
        if(i != 0)
        {
            // GRID FORCES (cf topology 0)

            // Horizontal
            // Liaison avec le voisin de gauche
            forceArray[k] += hookForce(K0, L0.x, positionArray[k], positionArray[k-1]);
            forceArray[k] += brakeForce(V0, dt, velocityArray[k], velocityArray[k-1]);
            // Liaison avec le voisin de droite uniquement si la particule n'est pas à l'extremité du drapeau
            if(!right_border)
            {
                forceArray[k] += hookForce(K0, L0.x, positionArray[k], positionArray[k+1]);
                forceArray[k] += brakeForce(V0, dt, velocityArray[k], velocityArray[k+1]);
            }

            // Vertical
            // Si c'est une particule en haut, on fait une liaison avec son voisin du bas uniquement
            if(top_border)
            {
                forceArray[k] += hookForce(K0, L0.y, positionArray[k], positionArray[k+gridWidth]);
                forceArray[k] += brakeForce(V0, dt, velocityArray[k], velocityArray[k+gridWidth]);
            }
            else if(bottom_border) // Sinon, si c'est une particule en bas, on fait une liaison avec son voisin du haut uniquement
            {
                forceArray[k] += hookForce(K0, L0.y, positionArray[k], positionArray[k-gridWidth]);
                forceArray[k] += brakeForce(V0, dt, velocityArray[k], velocityArray[k-gridWidth]);
            }
            else // Sinon c'est une particule au milieu donc on fait une liaison avec le voisin du haut et le voisin du bas
            {
                forceArray[k] += hookForce(K0, L0.y, positionArray[k], positionArray[k-gridWidth]);
                forceArray[k] += brakeForce(V0, dt, velocityArray[k], velocityArray[k-gridWidth]);
                forceArray[k] += hookForce(K0, L0.y, positionArray[k], positionArray[k+gridWidth]);
                forceArray[k] += brakeForce(V0, dt, velocityArray[k], velocityArray[k+gridWidth]); 
            }

            // DIAGONAL FORCES (cf topology 1)

            if(top_border) // Si c'est une particule de la première ligne
            {
                // Liaison avec le voisin en bas-gauche
                forceArray[k] += hookForce(K1, L1, positionArray[k], positionArray[k+gridWidth-1]);
                forceArray[k] += brakeForce(V1, dt, velocityArray[k], velocityArray[k+gridWidth-1]);
                // Liaison avec le voisin bas-droite uniquement si la particule n'est pas à l'extremité du drapeau
                if(!right_border)
                {
                    forceArray[k] += hookForce(K1, L1, positionArray[k], positionArray[k+gridWidth+1]);
                    forceArray[k] += brakeForce(V1, dt, velocityArray[k], velocityArray[k+gridWidth+1]);
                }
            }
            else if(bottom_border) // Sinon si c'est une particule de la dernière ligne
            {
                // Liaison avec le voisin en haut-gauche
                forceArray[k] += hookForce(K1, L1, positionArray[k], positionArray[k-gridWidth-1]);
                forceArray[k] += brakeForce(V1, dt, velocityArray[k], velocityArray[k-gridWidth-1]);
                // Liaison avec le voisin haut-droite uniquement si la particule n'est pas à l'extremité du drapeau
                if(!right_border)
                {
                    forceArray[k] += hookForce(K1, L1, positionArray[k], positionArray[k-gridWidth+1]);
                    forceArray[k] += brakeForce(V1, dt, velocityArray[k], velocityArray[k-gridWidth+1]);
                }
            }
            else // Sinon c'est une particule au milieu donc on fait une liaison avec les voisins haut-gauche bas-gauche
            {
                forceArray[k] += hookForce(K1, L1, positionArray[k], positionArray[k-gridWidth-1]);
                forceArray[k] += brakeForce(V1, dt, velocityArray[k], velocityArray[k-gridWidth-1]);
                forceArray[k] += hookForce(K1, L1, positionArray[k], positionArray[k+gridWidth-1]);
                forceArray[k] += brakeForce(V1, dt, velocityArray[k], velocityArray[k+gridWidth-1]);
                if(!right_border) // Et haut-droite bas-droite si ce n'est pas un point à l'extremité du drapeau
                {
                    forceArray[k] += hookForce(K1, L1, positionArray[k], positionArray[k-gridWidth+1]);
                    forceArray[k] += brakeForce(V1, dt, velocityArray[k], velocityArray[k-gridWidth+1]);
                    forceArray[k] += hookForce(K1, L1, positionArray[k], positionArray[k+gridWidth+1]);
                    forceArray[k] += brakeForce(V1, dt, velocityArray[k], velocityArray[k+gridWidth+1]);
                }
            }

            // SHEAR FORCES (cf topology 2)

            if(top_border || vertical_second)
            {
                forceArray[k] += hookForce(K2, L2.y, positionArray[k], positionArray[k+2*gridWidth]);
                forceArray[k] += brakeForce(V2, dt, velocityArray[k], velocityArray[k+2*gridWidth]);
            }
            else if(bottom_border || vertical_penultimate)
            {
                forceArray[k] += hookForce(K2, L2.y, positionArray[k], positionArray[k-2*gridWidth]);
                forceArray[k] += brakeForce(V2, dt, velocityArray[k], velocityArray[k-2*gridWidth]);
            }
            else
            {
                forceArray[k] += hookForce(K2, L2.y, positionArray[k], positionArray[k+2*gridWidth]);
                forceArray[k] += brakeForce(V2, dt, velocityArray[k], velocityArray[k+2*gridWidth]);
                forceArray[k] += hookForce(K2, L2.y, positionArray[k], positionArray[k-2*gridWidth]);
                forceArray[k] += brakeForce(V2, dt, velocityArray[k], velocityArray[k-2*gridWidth]);
            }

            if(horizontal_second)
            {
                forceArray[k] += hookForce(K2, L2.x, positionArray[k], positionArray[k+2]);
                forceArray[k] += brakeForce(V2, dt, velocityArray[k], velocityArray[k+2]);
            }
            else if(right_border || horizontal_penultimate)
            {
                forceArray[k] += hookForce(K2, L2.x, positionArray[k], positionArray[k-2]);
                forceArray[k] += brakeForce(V2, dt, velocityArray[k], velocityArray[k-2]);
            }
            else
            {
                forceArray[k] += hookForce(K2, L2.x, positionArray[k], positionArray[k+2]);
                forceArray[k] += brakeForce(V2, dt, velocityArray[k], velocityArray[k+2]);
                forceArray[k] += hookForce(K2, L2.x, positionArray[k], positionArray[k-2]);
                forceArray[k] += brakeForce(V2, dt, velocityArray[k], velocityArray[k-2]);
            }
        }
    }
}

// Applique une force externe sur chaque point du drapeau SAUF les points fixes
void Flag::applyExternalForce(const glm::vec3& F) {
    for (size_t k = 0; k < gridWidth*gridHeight; ++k){
        // On applique la force que si la particule n'est pas fixe
        if(k%gridWidth != 0)
        {
            forceArray[k] += F;
        }
    }
}

void Flag::update(float dt) {
    for (size_t k = 0; k < gridWidth*gridHeight; ++k){
        glm::vec3 newSpeed = velocityArray[k] + dt * forceArray[k]/massArray[k];
        glm::vec3 newPos = positionArray[k] + dt * newSpeed;
        velocityArray[k] = newSpeed;
        positionArray[k] = newPos;
        forceArray[k] = glm::vec3(0, 0, 0);
    }
}

// Calcule une force de type ressort de Hook entre deux particules de positions P1 et P2
// K est la résistance du ressort et L sa longueur à vide
inline glm::vec3 hookForce(float K, float L, const glm::vec3& P1, const glm::vec3& P2) {
    static const float epsilon = 0.0001;
    glm::vec3 P1P2 = P2 - P1;
    float distP1P2 = glm::length(P1P2);
    return K * (1 - ( L / glm::max( distP1P2 , epsilon ))) * P1P2;
}

// Calcule une force de type frein cinétique entre deux particules de vélocités v1 et v2
// V est le paramètre du frein et dt le pas temporel
inline glm::vec3 brakeForce(float V, float dt, const glm::vec3& V1, const glm::vec3& V2) {
    if(dt > 0)
    {
        return V * ((V2-V1) / dt);
    }
}

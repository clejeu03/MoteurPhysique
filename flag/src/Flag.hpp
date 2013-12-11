#ifndef _H_FLAG_H_
#define _H_FLAG_H_

#include <vector>
#include <glm/glm.hpp>

// Structure permettant de simuler un drapeau à l'aide un système masse-ressort
struct Flag {

    int gridWidth, gridHeight; // Dimensions de la grille de points

    // Propriétés physique des points:
    std::vector<glm::vec3> positionArray;
    std::vector<glm::vec3> velocityArray;
    std::vector<float> massArray;
    std::vector<glm::vec3> forceArray;

    // Paramètres des forces interne de simulation
    // Longueurs à vide
    glm::vec2 L0;
    float L1;
    glm::vec2 L2;

    float K0, K1, K2; // Paramètres de résistance
    float V0, V1, V2; // Paramètres de frein

	// Getters & setters
    inline void setK0(float k) { K0 = k; }
    inline float getK0() const { return K0; }

    const void setK1(float k) { K1 = k; }
    const float getK1() const { return K1; }

    const void setK2(float k) { K2 = k; }
    const float getK2() const { return K2; }

    const void setV0(float v) { V0 = v; }
    const float getV0() const { return V0; }

    const void setV1(float v) { V1 = v; }
    const float getV1() const { return V1; }

    const void setV2(float v) { V2 = v; }
    const float getV2() const { return V2; }

    const void setL0(glm::vec2 l) { L0 = l; }
    const glm::vec2 getL0() const { return L0; }

    const void setL1(float l) { L1 = l; }
    const float getL1() const { return L1; }

    const void setL2(glm::vec2 l) { L2 = l; }
    const glm::vec2 getL2() const { return L2; }
    

    // Créé un drapeau discretisé sous la forme d'une grille contenant gridWidth * gridHeight
    // points. Chaque point a pour masse mass / (gridWidth * gridHeight).
    // La taille du drapeau en 3D est spécifié par les paramètres width et height
    Flag(float mass, float width, float height, int gridWidth, int gridHeight);

    // Applique les forces internes sur chaque point du drapeau SAUF les points fixes
    void applyInternalForces(float dt);

    // Applique une force externe sur chaque point du drapeau SAUF les points fixes
    void applyExternalForce(const glm::vec3& F);

    // Met à jour la vitesse et la position de chaque point du drapeau
    // en utilisant un schema de type Leapfrog
    void update(float dt);
};

// Calcule une force de type ressort de Hook entre deux particules de positions P1 et P2
// K est la résistance du ressort et L sa longueur à vide
inline glm::vec3 hookForce(float K, float L, const glm::vec3& P1, const glm::vec3& P2);

// Calcule une force de type frein cinétique entre deux particules de vélocités v1 et v2
// V est le paramètre du frein et dt le pas temporel
inline glm::vec3 brakeForce(float V, float dt, const glm::vec3& V1, const glm::vec3& V2);

#endif
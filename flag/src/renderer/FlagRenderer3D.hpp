#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

namespace imac3 {

class FlagRenderer3D {
	struct Vertex {
		glm::vec3 position;
		glm::vec3 normal;
	};
public:
	FlagRenderer3D(int gridWidth, int gridHeight);

    ~FlagRenderer3D();

    FlagRenderer3D(const FlagRenderer3D&) = delete;

    FlagRenderer3D& operator =(const FlagRenderer3D&) = delete;

	void clear();

	void drawGrid(const glm::vec3* positionArray, bool wireframe);

	void setProjMatrix(glm::mat4 P) {
		m_ProjMatrix = P;
	}

	void setViewMatrix(glm::mat4 V) {
		m_ViewMatrix = V;
	}

private:
	static const GLchar *VERTEX_SHADER, *FRAGMENT_SHADER;

    // Ressources OpenGL
    GLuint m_ProgramID;
    GLuint m_VBOID, m_VAOID, m_IBOID;

    GLint m_uMVPMatrix, m_uMVMatrix;

    glm::mat4 m_ProjMatrix;
    glm::mat4 m_ViewMatrix;

    int m_nGridWidth, m_nGridHeight;
    uint32_t m_nIndexCount;

    std::vector<Vertex> m_VertexBuffer;
};

}
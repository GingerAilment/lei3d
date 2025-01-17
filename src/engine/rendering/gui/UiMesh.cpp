#include "UiMesh.hpp"

#include "fontrenderer/TGATexture.hpp"
#include "glad/glad.h"
#include "rendering/Shader.hpp"
#include "logging/GLDebug.hpp"

namespace lei3d
{
	UiMesh::UiMesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, const TGATexture& texture)
		: m_vertices(vertices)
		, m_indices(indices)
		, m_texture(texture)
	{
		SetupBuffers();
	}

	UiMesh::~UiMesh()
	{
		GLCall(glDeleteVertexArrays(1, &VAO));
		GLCall(glDeleteBuffers(1, &VBO));
	}

	void UiMesh::Draw() const
	{
		GLCall(glActiveTexture(GL_TEXTURE0));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_texture.m_GlID));

		GLCall(glBindVertexArray(VAO));
		GLCall(glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0));

		GLCall(glBindVertexArray(0));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	}

	void UiMesh::Draw(Shader& shader) const
	{
		shader.bind();

		Draw();
	}

	void UiMesh::SetupBuffers()
	{
		GLCall(glGenVertexArrays(1, &VAO));
		GLCall(glGenBuffers(1, &VBO));
		GLCall(glGenBuffers(1, &EBO));

		GLCall(glBindVertexArray(VAO));

		GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
		GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_vertices.size(), m_vertices.data(), GL_STATIC_DRAW));

		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_indices.size(), m_indices.data(), GL_STATIC_DRAW));

		GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(Vertex), (void*)0));
		GLCall(glEnableVertexAttribArray(0));

		GLCall(glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(Vertex), (void*)(2 * sizeof(float))));
		GLCall(glEnableVertexAttribArray(1));

		GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vertex), (void*)(6 * sizeof(float))));
		GLCall(glEnableVertexAttribArray(2));

		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
		GLCall(glBindVertexArray(0));
	}

}

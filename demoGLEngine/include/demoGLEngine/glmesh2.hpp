#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <gl/GL.h>
#include <string>
#include <vector>


class glMesh2 {
public:
    glMesh2();
    glMesh2(const std::vector<float>& vertices, const std::vector<float>& normals, const std::vector<float>& texCoords, const std::vector<unsigned int>& indices, GLenum _usage = GL_STATIC_DRAW);
    glMesh2(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, GLenum _usage = GL_STATIC_DRAW);
    ~glMesh2();

    //Delete the copy constructor/assignment.
    glMesh2(const glMesh2&) = delete;
    glMesh2& operator=(const glMesh2&) = delete;

    glMesh2(glMesh2&& other) noexcept;
    glMesh2& operator=(glMesh2&& other) noexcept;

    void bind() const;
    inline GLuint getIndicesCount() const { return indicesCount; }
    inline GLuint getVerticesCount() const { return verticesCount; }
    inline void render() const { glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0); }

private:
    glMesh2(unsigned int _verticesCount, unsigned int _indicesCount, GLenum _usage);

    void release();

    GLuint meshVAO;
    GLuint meshVBO;
    GLuint positionVBO;
    GLuint meshEBO;
    GLuint texCoordVBO;
    GLuint normalVBO;

    GLenum usage;

    GLuint indicesCount;
    GLuint verticesCount;
};
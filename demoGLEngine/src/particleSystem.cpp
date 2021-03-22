#include "demoGLEngine/particleSystem.hpp"

ParticleSystem::ParticleSystem(glm::vec3 _pos, const Shader& _shader) : pos(_pos), particle_count(0), last_used(0), shader(_shader) {
    particles = std::make_unique<Particle[]>(max_particles);

    this->init();

    g_particule_position_size_data = new GLfloat[max_particles * 4];
    g_particule_color_data = new GLubyte[max_particles * 4];
}

ParticleSystem::~ParticleSystem() {

}

void ParticleSystem::init() {

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);


    static const GLfloat g_vertex_buffer_data[] = {
     -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     -0.5f, 0.5f, 0.0f,
     0.5f, 0.5f, 0.0f,
    };

    glGenBuffers(1, &billboard_vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    glGenBuffers(1, &particles_position_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
    glBufferData(GL_ARRAY_BUFFER, max_particles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

    glGenBuffers(1, &particles_color_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
    glBufferData(GL_ARRAY_BUFFER, max_particles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
    glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void*)0);

    glVertexAttribDivisor(0, 0);
    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);
}

std::size_t ParticleSystem::findUnused() {
    for (std::size_t i = last_used; i < max_particles; i++) {
        if (particles[i].life <= 0) {
            last_used = i;
            return i;
        }
    }

    for (std::size_t i = 0; i < last_used; i++) {
        if (particles[i].life <= 0) {
            last_used = i;
            return i;
        }
    }

    return 0;
}

void ParticleSystem::sort() {
    std::sort(&particles[0], &particles[max_particles]);
}

void ParticleSystem::simulate(const double& delta) {
    int newparticles = (int)(delta * 10000.0);
    if (newparticles > (int)(0.016f * 10000.0))
        newparticles = (int)(0.016f * 10000.0);

    for (int i = 0; i < newparticles; i++) {
        int particleIndex = findUnused();
        particles[particleIndex].life = Utils::random(0, 5);
        particles[particleIndex].pos = pos;

        float spread = Utils::random(1, 2); //  1.5f 
        glm::vec3 maindir = glm::vec3(0.0f, 10.0f, 0.0f);
        glm::vec3 randomdir = glm::vec3(
            (rand() % 2000 - 1000.0f) / 1000.0f,
            (rand() % 2000 - 1000.0f) / 1000.0f,
            (rand() % 2000 - 1000.0f) / 1000.0f
        );

        particles[particleIndex].speed = maindir + randomdir * spread;


        // Very bad way to generate a random color
        particles[particleIndex].r = rand() % 256;
        particles[particleIndex].g = rand() % 256;
        particles[particleIndex].b = rand() % 256;
        particles[particleIndex].a = (rand() % 256) / 3;

        particles[particleIndex].size = (rand() % 1000) / 2000.0f + 0.1f;

    }

    particle_count = 0;
    for (std::size_t i = 0; i < max_particles; i++) {

        Particle& p = particles[i];

        if (p.life > 0.0f) {

            p.life -= delta;
            if (p.life > 0.0f) {

                p.speed += glm::vec3(0.0f, -9.81f, 0.0f) * (float)delta * 0.5f;
                p.pos += p.speed * (float)delta;
                p.cameraDistance = 1; // A MODIF

                g_particule_position_size_data[4 * particle_count + 0] = p.pos.x;
                g_particule_position_size_data[4 * particle_count + 1] = p.pos.y;
                g_particule_position_size_data[4 * particle_count + 2] = p.pos.z;

                g_particule_position_size_data[4 * particle_count + 3] = p.size;

                g_particule_color_data[4 * particle_count + 0] = p.r;
                g_particule_color_data[4 * particle_count + 1] = p.g;
                g_particule_color_data[4 * particle_count + 2] = p.b;
                g_particule_color_data[4 * particle_count + 3] = p.a;

            }
            else {
                p.cameraDistance = -1.0f;
            }

            particle_count++;

        }
    }

    sort();
}

void ParticleSystem::draw(glm::mat4 ViewMatrix, glm::mat4  ProjectionMatrix) {

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shader.bind();
    shader.setInt("textureParticle", 6);

    glm::mat4 ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
    shader.setVec3("CameraRight_worldspace", ViewMatrix[0][0], ViewMatrix[1][0], ViewMatrix[2][0]);
    shader.setVec3("CameraUp_worldspace", ViewMatrix[0][1], ViewMatrix[1][1], ViewMatrix[2][1]);
    shader.setMat4("VP", ViewProjectionMatrix);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
    glBufferData(GL_ARRAY_BUFFER, max_particles * 4 * sizeof(GLfloat), g_particule_position_size_data, GL_STREAM_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
    glBufferData(GL_ARRAY_BUFFER, max_particles * 4 * sizeof(GLubyte), g_particule_color_data, GL_STREAM_DRAW);

    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particle_count);

    //glDisable(GL_BLEND);
}
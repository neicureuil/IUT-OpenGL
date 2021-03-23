#include "demoGLEngine/particleSystem.hpp"

ParticleSystem::ParticleSystem(glm::vec3 _pos, const Shader& _shader, int _texId) : pos(_pos), particle_count(0), last_used(0), shader(_shader), texId(_texId) {
    particles = std::make_unique<Particle[]>(max_particles);

    g_particule_position_size_data = new GLfloat[max_particles * 4];
    g_particule_life_data = new GLfloat[max_particles * 2];

    init();
}

ParticleSystem::~ParticleSystem() {

}

void ParticleSystem::init() {

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Vertex du billboard de la particule
    static const GLfloat g_vertex_buffer_data[] = {
     -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     -0.5f, 0.5f, 0.0f,
     0.5f, 0.5f, 0.0f,
    };

    // Generation du buffer des vertex
    glGenBuffers(1, &billboard_vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    // Generation du buffer de position (x, y, z)
    glGenBuffers(1, &particles_position_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
    glBufferData(GL_ARRAY_BUFFER, max_particles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

    // Generation du buffer de vie (life, maxLife)
    glGenBuffers(1, &particles_life_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, particles_life_buffer);
    glBufferData(GL_ARRAY_BUFFER, max_particles * 2 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
    

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, particles_life_buffer);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

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

void ParticleSystem::simulate(const double& delta, const glm::vec3& camPos) {
    // Generations de nouvelles particules
    int newparticles = (int)(delta * 10000.0);
    for (int i = 0; i < newparticles; i++) {
        int particleIndex = findUnused();
        particles[particleIndex].life = Utils::random(0, 3);
        particles[particleIndex].maxLife = particles[particleIndex].life;
        particles[particleIndex].pos = pos;
        particles[particleIndex].cameraDistance = -1;

        float spread = 1.5f; 
        glm::vec3 maindir = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 randomdir = glm::vec3(
            (rand() % 2000 - 1000.0f) / 1000.0f,
            (rand() % 2000 - 1000.0f) / 1000.0f,
            (rand() % 2000 - 1000.0f) / 1000.0f
        );

        particles[particleIndex].speed = maindir + randomdir * spread;
        particles[particleIndex].size = (rand() % 1000) / 2000.0f + 0.1f;
    }

    // Simulation des particules
    particle_count = 0;
    for (std::size_t i = 0; i < max_particles; i++) {

        Particle& p = particles[i];

        if (p.life > 0.0f) {

            p.life -= delta;
            if (p.life > 0.0f) {

                p.speed += glm::vec3(0.0f, -9.81f, 0.0f) * (float)delta * 0.5f;
                p.pos += p.speed * (float)delta;
                p.cameraDistance = glm::length2(p.pos - camPos);

                // Update du buffer de position
                g_particule_position_size_data[4 * particle_count + 0] = p.pos.x;
                g_particule_position_size_data[4 * particle_count + 1] = p.pos.y;
                g_particule_position_size_data[4 * particle_count + 2] = p.pos.z;
                g_particule_position_size_data[4 * particle_count + 3] = p.size;

                // Update du buffer de vie/vieMax
                g_particule_life_data[2 * particle_count + 0] = (1.0f-p.life);
                g_particule_life_data[2 * particle_count + 1] = p.maxLife;

            }
            else {
                p.cameraDistance = -1.0f;
            }

            particle_count++;

        }
    }

    sort();
}

void ParticleSystem::render(glm::mat4 ViewMatrix, glm::mat4  ProjectionMatrix) {

    // Blend des particules
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    // Bind du shader des particules et definitions des valeurs
    shader.bind();
    shader.setInt("textureParticle", texId);

    glm::mat4 ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
    shader.setVec3("CameraRight_worldspace", ViewMatrix[0][0], ViewMatrix[1][0], ViewMatrix[2][0]);
    shader.setVec3("CameraUp_worldspace", ViewMatrix[0][1], ViewMatrix[1][1], ViewMatrix[2][1]);
    shader.setMat4("VP", ViewProjectionMatrix);


    // Mise a jours des buffers
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
    glBufferData(GL_ARRAY_BUFFER, max_particles * 4 * sizeof(GLfloat), g_particule_position_size_data, GL_STREAM_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, particles_life_buffer);
    glBufferData(GL_ARRAY_BUFFER, max_particles * 2 * sizeof(GLfloat), g_particule_life_data, GL_STREAM_DRAW);

    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particle_count);

    // Remise du blend a l'etat d'origin
    //glDisable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
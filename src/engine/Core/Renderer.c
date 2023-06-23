//
// Created by vd on 28.12.22.
//

#include "Renderer.h"

#include <GL/glew.h>
#include <stdlib.h>

#include "../Utils/Mat.h"
#include "ShaderProgram.h"
#include "../GUI/Label.h"

#include <string.h>


void prepareRenderer(Renderer* renderer, ShaderProgram* program, Camera* camera){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.52f, 0.52f, 0.51f, 1);
    useProgram(program);
    loadMatrix(program, renderer->projection, "Projection");
    loadMatrix(program, camera->view, "View");
    stopProgram(program);
}

void renderTexture(Mesh* mesh, ShaderProgram* shaderProgram){
    GLuint sampler = glGetUniformLocation(shaderProgram->programId, "myTextureSampler");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mesh->textureID);
    glUniform1i((GLint)sampler, 0);
}

void renderMesh(Renderer* renderer, Mesh* mesh, ShaderProgram* shaderProgram){
    glBindVertexArray(mesh->vao);
    if(mesh->uv != -1 && mesh->textureID != -1) renderTexture(mesh, shaderProgram);
    glEnableVertexAttribArray(0);
    if(mesh->ebo != -1)
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
    glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void*) 0
            );
    if(mesh->cbo != -1){
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, mesh->cbo);
        glVertexAttribPointer(
                1,
                3,
                GL_FLOAT,
                GL_FALSE,
                0,
                (void*) 0
                );
    }else if(mesh->uv != -1 && mesh->textureID != -1){
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, mesh->uv);
        glVertexAttribPointer(
                1,
                2,
                GL_FLOAT,
                GL_FALSE,
                0,
                (void*) 0
                );
    }

    if(mesh->nbo != -1){
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, mesh->nbo);
        glVertexAttribPointer(
                2,
                3,
                GL_FLOAT,
                GL_FALSE,
                0,
                (void*) 0
                );
    }

    if(mesh->ebo != -1){
        glDrawElements(GL_TRIANGLES, (GLsizei)(mesh->dataSize / sizeof(GLuint)), GL_UNSIGNED_INT, NULL);
    }else{
        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)(mesh->dataSize / sizeof(GLfloat)));
    }

    glDisableVertexAttribArray(0);
    if(mesh->cbo != -1 || (mesh->uv != -1 && mesh->textureID != -1)) glDisableVertexAttribArray(1);
    if(mesh->nbo != -1) glDisableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    if(mesh->ebo != -1) glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

void renderEntity(Renderer* renderer, Entity* entity, ShaderProgram* program, Light* light){

    loadMatrix(program, entity->mvp, "Model");

    if(light != NULL){
        loadVec3(program, light->position, "lightPosition");
        loadVec3(program, light->colour, "lightColor");

        loadFloat(program, entity->mesh->reflectivity, "reflectivity");
        loadFloat(program, entity->mesh->shineDamper, "shineDamper");
    }

    renderMesh(renderer, entity->mesh, program);
}

void prepareEntity(Entity* entity, ShaderProgram* program){
    loadFloat(program, entity->mesh->reflectivity, "reflectivity");
    loadFloat(program, entity->mesh->shineDamper, "shineDamper");

    Mesh* mesh = entity->mesh;


    glBindVertexArray(mesh->vao);
    if(mesh->uv != -1 && mesh->textureID != -1) renderTexture(mesh, program);
    glEnableVertexAttribArray(0);
    if(mesh->ebo != -1)
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
    glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void*) 0
    );
    if(mesh->cbo != -1){
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, mesh->cbo);
        glVertexAttribPointer(
                1,
                3,
                GL_FLOAT,
                GL_FALSE,
                0,
                (void*) 0
        );
    }else if(mesh->uv != -1 && mesh->textureID != -1){
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, mesh->uv);
        glVertexAttribPointer(
                1,
                2,
                GL_FLOAT,
                GL_FALSE,
                0,
                (void*) 0
        );
    }

    if(mesh->nbo != -1){
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, mesh->nbo);
        glVertexAttribPointer(
                2,
                3,
                GL_FLOAT,
                GL_FALSE,
                0,
                (void*) 0
        );
    }
}

void prepareInstance(Entity* entity, ShaderProgram* program){
    loadMatrix(program, entity->mvp, "Model");
}

void unbindEntity(Entity* entity){
    Mesh* mesh = entity->mesh;

    glDisableVertexAttribArray(0);
    if(mesh->cbo != -1 || (mesh->uv != -1 && mesh->textureID != -1)) glDisableVertexAttribArray(1);
    if(mesh->nbo != -1) glDisableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    if(mesh->ebo != -1) glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void renderEntities(Renderer* renderer, Entity** entities, ShaderProgram* program, size_t count){
    prepareEntity(entities[0], program);

    for(size_t i = 0; i < count; i++){
        Entity* en = entities[i];
        prepareInstance(en, program);
        if(en->mesh->ebo != -1)
            glDrawElements(GL_TRIANGLES, (GLsizei)(en->mesh->dataSize / sizeof(GLuint)), GL_UNSIGNED_INT, NULL);
        else
            glDrawArrays(GL_TRIANGLES, 0, (GLsizei)(en->mesh->dataSize / sizeof(GLfloat)));

    }

    unbindEntity(entities[0]);
}

Renderer* newRenderer(float fov, float height, float width, float near, float far){
    Renderer* renderer = (Renderer*) malloc(sizeof(Renderer));
    renderer->projection = createPerspectiveMatrix(fov, width, height, near, far);
    renderer->near = near;
    renderer->height = height;
    renderer->width = width;
    renderer->fov = fov;
    renderer->far = far;
    renderer->ortho = createOrthoMatrix(0.0, width, 0.0, height, near, far);

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    renderer->textVAO = VAO;
    renderer->textVBO = VBO;

    printf("Renderer vao %d vbo %d\n", VAO, VBO);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    return renderer;
}

void renderText(ShaderProgram* program, char* text, float x, float y, float scale, vec3 color, Renderer* renderer, Character** charset){
    loadMatrix(program, renderer->ortho, "projection");
    loadVec3(program, color, "textColor");
    
    glBindVertexArray(renderer->textVAO);
    glActiveTexture(GL_TEXTURE0);

    for(int i = 0; i < strlen(text); i++){
        Character* ch = charset[text[i]];

        float xpos = x + ((float)ch->bearing_x) * scale;
        float ypos = y - ((float)(ch->size_y - ch->bearing_y)) * scale;

        float w = ((float)ch->size_x) * scale;
        float h = ((float)ch->size_y) * scale;

        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },            
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f } 
        };

        glBindTexture(GL_TEXTURE_2D, ch->textureID);
        glBindBuffer(GL_ARRAY_BUFFER, renderer->textVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += (ch->advance >> 6) * scale;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void updateProjection(Renderer* renderer){
    renderer->projection = createPerspectiveMatrix(
            renderer->fov,
            renderer->width,
            renderer->height,
            renderer->near,
            renderer->far
            );
}

void rendererCleanup(Renderer* renderer){
    freeMatrixVector(renderer->projection);
    freeMatrixVector(renderer->ortho);
    free(renderer);
}
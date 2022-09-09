
#pragma once

// glm
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


// glm
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

// project
#include "opengl.hpp"
#include "cgra/cgra_mesh.hpp"
#include "bounding_box.hpp"


// Basic model that holds the shader, mesh and transform for drawing.
// Can be copied and/or modified for adding in extra information for drawing
// including colors for diffuse/specular, and textures for texture mapping etc.
struct basic_model {
	GLuint shader = 0;
	cgra::gl_mesh mesh;
	glm::vec3 color{1, 0, 0};
	glm::vec3 lightcolor{ 1, 1, 1};
	glm::vec3 lightdirection{ 1, 1, 1 };
	glm::vec3 specularcolor{ 1, 1, 1 };
	glm::vec3 extraattributes{ 1, 0, 0.1 };
	glm::mat4 modelTransform{1.0};
	bool useTexture = false;
	glm::vec3 meshTranslationOffset{ 1, 1, 1 };


	void draw(const glm::mat4 &view, const glm::mat4 proj) {
		using namespace glm;

		// cacluate the modelview transform
		mat4 modelview = view * modelTransform;

		// load shader and variables
		glUseProgram(shader);
		glUniformMatrix4fv(glGetUniformLocation(shader, "uProjectionMatrix"), 1, false, value_ptr(proj));
		glUniformMatrix4fv(glGetUniformLocation(shader, "uModelViewMatrix"), 1, false, value_ptr(modelview));
		glUniform3fv(glGetUniformLocation(shader, "uColor"), 1, value_ptr(color));
		glUniform3fv(glGetUniformLocation(shader, "uLightColour"), 1, value_ptr(lightcolor));
		glUniform3fv(glGetUniformLocation(shader, "uLightDirection"), 1, value_ptr(view*vec4(0.0,0.0,0.0,1.0)));
		glUniform3fv(glGetUniformLocation(shader, "uSpecularColour"), 1, value_ptr(specularcolor));
		glUniform3fv(glGetUniformLocation(shader, "uExtraAttributes"), 1, value_ptr(extraattributes));
		glUniform1i(glGetUniformLocation(shader, "uTextured"), useTexture);

		GLfloat transforms[1600];
		GLfloat colours[300];
		float negative = 1.0;
		for (int i = 0.0; i < 10; i++) {
			for (int j = 0.0; j < 10; j++) {

				mat4 transformation = mat4(1);

				vec3 colour = vec3(1.0 - (i/10.0), 1.0 - (j / 10.0), 1.0 - (i + j / 10.0));

				if (i + j == 0) {
					colour = color;
				}

				colours[(i * 10 + j) * 3] = colour.r;
				colours[(i * 10 + j) * 3 + 1] = colour.g;
				colours[(i * 10 + j) * 3 + 2] = colour.b;

				transformation = scale(transformation, vec3(i * j * 0.5 + negative * (1)));
				transformation = translate(transformation, meshTranslationOffset * vec3(negative * i/2 * 20.0f, negative * j * 6.0f, negative * i + j * 8.0f));
				transformation = rotate(transformation, (float)(i * j) , vec3(negative * 0.5 * (i) + 0.5 , negative * 0.5 * (j) + 0.5, 0.3 * i + j  + 0.5));
				negative = negative * -1;
				
				for (int k = 0; k < 4; k++) {
					for (int l = 0; l < 4; l++) {
						transforms[((i * 10 + j) * 16) + (k*4) + l] = transformation[k][l];
					}
				}
			}
		}

		unsigned int buffer;
		glGenBuffers(1,&buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * 100, transforms, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glEnableVertexAttribArray(4);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(vec4), (void*)0);
		glVertexAttribDivisor(4, 1);
		glEnableVertexAttribArray(5);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(vec4), (void*)sizeof(vec4));
		glVertexAttribDivisor(5, 1);
		glEnableVertexAttribArray(6);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(vec4), (void*)(2*sizeof(vec4)));
		glVertexAttribDivisor(6, 1);
		glEnableVertexAttribArray(7);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(vec4), (void*)(3 * sizeof(vec4)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glVertexAttribDivisor(7, 1);

		unsigned int bufferCol;
		glGenBuffers(1, &bufferCol);
		glBindBuffer(GL_ARRAY_BUFFER, bufferCol);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * 100, colours, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glEnableVertexAttribArray(3);
		glBindBuffer(GL_ARRAY_BUFFER, bufferCol);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 1 * sizeof(vec3), (void*)(0));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glVertexAttribDivisor(3, 1);


	//	mesh.createBoundingBox();

		// draw the mesh
		//mesh.draw();
		mesh.drawInstances(); 

	}
};
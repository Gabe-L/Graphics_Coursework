// Below ifdef required to remove warnings for unsafe version of fopen and fscanf.
// Secure version won't work cross-platform, forcing this small hack.
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "customShape.h"

void customShape::generateDisc(float size, int segments, char* textureFilename) {

	float angle = 0;

	for (int i = 0; i < segments + 1; i++) {
		
		//centre vertex
		normals.push_back(0.0f);
		normals.push_back(0.0f);
		normals.push_back(1.0f);

		texCoords.push_back(0.5f);
		texCoords.push_back(0.5f);

		vertex.push_back(0.0f);
		vertex.push_back(0.0f);
		vertex.push_back(0.0f);

		//outer vertex
		normals.push_back(0.0f);
		normals.push_back(0.0f);
		normals.push_back(1.0f);

		texCoords.push_back((cos(angle) / (size * 2)) + 0.5);
		texCoords.push_back((sin(angle) / (size * 2)) + 0.5);

		vertex.push_back(size*cos(angle));
		vertex.push_back(size*sin(angle));
		vertex.push_back(0.0f);

		angle += (2 * 3.1416) / segments;

		normals.push_back(0.0f);
		normals.push_back(0.0f);
		normals.push_back(1.0f);

		texCoords.push_back((cos(angle) / (size * 2)) + 0.5);
		texCoords.push_back((sin(angle) / (size * 2)) + 0.5);

		vertex.push_back(size*cos(angle));
		vertex.push_back(size*sin(angle));
		vertex.push_back(0.0f);
	}

	loadTexture(textureFilename);
}

void customShape::generateQuad(float size, int segments, char * textureFilename, Vector3 norm, float xScale, float yScale)
{
	float segSize = size / segments;
	float texIncrement = 1 / segments;

	for (int y = 0; y < (segments*yScale); y++) {
		for (int x = 0; x < (segments*xScale); x++)
		{
			//top left
			normals.push_back(norm.x);
			normals.push_back(norm.y);
			normals.push_back(norm.z);

			texCoords.push_back((segSize*x)/1);
			texCoords.push_back((segSize*y)/1);

			vertex.push_back(segSize*x);
			vertex.push_back(segSize*y);
			vertex.push_back(1.0f);

			//top right
			normals.push_back(norm.x);
			normals.push_back(norm.y);
			normals.push_back(norm.z);

			texCoords.push_back((segSize*(x + 1))/1);
			texCoords.push_back((segSize*y)/1);

			vertex.push_back(segSize*(x + 1));
			vertex.push_back(segSize*y);
			vertex.push_back(1.0f);

			//bottom left
			normals.push_back(norm.x);
			normals.push_back(norm.y);
			normals.push_back(norm.z);

			texCoords.push_back((segSize*x)/1);
			texCoords.push_back((segSize*(y + 1)) / 1);

			vertex.push_back(segSize*x);
			vertex.push_back(segSize*(y + 1));
			vertex.push_back(1.0f);

			//bottom left
			normals.push_back(norm.x);
			normals.push_back(norm.y);
			normals.push_back(norm.z);

			texCoords.push_back((segSize*x) / 1);
			texCoords.push_back((segSize*(y + 1)) / 1);

			vertex.push_back(segSize*x);
			vertex.push_back(segSize*(y + 1));
			vertex.push_back(1.0f);

			//top right
			normals.push_back(norm.x);
			normals.push_back(norm.y);
			normals.push_back(norm.z);

			texCoords.push_back((segSize*(x + 1)) / 1);
			texCoords.push_back((segSize*y) / 1);

			vertex.push_back(segSize*(x + 1));
			vertex.push_back(segSize*y);
			vertex.push_back(1.0f);

			//bottom right
			normals.push_back(norm.x);
			normals.push_back(norm.y);
			normals.push_back(norm.z);

			texCoords.push_back((segSize*(x + 1))/1);
			texCoords.push_back((segSize*(y + 1))/1);

			vertex.push_back(segSize*(x + 1));
			vertex.push_back(segSize*(y + 1));
			vertex.push_back(1.0f);
		}
	}
	loadTexture(textureFilename);
}

void customShape::render() {
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, vertex.data());
	glNormalPointer(GL_FLOAT, 0, normals.data());
	glTexCoordPointer(2, GL_FLOAT, 0, texCoords.data());

	glDrawArrays(GL_TRIANGLES, 0, vertex.size() / 3);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void customShape::loadTexture(char * filename)
{
	texture = SOIL_load_OGL_texture
	(
		filename,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT | SOIL_FLAG_INVERT_Y // Depending on texture file type some need inverted others don't.
	);

	//check for an error during the load process
	if (texture == 0)
	{
		printf("SOIL loading error: '%s'\n", SOIL_last_result());
	}
}
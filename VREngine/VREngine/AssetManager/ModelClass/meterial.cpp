#include "meterial.h"
#include "../AssetManager.h"


unsigned int Meterial::bindMeterial(GLuint shader, unsigned int startTexture) {
	if (!isLoaded) {
		AssetManager::getInstance()->loadMeterial(*this);
		isLoaded = true;
	}

	glUniform3fv(glGetUniformLocation(shader, "material.specularColor"), 1, &(specularColor)[0]);
	glUniform3fv(glGetUniformLocation(shader, "material.diffuseColor"), 1, &(diffuseColor)[0]);
	glUniform3fv(glGetUniformLocation(shader, "material.ambientColor"), 1, &(ambientColor)[0]);
	glUniform1f(glGetUniformLocation(shader, "material.shininess"), shininess);

	glUniform1i(glGetUniformLocation(shader, "material.hasSpecular"), hasSpecular ? 1 : 0);
	glUniform1i(glGetUniformLocation(shader, "material.hasDiffuse"), hasDiffuse ? 1 : 0);
	glUniform1i(glGetUniformLocation(shader, "material.hasAmbient"), hasAmbient ? 1 : 0);
	glUniform1i(glGetUniformLocation(shader, "material.hasShininess"), hasShininess ? 1 : 0);

	glUniform1i(glGetUniformLocation(shader, "material.hasNormal"), hasNormal ? 1 : 0);
	glUniform1i(glGetUniformLocation(shader, "material.hasHeight"), hasHeight ? 1 : 0);
	glUniform1i(glGetUniformLocation(shader, "material.hasLightmap"), hasLightmap ? 1 : 0);
	glUniform1i(glGetUniformLocation(shader, "material.hasEmissive"), hasEmissive ? 1 : 0);


	int i = startTexture;
	if (hasAmbient)
	{
		glActiveTexture(GL_TEXTURE0 + i); // Active proper texture unit before binding
		glUniform1i(glGetUniformLocation(shader, "material.ambientTexture"), i);
		// And finally bind the texture
		glBindTexture(GL_TEXTURE_2D, ambientId);
		i++;
	}
	if (hasDiffuse)
	{
		glActiveTexture(GL_TEXTURE0 + i); // Active proper texture unit before binding
		glUniform1i(glGetUniformLocation(shader, "material.diffuseTexture"), i);
		// And finally bind the texture
		glBindTexture(GL_TEXTURE_2D, diffuseId);
		i++;
	}
	if (hasSpecular)
	{
		glActiveTexture(GL_TEXTURE0 + i); // Active proper texture unit before binding
		glUniform1i(glGetUniformLocation(shader, "material.specularTexture"), i);
		// And finally bind the texture
		glBindTexture(GL_TEXTURE_2D, specularId);
		i++;
	}
	if (hasShininess)
	{
		glActiveTexture(GL_TEXTURE0 + i); // Active proper texture unit before binding
		glUniform1i(glGetUniformLocation(shader, "material.shininessTexture"), i);
		// And finally bind the texture
		glBindTexture(GL_TEXTURE_2D, shininessId);
		i++;
	}
	if (hasNormal)
	{
		glActiveTexture(GL_TEXTURE0 + i); // Active proper texture unit before binding
		glUniform1i(glGetUniformLocation(shader, "material.normalTexture"), i);
		// And finally bind the texture
		glBindTexture(GL_TEXTURE_2D, normalId);
		i++;
	}
	if (hasHeight)
	{
		glActiveTexture(GL_TEXTURE0 + i); // Active proper texture unit before binding
		glUniform1i(glGetUniformLocation(shader, "material.heightTexture"), i);
		// And finally bind the texture
		glBindTexture(GL_TEXTURE_2D, heightId);
		i++;
	}
	if (hasLightmap)
	{
		glActiveTexture(GL_TEXTURE0 + i); // Active proper texture unit before binding
		glUniform1i(glGetUniformLocation(shader, "material.lightMapTexture"), i);
		// And finally bind the texture
		glBindTexture(GL_TEXTURE_2D, lightmapId);
		i++;
	}
	if (hasEmissive)
	{
		glActiveTexture(GL_TEXTURE0 + i); // Active proper texture unit before binding
		glUniform1i(glGetUniformLocation(shader, "material.emissiveTexture"), i);
		// And finally bind the texture
		glBindTexture(GL_TEXTURE_2D, emissiveId);
		i++;
	}
	return i;
}
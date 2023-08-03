#pragma once
#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

//������Root node�����ڵ㣩���ܰ����ӽڵ㣨�������Ľڵ�һ������������һϵ��ָ�򳡾�������mMeshes�����д�����������ݵ�������
//Scene�µ�mMeshes���鴢����������Mesh���󣬽ڵ��е�mMeshes���鱣���ֻ�ǳ��������������������

unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false);

class Model {
public:
	//ģ������
	vector<Mesh> meshes;
	string directory;
	//�����洢�����Ѿ����ع�����ͼ��Ҫ�Ǻ������ظ�ʹ�õ�����Ͳ���Ҫ���¼���
	vector<Texture> texture_loaded;

	//����
	Model(string const& path) {
		LoadModel(path);
	}

	void Draw(Shader shader) {
		for (int i = 0; i < meshes.size(); i++)
		{
			meshes[i].Draw(shader);
		}
	}

private:

	//����
	//����ģ��
	void LoadModel(string const& path) {
		Assimp::Importer importer;
		//aiProcess_Triangulate�����������ε�ͼԪ��״ȫ��ת��Ϊ�����Σ�aiProcess_FlipUVs��תy��
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			std::cout << "ERROR::Assimp::" << importer.GetErrorString() << std::endl;
			return;
		}
		
		directory = path.substr(0, path.find_last_of('\\'));

		//�����һ���ڵ�
		ProcessNode(scene->mRootNode, scene);
	}

	//����ÿ���ڵ�
	void ProcessNode(aiNode* node, const aiScene* scene) {
		//����ǰ�ڵ����������
		for (int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];	//��������
			meshes.push_back(ProcessMesh(mesh, scene));	//ѹ����ջ��
		}

		//����ǰ�ڵ���ӽڵ����������Ҳ�ǵݹ������
		for (int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene);
		}

	}

	//��Assimp�е�aiMesh�ṹת��Ϊ��Mesh�ṹ
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene) {
		vector<Vertex> vertices;
		vector<unsigned int> indices;
		vector<Texture> textures;

		for (int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			//λ������
			glm::vec3 vect3;
			vect3.x = mesh->mVertices[i].x;
			vect3.y = mesh->mVertices[i].y;
			vect3.z = mesh->mVertices[i].z;
			vertex.Position = vect3;

			//��������
			if (mesh->HasNormals()) {
				vect3.x = mesh->mNormals[i].x;
				vect3.y = mesh->mNormals[i].y;
				vect3.z = mesh->mNormals[i].z;
				vertex.Normal = vect3;
			}

			//�����������ԣ�ֻ���ĵ�һ��������������
			if (mesh->mTextureCoords[0]) {
				//��������
				glm::vec2 tex;
				tex.x = mesh->mTextureCoords[0][i].x;
				tex.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = tex;
				//����
				vect3.x = mesh->mTangents[i].x;
				vect3.y = mesh->mTangents[i].y;
				vect3.z = mesh->mTangents[i].z;
				vertex.Tangent = vect3;
				// ������
				vect3.x = mesh->mBitangents[i].x;
				vect3.y = mesh->mBitangents[i].y;
				vect3.z = mesh->mBitangents[i].z;
				vertex.Bitangent = vect3;
			}
			else {
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);
			}
			vertices.push_back(vertex);
		}

		//����
		//���������е��棬ÿ���涼��һ��ͼԪ�������Եõ�ͼԪ�еĶ��������Լ��������˳��
		for (int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (int j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		//����
		if (mesh->mMaterialIndex >= 0) {
			aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
			vector<Texture> diffuseMaps = LoadMaterialTextures(mat, aiTextureType_DIFFUSE, "texture_diffuse");	//��ȡ��������ͼ����
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());	//��������뵽��ͼ������
			vector<Texture> specularMaps = LoadMaterialTextures(mat, aiTextureType_SPECULAR, "texture_specular");	//��þ��淴����ͼ����
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());	//��������뵽��ͼ������
			vector<Texture> normalMaps = LoadMaterialTextures(mat, aiTextureType_HEIGHT, "texture_normal");		//������ͼ
			textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
			vector<Texture> heightMaps = LoadMaterialTextures(mat, aiTextureType_AMBIENT, "texture_height");		//�߶�ͼ
			textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
		}
		return Mesh(vertices, indices, textures);
	}

	//��ȡ������ͼ����
	vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName) {
		vector<Texture> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);	//��ȡÿһ��������ļ�λ��			
			bool isSkip = false;

			for (unsigned int j = 0; j < texture_loaded.size(); j++)
			{
				if (std::strcmp(texture_loaded[j].path.data(), str.C_Str()) == 0) {
					textures.push_back(texture_loaded[j]);
					isSkip = true;
					break;
				}
			}

			if (!isSkip) {
				Texture texture;
				texture.id = TextureFromFile(str.C_Str(), directory);
				texture.type = typeName;
				texture.path = str.C_Str();
				textures.push_back(texture);
				texture_loaded.push_back(texture);
			}
		}
		return textures;
	}

};

//����һ��������������������ID
unsigned int TextureFromFile(const char* path, const string& directory, bool gamma) {
	string fileName = string(path);
	fileName = directory + "/" + fileName;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &nrComponents, 0);

	if (data) {
		GLenum format;
		if (nrComponents == 1) {
			format = GL_RED;
		}
		else if (nrComponents == 3) {
			format = GL_RGB;
		}
		else if (nrComponents == 4) {
			format = GL_RGBA;
		}

		//��
		glBindTexture(GL_TEXTURE_2D, textureID);

		//����
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

		//�Զ����ɶ༶��������
		glGenerateMipmap(GL_TEXTURE_2D);

		//Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else {
		std::cout << "Failed to load texture at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}


#endif
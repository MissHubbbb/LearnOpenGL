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

//场景的Root node（根节点）可能包含子节点（和其它的节点一样），它会有一系列指向场景对象中mMeshes数组中储存的网格数据的索引。
//Scene下的mMeshes数组储存了真正的Mesh对象，节点中的mMeshes数组保存的只是场景中网格数组的索引。

unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false);

class Model {
public:
	//模型数据
	vector<Mesh> meshes;
	string directory;
	//用来存储我们已经加载过的贴图，要是后面有重复使用的情况就不需要重新加载
	vector<Texture> texture_loaded;

	//函数
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

	//函数
	//加载模型
	void LoadModel(string const& path) {
		Assimp::Importer importer;
		//aiProcess_Triangulate将不是三角形的图元形状全部转换为三角形，aiProcess_FlipUVs翻转y轴
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			std::cout << "ERROR::Assimp::" << importer.GetErrorString() << std::endl;
			return;
		}
		
		directory = path.substr(0, path.find_last_of('\\'));

		//传入第一个节点
		ProcessNode(scene->mRootNode, scene);
	}

	//处理每个节点
	void ProcessNode(aiNode* node, const aiScene* scene) {
		//处理当前节点包含的网格
		for (int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];	//加载网格
			meshes.push_back(ProcessMesh(mesh, scene));	//压网格到栈中
		}

		//处理当前节点的子节点包含的网格，也是递归的主体
		for (int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene);
		}

	}

	//将Assimp中的aiMesh结构转化为类Mesh结构
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene) {
		vector<Vertex> vertices;
		vector<unsigned int> indices;
		vector<Texture> textures;

		for (int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			//位置属性
			glm::vec3 vect3;
			vect3.x = mesh->mVertices[i].x;
			vect3.y = mesh->mVertices[i].y;
			vect3.z = mesh->mVertices[i].z;
			vertex.Position = vect3;

			//法线属性
			if (mesh->HasNormals()) {
				vect3.x = mesh->mNormals[i].x;
				vect3.y = mesh->mNormals[i].y;
				vect3.z = mesh->mNormals[i].z;
				vertex.Normal = vect3;
			}

			//纹理坐标属性，只关心第一组纹理坐标属性
			if (mesh->mTextureCoords[0]) {
				//纹理坐标
				glm::vec2 tex;
				tex.x = mesh->mTextureCoords[0][i].x;
				tex.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = tex;
				//切线
				vect3.x = mesh->mTangents[i].x;
				vect3.y = mesh->mTangents[i].y;
				vect3.z = mesh->mTangents[i].z;
				vertex.Tangent = vect3;
				// 副切线
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

		//索引
		//遍历网格中的面，每个面都是一个图元，遍历以得到图元中的顶点索引以及顶点绘制顺序
		for (int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (int j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		//材质
		if (mesh->mMaterialIndex >= 0) {
			aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
			vector<Texture> diffuseMaps = LoadMaterialTextures(mat, aiTextureType_DIFFUSE, "texture_diffuse");	//获取漫反射贴图数组
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());	//将数组插入到贴图数组中
			vector<Texture> specularMaps = LoadMaterialTextures(mat, aiTextureType_SPECULAR, "texture_specular");	//获得镜面反射贴图数组
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());	//将数组插入到贴图数组中
			vector<Texture> normalMaps = LoadMaterialTextures(mat, aiTextureType_HEIGHT, "texture_normal");		//法线贴图
			textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
			vector<Texture> heightMaps = LoadMaterialTextures(mat, aiTextureType_AMBIENT, "texture_height");		//高度图
			textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
		}
		return Mesh(vertices, indices, textures);
	}

	//获取单张贴图数据
	vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName) {
		vector<Texture> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);	//获取每一个纹理的文件位置			
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

//加载一个纹理，并返回这个纹理的ID
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

		//绑定
		glBindTexture(GL_TEXTURE_2D, textureID);

		//采样
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

		//自动生成多级渐进纹理
		glGenerateMipmap(GL_TEXTURE_2D);

		//为当前绑定的纹理对象设置环绕、过滤方式
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
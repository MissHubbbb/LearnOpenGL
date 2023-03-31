#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "Shader.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

const char* glsl_version = "#version 130";

class M_GUI {
public:
	GLFWwindow* window;

	M_GUI() {}

	M_GUI(GLFWwindow* window)
	{
		this->window = window;
	}

	void SetWindow(GLFWwindow* window) {
		this->window = window;

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(glsl_version);
	}

	//first step
	void SetGUIContext() {
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(glsl_version);
	}

	//and second
	void StartGUIFrame() {
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void RenderGUI() {
		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	//final step
	void DeleteGUI() {
		// Cleanup
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void DrawOneFloat(const char * name,float * f) {
		ImGui::SliderFloat(name, f, 0.0f, 1.0f);
	}

	void DrawOneColor(const char* name, float* col) {
		ImGui::ColorEdit3(name, col);	// Edit 3 floats representing a color
	}

	void SameLine() {
		ImGui::SameLine();
	}

	void Begin(const char* text)
	{
		ImGui::Begin(text);
	}

	void Text(const char* text) {
		ImGui::Text(text);
	}

	void End() {
		ImGui::End();
	}
};

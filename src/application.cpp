
// std
#include <iostream>
#include <string>
#include <chrono>

// glm
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// project
#include "application.hpp"
#include "bounding_box.hpp"
#include "cgra/cgra_geometry.hpp"
#include "cgra/cgra_gui.hpp"
#include "cgra/cgra_image.hpp"
#include "cgra/cgra_shader.hpp"
#include "cgra/cgra_wavefront.hpp"

#include "cgra/cgra_image.hpp"
GLuint m_texture; // member variable to hold texture handle



using namespace std;
using namespace cgra;
using namespace glm;


Application::Application(GLFWwindow *window) : m_window(window) {
	
	// build the shader for the model
	shader_builder color_sb;
	color_sb.set_shader(GL_VERTEX_SHADER, CGRA_SRCDIR + std::string("//res//shaders//default_vert.glsl"));
	color_sb.set_shader(GL_FRAGMENT_SHADER, CGRA_SRCDIR + std::string("//res//shaders//default_frag.glsl"));
	GLuint color_shader = color_sb.build();

	vec3 max = vec3(FLT_MIN, FLT_MIN, FLT_MIN);
	vec3 min = vec3(FLT_MAX, FLT_MAX, FLT_MAX);

	// build the mesh for the model
	mesh_builder teapot_mb = load_wavefront_data(CGRA_SRCDIR + std::string("//res//assets//teapot.obj"));
	gl_mesh teapot_mesh = teapot_mb.build();
	teapot_mb.getMaxgetMin(max,min);
	gl_mesh boundingBox = createBoundingBoxMesh(min,max);

	

	//bind texture
	cgra::rgba_image texture_data(CGRA_SRCDIR + std::string("//res//textures//uv_texture.jpg"));
	m_texture = texture_data.uploadTexture();

	// put together an object
	m_model.shader = color_shader;
	m_model.mesh = teapot_mesh;
	m_model.color = glm::vec3(1, 0, 0);
	m_model.modelTransform = glm::mat4(1);

	m_boundingBox.shader = color_shader;
	m_boundingBox.mesh = boundingBox;
	m_boundingBox.color = vec3(1, 0, 0);
	m_boundingBox.modelTransform = glm::mat4(1);



}


void Application::render() {
	
	// retrieve the window hieght
	int width, height;
	glfwGetFramebufferSize(m_window, &width, &height); 

	m_windowsize = vec2(width, height); // update window size
	glViewport(0, 0, width, height); // set the viewport to draw to the entire window

	// clear the back-buffer
	glClearColor(0.3f, 0.3f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	// enable flags for normal/forward rendering
	glEnable(GL_DEPTH_TEST); 
	glDepthFunc(GL_LESS);

	float x = m_distance * (cos(cameraPitch) * cos(cameraYaw));
	float z = m_distance * (cos(cameraPitch) * sin(cameraYaw));
	float y = m_distance * sin(cameraPitch);
	vec3 cameraLocation(x, y, z);

	// calculate the projection and view matrix
	mat4 proj = perspective(1.f, float(width) / height, 0.1f, 1000.f);
	mat4 view = translate(mat4(1), vec3(0, -5, -m_distance)); // TODO replace view matrix with the camera transform
	mat4 cameraView = glm::lookAt(cameraLocation, vec3(0, 0, 0), vec3(0,1,0));

	// draw options
	if (m_show_grid) cgra::drawGrid(cameraView, proj);
	if (m_show_axis) cgra::drawAxis(cameraView, proj);
	glPolygonMode(GL_FRONT_AND_BACK, (m_showWireframe) ? GL_LINE : GL_FILL);




	// draw the model
	m_model.draw(cameraView, proj);
	if (showBoundingBoxes) {
		m_boundingBox.draw(cameraView, proj);
	}
}


void Application::renderGUI() {

	// setup window
	ImGui::SetNextWindowPos(ImVec2(5, 5), ImGuiSetCond_Once);
	ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiSetCond_Once);
	ImGui::Begin("Camera", 0);

	// display current camera parameters
	ImGui::Text("Application %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::SliderFloat("Distance", &m_distance, 0, 100, "%.1f");
	ImGui::SliderFloat("Pitch", &cameraPitch, -3.14 / 2, 3.14 / 2);
	ImGui::SliderFloat("Yaw", &cameraYaw, 0, 2 * 3.14);
	ImGui::SliderFloat3("Model Color", value_ptr(m_model.color), 0, 1, "%.2f");
	ImGui::SliderFloat3("Light Color", value_ptr(m_model.lightcolor), 0, 1, "%.2f");
	ImGui::SliderFloat3("Specular Color", value_ptr(m_model.specularcolor), 0, 1, "%.2f");
	ImGui::SliderFloat("Specular Strength", &m_model.extraattributes.x, 0, 5);
	ImGui::SliderFloat("Ambient Strength", &m_model.extraattributes.z, 0, 1);
	ImGui::SliderFloat("Toon Strength", &m_model.extraattributes.y, -1, 1);
	ImGui::SliderFloat3("Translation Offset", value_ptr(m_model.meshTranslationOffset), 0, 10, "%.2f");
	m_boundingBox.meshTranslationOffset = m_model.meshTranslationOffset;
	m_boundingBox.color = m_model.color;


	// extra drawing parameters
	ImGui::Checkbox("Show axis", &m_show_axis);
	ImGui::SameLine();
	ImGui::Checkbox("Show grid", &m_show_grid);
	ImGui::SameLine();
	ImGui::Checkbox("Textured", &m_model.useTexture);
	ImGui::Checkbox("Wireframe", &m_showWireframe);
	ImGui::SameLine();
	if (ImGui::Button("Screenshot")) rgba_image::screenshot(true);
	ImGui::SameLine();
	ImGui::Checkbox("Bounding Boxes", &showBoundingBoxes);

	// finish creating window
	ImGui::End();
}


void Application::cursorPosCallback(double xpos, double ypos) {	
	mouseX = xpos;
	mouseY = ypos;
	if (currentMouseX != -4) {
		float xDiff = mouseX - currentMouseX;
		float yDiff = mouseY - currentMouseY;
		cameraYaw = currentCameraYaw + xDiff / 314;
		cameraPitch = currentCameraPitch + yDiff / 314;
	}


}


void Application::mouseButtonCallback(int button, int action, int mods) {
	if (action == 1) {
		currentMouseX = mouseX;
		currentMouseY = mouseY;
		currentCameraPitch = cameraPitch;
		currentCameraYaw = cameraYaw;
	}
	if (action == 0) {
		currentMouseX = -4;
		currentMouseY = -4;
		
	}


}


void Application::scrollCallback(double xoffset, double yoffset) {
	(void)xoffset, yoffset; // currently un-used
	m_distance = m_distance + yoffset;
}


void Application::keyCallback(int key, int scancode, int action, int mods) {
	(void)key, (void)scancode, (void)action, (void)mods; // currently un-used
}


void Application::charCallback(unsigned int c) {
	(void)c; // currently un-used
}
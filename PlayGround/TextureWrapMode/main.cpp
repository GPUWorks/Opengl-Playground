#include <iostream>
#include <glew.h>  // should include before glfw3.h
#include <glfw3.h>
#include <vector>
#include <cmath>
#include <glm/glm.hpp>
using namespace glm;

#include "shader.hpp"
GLFWwindow* window;

void generateSphereGeometry(float radius, std::vector<float>& vertices, std::vector<unsigned int>& indices);

int main(void)
{
	// Initialize GLFW
	if (!glfwInit())
	{
		std::cout << "Failed to initialize glfw." << std::endl;
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(1024, 768, "PlayGround for OpenGL", NULL, NULL);
	if (window == nullptr)
	{
		std::cout << "Failed to create window for OpenGL." << std::endl;
		glfwTerminate();
		getchar();
		return -2;
	}

	glfwMakeContextCurrent(window);
	  
	// Initialize GLEW
	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialize glew." << std::endl;
		glfwTerminate();
		getchar();
		return -3;
	}

	std::vector<float> vert_coords;
	std::vector<unsigned int> vert_indices;
	generateSphereGeometry(1.0f, vert_coords, vert_indices);

	// Prepare data for drawing.
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vert_buffer;
	glGenBuffers(1, &vert_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vert_buffer);
	glBufferData(GL_ARRAY_BUFFER, vert_coords.size(), vert_coords.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, vert_coords.data());
	glEnableVertexAttribArray(1);

	GLuint indices_buffer;
	glGenBuffers(1, &indices_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buffer);
	
	 
	GLuint programID = LoadShaders("simple.vert", "simple.frag"/*, "simple.geom", true*/);

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glClearColor(0.0, 0.0, 0.4, 0.0);

	do
	{
		glClear(GL_COLOR_BUFFER_BIT);

		// do something drawing.
		glUseProgram(programID);

		 

		glfwSwapBuffers(window);
		glfwPollEvents();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	glfwTerminate();
	getchar();

	return 0;
}

void generateSphereGeometry(float radius, std::vector<float>& vertices, std::vector<unsigned int>& indices)
{
	static const int PI = 3.1415926;
	// ���ȷ����и�100��
	const int VERTICAL_SLICE = 100;
	float vertical_step = (float)(PI*2 / VERTICAL_SLICE);
	// γ�ȷ����и�50��
	const int HORIZONTAL_SLICE = 50;
	float horizontal_step = (float)(PI / HORIZONTAL_SLICE);
	
	unsigned int start_index = 0;
	unsigned int current_index = 0;
	// γ�ȷ����Ͻ�����ָ��50�Σ����и��50����ͬ�뾶��ͬ��Բ
	for (size_t i = 0; i < HORIZONTAL_SLICE; ++i)
	{
		start_index = current_index;
		float vertical_angle = horizontal_step * i;
		float z_coord = radius * std::cos(vertical_angle);
		float sub_radius = radius * std::sin(vertical_angle);
		// ���ȷ��������и��100��
		for (size_t j = 0; j < VERTICAL_SLICE; j++)
		{
			float horizontal_angle = vertical_step * j;
			float x_coord = sub_radius * std::cos(horizontal_angle);
			float y_coord = sub_radius * std::sin(horizontal_angle);

			// һȦ�����ˣ������ص��غ�
			if (j == VERTICAL_SLICE)
			{
				vertices.push_back(vertices[start_index]);
				vertices.push_back(vertices[start_index + 1]);
				vertices.push_back(vertices[start_index + 2]);
			}
			else
			{
				// �����ڼ���x, y, z����ʱ��Y���Z�������
				// ���������Ҫ��������ϵΪ��������ϵ
				vertices.push_back(x_coord);
				vertices.push_back(z_coord);
				vertices.push_back(y_coord);
			}
			// ����ÿһȦ����ʼ���������������ÿһȦ���غϵ�
			current_index += 3;

			if (i > 0 && j > 0)
			{
				// ������һȦ�еĶ�������
				unsigned int bottom_ring_a = (VERTICAL_SLICE + 1)*i + j;
				unsigned int bottom_ring_b = (VERTICAL_SLICE + 1)*i + j - 1;
				// ������һȦ�еĶ�������
				unsigned int top_ring_a = (VERTICAL_SLICE + 1)*(i - 1) + j;
				unsigned int top_ring_b = (VERTICAL_SLICE + 1)*(i - 1) + j - 1;

				// j == 1ʱ��������һȦ������һ����
				if (j == 1)
				{
					indices.push_back(bottom_ring_a);
					indices.push_back(top_ring_a);
					indices.push_back(top_ring_b);
				}
				// j == HORIZONTAL_SLICEʱ��������һȦ������һ����
				else if (j == HORIZONTAL_SLICE)
				{
					indices.push_back(bottom_ring_a);
					indices.push_back(top_ring_b);
					indices.push_back(bottom_ring_b);
				}
				else
				{
					// ��ʱ�ӷ������Ӷ��㣬ÿ�����ӳ�һ���ı���
					// ��ʱ�ӷ�����������α������޳������
					indices.push_back(bottom_ring_a);
					indices.push_back(top_ring_a);
					indices.push_back(top_ring_b);
					indices.push_back(bottom_ring_a);
					indices.push_back(top_ring_b);
					indices.push_back(bottom_ring_b);
				}
			}
		}
	}
}
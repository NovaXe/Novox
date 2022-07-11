#pragma once
#include <vector>
#include <memory>
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <stdexcept>
#include <fmt/core.h>

namespace novox::util {

	template <class T> class Array3D {
	private:
		int x_size;
		int y_size;
		int z_size;

		T* arr;

	public:
		Array3D(int x_size, int y_size, int z_size) : x_size(x_size), y_size(y_size), z_size(z_size) {
			this->arr = new T[x_size * y_size * z_size];
		}


		T& at(int x_pos, int y_pos, int z_pos) {
			if (inRange(x_pos, y_pos, z_pos)) {
				return arr[x_pos + (y_pos * x_size) + (z_pos * x_size * y_size)];
				
			}
			else {
				throw std::out_of_range("3D array access out of bounds");
			}
			
		}

		bool inRange(int x_pos, int y_pos, int z_pos) {
			if (x_pos >= 0 && x_pos < x_size) {
				if (y_pos >= 0 && y_pos < y_size) {
					if (z_pos >= 0 && z_pos < z_size) {
						return true;
					}
				}
			}
			return false;
		}


	};

	template <typename Enumeration>
	auto as_int(Enumeration const value)
		-> typename std::underlying_type<Enumeration>::type
	{
		return static_cast<typename std::underlying_type<Enumeration>::type>(value);
	}


	inline float deltaTime() {
		static float deltaTime = 0.0;
		static float lastFrame = 0.0;
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//fmt::print("delta time: {}\n", deltaTime);
		return deltaTime;
	}

	

}



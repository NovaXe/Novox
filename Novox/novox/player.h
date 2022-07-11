#pragma once
#include <glm/glm.hpp>
#include <vector>

namespace novox {
	class Player;
	class Camera;

	extern std::vector<Camera*> cameras;

	extern std::vector<Player*> players;
	extern int currentPlayer;
	
	class Camera {
	public:
		glm::vec3 position;
		glm::vec3 up;
		glm::vec3 front;
		glm::vec3 target;
		glm::vec3 right;


		float pitch;
		float yaw;
		Camera(const glm::vec3& playerPosition, float direction);
		void updateVectors();

		void setPos(const glm::vec3& newpos);

		void changeAngleBy(float yaw, float pitch);
	};

	class Player {
	private:
		Camera* camera;
		glm::vec3 position;
		float movement_speed;
	public:
		Player();
		
		float speed();
		void speed(float speed);

		void displaceRelative(const float yaw, const float pitch, const float change);
		void setpos(const glm::vec3& newpos);
		glm::vec3 getPos();
		glm::mat4 getView();
		Camera* getCamera();

		glm::vec3 getSelectionPos();

		void lookTowards(float yaw, float pitch);


	};



}
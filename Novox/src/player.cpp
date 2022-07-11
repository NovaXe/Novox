#include <glm/glm.hpp>
#include <glm/gtc/bitfield.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "novox/player.h"


namespace novox{
	Player::Player() {
		this->position = glm::vec3(0);
		this->movement_speed = 2.5f;

		this->camera = new Camera(this->position, -90);

	}
	void Player::displaceRelative(const float yaw, const float pitch, const float change){
		glm::vec3 dir = glm::vec3(
			cos(glm::radians(this->camera->yaw + yaw)) * cos(glm::radians(pitch)),
			sin(glm::radians(pitch)),
			sin(glm::radians(this->camera->yaw + yaw)) * cos(glm::radians(pitch))
		);
		dir = glm::normalize(dir);
		dir = dir * change;



		setpos(position + dir);
	}
	void Player::setpos(const glm::vec3& newpos){
		this->position = newpos;
		this->camera->position = newpos + glm::vec3(0, 2, 0);
	}

	float Player::speed() {
		return this->movement_speed;
	}
	void Player::speed(float speed) {
		this->movement_speed = speed;
	}


	glm::vec3 Player::getPos()
	{
		return this->position;
	}

	glm::mat4 Player::getView()
	{
		this->camera->target = this->camera->position + this->camera->front;
		
		glm::mat4 view = glm::lookAt(this->camera->position, this->camera->position + this->camera->front, this->camera->up);
		return view;
	}

	Camera* Player::getCamera()
	{
		return this->camera;
	}

	glm::vec3 Player::getSelectionPos()
	{
		const float selection_distance = 2.0;
		glm::vec3 sel = this->camera->position + this->camera->front * selection_distance;
		

		return sel;
	}

	void Player::lookTowards(float yaw, float pitch)
	{
		this->camera->changeAngleBy(yaw, pitch);
	}

	// DIVIDER LINE ----------------------------------------------------------

	Camera::Camera(const glm::vec3& playerPosition, float direction)
	{
		this->position = playerPosition + glm::vec3(0, 2, 0);
		this->up = { 0,1,0 };
		//this->front = glm::rotateY(glm::vec3(1,0,0), glm::radians(direction));
		this->target = this->position + this->front;

		this->yaw = direction;
		this->pitch = 0;
		updateVectors();
	}
	void Camera::updateVectors() {
		glm::vec3 front = glm::vec3(
			cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
			sin(glm::radians(pitch)),
			sin(glm::radians(yaw)) * cos(glm::radians(pitch))
		);
		this->front = glm::normalize(front);
		right = glm::normalize(glm::cross(front, glm::vec3(0.0, 1.0, 0.0)));
		up = glm::normalize(glm::cross(right, front));
	}
	void Camera::setPos(const glm::vec3& newpos)
	{
		this->position = newpos;
	}
	void Camera::changeAngleBy(float yaw, float pitch)
	{
		this->yaw += yaw;
		this->pitch += pitch;

		updateVectors();
	}
}
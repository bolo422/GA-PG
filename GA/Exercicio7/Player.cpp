#include "Player.h"




void Player::jump(float force, float speed)
{
	if (jumping) {

		if (force > 120)
			force = 120;

		jumpHeight = force + position.y;
		jumping = false;
		jumped = true;
	}

	if (!falling && jumped && position.y < jumpHeight) {
		addPositionY(2 * speed);
		if (position.y >= jumpHeight) {
			falling = true;
			jumped = false;
		}
	}
	else if (falling && position.y > ground) {
		removePositionY(2 * speed);
		if (position.y <= ground) {
			falling = false;
		}
	}

}
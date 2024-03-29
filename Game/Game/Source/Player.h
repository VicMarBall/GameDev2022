#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "List.h"
#include "Animation.h"
#include "Bullet.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class Player : public Entity
{
public:

	Player();
	
	virtual ~Player();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

	void OnCollision(PhysBody* otherBody);

	void EndCollision(PhysBody* otherBody);

	void SetPosition(int posX, int posY);

	void Die();

public:

	const bool Living() {
		return isAlive;
	}

	void GodSwitch();

	const bool isGod() {
		return godMode;
	}

	const bool Won() {
		return win;
	}

	void CanShoot(bool can);

	void AddLife() {
		lives++;
	}

	int GetRemainingLives() {
		return lives;
	}

	void SetLives(int amount) {
		lives = amount;
	}
	
	void PauseMovement();

	void ResumeMovement();

private:
	bool win = false;
	bool onAir;
	bool canDoubleJump;

	List<Bullet*> storedBullets;
	
	bool groundPounding;

	float maxSpeed;

	bool isAlive;

	int lives;

	int invencibilityFrames;

	bool godMode = false;

	bool canShoot;

	//L02: DONE 1: Declare player parameters
	SDL_Texture* texture;
	const char* texturePath;

	Directions facing;

	Animation idleRight;
	Animation walkingRight;
	Animation jumpingRight;
	Animation idleLeft;
	Animation walkingLeft;
	Animation jumpingLeft;

	Animation shootingRight;
	Animation shootingLeft;

	Animation death;

	Animation winning;

	Animation* previousAnimation;

	// L07 TODO 5: Add physics to the player - declare a Physics body
	PhysBody* pBody = nullptr;

};

#endif // __PLAYER_H__
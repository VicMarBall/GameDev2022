#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL_rect.h"
#define MAX_FRAMES 25

class Animation {
public:
	bool loop = true;
	bool pingpong = false;
	float speed = 1.0f;
	SDL_Rect frames[MAX_FRAMES];

private:
	float currentFrame = 0.0f;
	int totalFrames = 0;
	int loopCount = 0;
	int pingpongDirection = 1;

public:

	void PushBack(const SDL_Rect& rect) {
		frames[totalFrames++] = rect;
	}

	void Reset() {
		this->currentFrame = 0;
		this->loopCount = 0;
	}

	void FullReset() {
		this->currentFrame = 0;
		this->totalFrames = 0;
		this->loopCount = 0;
		this->pingpongDirection = 1;
	}

	bool HasFinished() {
		return !loop && !pingpong && loopCount > 0;
	}

	void Update() {
		OPTICK_EVENT();
		currentFrame += speed;
		if (currentFrame >= totalFrames)
		{
			currentFrame = (loop || pingpong) ? 0.0001f : totalFrames - 1;
			++loopCount;

			if (pingpong)
				pingpongDirection = -pingpongDirection;
		}
	}

	int GetCurrentFrameNum() {
		return (int)currentFrame;
	}

	void BeginAnimationIn(int index) {
		this->currentFrame = index;
	}

	/*SDL_Rect& GetCurrentFrame() {
		return frames[(int)currentFrame];
	}*/

	SDL_Rect& GetCurrentFrame()
	{
		int actualFrame = currentFrame;
		if (pingpongDirection == -1)
			actualFrame = totalFrames - currentFrame;

		return frames[actualFrame];
	}
};

#endif
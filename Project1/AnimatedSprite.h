#ifndef ANIMATED_SPRITE_H
#define ANIMATED_SPRITE_H

#include <SFML\Graphics\Sprite.hpp>
#include <SFML\System\Clock.hpp>
#include <vector>
#include "Debug.h"

using namespace std;
using namespace sf;

class AnimatedSprite : public Sprite {
public:
	AnimatedSprite();
	AnimatedSprite(const Texture&);
	AnimatedSprite(const Texture&, const IntRect&);
	~AnimatedSprite();
	//function to get the clock and time
	const Clock& getClock();
	const Time& getTime();
	//function to get the frames and to clear them,add a frame
	const vector<IntRect>& getFrames();
	void clearFrames();
	const IntRect& getFrame(int);
	void addFrame(const IntRect&);
	//function to get the current frame
	const int getCurrentFrame();
	//function to set the animation to loop and get
	void setLooped(bool);
	const bool getLooped();
	//function to set animation to play
	void setPlayed(bool);
	const bool getPlayed();
	//function to set teh time
	void setTime(Time);
	void update();

private:
	//set up the clock and time,frames and current frame
	Clock m_clock;
	Time m_time;
	vector<IntRect> m_frames;
	unsigned int m_current_frame;
	bool m_loop;
	//values for play count and max plays
	unsigned int m_play_count;
	unsigned int m_max_plays;
	bool m_played;
};

#endif // !ANIMATED_SPRITE_H
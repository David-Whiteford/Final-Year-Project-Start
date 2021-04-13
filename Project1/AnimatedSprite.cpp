#include "AnimatedSprite.h"

AnimatedSprite::AnimatedSprite() :
	//set the current frames,time,loop,played bools,max player and play when animated sprite created
	m_current_frame(0),
	m_time(seconds(0.5f)),
	m_loop(true),
	m_play_count(0),
	m_max_plays(0),
	m_played(false)
{
	//if debug is 2 then output the below debug message
	if (DEBUG == 2)
	{
		DEBUG_MSG("AnimatedSprite()");
	}
}

AnimatedSprite::AnimatedSprite(const sf::Texture& t) : AnimatedSprite() {
	//if debug is 2 then output the below debug message
	if (DEBUG == 2)
	{
		DEBUG_MSG("AnimatedSprite(const Texture&)");
	}
	//set the texture of the sprite to passed t
	this->setTexture(t);
}

AnimatedSprite::AnimatedSprite(const sf::Texture& t, const sf::IntRect& rect) :
	AnimatedSprite(t)
{
	//if debug is 2 then output the below debug message
	if (DEBUG == 2)
	{
		DEBUG_MSG("AnimatedSprite(const Texture&, const IntRect&)");
	}
	//push into frames vec the passed in rect
	m_frames.push_back(rect);
}

AnimatedSprite::~AnimatedSprite() {
	//if debug is 2 then output the below debug message
	if (DEBUG == 2)
	{
		DEBUG_MSG("~AnimatedSprite()");
	}
}

const sf::Clock& AnimatedSprite::getClock() {
	//return the local clock
	return m_clock;
}

const sf::Time& AnimatedSprite::getTime() {
	//return the local time
	return m_time;
}

void AnimatedSprite::setTime(Time t)
{
	//set the time to passed in t time
	this->m_time = t;
}

const vector<IntRect>& AnimatedSprite::getFrames() {
	//return frames vector
	return m_frames;
}

void AnimatedSprite::clearFrames() {
	//set the current frame to 0 and played to false
	m_current_frame = 0;
	m_played = false;
	//check that the frames vector not enpty
	if (!m_frames.empty())
	{
		//if not clear vec
		m_frames.clear();
	}
}

const IntRect& AnimatedSprite::getFrame(int n) {
	//get the frame stored at n
	return m_frames[n];
}

void AnimatedSprite::addFrame(const IntRect& frame) {
	//add frames to vector for storing frames
	m_frames.push_back(frame);
}

const int AnimatedSprite::getCurrentFrame() {
	//return the current frame
	return m_current_frame;
}

void AnimatedSprite::setLooped(bool loop) {
	//set the bool to loop
	this->m_loop = loop;
}

const bool AnimatedSprite::getLooped() {
	//return bool for if to loop
	return this->m_loop;
}

void AnimatedSprite::setPlayed(bool played) {
	//return the played bool
	this->m_played = played;
}

const bool AnimatedSprite::getPlayed() {
	return this->m_played;
}

void AnimatedSprite::update() {
	// If not looped only play once
	if (m_played == true && !m_loop)
	{
		m_current_frame = m_frames.size() - 1;
	}
	else {
		///chck the time greater than local time and increment the frames
		if (m_clock.getElapsedTime() > m_time) {
			if (m_frames.size() > m_current_frame + 1)
			{
				m_current_frame++;
			}
			//else reset the current frame and set played to false 
			else {
				m_current_frame = 0;
				m_played = true;
			}
			//restart clock
			m_clock.restart();
		}
	}

}
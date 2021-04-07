#include "AnimatedSprite.h"

AnimatedSprite::AnimatedSprite() :
	m_current_frame(0),
	m_time(seconds(0.5f)),
	m_loop(true),
	m_play_count(0),
	m_max_plays(0),
	m_played(false)
{
	if (DEBUG == 2)
	{
		DEBUG_MSG("AnimatedSprite()");
	}
}

AnimatedSprite::AnimatedSprite(const sf::Texture& t) : AnimatedSprite() {
	if (DEBUG == 2)
	{
		DEBUG_MSG("AnimatedSprite(const Texture&)");
	}
	this->setTexture(t);
}

AnimatedSprite::AnimatedSprite(const sf::Texture& t, const sf::IntRect& rect) :
	AnimatedSprite(t)
{
	if (DEBUG == 2)
	{
		DEBUG_MSG("AnimatedSprite(const Texture&, const IntRect&)");
	}
	m_frames.push_back(rect);
}

AnimatedSprite::~AnimatedSprite() {
	if (DEBUG == 2)
	{
		DEBUG_MSG("~AnimatedSprite()");
	}
}

const sf::Clock& AnimatedSprite::getClock() {
	return m_clock;
}

const sf::Time& AnimatedSprite::getTime() {
	return m_time;
}

void AnimatedSprite::setTime(Time t)
{
	this->m_time = t;
}

const vector<IntRect>& AnimatedSprite::getFrames() {
	return m_frames;
}

void AnimatedSprite::clearFrames() {
	m_current_frame = 0;
	m_played = false;
	if (!m_frames.empty())
	{
		m_frames.clear();
	}
}

const IntRect& AnimatedSprite::getFrame(int n) {
	return m_frames[n];
}

void AnimatedSprite::addFrame(const IntRect& frame) {
	m_frames.push_back(frame);
}

const int AnimatedSprite::getCurrentFrame() {
	return m_current_frame;
}

void AnimatedSprite::setLooped(bool loop) {
	this->m_loop = loop;
}

const bool AnimatedSprite::getLooped() {
	return this->m_loop;
}

void AnimatedSprite::setPlayed(bool played) {
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
		if (m_clock.getElapsedTime() > m_time) {
			if (m_frames.size() > m_current_frame + 1)
			{
				m_current_frame++;
			}
			else {
				m_current_frame = 0;
				m_played = true;
			}
			m_clock.restart();
		}
	}

}
#include "Events.h"

gpp::Events::Events()
{
	m_current = Event::NONE;
}
gpp::Events::~Events() {}

void gpp::Events::setCurrent(gpp::Events::Event event) {
	m_current = event;
}

gpp::Events::Event gpp::Events::getCurrent() {
	return m_current;
}

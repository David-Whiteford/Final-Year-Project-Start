#ifndef EVENT_H
#define EVENT_H

namespace gpp {
	class Events
	{
	public:
		Events();
		~Events();
		//enume for the movements
		enum class Event
		{
			NONE,
			IDLE,
			PLAYERMOVERIGTH,
			PLAYERMOVELEFT,
			PLAYERMOVEDOWN,
			PLAYERMOVEUP,
			Count
		};

	
		//function to set the current animation state
		void setCurrent(Event);
		Event getCurrent();

	private:
		Event m_current;
	};
}

#endif

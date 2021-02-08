#ifndef EVENT_H
#define EVENT_H

namespace gpp {
	class Events
	{
	public:
		Events();
		~Events();

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

	

		void setCurrent(Event);
		Event getCurrent();

	private:
		Event m_current;
	};
}

#endif

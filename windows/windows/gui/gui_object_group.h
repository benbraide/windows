#pragma once

#ifndef WINPP_GUI_OBJECT_GROUP_H
#define WINPP_GUI_OBJECT_GROUP_H

#include "generic_gui_object.h"

namespace winpp{
	namespace gui{
		class object_group : public generic_object{
		public:
			typedef std::recursive_mutex lock_type;
			typedef std::lock_guard<lock_type> guard_type;

		protected:
			mutable lock_type lock_;
		};
	}
}

#endif /* !WINPP_GUI_OBJECT_GROUP_H */

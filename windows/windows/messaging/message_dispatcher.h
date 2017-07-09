#pragma once

#ifndef WINPP_MESSAGE_DISPATCHER_H
#define WINPP_MESSAGE_DISPATCHER_H

#include <any>

#include "message_object.h"

namespace winpp{
	namespace messaging{
		class dispatcher{
		public:
			typedef ::LRESULT lresult_type;

			typedef wrappers::msg msg_type;
			typedef wrappers::hwnd hwnd_type;

			typedef gui::object gui_object_type;

			lresult_type dispatch(const msg_type &info, bool is_sent, const std::any &handler, gui_object_type *target = nullptr);

		private:
			lresult_type dispatch_nc_create_(const msg_type &info, bool is_sent, const std::any &handler, gui_object_type *target);

			lresult_type dispatch_create_(const msg_type &info, bool is_sent, const std::any &handler, gui_object_type *target);

			lresult_type dispatch_destroy_(const msg_type &info, bool is_sent, const std::any &handler, gui_object_type *target);

			lresult_type dispatch_nc_destroy_(const msg_type &info, bool is_sent, const std::any &handler, gui_object_type *target);
		};
	}
}

#endif /* !WINPP_MESSAGE_DISPATCHER_H */

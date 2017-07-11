#pragma once

#ifndef WINPP_APPLICATION_CLASSES_H
#define WINPP_APPLICATION_CLASSES_H

#include "../wrappers/wnd_class_wrapper.h"
#include "application_object.h"

namespace winpp{
	namespace application{
		class classes{
		public:
			typedef wrappers::wnd_class wnd_class_type;

			classes();

			wnd_class_type &general();

			wnd_class_type &dialog();

		private:
			wnd_class_type general_;
			wnd_class_type dialog_;
		};
	}
}

#endif /* !WINPP_APPLICATION_CLASSES_H */

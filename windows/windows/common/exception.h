#pragma once

#ifndef WINPP_EXCEPTION_H
#define WINPP_EXCEPTION_H

#include <exception>

namespace winpp{
	namespace common{
		class no_app_exception : public std::exception{
		public:
			no_app_exception()
				: exception("No application instance found"){}
		};

		class multiple_apps_exception : public std::exception{
		public:
			multiple_apps_exception()
				: exception("Multiple application instances found"){}
		};
	}
}

#endif /* !WINPP_EXCEPTION_H */

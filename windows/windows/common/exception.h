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

		class unsupported_exception : public std::exception{
		public:
			unsupported_exception()
				: exception("Operation is not supported"){}
		};

		class invalid_object_exception : public std::exception{
		public:
			invalid_object_exception()
				: exception("Object is invalid"){}
		};

		class invalid_arg_exception : public std::exception{
		public:
			invalid_arg_exception()
				: exception("Argument is invalid"){}
		};
	}
}

#endif /* !WINPP_EXCEPTION_H */

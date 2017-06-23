#pragma once

#ifndef WINPP_EXCEPTION_H
#define WINPP_EXCEPTION_H

#include <string>
#include <exception>

#include "common_headers.h"

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

		class cross_thread_exception : public std::exception{
		public:
			cross_thread_exception()
				: exception("Cross thread access is forbidden"){}
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

		class windows_error : public std::exception{
		public:
			typedef ::DWORD dword_type;

			windows_error()
				: code_(GetLastError()){
				if (code_ != ERROR_SUCCESS){
					void *allocated_buffer = nullptr;
					auto count = ::FormatMessageW(
						FORMAT_MESSAGE_ALLOCATE_BUFFER |
						FORMAT_MESSAGE_FROM_SYSTEM |
						FORMAT_MESSAGE_IGNORE_INSERTS,
						nullptr,
						code_,
						MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
						reinterpret_cast<wchar_t *>(&allocated_buffer),
						0u,
						nullptr
					);

					if (allocated_buffer != nullptr){
						if (count > 0u)
							text_ = std::move(std::wstring(reinterpret_cast<wchar_t *>(allocated_buffer), count));

						::LocalFree(allocated_buffer);
					}
				}
				else
					text_ = L"Success";
			}

			dword_type code() const{
				return code_;
			}

			const std::wstring &text() const{
				return text_;
			}

		private:
			dword_type code_;
			std::wstring text_;
		};
	}
}

#endif /* !WINPP_EXCEPTION_H */

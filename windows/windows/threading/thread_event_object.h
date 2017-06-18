#pragma once

#ifndef WINPP_THREAD_EVENT_OBJECT_H
#define WINPP_THREAD_EVENT_OBJECT_H

#include "../common/common_headers.h"
#include "../wrappers/value_wrapper.h"

namespace winpp{
	namespace threading{
		template <bool manual>
		class event_object : public wrappers::value<event_object<manual>, ::HANDLE>{
		public:
			typedef wrappers::value<event_object<manual>, ::HANDLE> base_type;
			typedef ::DWORD dword_type;
			
			event_object()
				: base_type(nullptr){}

			explicit event_object(const std::wstring &name)
				: base_type(nullptr){
				create(name);
			}

			event_object(const event_object &) = delete;

			event_object(event_object &&target)
				: base_type(target.value_){
				target.value_ = nullptr;
			}
			
			event_object &operator =(const event_object &) = delete;

			event_object &operator =(event_object &&target){
				destroy();//Destroy previous object, if any
				base_type::value_ = target.value_;
				target.value_ = nullptr;
				return *this;
			}

			bool create(const std::wstring &name){
				return (!is_created() && (base_type::value_ = ::CreateEventW(nullptr, manual ? TRUE : FALSE, FALSE, name.c_str())) != nullptr);
			}

			bool open(const std::wstring &name){
				return (!is_created() && (base_type::value_ = ::OpenEventW(0u, FALSE, name.c_str())) != nullptr);
			}

			bool destroy(){
				if (is_created() && ::CloseHandle(base_type::value_) != FALSE)
					base_type::value_ = nullptr;

				return (base_type::value_ == nullptr);
			}

			bool reset(){
				return (is_created() && ::ResetEvent(base_type::value_) != FALSE);
			}

			bool set(){
				return (is_created() && ::SetEvent(base_type::value_) != FALSE);
			}

			bool is_created() const{
				return (base_type::value_ != nullptr);
			}

			static const bool is_manual = manual;
		};
	}
}

#endif /* !WINPP_THREAD_EVENT_OBJECT_H */

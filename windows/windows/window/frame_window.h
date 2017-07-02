#pragma once

#ifndef WINPP_FRAME_WINDOW_H
#define WINPP_FRAME_WINDOW_H

#include "child_window.h"
#include "top_level_window.h"

namespace winpp{
	namespace window{
		template <class base_type>
		class frame{
		public:
			typedef base_type base_type;

			typedef ::DWORD dword_type;

			frame(){
				reset_persistent_styles_();
			}

			template <typename... args_types>
			explicit frame(args_types &&... args){
				reset_persistent_styles_();
				base_type::create(std::forward<args_types>(args)...);
			}

			virtual ~frame(){
				base_type::destroy(true);
			}

			virtual dword_type black_listed_styles(bool is_extended) const override{
				return (base_type::black_listed_styles(is_extended) | (is_extended ? 0u : (WS_CAPTION | WS_BORDER | WS_SYSMENU)));
			}

		protected:
			virtual void reset_persistent_styles_() override{
				base_type::reset_persistent_styles_();
				WINPP_SET(base_type::persistent_styles_.basic, WS_OVERLAPPEDWINDOW);
			}
		};
	}
}

#endif /* !WINPP_FRAME_WINDOW_H */

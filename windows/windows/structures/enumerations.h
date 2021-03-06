#pragma once

#ifndef WINPP_ENUMERATIONS_H
#define WINPP_ENUMERATIONS_H

#include "../common/preprocessor.h"
#include "../common/common_headers.h"

namespace winpp{
	namespace structures{
		namespace enumerations{
			enum class random_string_char_set : unsigned int{
				nil					= (0 << 0x0000),
				digits				= (1 << 0x0000),
				alpha				= (1 << 0x0001),
				uppercase			= (1 << 0x0002),
				lowercase			= (1 << 0x0003),
				full_range			= (1 << 0x0004),
			};

			enum class placement_type{
				left,
				top_left,
				top,
				top_right,
				right,
				bottom_right,
				bottom,
				bottom_left,
				center_left,
				top_center,
				center_right,
				bottom_center,
				center,
				unspecified,
			};

			enum class gui_sibling_type{
				previous,
				next,
			};

			enum class event_type{
				nil,
				pre_create,
				create,
				destroy,
				post_destroy,
				pre_activate,
				activate,
				close,
				move,
				size,
				maximize,
				minimize,
				restore,
				show,
				hide,
				erase_background,
				paint,
				timer,
				interval,
				context_menu,
				menu_init,
				menu_highlight,
				menu_select,
				mouse_enter,
				mouse_leave,
				mouse_hover,
				mouse_move,
				mouse_wheel,
				mouse_down,
				mouse_up,
				click,
				dbl_click,
				rclick,
				mclick,
				drag,
				key_down,
				key_up,
				key_press,
			};

			enum class named_color_type : ::COLORREF{
				alice_blue					= RGB(0xf0, 0xf8, 0xff),
				antique_white				= RGB(0xfa, 0xeb, 0xd7),
				aqua						= RGB(0x00, 0xff, 0xff),
				aquamarine					= RGB(0x7f, 0xff, 0xd4),
				azure						= RGB(0xf0, 0xff, 0xff),
				beige						= RGB(0xf5, 0xf5, 0xdc),
				bisque						= RGB(0xff, 0xe4, 0xce),
				black						= RGB(0x00, 0x00, 0x00),
				blanched_almond				= RGB(0xff, 0xeb, 0xcd),
				blue						= RGB(0x00, 0x00, 0xff),
				blue_violet					= RGB(0x8a, 0x2b, 0xe2),
				brown						= RGB(0xa5, 0x2a, 0x2a),
				burly_wood					= RGB(0xde, 0xb8, 0x87),
				cadet_blue					= RGB(0x5f, 0x9e, 0xa0),
				chartreuse					= RGB(0x7f, 0xff, 0x00),
				chocolate					= RGB(0xd2, 0x69, 0x1e),
				coral						= RGB(0xff, 0x7f, 0x50),
				cornflower_blue				= RGB(0x64, 0x95, 0xed),
				cornsilk					= RGB(0xff, 0xf8, 0xdc),
				crimson						= RGB(0xdc, 0x14, 0x3c),
				cyan						= RGB(0x00, 0xff, 0xff),
				dark_blue					= RGB(0x00, 0x00, 0x8b),
				dark_cyan					= RGB(0x00, 0x8b, 0x8b),
				dark_goldenrod				= RGB(0xb8, 0x86, 0x0b),
				dark_gray					= RGB(0xa9, 0xa9, 0xa9),
				dark_green					= RGB(0x00, 0x64, 0x00),
				dark_grey					= RGB(0xa9, 0xa9, 0xa9),
				dark_khaki					= RGB(0xbd, 0xb7, 0x6b),
				dark_magenta				= RGB(0x8b, 0x00, 0x8b),
				dark_olive_green			= RGB(0x55, 0x6b, 0x2f),
				dark_orange					= RGB(0xff, 0x8c, 0x00),
				dark_orchid					= RGB(0x99, 0x32, 0xcc),
				dark_red					= RGB(0x8b, 0x00, 0x00),
				dark_salmon					= RGB(0xe9, 0x97, 0x6a),
				dark_sea_green				= RGB(0x8f, 0xbc, 0x8f),
				dark_slate_blue				= RGB(0x48, 0x3d, 0x8b),
				dark_slate_gray				= RGB(0x2f, 0x4f, 0x4f),
				dark_slate_grey				= RGB(0x2f, 0x4f, 0x4f),
				dark_turquoise				= RGB(0x00, 0xce, 0xd1),
				dark_violet					= RGB(0x94, 0x00, 0xd3),
				deep_pink					= RGB(0xff, 0x14, 0x93),
				deep_sky_blue				= RGB(0x00, 0xbf, 0xff),
				dim_gray					= RGB(0x69, 0x69, 0x69),
				dim_grey					= RGB(0x69, 0x69, 0x69),
				dodger_blue					= RGB(0x1e, 0x90, 0xff),
				firebrick					= RGB(0xb2, 0x22, 0x22),
				floral_white				= RGB(0xff, 0xfa, 0xf0),
				forest_green				= RGB(0x22, 0x8b, 0x22),
				fuchsia						= RGB(0xff, 0x00, 0xff),
				gainsboro					= RGB(0xdc, 0xdc, 0xdc),
				ghost_white					= RGB(0xf8, 0xf8, 0xff),
				gold						= RGB(0xff, 0xd7, 0x00),
				goldenrod					= RGB(0xda, 0xa5, 0x20),
				gray						= RGB(0x80, 0x80, 0x80),
				green						= RGB(0x00, 0x80, 0x00),
				green_yellow				= RGB(0xad, 0xff, 0x2f),
				grey						= RGB(0x80, 0x80, 0x80),
				honeydew					= RGB(0xf0, 0xff, 0xf0),
				hot_pink					= RGB(0xff, 0x69, 0xb4),
				indian_red					= RGB(0xcd, 0x5c, 0x5c),
				indigo						= RGB(0x4b, 0x00, 0x82),
				ivory						= RGB(0xff, 0xff, 0xf0),
				khaki						= RGB(0xf0, 0xe6, 0x8c),
				lavendar					= RGB(0xe6, 0xe6, 0xfa),
				lavender_blush				= RGB(0xff, 0xf0, 0xf5),
				lawn_green					= RGB(0x7c, 0xfc, 0x00),
				lemon_chiffon				= RGB(0xff, 0xfa, 0xcd),
				light_blue					= RGB(0xad, 0xd8, 0xe6),
				light_coral					= RGB(0xf0, 0x80, 0x80),
				light_cyan					= RGB(0xe0, 0xff, 0xff),
				light_goldenrod_yellow		= RGB(0xfa, 0xfa, 0xd2),
				light_gray					= RGB(0xd3, 0xd3, 0xd3),
				light_green					= RGB(0x90, 0xee, 0x90),
				light_grey					= RGB(0xd3, 0xd3, 0xd3),
				light_pink					= RGB(0xff, 0xb6, 0xc1),
				light_salmon				= RGB(0xff, 0xa0, 0x7a),
				light_sea_green				= RGB(0x20, 0xb2, 0xaa),
				light_sky_blue				= RGB(0x87, 0xce, 0xfa),
				light_slate_gray			= RGB(0x77, 0x88, 0x99),
				light_slate_grey			= RGB(0x77, 0x88, 0x99),
				light_steel_blue			= RGB(0xb0, 0xc4, 0xde),
				light_yellow				= RGB(0xff, 0xff, 0xe0),
				lime						= RGB(0x00, 0xff, 0x00),
				lime_green					= RGB(0x32, 0xcd, 0x32),
				linen						= RGB(0xfa, 0xf0, 0xe6),
				magenta						= RGB(0xff, 0x00, 0xff),
				maroon						= RGB(0x80, 0x00, 0x00),
				medium_aquamarine			= RGB(0x66, 0xcd, 0xaa),
				medium_blue					= RGB(0x00, 0x00, 0xcd),
				medium_orchid				= RGB(0xba, 0x55, 0xd3),
				medium_purple				= RGB(0x93, 0x70, 0xdb),
				medium_sea_green			= RGB(0x3c, 0xb3, 0x71),
				medium_slate_blue			= RGB(0x7b, 0x68, 0xee),
				medium_spring_green			= RGB(0x00, 0xfa, 0x9a),
				medium_turquoise			= RGB(0x48, 0xd1, 0xcc),
				medium_violet_red			= RGB(0xc7, 0x15, 0x85),
				midnight_blue				= RGB(0x19, 0x19, 0x70),
				mint_cream					= RGB(0xf5, 0xff, 0xfa),
				misty_rose					= RGB(0xff, 0xe4, 0xe1),
				moccasin					= RGB(0xff, 0xe4, 0xb5),
				navajo_white				= RGB(0xff, 0xde, 0xad),
				navy						= RGB(0x00, 0x00, 0x80),
				old_lace					= RGB(0xfd, 0xf5, 0xe6),
				olive						= RGB(0x80, 0x80, 0x00),
				olive_drab					= RGB(0x6b, 0x8e, 0x23),
				orange						= RGB(0xff, 0xa5, 0x00),
				orange_red					= RGB(0xff, 0x45, 0x00),
				orchid						= RGB(0xda, 0x70, 0xd6),
				pale_goldenrod				= RGB(0xee, 0xe8, 0xaa),
				pale_green					= RGB(0x98, 0xfb, 0x98),
				pale_turquoise				= RGB(0xaf, 0xee, 0xee),
				pale_violet_red				= RGB(0xdb, 0x70, 0x93),
				papaya_whip					= RGB(0xff, 0xef, 0xd5),
				peach_puff					= RGB(0xff, 0xda, 0xb9),
				peru						= RGB(0xcd, 0x85, 0x3f),
				pink						= RGB(0xff, 0xc0, 0xcb),
				plum						= RGB(0xdd, 0xa0, 0xdd),
				powder_blue					= RGB(0xb0, 0xe0, 0xe6),
				purple						= RGB(0x80, 0x00, 0x80),
				red							= RGB(0xff, 0x00, 0x00),
				rosy_brown					= RGB(0xbc, 0x8f, 0x8f),
				royal_blue					= RGB(0x41, 0x69, 0xe1),
				saddle_brown				= RGB(0x8b, 0x45, 0x13),
				salmon						= RGB(0xfa, 0x80, 0x72),
				sandy_brown					= RGB(0xf4, 0xa4, 0x60),
				sea_green					= RGB(0x2e, 0x8b, 0x57),
				sea_shell					= RGB(0xff, 0xf5, 0xee),
				sienna						= RGB(0xa0, 0x52, 0x2d),
				silver						= RGB(0xc0, 0xc0, 0xc0),
				sky_blue					= RGB(0x87, 0xce, 0xeb),
				slate_blue					= RGB(0x6a, 0x5a, 0xcd),
				slate_gray					= RGB(0x70, 0x80, 0x90),
				slate_grey					= RGB(0x70, 0x80, 0x90),
				snow						= RGB(0xff, 0xfa, 0xfa),
				spring_green				= RGB(0x00, 0xff, 0x7f),
				steel_blue					= RGB(0x46, 0x82, 0xb4),
				tan							= RGB(0xd2, 0xb4, 0x8c),
				teal						= RGB(0x00, 0x80, 0x80),
				thistle						= RGB(0xd8, 0xbf, 0xd8),
				tomato						= RGB(0xff, 0x63, 0x47),
				turquoise					= RGB(0x40, 0xe0, 0xd0),
				violet						= RGB(0xee, 0x82, 0xee),
				wheat						= RGB(0xf5, 0xde, 0xb3),
				white						= RGB(0xff, 0xff, 0xff),
				white_smoke					= RGB(0xf5, 0xf5, 0xf5),
				yellow						= RGB(0xff, 0xff, 0x00),
				yellow_green				= RGB(0x9a, 0xcd, 0x32),
				button_face_shadow_start	= RGB(0xf5, 0xf4, 0xf2),
				button_face_shadow_end		= RGB(0xd5, 0xd2, 0xca),
				button_face					= RGB(0xd4, 0xd0, 0xc8),
				dark_border					= RGB(0x40, 0x40, 0x40),
				gray_border					= RGB(0x80, 0x80, 0x80),
				highlight					= RGB(0x1c, 0xc4, 0xf7),
			};

			enum class color_alpha_type : ::BYTE{
				opaque				= 0xff,
				translucent			= 0x7a,
				transparent			= 0x00,
			};

			enum class system_color_index_type{
				_3d_dark_shadow					= COLOR_3DDKSHADOW,
				_3d_face						= COLOR_3DFACE,
				_3d_highlight					= COLOR_3DHIGHLIGHT,
				_3d_shadow						= COLOR_3DSHADOW,
				active_border					= COLOR_ACTIVEBORDER,
				active_caption					= COLOR_ACTIVECAPTION,
				app_work_space					= COLOR_APPWORKSPACE,
				backgorund						= COLOR_BACKGROUND,
				button_face						= COLOR_BTNFACE,
				button_highlight				= COLOR_BTNHIGHLIGHT,
				button_shadow					= COLOR_BTNSHADOW,
				button_text						= COLOR_BTNTEXT,
				caption_text					= COLOR_CAPTIONTEXT,
				desktop							= COLOR_DESKTOP,
				gradient_active_caption			= COLOR_GRADIENTACTIVECAPTION,
				gradient_inactive_caption		= COLOR_GRADIENTINACTIVECAPTION,
				gray_text						= COLOR_GRAYTEXT,
				grey_text						= COLOR_GRAYTEXT,
				highlight						= COLOR_HIGHLIGHT,
				highlight_text					= COLOR_HIGHLIGHTTEXT,
				hot_light						= COLOR_HOTLIGHT,
				inactive_border					= COLOR_INACTIVEBORDER,
				inactive_caption				= COLOR_INACTIVECAPTION,
				inactive_caption_text			= COLOR_INACTIVECAPTIONTEXT,
				info_background					= COLOR_INFOBK,
				info_text						= COLOR_INFOTEXT,
				menu							= COLOR_MENU,
				menu_bar						= COLOR_MENUBAR,
				menu_highlight					= COLOR_MENUHILIGHT,
				menu_text						= COLOR_MENUTEXT,
				scroll_bar						= COLOR_SCROLLBAR,
				window							= COLOR_WINDOW,
				window_frame					= COLOR_WINDOWFRAME,
				window_text						= COLOR_WINDOWTEXT,
			};

			enum class msg_peek_type : ::UINT{
				nil					= PM_NOREMOVE,
				remove				= PM_REMOVE,
				dont_yield			= PM_NOYIELD,
			};

			enum class msg_sent_type : ::DWORD{
				nil					= ISMEX_NOSEND,
				send				= ISMEX_SEND,
				notify				= ISMEX_NOTIFY,
				callback			= ISMEX_CALLBACK,
				replied				= ISMEX_REPLIED,
			};

			enum class key_event_state_type : unsigned int{
				nil					= (0 << 0x0000),
				is_system			= (1 << 0x0000),
				is_extended			= (1 << 0x0001),
				was_down			= (1 << 0x0002),
				being_released		= (1 << 0x0003),
				alt_down			= (1 << 0x0004),
				down				= (1 << 0x0005),
				up					= (1 << 0x0006),
				pressed				= (1 << 0x0007),
				dead				= (1 << 0x0008),
			};

			enum class key_state_type : ::UINT{
				nil				= (0 << 0x0000),
				left_shift		= (1 << 0x0000),
				right_shift		= (1 << 0x0001),
				left_ctrl		= (1 << 0x0002),
				right_ctrl		= (1 << 0x0003),
				left_alt		= (1 << 0x0004),
				right_alt		= (1 << 0x0005),
				left_win		= (1 << 0x0006),
				right_win		= (1 << 0x0007),
				caps			= (1 << 0x0008),
				numpad			= (1 << 0x0009),
				insert			= (1 << 0x000A),
				scroll			= (1 << 0x000B),
			};

			enum class mouse_key_state_type : ::UINT{
				nil				= 0,
				left_button		= MK_LBUTTON,
				middle_button	= MK_MBUTTON,
				right_button	= MK_RBUTTON,
				x_button_1		= MK_XBUTTON1,
				x_button_2		= MK_XBUTTON2,
				control			= MK_CONTROL,
				shift			= MK_SHIFT,
			};

			enum class hit_target_type{
				nil						= HTNOWHERE,
				border					= HTBORDER,
				bottom					= HTBOTTOM,
				bottom_left				= HTBOTTOMLEFT,
				bottom_right			= HTBOTTOMRIGHT,
				caption					= HTCAPTION,
				client					= HTCLIENT,
				close					= HTCLOSE,
				error					= HTERROR,
				help					= HTHELP,
				horizontal_scroll		= HTHSCROLL,
				left					= HTLEFT,
				menu					= HTMENU,
				maximize_button			= HTMAXBUTTON,
				minimize_button			= HTMINBUTTON,
				right					= HTRIGHT,
				size					= HTSIZE,
				system_menu				= HTSYSMENU,
				top						= HTTOP,
				top_left				= HTTOPLEFT,
				top_right				= HTTOPRIGHT,
				transparent				= HTTRANSPARENT,
				vertical_scroll			= HTVSCROLL,
			};

			enum class animation_type : ::DWORD{
				show			= 0,
				hide			= AW_HIDE,
				activate		= AW_ACTIVATE,
				roll			= 0,
				slide			= AW_SLIDE,
				fade			= AW_BLEND,
				left_to_right	= AW_HOR_POSITIVE,
				right_to_left	= AW_HOR_NEGATIVE,
				top_to_bottom	= AW_VER_POSITIVE,
				bottom_to_top	= AW_VER_NEGATIVE,
				center			= AW_CENTER,
			};

			enum class show_mode_type{
				hide							= SW_HIDE,
				normal							= SW_NORMAL,
				minimized						= SW_SHOWMINIMIZED,
				maximized						= SW_SHOWMAXIMIZED,
				normal_without_activating		= SW_SHOWNOACTIVATE,
				show							= SW_SHOW,
				minimize						= SW_MINIMIZE,
				maximize						= SW_MAXIMIZE,
				minimized_without_activating	= SW_SHOWMINNOACTIVE,
				without_activating				= SW_SHOWNA,
				restore							= SW_RESTORE,
				default							= SW_SHOWDEFAULT,
				force_minimize					= SW_FORCEMINIMIZE,
			};

			enum class show_reason_type{
				nil							= 0,
				parent_minimized			= SW_PARENTCLOSING,
				other_window_maximized		= SW_OTHERZOOM,
				parent_restored				= SW_PARENTOPENING,
				other_window_restored		= SW_OTHERUNZOOM,
			};

			enum class position_type : ::UINT{
				nil					= 0,
				no_size				= SWP_NOSIZE,
				no_move				= SWP_NOMOVE,
				no_z_order			= SWP_NOZORDER,
				no_redraw			= SWP_NOREDRAW,
				no_activate			= SWP_NOACTIVATE,
				draw_frame			= SWP_DRAWFRAME,
				show				= SWP_SHOWWINDOW,
				hide				= SWP_HIDEWINDOW,
				discard_client		= SWP_NOCOPYBITS,
				no_owner_z_order	= SWP_NOOWNERZORDER,
				no_send_changing	= SWP_NOSENDCHANGING,
				defer_erase			= SWP_DEFERERASE,
				async				= SWP_ASYNCWINDOWPOS,
			};

			enum class window_placement_type : ::UINT{
				nil						= 0,
				async					= WPF_ASYNCWINDOWPLACEMENT,
				set_minimized_position	= WPF_SETMINPOSITION,
				restore_to_maximized	= WPF_RESTORETOMAXIMIZED,
			};

			enum class window_edge_type : ::UINT{
				nil						= 0,
				left					= WMSZ_LEFT,
				right					= WMSZ_RIGHT,
				top						= WMSZ_TOP,
				top_left				= WMSZ_TOPLEFT,
				top_right				= WMSZ_TOPRIGHT,
				bottom					= WMSZ_BOTTOM,
				bottom_left				= WMSZ_BOTTOMLEFT,
				bottom_right			= WMSZ_BOTTOMRIGHT,
			};

			enum class window_size_type{
				restored				= SIZE_RESTORED,
				minimized				= SIZE_MINIMIZED,
				maximized				= SIZE_MAXIMIZED,
				max_show				= SIZE_MAXSHOW,
				max_hide				= SIZE_MAXHIDE,
			};

			enum class data_index_type{
				procedure				= GWLP_WNDPROC,
				instance				= GWLP_HINSTANCE,
				parent					= GWLP_HWNDPARENT,
				id						= GWL_ID,
				styles					= GWL_STYLE,
				extended_styles			= GWL_EXSTYLE,
				user_data				= GWLP_USERDATA,
				dlg_message_result		= DWLP_MSGRESULT,
				dlg_procedure			= DWLP_DLGPROC,
				dlg_user_data			= DWLP_USER,
			};

			enum class layered_attributes_option : ::DWORD{
				nil				= 0,
				use_rgb			= LWA_COLORKEY,
				use_alpha		= LWA_ALPHA,
				all				= LWA_COLORKEY | LWA_ALPHA,
			};

			enum class mouse_activate_type{
				nil							= 0,
				dont_activate				= MA_NOACTIVATE,
				dont_activate_and_eat		= MA_NOACTIVATEANDEAT,
				activate					= MA_ACTIVATE,
				activate_and_eat			= MA_ACTIVATEANDEAT,
			};

			enum class print_option : unsigned int{
				nil					= 0u,
				check_visible		= PRF_CHECKVISIBLE,
				non_client			= PRF_NONCLIENT,
				client				= PRF_CLIENT,
				erase_background	= PRF_ERASEBKGND,
				children			= PRF_CHILDREN,
				owned				= PRF_OWNED,
			};

			enum class menu_state_type : unsigned int{
				nil				= 0,
				disabled		= MFS_DISABLED,
				checked			= MFS_CHECKED,
				highlighted		= MFS_HILITE,
				is_default		= MFS_DEFAULT,
			};

			enum class menu_popup_track_option : unsigned int{
				nil					= 0,
				left_align			= TPM_LEFTALIGN,
				right_align			= TPM_RIGHTALIGN,
				center_align		= TPM_CENTERALIGN,
				top_align			= TPM_TOPALIGN,
				bottom_align		= TPM_BOTTOMALIGN,
				vcenter_align		= TPM_VCENTERALIGN,
				dont_notify			= TPM_NONOTIFY,
				return_cmd			= TPM_RETURNCMD,
				left_button			= TPM_LEFTBUTTON,
				right_button		= TPM_RIGHTBUTTON,
				no_animation		= TPM_NOANIMATION,
				hor_pos_animation	= TPM_HORPOSANIMATION,
				hor_neg_animation	= TPM_HORNEGANIMATION,
				ver_pos_animation	= TPM_VERPOSANIMATION,
				ver_neg_animation	= TPM_VERNEGANIMATION,
				recursive			= TPM_RECURSE,
				right_to_left		= TPM_LAYOUTRTL,
			};

			WINPP_MAKE_OPERATORS(random_string_char_set);
			WINPP_MAKE_OPERATORS(key_event_state_type);
			WINPP_MAKE_OPERATORS(key_state_type);
			WINPP_MAKE_OPERATORS(mouse_key_state_type);
			WINPP_MAKE_OPERATORS(animation_type);
			WINPP_MAKE_OPERATORS(position_type);
			WINPP_MAKE_OPERATORS(placement_type);
			WINPP_MAKE_OPERATORS(layered_attributes_option);
			WINPP_MAKE_OPERATORS(print_option);
			WINPP_MAKE_OPERATORS(menu_state_type);
			WINPP_MAKE_OPERATORS(menu_popup_track_option);
		}
	}
}

#endif /* !WINPP_ENUMERATIONS_H */

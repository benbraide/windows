#include "thread_message_queue.h"

winpp::threading::message_queue::message_queue()
	: event_(L""){}

bool winpp::threading::message_queue::get(msg_type &msg){
	return (::GetMessageW(msg, nullptr, 0u, 0u) != FALSE);
}

bool winpp::threading::message_queue::peek(msg_type &msg, peek_type type){
	return (::PeekMessageW(msg, nullptr, 0u, 0u, static_cast<uint_type>(type)) != FALSE);
}

bool winpp::threading::message_queue::wait(dword_type timeout){
	auto value = event_.native_value();
	return (::MsgWaitForMultipleObjectsEx(1, &value, timeout, QS_ALLINPUT, MWMO_INPUTAVAILABLE) != WAIT_FAILED);
}

bool winpp::threading::message_queue::wake(){
	return event_.set();
}

winpp::threading::message_queue::point_type winpp::threading::message_queue::last_mouse_position(){
	return WINPP_MAKE_MOUSE_POSITION(::GetMessagePos());
}

long winpp::threading::message_queue::last_time(){
	return ::GetMessageTime();
}

bool winpp::threading::message_queue::is_sent(){
	return (::InSendMessage() != FALSE);
}

winpp::threading::message_queue::sent_type winpp::threading::message_queue::sent_info(){
	return static_cast<sent_type>(::InSendMessageEx(nullptr));
}

bool winpp::threading::message_queue::reply(result_type value){
	return (::ReplyMessage(value) != FALSE);
}

void winpp::threading::message_queue::post_quit(int exit_code){
	::PostQuitMessage(exit_code);
}

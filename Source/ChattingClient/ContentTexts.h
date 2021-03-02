#pragma once
#include <string>


enum class e_packet_filter
{
	EPF_LOGIN_OK,
	EPF_ROOM_CREATING_OK,
	EPF_ROOM_EXITING_OK,
	EPF_ROOM_ENTERING_OK,
	EPF_CHAT,
	EPF_WHISPER,
	EPF_SELECT_ROOM_LIST,
	EPF_SELECT_USER_LIST,
	EPF_SELECT_USER_LIST_IN_ROOM,
	EPF_MAX,
};

extern const std::wstring PacketFilterKeywords[9];


//// Fill out your copyright notice in the Description page of Project Settings.
//#include "NetFunction.h"
//
//void UNetFunction::TestCPPCallToBP(AActor* c)
//{
//	FOutputDeviceNull ar;
//	c->CallFunctionByNameWithArguments(TEXT("CallAddChat", L"123"), ar, NULL, true);
//}
//
//
#include "NetFunction.h"
#include <vector>
#include <codecvt>

std::wstring MbsToWcs(const std::string& str)
{
	static std::locale loc("");
	using codecvt_t = std::codecvt<wchar_t, char, mbstate_t>;
	const codecvt_t& codecvt = std::use_facet<codecvt_t>(loc);
	mbstate_t state{};
	std::vector<wchar_t> buf(str.size() + 1);
	const char* in_next = str.c_str();
	wchar_t* out_next = &buf[0];
	codecvt.in(state, str.c_str(), str.c_str() + str.size(), in_next, &buf[0], &buf[0] + buf.size(), out_next);
	return std::wstring(&buf[0]);
}

std::string WcsToMbs(const std::wstring& str)
{
	static std::locale loc("");
	using codecvt_t = std::codecvt<wchar_t, char, mbstate_t>;
	const codecvt_t& codecvt = std::use_facet<codecvt_t>(loc);
	mbstate_t state{};
	std::vector<char> buf((str.size() + 1) * codecvt.max_length());
	const wchar_t* in_next = str.c_str();
	char* out_next = &buf[0];
	codecvt.out(state, str.c_str(), str.c_str() + str.size(), in_next, &buf[0], &buf[0] + buf.size(), out_next);
	return std::string(&buf[0]);
}
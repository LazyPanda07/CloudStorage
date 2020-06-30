#include "pch.h"

#include "UtilityFunction.h"

#include <Windows.h>

using namespace std;

wstring to_wstring(const string& source)
{
	wstring res;

	res.resize(MultiByteToWideChar
	(
		CP_ACP,
		NULL,
		source.data(),
		source.size(),
		res.data(),
		NULL
	));

	MultiByteToWideChar
	(
		CP_ACP,
		NULL,
		source.data(),
		source.size(),
		res.data(),
		res.size()
	);

	return res;
}

string to_string(const wstring& source)
{
	string result;

	result.resize(WideCharToMultiByte
	(
		CP_ACP,
		NULL,
		source.data(),
		source.size(),
		result.data(),
		NULL,
		NULL,
		NULL
	));

	WideCharToMultiByte
	(
		CP_ACP,
		NULL,
		source.data(),
		source.size(),
		result.data(),
		result.size(),
		NULL,
		NULL
	);

	return result;
}
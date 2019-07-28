#pragma once

namespace Galactic3D
{
	typedef int32_t Result;

	static const Result RESULT_ERROR = -1;
	static const Result RESULT_NOTIMPLEMENTED = -2;
	static const Result RESULT_OUTOFMEMORY = -3;
	static const Result RESULT_BADARGUMENT = -4;
	static const Result RESULT_NOTFOUND = -5;
	static const Result RESULT_BADPOINTER = -6;
	static const Result RESULT_BADCALL = -7;

	static const Result RESULT_OK = 0;
	static const Result RESULT_TRUE = 0;
	static const Result RESULT_FALSE = 1;

	inline bool Failed(Result r) { return r < 0; }
}

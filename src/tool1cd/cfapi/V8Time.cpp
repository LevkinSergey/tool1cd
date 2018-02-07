#include "V8Time.h"

using namespace System;

//504911232000000 = ((365 * 4 + 1) * 100 - 3) * 4 * 24 * 60 * 60 * 10000
const int64_t V8Time::EPOCH_START_WIN = 504911232000000;

V8Time V8Time::current_time() {
	SYSTEMTIME st;
	FILETIME ft;

	GetSystemTime(&st);
	SystemTimeToFileTime(&st, &ft);

	V8Time result(ft);

	return result;
}

V8Time::V8Time() {

}

V8Time::V8Time(const int64_t value)
	: _data(value)
{

}

V8Time::V8Time(const System::FILETIME &value) {
	_data = inner_from_file_time(value);
}

FILETIME V8Time::to_file_time() const {
	FILETIME local_file_time;
	FILETIME result;

	int64_t tmp = _data;
	tmp -= EPOCH_START_WIN;
	tmp *= 1000;
	*(int64_t*)&local_file_time = tmp;
	LocalFileTimeToFileTime(&local_file_time, &result);

	return result;
}

void V8Time::from_file_time(const FILETIME &value) {
	_data = inner_from_file_time(value);
}

int64_t V8Time::inner_from_file_time(const FILETIME &value) {
	FILETIME local_file_time;

	FileTimeToLocalFileTime(&value, &local_file_time);
	int64_t tmp = *(int64_t*)&local_file_time;
	tmp /= 1000;
	tmp += EPOCH_START_WIN;

	return tmp;
}

size_t V8Time::write_to_stream(TMemoryStream *out_stream) const {
	return out_stream->Write(&_data, sizeof(_data));
}

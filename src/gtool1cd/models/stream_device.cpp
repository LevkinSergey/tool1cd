#include "stream_device.h"
#include "QDebug"

StreamDevice::StreamDevice(TStream *stream)
    : stream(stream)
{
	stream->SetPosition(0);
}

StreamDevice::~StreamDevice()
{
	delete stream;
	stream = nullptr;
}

qint64 StreamDevice::readData(char *data, qint64 maxlen)
{
	return stream->Read(data, maxlen);
}

qint64 StreamDevice::writeData(const char *data, qint64 len)
{
	return stream->Write(data, len);
}

qint64 StreamDevice::size() const
{
	return stream->GetSize();
}

qint64 StreamDevice::pos() const
{
	return stream->GetPosition();
}

bool StreamDevice::seek(qint64 pos)
{
	stream->SetPosition(pos);
	return true;
}

TStream *StreamDevice::get_stream()
{
	return stream;
}

void StreamDevice::close()
{
	emit aboutToClose();
	setErrorString(QString(""));
	stream->Close();
	setOpenMode(NotOpen);
}

bool StreamDevice::isSequential() const
{
	return false;
}


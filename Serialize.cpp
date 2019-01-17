#ifdef LINUX
#include <arpa/inet.h>
#else
#include <winsock.h>
#endif
#include "Serialize.h"

void Serialize::Serialize(std::ostream & a, const uint32_t s)
{
	auto n = htonl(s);
	a.write((const char*)&n, sizeof n);
}

void Serialize::Serialize(std::ostream & a, const uint16_t s)
{
	auto n = htons(s);
	a.write((const char*)&n, sizeof n);
}

void Serialize::Serialize(std::ostream & a, const int32_t s)
{
	auto n = htonl(s);
	a.write((const char*)&n, sizeof n);
}

void Serialize::Serialize(std::ostream & a, const int16_t s)
{
	auto n = htons(s);
	a.write((const char*)&n, sizeof n);
}

void Serialize::Serialize(std::ostream & a, const std::string & s)
{
	Serialize(a, s.size());
	a.write(s.data(), s.length());
}

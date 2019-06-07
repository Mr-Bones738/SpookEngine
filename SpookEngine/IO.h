#ifndef _H_SEIO_
#define _H_SEIO_
#include <string>
class SEIO {
public:
	SEIO();
	void Init();
	std::string GetContentPath(const std::string& in) const;
private:
	struct Impl;
	Impl* pImpl;
};
#endif
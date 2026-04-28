#pragma once

#include <string>

namespace dl::util
{
	std::string demangle(const std::string& mangledName);
}

#include <dl_raii/impl/util.ipp>

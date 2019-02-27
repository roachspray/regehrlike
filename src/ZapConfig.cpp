#include <string>
#include <map>

#include <iostream>
#include <fstream>

#include <json/config.h>
#include <json/value.h>
#include <json/reader.h>

#include "ZapConfig.hpp"

ZapConfig::ZapConfig(std::string c)
{
	_configFile = c;
	std::ifstream cStream;
	cStream.open(_configFile);

	cStream >> _configDict;
	cStream.close();

//	std::cout << "root: " << root["dummy"].asString() << "\n";

	// We would eat, parse, and set whatever internal variables need to be set.
//	_configMap["dummy"] = "MDK4ALL";
}


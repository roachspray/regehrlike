#ifndef	__ZAPCONFIG_HPP
#define	__ZAPCONFIG_HPP

#include <string>
#include <map>
#include <json/value.h>

class ZapConfig {
	private:
		std::string	_configFile;
		std::map<std::string, std::string> _configMap;	// XXX THIS IS JUST A PLACEHOLDER FOR NOW.
		Json::Value _configDict;

	public:
		ZapConfig(std::string c);
		void	setConfigFile(std::string configFile) { _configFile = configFile; }
		std::string	getConfigFile() { return _configFile; }
};
#endif	// !__ZAPCONFIG_HPP

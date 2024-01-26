#pragma once
#include <string>
#include <memory>

class c_cfg_sys {
public:
	void save_cfg( const std::string& filename );
	void load_cfg( const std::string& filename );
};

extern c_cfg_sys g_cfg;
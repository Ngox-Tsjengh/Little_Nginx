#ifndef __NGX_CONF_H__
#define __NGX_CONF_H__

#include <vector>

typedef struct _NGX_ConfItem {
	char Name[32];
	char Content[256];
}NGX_ConfItem, *P_NGX_ConfItem;

class NGX_Config {
	
private:		//!Implement singelton by making the constructor private
	NGX_Config();

	static NGX_Config *conf_instance;

public:
	~NGX_Config();
			
	static NGX_Config* GetInstance() { //!Double Checking Lock
		// First Lock
		if (conf_instance == NULL) {
			// Double Lock
			if (conf_instance == NULL) {
				conf_instance = new NGX_Config();
				
			}
			// Release Lock
		}
		return conf_instance;
	}
	class NGX_Conf_Free {
	public:
		~NGX_Conf_Free() {
			if (NGX_Config::conf_instance) {
				delete NGX_Config::conf_instance;
				NGX_Config::conf_instance = NULL;
			}
		}
	};
// ------------------------------------------------------
public:
	bool Load(const char *pConfName);	//Load Configuration File

public:
	//Vector to store configuration items
	std::vector<P_NGX_ConfItem> m_ConfigItemList;

};

#endif //__NGX_CONF_H__

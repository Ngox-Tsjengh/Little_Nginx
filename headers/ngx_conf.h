#ifndef __NGX_CONF_H__
#define __NGX_CONF_H__

#include <vector>
using std::vector;

typedef struct _NGX_ConfItem {
	char Name[32];
	char Content[256];
}NGX_ConfItem, *pNGX_ConfItem;

class NGX_Config {
	
private:		//!Implement singelton by making the constructor private
	NGX_Config();

	static NGX_Config *m_instance;

public:
	~NGX_Config();
			
	static NGX_Config* GetInstance() { //!Double Checking Lock
		// First Lock
		if (m_instance == NULL) {
			// Double Lock
			if (m_instance == NULL) {
				m_instance = new NGX_Config();
				
			}
			// Release Lock
		}
		return m_instance;
	}
	class NGX_Conf_Free {
	public:
		~NGX_Conf_Free() {
			if (NGX_Config::m_instance) {
				delete NGX_Config::m_instance;
				NGX_Config::m_instance = NULL;
			}
		}
	};

// ------------------------------------------------------
public:
	bool Load(const char *pConfName);	//Load Configuration File
	const char *GetString(const char *p_ItemName); //Read string
	int  GetInt(const char *p_ItemName,const int def); //Read Integer

public:
	//Vector to store configuration items
	vector<pNGX_ConfItem> m_ConfigItemList;

};

#endif //__NGX_CONF_H__

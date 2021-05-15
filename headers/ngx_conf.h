#ifndef __NGX_CONF_H__
#define __NGX_CONF_H__


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
};

#endif //__NGX_CONF_H__

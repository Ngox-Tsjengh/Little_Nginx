#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <vector>

#include "ngx_conf.h"

NGX_Config *NGX_Config::conf_instance = NULL;

/* Constructor */
NGX_Config::NGX_Config() { }

/* Destructor */
NGX_Config::~NGX_Config() {

}

/* Load Configuration File */
bool NGX_Config::Load(const char *pConfName) {

}


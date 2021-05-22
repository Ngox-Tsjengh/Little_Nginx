#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ngx_conf.h"
#include "ngx_string.h"

//Assign Value for Static Member
NGX_Config *NGX_Config::m_instance = NULL;

/* Constructor */
NGX_Config::NGX_Config() { }

/* Destructor */
NGX_Config::~NGX_Config() {
	vector<pNGX_ConfItem>::iterator item;	
	//Relaese all memory occupied by configuration
	for(item = m_ConfigItemList.begin(); item != m_ConfigItemList.end(); ++item)
	{		
		delete (*item);
	}
	m_ConfigItemList.clear(); 
    return;
}

/* Load Configuration File
 *
 * [example]
 * ListenPort = 80
 *
 * In the left of the = is the name
 * In the right of the = is the content
 *
 * One line stores one configuration
 * One line should have less than 512 characters
 *
 * ';' and '#' are annotations
 * ' ' ';' '\t' '\n': lines begin with then will be ignored
 * [Name] are ignored, they indicates type of configurations 
 *
 * */
bool NGX_Config::Load(const char *pConfName) {

    FILE *fp;
    fp = fopen(pConfName,"r");
    if(fp == NULL) return false;

    char  linebuf[512];  //Buffer to store each configuration, should be less than 5512 characters 
    
    while(!feof(fp))  //Check if the file is ended
    {    
        if(fgets(linebuf,512,fp) == NULL) continue; //read one line from file, and it should be less than 512 characters
            
        if(linebuf[0] == 0) continue; //begin with /0

        if(*linebuf==';' || *linebuf==' ' || *linebuf=='#' || *linebuf=='\t'|| *linebuf=='\n') continue; //Annotations and empty lines
        

		//Remove Newline('\n',10) Carriage-Return(‘\t’,13) and Space(' ',32)
		//At the end of the line
    lblprocstring:
		if(strlen(linebuf) > 0)
		{
			if(linebuf[strlen(linebuf)-1] == 10 || linebuf[strlen(linebuf)-1] == 13 || linebuf[strlen(linebuf)-1] == 32) 
			{
				linebuf[strlen(linebuf)-1] = 0;
				goto lblprocstring;
			}		
		}
        if(linebuf[0] == 0) continue;
        if(*linebuf=='[') continue;	//type annotations

        char *ptmp = strchr(linebuf,'=');
        if(ptmp != NULL)
        {
            pNGX_ConfItem p_confitem = new NGX_ConfItem;
            memset(p_confitem,0,sizeof(NGX_ConfItem));

            strncpy(p_confitem->Name,linebuf,(int)(ptmp-linebuf)); //string in the left of "=" is copied to p_confitem->Name
            strcpy(p_confitem->Content,ptmp+1); // string in the right of "=" is copied to p_confitem->Content

            Rtrim(p_confitem->Name);
			Ltrim(p_confitem->Name);
			Rtrim(p_confitem->Content);
			Ltrim(p_confitem->Content);

            //printf("itemname=%s | itemcontent=%s\n",p_confitem->ItemName,p_confitem->ItemContent);            
            m_ConfigItemList.push_back(p_confitem);  
        } //end if
    } //end while(!feof(fp)) 

    fclose(fp); //close file
    return true;
}

/* Read Configuration String due to ItemName
 * 
 *	@p_ItemName: the name of the item that need to be read
 *
 *	if success, return the pointer to cointent read from file
 *
 *	*/
const char *NGX_Config::GetString(const char *p_ItemName)
{
	std::vector<pNGX_ConfItem>::iterator item;	
	for(item = m_ConfigItemList.begin(); item != m_ConfigItemList.end(); ++item)
	{	
		if(strcasecmp( (*item)->Name,p_ItemName) == 0)
			return (*item)->Content;
	}
	return NULL;
}

/* Read Configuration Integer due to ItemName
 * 
 *	@p_ItemName: the name of the item that need to be read
 *	@def: default return number
 *
 *	if success, return the number read from file
 *	if fail, return default number
 *
 *	*/
int NGX_Config::GetInt(const char *p_ItemName,const int def)
{
	std::vector<pNGX_ConfItem>::iterator item;	
	for(item = m_ConfigItemList.begin(); item !=m_ConfigItemList.end(); ++item)
	{	
		if(strcasecmp( (*item)->Name,p_ItemName) == 0)
			return atoi((*item)->Content);
	}
	return def;
}


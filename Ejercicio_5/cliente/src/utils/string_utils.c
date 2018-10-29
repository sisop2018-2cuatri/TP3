#include <string.h>
#include <ctype.h>
#include <string_utils.h>

int prefix(const char *pre, const char *str)
{
	return *pre!='\0' && *str!='\0' ? (strncmp(pre, str, strlen(pre))==0):0;
}

int remove_trailing_spaces(char* palabra){
	
	char* fin_pal;
	char* pos_ant;

	if(strlen(palabra)==0)
		return 0;

	fin_pal = palabra + (strlen(palabra)-1);
	pos_ant = fin_pal;

	while(isspace(*fin_pal)){
		pos_ant = fin_pal;
		fin_pal--;
	}
	*pos_ant = '\0';

	return 1;
}

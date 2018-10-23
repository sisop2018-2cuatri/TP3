#include <string.h>
#include <string_utils.h>

int prefix(const char *pre, const char *str)
{
	return *pre!='\0' && *str!='\0' ? (strncmp(pre, str, strlen(pre))==0):0;
}

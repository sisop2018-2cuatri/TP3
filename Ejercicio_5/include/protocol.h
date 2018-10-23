#ifndef PROTOCOL_H
#define PROTOCOL_H

typedef struct config{
	int size;
	char name[20];
} s_config;

int load_config(char*, s_config*);

#endif

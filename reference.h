//
// Created by maxim on 15.10.19.
//
#define DEF_CMD(name, num, bytes) CMD_##name=num,

enum Commands{

#include "commands.h"

};
#undef DEF_CMD
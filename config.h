//
// Created by maxim on 28.10.19.
//

const int N_REGISTERS  = 4;

const int MAX_MARK_NAME_LENGTH = 64;

const int MAX_ARG_LENGTH = 64;

const int MAX_REGISTER_NAME_LENGTH  = 3;

const int MAX_COMMAND_NAME_LENGTH = 5;

const int GRAPHICS_BUFFER_SIZE = 256;

const int GRAPHICS_LINE_SIZE  = 16;

enum Arg_types {
    NO_ARGS = 0,
    IMMED  = 1,
    REG  = 2,
    MEM_IMMED  = 4,
    MEM_REG  = 5
};

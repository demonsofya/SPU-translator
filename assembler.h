#ifndef ASSEMBLER_H_INCLUDED
#define ASSEMBLER_H_INCLUDED

//-----------------------------------------------------------------------------

#define DEBUG

#ifdef DEBUG
    #define ON_DEBUG(...) __VA_ARGS__
#else
    #define ON_DEBUG(...)
#endif

//-----------------------------------------------------------------------------

#define COMPILE_ASM
int CountStringHashDJB2(const char *curr_string);

#include "../include/commands.h"

//-----------------------------------------------------------------------------


#define Return_If_ASM_Error(asm_struct, error)                                      \
    {                                                                               \
        int asm_error = ASMVerify(asm_struct);                                      \
        if (error != NO_ASM_ERROR || asm_error != NO_ASM_ERROR) {                   \
            ASMDump(error, asm_struct, __FILE__, __FUNCTION__, __LINE__);           \
            return error;                                                           \
        }                                                                           \
    }

#define Return_ASM_Error(asm_struct, error)                                         \
    {                                                                               \
        int asm_error = ASMVerify(asm_struct);                                      \
        if (error != NO_ASM_ERROR || asm_error != NO_ASM_ERROR)                     \
            ASMDump(error, asm_struct, __FILE__, __FUNCTION__, __LINE__);           \
        return error;                                                               \
    }

const int LABELS_ARRAY_SIZE = 10;
const int MAX_COMMAND_SIZE = 20;

//-----------------------------------------------------------------------------

struct StringLabel_t {

    int label_hash;
    char label_name[MAX_COMMAND_SIZE];
    int label_num;
};

//-----------------------------------------------------------------------------

struct ASM_t {

    int *output_arr;
    int command_num;

    char **ptrs_to_code_lines_array;
    int ptrs_to_code_lines_array_size;

    char curr_command_string[MAX_COMMAND_SIZE];

    StringLabel_t string_labels_array[LABELS_ARRAY_SIZE];
    int string_labels_counter;
};

//-----------------------------------------------------------------------------

char **GetStringsPtrArrayFromFile(const char *file_name, int *ptr_array_size);
void FillCurrentOpcode(ASM_t *asm_stuct, char *argument_string, int *error);

//-----------------------------------------------------------------------------

void FillCurrentOpcode(int *output_arr, char *command_string, int *labels,
                       int *command_num, char *argument_string, int *error);

//-----------------------------------------------------------------------------

int FillCodeArray(ASM_t *asm_struct);

void FillCommandWithNumberArgument(ASM_t *asm_struct, char *arg_command_string, int *error);
void FillCommandWithRegisterArgiment(ASM_t *asm_struct, char *arg_command_string, int *error);
void FillCommandWithRAMArgiment(ASM_t *asm_struct, char *arg_command_string, int *error);

void FillLabel(int *counter, ASM_t *asm_struct, int *error);

//-----------------------------------------------------------------------------

void LabelsDump(StringLabel_t *string_labels_array, int labels_counter);
int ASMVerify(ASM_t *asm_struct);
void ASMDump(int *error, ASM_t *asm_struct, const char *file_name, const char *func_name, int line);

//-----------------------------------------------------------------------------

int *CreateCodeArray(const char *input_file_name, int *commands_count);

void CreateBinaryFile(const char *file_name, int commands_count, int *output_arr);
void CreateNormalFile(const char *file_name, int commands_count, int *output_arr);

//-----------------------------------------------------------------------------

bool CheckIfCommandHaveArgument(int command);
bool CheckIfCommandLabel(char *command);

int CheckRegister(char *reg, int *error);

//-----------------------------------------------------------------------------

bool CheckStrings();

//=============================================================================

enum AsmErrors {

    NO_ASM_ERROR                    = 0,
    COMMAND_ASM_ERROR               = 1 << 0,
    REGISTER_ASM_ERROR              = 1 << 1,
    NUMBER_ARGUMENT_ASM_ERROR       = 1 << 2,
    LABELS_ASM_ERROR                = 1 << 3,
    RAM_ARGUMENT_ERROR              = 1 << 4,
    SINTAXYS_ASM_ERROR              = 1 << 5,
    POINTER_ASM_ERROR               = 1 << 6,
    OUTPUT_ARRAY_POINTER_ASM_ERROR  = 1 << 7,
    INPUT_ARRAY_POINTER_ASM_ERROR   = 1 << 8,
    INPUT_ARRAY_SIZE_ASM_ERROR      = 1 << 9

};

//-----------------------------------------------------------------------------

/*
static Command_t commands_array[COMMANDS_COUNT] = {

    {"HLT",     HALT_COMMAND,               NO_ARGUMENT,         },
    {"PUSH",    PUSH_COMMAND,               NUMBER_ARGUMENT,     },
    {"OUT",     OUTPUT_COMMAND,             NO_ARGUMENT,         },
    {"ADD",     ADDICTION_COMMAND,          NO_ARGUMENT,         },
    {"SUB",     SUBTRACTION_COMMAND,        NO_ARGUMENT,         },
    {"MUL",     MULTIPLICATION_COMMAND,     NO_ARGUMENT,         },
    {"DIV",     DIVISION_COMMAND,           NO_ARGUMENT,         },
    {"POW",     POW_COMMAND,                NO_ARGUMENT,         },
    {"SQRT",    SQUARE_ROOT_COMMAND,        NO_ARGUMENT,         },
    {"PUSHREG", PUSH_REGISTER_COMMAND,      REGISTER_ARGUMENT,   },
    {"POPREG",  POP_REGISTER_COMMAND,       REGISTER_ARGUMENT,   },
    {"JUMP",    JUMP_COMMAND,               NUMBER_ARGUMENT,     },
    {"IN",      IN_COMMAND,                 NO_ARGUMENT,         },
    {"JB",      JUMP_BELOW_COMMAND,         NUMBER_ARGUMENT,     },
    {"JBE",     JUMP_BELOW_EQUAL_COMMAND,   NUMBER_ARGUMENT,     },
    {"JA",      JUMP_ABOVE_COMMAND,         NUMBER_ARGUMENT,     },
    {"JAE",     JUMP_ABOVE_EQUAL_COMMAND,   NUMBER_ARGUMENT,     },
    {"JE",      JUMP_EQUAL_COMMAND,         NUMBER_ARGUMENT,     },
    {"JNE",     JUMP_NOT_EQUAL_COMMAND,     NUMBER_ARGUMENT,     },
    {"CALL",    CALL_FUNCTION_COMMAND,      NUMBER_ARGUMENT,     },
    {"RET",     RETERN_COMMAND,             NO_ARGUMENT,         },
    {"PUSHM",   PUSH_MEMORY_COMMAND,        RAM_ARGUMENT,        },
    {"POPM",    POP_MEMORY_COMMAND,         RAM_ARGUMENT,        },
    {"DRAW",    DRAW_COMMAND,               NO_ARGUMENT,         }

};
*/


//-----------------------------------------------------------------------------

//static StringLabel_t string_labels_array[LABELS_ARRAY_SIZE] = {};

//-----------------------------------------------------------------------------

#endif // ASSEMBLER_H_INCLUDED

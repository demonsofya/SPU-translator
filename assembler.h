#ifndef ASSEMBLER_H_INCLUDED
#define ASSEMBLER_H_INCLUDED

//-----------------------------------------------------------------------------

//#define DEBUG

#ifdef DEBUG
    #define ON_DEBUG(...) __VA_ARGS__
#else
    #define ON_DEBUG(...)
#endif

const int LABELS_ARRAY_SIZE = 10;

//-----------------------------------------------------------------------------

char **GetStringsPtrArrayFromFile(const char *file_name, int *ptr_array_size);
int GetCurrentOpcode(const char* command);

//-----------------------------------------------------------------------------

int CheckRegister(char *reg);
void TranslateCommand(int *output_arr, int current_opcode, char *command_string,
                  int *labels, int *command_num);

//-----------------------------------------------------------------------------

void FillCommandWithNumberArgument(int *output_arr, char *command_string, int *labels, int *command_num);
int FillCodeArray(int *output_arr, int ptr_array_size, char **text_ptr_array, int *labels);
void FillCommandWithRegisterArgiment(int *output_arr, char *command_string,
                                     int *labels, int *command_num);
void FillCommandWithASMArgiment(int *output_arr, char *command_string, int *labels, int *command_num);
void FillLabel(char *command, int *labels, int command_num, int *counter);

//-----------------------------------------------------------------------------

void LabelsDump(int *labels);

//-----------------------------------------------------------------------------

int *CreateCodeArray(const char *input_file_name, int *commands_count);

void CreateBinaryFile(const char *file_name, int commands_count, int *output_arr);
void CreateNormalFile(const char *file_name, int commands_count, int *output_arr);

//-----------------------------------------------------------------------------

bool CheckIfCommandHaveArgument(int command);
bool CheckIfCommandLabel(char *command);

//=============================================================================

enum AsmErrors {
    No_Asm_Error = 0,
    Command_Asm_Error = 1111,      // значения такие, чтобы было видно, что что-то пошло не так
    Register_Asm_Error = 2655

};

//-----------------------------------------------------------------------------

#endif // ASSEMBLER_H_INCLUDED

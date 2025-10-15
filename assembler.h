#ifndef ASSEMBLER_H_INCLUDED
#define ASSEMBLER_H_INCLUDED

char **GetStringsPtrArray(const char *file_name, int *ptr_array_size);

int GetCurrentOpcode(const char* command);
int *CreateCodeArray(const char *input_file_name, int *commands_count);

void CreateBinaryFile(const char *file_name, int commands_count, int *output_arr);
void CreateNormalFile(const char *file_name, int commands_count, int *output_arr);

#endif // ASSEMBLER_H_INCLUDED

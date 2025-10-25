#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#include "lib/onegin/buffer.h"
//#include "buffer.h"
//#include "../include/commands.h"
#include "assembler.h"
#include "string.h"

#define LOGFILE stderr

//=============================================================================

char **GetStringsPtrArrayFromFile(const char *file_name, int *ptrs_to_code_lines_array_size) {

    assert(file_name);
    assert(ptrs_to_code_lines_array_size);

    size_t file_size = 0;
    char *buffer = GetBuffer(file_name, &file_size);

    assert(buffer);
ON_DEBUG(printf("%s\n\n", buffer));


    *ptrs_to_code_lines_array_size = StringsCount(buffer);
    char **strings_ptr_array = GetStringPtrArray(buffer, *ptrs_to_code_lines_array_size);

//ON_DEBUG(printf("First string is %s\n\n", strings_ptr_array[0]));

    assert(strings_ptr_array);

    return strings_ptr_array;
}


//=============================================================================

int FillCurrentOpcode(ASM_t *asm_struct, char *argument_string, int *error) {

    Return_If_ASM_Error(*asm_struct, *error);
    assert(argument_string);

ON_DEBUG(printf("Curr command is %s number is %d\n", asm_struct->curr_command_string, asm_struct->command_num));

    int curr_hash = CountStringHashDJB2(asm_struct->curr_command_string);

    for (int curr_num = 0; curr_num < COMMANDS_COUNT; curr_num++)

        //if (strcmp(command_string, commands_array[curr_num].command_name) == 0) {
        if (commands_array[curr_num].hash == curr_hash) {

            if (strcmp(asm_struct->curr_command_string, commands_array[curr_num].command_name) != 0)
                continue;

            asm_struct->output_arr[asm_struct->command_num++] =
                                                            commands_array[curr_num].command_number;

            if (commands_array[curr_num].agrument_type == NUMBER_ARGUMENT)
                FillCommandWithNumberArgument(asm_struct, argument_string, error);

            else if (commands_array[curr_num].agrument_type == REGISTER_ARGUMENT)
                FillCommandWithRegisterArgiment(asm_struct, argument_string, error);

            else if (commands_array[curr_num].agrument_type == RAM_ARGUMENT)
                FillCommandWithRAMArgiment(asm_struct, argument_string, error);

            Return_ASM_Error(*asm_struct, *error);

        }

    *error |= COMMAND_ASM_ERROR;

    Return_ASM_Error(*asm_struct, *error);
}

//=============================================================================

int CheckRegister(char *reg, int *error) {

    if (error == NULL)
        return REGISTER_ASM_ERROR;

    if (reg == NULL)
        return REGISTER_ASM_ERROR;

        int curr_hash = CountStringHashDJB2(reg);

ON_DEBUG(printf("Register is %s, size is %d\n\n", reg, strlen(reg)));

    for (int curr_num = 0; curr_num < REGISTERS_COUNT; curr_num++) {

        if (curr_hash == registers_array[curr_num].reg_hash)

            if (strcmp(reg, registers_array[curr_num].reg_name) == 0)
                return registers_array[curr_num].reg_num;

ON_DEBUG(printf("Curr register number %d is %s\n", curr_num, registers_array[curr_num].reg_name));

    }

    *error |= REGISTER_ASM_ERROR;
ON_DEBUG(printf("Error with register. Register is %s\n", reg));
    return REGISTER_ASM_ERROR;
}


//=============================================================================


int FillCommandWithNumberArgument(ASM_t *asm_struct, char *arg_command_string, int *error) {

    Return_If_ASM_Error(*asm_struct, *error);
    assert(arg_command_string); // Катя: добавлен ассерт

    int curr_number = 0, readen_elements_count = 0;

    if (CheckIfCommandLabel(arg_command_string + 1)) {  // скип пробела
        char curr_string_label[MAX_COMMAND_SIZE] = "";
        readen_elements_count = sscanf(arg_command_string, " %*c%s", curr_string_label);

ON_DEBUG(printf("Current Label argument is %s\n", curr_string_label));

        int curr_label_hash = CountStringHashDJB2(curr_string_label);

        for (int curr_num = 0; curr_num < LABELS_ARRAY_SIZE; curr_num++) {

            if (curr_label_hash == asm_struct->string_labels_array[curr_num].label_hash) {

                if (strcmp(curr_string_label, asm_struct->string_labels_array[curr_num].label_name) == 0) {
                    asm_struct->output_arr[asm_struct->command_num++] =
                                asm_struct->string_labels_array[curr_num].label_num;

                    Return_ASM_Error(*asm_struct, *error);
                }
            }
        }

        asm_struct->command_num++;

ON_DEBUG(printf("Label string argument is %s\n", curr_string_label));

    } else {
        readen_elements_count = sscanf(arg_command_string, "%d", &curr_number);
        asm_struct->output_arr[asm_struct->command_num++] = curr_number;
    }

    if (readen_elements_count == 0)
        *error |= NUMBER_ARGUMENT_ASM_ERROR;

    Return_ASM_Error(*asm_struct, *error);
}


//-----------------------------------------------------------------------------

int FillCommandWithRegisterArgiment(ASM_t *asm_struct, char *arg_command_string, int *error) {

    Return_If_ASM_Error(*asm_struct, *error);

    int readen_elements_count = 0;

    char reg[10] = "";

    readen_elements_count = sscanf(arg_command_string, " %s", reg);

    if (readen_elements_count != 1)
        *error |= REGISTER_ASM_ERROR;

    asm_struct->output_arr[asm_struct->command_num++] = CheckRegister(reg, error);

    Return_ASM_Error(*asm_struct, *error);
}


//-----------------------------------------------------------------------------

int FillCommandWithRAMArgiment(ASM_t *asm_struct, char *arg_command_string, int *error) {

    Return_If_ASM_Error(*asm_struct, *error);
    assert(arg_command_string);

    char reg[10] = "";
    char end_symbol = '\0';

    int readen_elements_count = 0;

    readen_elements_count = sscanf(arg_command_string, " [%[A-Z]%c", reg, &end_symbol);
    // Катя: проверка существования имени регистра (ZX - не выдаст ошибку)  // Соня: выдаст)
ON_DEBUG(printf("Curr RAM register is %s\n", reg));

    if (end_symbol != ']' || readen_elements_count != 2) {
        *error |= SINTAXYS_ASM_ERROR;
ON_DEBUG(printf("Wrong register sintaxys.\n"));
        asm_struct->command_num++;

    } else {
        asm_struct->output_arr[asm_struct->command_num++] = CheckRegister(reg, error);
    }

    Return_ASM_Error(*asm_struct, *error);
}

//=============================================================================

bool CheckIfCommandLabel(char *command) {

    assert(command);

    if (command[0] == ':')
        return true;

    return false;
}

//-----------------------------------------------------------------------------

int FillLabel(int *counter, ASM_t *asm_struct, int *error) {

    Return_If_ASM_Error(*asm_struct, *error);

    int readen_elements_count = 0;

    char curr_string_label[MAX_COMMAND_SIZE] = "";
    readen_elements_count = sscanf(asm_struct->curr_command_string, "%*c%s", curr_string_label);

    asm_struct->string_labels_array[asm_struct->string_labels_counter].label_num = asm_struct->command_num;

    strncpy(asm_struct->string_labels_array[asm_struct->string_labels_counter].label_name,
            curr_string_label, MAX_COMMAND_SIZE);

    asm_struct->string_labels_array[asm_struct->string_labels_counter].label_hash = CountStringHashDJB2(curr_string_label);

    asm_struct->string_labels_counter++;

ON_DEBUG(printf("Label name is %s, current command is %d\n", curr_string_label, asm_struct->command_num));

    if (readen_elements_count != 1)
        *error |= LABELS_ASM_ERROR;

    //asm_struct->labels[label_number] = asm_struct->command_num;
    (*counter)++;

    Return_ASM_Error(*asm_struct, *error);
}

//=============================================================================

int FillCodeArray(ASM_t *asm_struct) {

    assert(asm_struct);

    //CountHashTable();

    //char command_string[10] = "";
    int counter = 0;
    asm_struct->command_num = 0;
    asm_struct->string_labels_counter = 0;

    CheckCommandsArray();

    while (counter < asm_struct->ptrs_to_code_lines_array_size) {
        int command_size = 0;
        int readen_elements = sscanf(asm_struct->ptrs_to_code_lines_array[counter], "%s%n",
               asm_struct->curr_command_string, &command_size);

        int error = NO_ASM_ERROR;

        if (readen_elements == 0 || isspace(asm_struct->ptrs_to_code_lines_array[counter][0])) {
            counter++;
            continue;
        }

        if (CheckIfCommandLabel(asm_struct->curr_command_string)) {
            FillLabel(&counter, asm_struct, &error);
            continue;
        }

        FillCurrentOpcode(asm_struct, asm_struct->ptrs_to_code_lines_array[counter] + command_size,
                          &error);

        Return_If_ASM_Error(*asm_struct, error);

        counter++;

    }

    return asm_struct->command_num;
}

//-----------------------------------------------------------------------------

bool CheckCommandsArray() {

    bool result = true;

    for (int curr_num = 0; curr_num < COMMANDS_COUNT; curr_num++)
        if (commands_array[curr_num].command_number != curr_num) {
            fprintf(LOGFILE, "%s:%d error. Position in array is %d, command is %s, command number is %d\n%s\n",
                    commands_array[curr_num].file_name_of_source_code, commands_array[curr_num].line_number_in_source_code,
                    curr_num, commands_array[curr_num].command_name, commands_array[curr_num].command_number,
                    commands_array[curr_num].line_content_in_source_code);

            result = false;
        }

    return result;
}

//-----------------------------------------------------------------------------

int *CreateCodeArray(const char *input_file_name, int *commands_count) {

    assert(input_file_name);
    assert(commands_count);

    //int labels[LABELS_ARRAY_SIZE] = {};

    ASM_t asm_struct = {};

    asm_struct.ptrs_to_code_lines_array_size = 0;
    asm_struct.ptrs_to_code_lines_array = GetStringsPtrArrayFromFile(input_file_name,
                                                         &asm_struct.ptrs_to_code_lines_array_size);

ON_DEBUG(printf("Strings count is %d\n\n", asm_struct.ptrs_to_code_lines_array_size));

    asm_struct.output_arr = (int *) calloc(asm_struct.ptrs_to_code_lines_array_size * 2 + SIZE_OF_HEADER_IN_BYTES,
                                     sizeof(int));


    asm_struct.output_arr[0] = DEFAULT_INFORMATION.version;
    asm_struct.output_arr[1] = DEFAULT_INFORMATION.signature;

    asm_struct.output_arr += SIZE_OF_HEADER_IN_BYTES;

    for (int i = 0; i < 2; i++)
        asm_struct.command_num = FillCodeArray(&asm_struct);

    asm_struct.output_arr -= SIZE_OF_HEADER_IN_BYTES;

    *commands_count = asm_struct.command_num;

ON_DEBUG(LabelsDump(asm_struct.string_labels_array, asm_struct.string_labels_counter));

    return asm_struct.output_arr;
}

//=============================================================================

void LabelsDump(StringLabel_t *string_labels_array, int labels_counter) {

    if (string_labels_array == NULL) {
        fprintf(LOGFILE, "ERROR: labels pointer is wrong.\n\n");
        return;
    }

    fprintf(LOGFILE, "--------LABELS---------\n\nLabels counter is %d\n", labels_counter);

    for (int curr_num = 0; curr_num < LABELS_ARRAY_SIZE; curr_num++)
        fprintf(LOGFILE, "[%d] %s\n", string_labels_array[curr_num].label_num,
                                      string_labels_array[curr_num].label_name);

    fprintf(LOGFILE, "\n-----------------------\n");
}

//-----------------------------------------------------------------------------

int ASMVerify(ASM_t *asm_struct) {

    int error = NO_ASM_ERROR;

    if (asm_struct == 0)
        return POINTER_ASM_ERROR;

    if (asm_struct->output_arr == 0)
        error |= OUTPUT_ARRAY_POINTER_ASM_ERROR;

    if (asm_struct->ptrs_to_code_lines_array == 0)
        error |= INPUT_ARRAY_POINTER_ASM_ERROR;

    if (asm_struct->ptrs_to_code_lines_array_size <= 0)
        error |= INPUT_ARRAY_SIZE_ASM_ERROR;

    if (asm_struct->string_labels_counter < 0)
        error |= LABELS_ASM_ERROR;

    return error;
}

//-----------------------------------------------------------------------------

void ASMDump(int *error, ASM_t *asm_struct, const char *file_name, const char *func_name, int line) {

    fprintf(LOGFILE, "============ASM DUMP=============\n\n");

    if (error == NULL || asm_struct == NULL) {
        fprintf(LOGFILE, "ERROR: Wrong pointer. DUMP ended.\n");
        return;
    }

    if (file_name == NULL || func_name == NULL)
        fprintf(LOGFILE, "ERROR: Wrong file_name or func_name pointer");
    else
        fprintf(LOGFILE, "ASMDump() from %s at %s:%d:\n", file_name, func_name, line);

    if (*error & COMMAND_ASM_ERROR)
        fprintf(LOGFILE, "ERROR: Wrong command\n");

    if (*error & REGISTER_ASM_ERROR)
        fprintf(LOGFILE, "ERROR: Wrong Register\n");

    if (*error & NUMBER_ARGUMENT_ASM_ERROR)
        fprintf(LOGFILE, "ERROR: Wrong Number Argument\n");

    if (*error & LABELS_ASM_ERROR)
        fprintf(LOGFILE, "Wrong Label Argument\n");

    if (*error & RAM_ARGUMENT_ERROR)
        fprintf(LOGFILE, "Wrong RAM Argument\n");

    if (*error & SINTAXYS_ASM_ERROR)
        fprintf(LOGFILE, "ERROR: Incorrect sintaxys\n");

    LabelsDump(asm_struct->string_labels_array, asm_struct->string_labels_counter);

    int asm_error = ASMVerify(asm_struct);

    fprintf(LOGFILE, "ASM struct [%p]\ncommand counter is %d, output array [%p]\n\n", asm_struct, asm_struct->command_num, asm_struct->output_arr);
    fprintf(LOGFILE, "\noutput array\n");
    if (!(asm_error & OUTPUT_ARRAY_POINTER_ASM_ERROR))
        for (int i = 0; i < asm_struct->command_num; i++)
            fprintf(LOGFILE, "[%d] = %d\n", i, asm_struct->output_arr[i]);


    printf("\n========================\n\n");
}

//=============================================================================


void CreateBinaryFile(const char *file_name, int commands_count, int *output_arr) {

    assert(file_name);

    FILE *output_file = fopen(file_name, "wb");

    fwrite(output_arr, sizeof(int), commands_count +  SIZE_OF_HEADER_IN_BYTES, output_file);

    fclose(output_file);
}

//-----------------------------------------------------------------------------

bool CheckIfCommandHaveArgument(int command) {

    for (int curr_num = 0; curr_num < COMMANDS_COUNT; curr_num++)

        if (commands_array[curr_num].command_number == command) {
            if(commands_array[curr_num].agrument_type == NO_ARGUMENT)
                return false;

            return true;
        }

    return false;
}

//-----------------------------------------------------------------------------

void CreateNormalFile(const char *file_name, int commands_count, int *output_arr) {

    assert(file_name);

    FILE *output_file = fopen(file_name, "w");

    fprintf(output_file, "SPU VERSION: %d\n", output_arr[0]);
    fprintf(output_file, "FILE SIGNATURE: %d\n\n", output_arr[1]);

    for (int curr_num = SIZE_OF_HEADER_IN_BYTES;
         curr_num < commands_count + SIZE_OF_HEADER_IN_BYTES; curr_num++) {

        if (CheckIfCommandHaveArgument(output_arr[curr_num]))

            fprintf(output_file, "%d ", output_arr[curr_num++]);

        fprintf(output_file, "%d\n", output_arr[curr_num]);
    }

    fclose(output_file);
}

//=============================================================================

int CountStringHashDJB2(const char *curr_string) {

    int hash = 5381, curr_num = 0;    // магическое число

    while (curr_string[curr_num]) {
        hash = ((hash << 5) + hash) + curr_string[curr_num];   // *33

        curr_num++;
    }

    return hash;
}

//=============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "lib/onegin/buffer.h"
//#include "buffer.h"
#include "../include/commands.h"
#include "assembler.h"

//=============================================================================

char **GetStringsPtrArrayFromFile(const char *file_name, int *ptrs_to_code_lines_array_size) {

    assert(file_name);
    assert(ptrs_to_code_lines_array_size);

    size_t file_size = 0;
    char *buffer = GetBuffer(file_name, &file_size);

    assert(buffer);

    *ptrs_to_code_lines_array_size = StringsCount(buffer);
    char **strings_ptr_array = GetStringPtrArray(buffer, *ptrs_to_code_lines_array_size);

    assert(strings_ptr_array);

    return strings_ptr_array;
}


//=============================================================================

int GetCurrentOpcode(const char* command) {

    assert(command);

ON_DEBUG(printf("Curr command is %s\n", command));

    for (int curr_num = 0; curr_num < COMMANDS_COUNT; curr_num++)

        if (strcmp(command, commands_array[curr_num].command_name) == 0)
            return commands_array[curr_num].command_number;

    return Command_Asm_Error;
}

//=============================================================================

int CheckRegister(char *reg) {

    if (reg == NULL)
        return Register_Asm_Error;

    for (int curr_num = 0; curr_num < REGISTERS_COUNT; curr_num++)

        if (strcmp(reg, registers_array[curr_num].reg_name) == 0)
            return registers_array[curr_num].reg_num;

    return Register_Asm_Error;
}


//=============================================================================


void FillCommandWithNumberArgument(int *output_arr, char *command_string, int *labels, int *command_num) {

    assert(output_arr);
    assert(command_string);
    assert(labels);
    assert(command_num);

    int curr_number = 0;

    char label = '\0';
    sscanf(command_string, " %c", &label);

    if (label == ':') {
        sscanf(command_string, " %*c%d", &curr_number);
        output_arr[(*command_num)++] = labels[curr_number];

    } else {
        sscanf(command_string, "%d", &curr_number);
        output_arr[(*command_num)++] = curr_number;
    }
}


//-----------------------------------------------------------------------------

void FillCommandWithRegisterArgiment(int *output_arr, char *command_string,
                                     int *labels, int *command_num) {

    assert(output_arr);
    assert(command_string);
    assert(labels);
    assert(command_num);

    if (*command_num < 0)
        return;

    char reg[2] = "";

    sscanf(command_string, "%s", reg);
    output_arr[(*command_num)++] = CheckRegister(reg);
}


//-----------------------------------------------------------------------------

void FillCommandWithASMArgiment(int *output_arr, char *command_string, int *labels, int *command_num) {

    assert(output_arr);
    assert(command_string);
    assert(labels);
    assert(command_num);

    if (*command_num < 0)
        return;

    char reg[2] = "";

    sscanf(command_string, " [%[A-Z]", reg);
    output_arr[(*command_num)++] = CheckRegister(reg);
}

//=============================================================================

bool CheckIfCommandLabel(char *command) {

    assert(command);

    if (command[0] == ':')
        return true;

    return false;
}

//-----------------------------------------------------------------------------

void TranslateCommand(int *output_arr, int current_opcode, char *command_string,
                  int *labels, int *command_num) {

    assert(output_arr);
    assert(command_string);
    assert(labels);
    assert(command_num);

    switch (current_opcode) {
        case PUSH:
        case JUMP:
        case JB:
        case JBE:
        case JE:
        case JNE:
        case JA:
        case JAE:
        case CALL:
            FillCommandWithNumberArgument(output_arr, command_string, labels, command_num);
            break;

        case PUSHREG:
        case POPREG:
            FillCommandWithRegisterArgiment(output_arr, command_string, labels, command_num);
            break;

        case PUSHM:
        case POPM:
            FillCommandWithASMArgiment(output_arr, command_string, labels, command_num);
            break;

        default:
            break;
    }
}

//-----------------------------------------------------------------------------

void FillLabel(char *command, int *labels, int command_num, int *counter) {

    assert(command);
    assert(labels);
    assert(counter);

    int label_number = 0;
    sscanf(command + 1, "%d", &label_number);

    labels[label_number] = command_num;
    (*counter)++;

ON_DEBUG(printf("Label number is %d, current command is %d\n", label_number, command_num));

}

//=============================================================================

int FillCodeArray(int *output_arr, int ptrs_to_code_lines_array_size,
                  char **ptrs_to_code_lines_array, int *labels) {

    assert(output_arr);
    assert(ptrs_to_code_lines_array_size);
    assert(labels);

    char command[10] = "";
    int counter = 0, command_num = 0;

    while (counter < ptrs_to_code_lines_array_size) {
        int command_size = 0;
        sscanf(ptrs_to_code_lines_array[counter], "%s%n", command, &command_size);

        if (strcmp(command, "") == 0) {
            counter++;
            continue;
        }

        if (CheckIfCommandLabel(command)) {

            FillLabel(command, labels, command_num, &counter);
            continue;
        }

        int current_opcode = GetCurrentOpcode(command);
        output_arr[command_num++] = current_opcode;

        TranslateCommand(output_arr, current_opcode, ptrs_to_code_lines_array[counter] + command_size,
                     labels, &command_num);

        counter++;
    }

    return command_num;
}

//-----------------------------------------------------------------------------

int *CreateCodeArray(const char *input_file_name, int *commands_count) {

    assert(input_file_name);
    assert(commands_count);

    int labels[LABELS_ARRAY_SIZE] = {};

    int ptrs_to_code_lines_array_size = 0;
    char **ptrs_to_code_lines_array = GetStringsPtrArrayFromFile(input_file_name,
                                                         &ptrs_to_code_lines_array_size);

ON_DEBUG(printf("Strings count is %d\n\n", ptrs_to_code_lines_array_size));

    int *output_arr = (int *) calloc(ptrs_to_code_lines_array_size * 2 + INFORMATION_MEMORY_COUNT,
                                     sizeof(int));

    int command_num = 0;

    output_arr[0] = Version;
    output_arr[1] = Constant;


    for (int i = 0; i < 2; i++)
        command_num = FillCodeArray(output_arr + INFORMATION_MEMORY_COUNT,
                                    ptrs_to_code_lines_array_size, ptrs_to_code_lines_array, labels);

    *commands_count = command_num;

ON_DEBUG(LabelsDump(labels));

    return output_arr;
}

//=============================================================================

void LabelsDump(int *labels) {

    assert(labels);

    printf("------LABELS---------\n\n");

    for (int curr_num = 0; curr_num < LABELS_ARRAY_SIZE; curr_num++)
        printf("[%d] ", labels[curr_num]);

    printf("\n\n-----------------------\n");
}


//=============================================================================


void CreateBinaryFile(const char *file_name, int commands_count, int *output_arr) {

    assert(file_name);

    FILE *output_file = fopen(file_name, "wb");

    fwrite(output_arr, sizeof(int), commands_count + INFORMATION_MEMORY_COUNT, output_file);

    fclose(output_file);
}

//-----------------------------------------------------------------------------

bool CheckIfCommandHaveArgument(int command) {

    switch(command) {
        case POPREG:
            return true;
        case PUSHREG:
            return true;
        case PUSH:
            return true;
        case JUMP:
            return true;
        case JB:
            return true;
        case JBE:
            return true;
        case JA:
            return true;
        case JAE:
            return true;
        case JE:
            return true;
        case JNE:
            return true;
        case CALL:
            return true;
        case PUSHM:
            return true;
        case POPM:
            return true;

        default:
            return false;
    }

    return false;
}

//-----------------------------------------------------------------------------

void CreateNormalFile(const char *file_name, int commands_count, int *output_arr) {

    assert(file_name);

    FILE *output_file = fopen(file_name, "w");

    fprintf(output_file, "FILE VERSION: %d\n", output_arr[0]);
    fprintf(output_file, "CODE: %d\n\n", output_arr[1]);

    for (int curr_num = INFORMATION_MEMORY_COUNT;
         curr_num < commands_count + INFORMATION_MEMORY_COUNT; curr_num++) {      // функци€, и в ней умол€ю сделай свитч кейс

        if (CheckIfCommandHaveArgument(output_arr[curr_num]))

            fprintf(output_file, "%d ", output_arr[curr_num++]);

        fprintf(output_file, "%d\n", output_arr[curr_num]);
    }

    fclose(output_file);
}


//=============================================================================

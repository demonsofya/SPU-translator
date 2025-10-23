#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#include "lib/onegin/buffer.h"
//#include "buffer.h"
#include "../include/commands.h"
#include "assembler.h"
#include "string.h"

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

void CountHashTable() {

    for (int curr_num = 0; curr_num < COMMANDS_COUNT; curr_num++)
        commands_array[curr_num].hash = CountStringHashDJB2(commands_array[curr_num].command_name);

    for (int curr_num = 0; curr_num < REGISTERS_COUNT; curr_num++)
        registers_array[curr_num].reg_hash = CountStringHashDJB2(registers_array[curr_num].reg_name);
}

//=============================================================================

void FillCurrentOpcode(ASM_t *asm_struct, char *argument_string, int *error) {

    assert(error);
    assert(asm_struct);
    assert(argument_string);

ON_DEBUG(printf("Curr command is %s\n", asm_struct->curr_command_string));

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

            return;

        }

    *error |= COMMAND_ASM_ERROR;
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


void FillCommandWithNumberArgument(ASM_t *asm_struct, char *arg_command_string, int *error) {

    assert(asm_struct);
    assert(arg_command_string); // Катя: добавлен ассерт
    assert(error);

    int curr_number = 0, readen_elements_count = 0;

    char label = '\0';
    sscanf(arg_command_string, " %c", &label);

    if (label == ':') {
        char curr_string_label[10] = "";
        readen_elements_count = sscanf(arg_command_string, " %*c%s", curr_string_label);

ON_DEBUG(printf("Current Label argument is %s\n", curr_string_label));

        if (isdigit(curr_string_label[0])) {
            readen_elements_count = sscanf(arg_command_string, " %*c%d", &curr_number);
            asm_struct->output_arr[asm_struct->command_num++] = asm_struct->labels[curr_number];

ON_DEBUG(printf("Label number argument is %d\n", curr_number));

        } else {
            int curr_label_hash = CountStringHashDJB2(curr_string_label);

            for (int curr_num = 0; curr_num < REGISTERS_COUNT; curr_num++) {
                if (curr_label_hash == asm_struct->string_labels_array[curr_num].label_hash) {
                    if (strcmp(curr_string_label, asm_struct->string_labels_array[curr_num].label_name) == 0) {
                        asm_struct->output_arr[asm_struct->command_num++] =
                                    asm_struct->string_labels_array[curr_num].label_num;

                        return;
                    }
                }
            }

ON_DEBUG(printf("Label string argument is %s\n", curr_string_label));

        }


    } else {
        readen_elements_count = sscanf(arg_command_string, "%d", &curr_number);
        asm_struct->output_arr[asm_struct->command_num++] = curr_number;
    }

    if (readen_elements_count == 0)
        *error |= NUMBER_ARGUMENT_ASM_ERROR;
}


//-----------------------------------------------------------------------------

void FillCommandWithRegisterArgiment(ASM_t *asm_struct, char *arg_command_string, int *error) {

    assert(asm_struct);
    assert(error);

    int readen_elements_count = 0;

    if (asm_struct->command_num < 0) {
        *error |= COMMAND_ASM_ERROR;
        return;
    }

    char reg[10] = "";

    readen_elements_count = sscanf(arg_command_string, " %s", reg);

    if (readen_elements_count != 1)
        *error |= REGISTER_ASM_ERROR;

    asm_struct->output_arr[asm_struct->command_num++] = CheckRegister(reg, error);
}


//-----------------------------------------------------------------------------

void FillCommandWithRAMArgiment(ASM_t *asm_struct, char *arg_command_string, int *error) {

    assert(asm_struct);
    assert(arg_command_string); // Катя: добавлен ассерт
    assert(error);

    if (asm_struct->command_num < 0)
        return;

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
}

//=============================================================================

bool CheckIfCommandLabel(char *command) {

    assert(command);

    if (command[0] == ':')
        return true;

    return false;
}

//-----------------------------------------------------------------------------

void FillLabel(int *counter, ASM_t *asm_struct, int *error) {

    assert(error);
    assert(asm_struct);

    int label_number = 0, readen_elements_count = 0;

    char curr_string_label[MAX_COMMAND_SIZE] = "";
    readen_elements_count = sscanf(asm_struct->curr_command_string, "%*c%s", curr_string_label);

    if (isdigit(curr_string_label[0])) {
        readen_elements_count = sscanf(asm_struct->curr_command_string, "%*c%d", &label_number);
        asm_struct->labels[label_number] = asm_struct->command_num;

ON_DEBUG(printf("Label number is %d, current command is %d\n", label_number, asm_struct->command_num));

    } else {
        int curr_label_hash = CountStringHashDJB2(curr_string_label);

        for (int curr_num = 0; curr_num < REGISTERS_COUNT; curr_num++) {
            if (curr_label_hash != asm_struct->string_labels_array[curr_num].label_hash) {

                asm_struct->string_labels_array[curr_num].label_num = asm_struct->command_num;
                strncpy(asm_struct->string_labels_array[curr_num].label_name,
                        curr_string_label, MAX_COMMAND_SIZE);
                asm_struct->string_labels_array[curr_num].label_hash = CountStringHashDJB2(curr_string_label);


                (*counter)++;
                return;
            }
        }

ON_DEBUG(printf("Label name is %s, current command is %d\n", curr_string_label, asm_struct->command_num));

    }

    if (readen_elements_count != 1)
        *error |= LABELS_ASM_ERROR;

    //asm_struct->labels[label_number] = asm_struct->command_num;
    (*counter)++;

}

//=============================================================================

int FillCodeArray(ASM_t *asm_struct) {

    assert(asm_struct);

    CountHashTable();

    //char command_string[10] = "";
    int counter = 0;
    asm_struct->command_num = 0;

    CheckStrings();

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

        if (error != NO_ASM_ERROR) {
            ErrorDump(&error);
            LabelsDump(asm_struct->labels);

            return asm_struct->command_num;
        }

        counter++;

    }

    return asm_struct->command_num;
}

//-----------------------------------------------------------------------------

bool CheckStrings() {
    bool result = true;

    for (int curr_num = 0; curr_num < COMMANDS_COUNT; curr_num++)
        if (commands_array[curr_num].command_number != curr_num) {
            printf("Error. Position in array is %d, command is %s, command number is %d\n",
                    curr_num, commands_array[curr_num].command_name, commands_array[curr_num].command_number);

            result = false;
        }

    return result;
}

int *CreateCodeArray(const char *input_file_name, int *commands_count) {

    assert(input_file_name);
    assert(commands_count);

    int labels[LABELS_ARRAY_SIZE] = {};

    ASM_t asm_struct = {};

    asm_struct.ptrs_to_code_lines_array_size = 0;
    asm_struct.ptrs_to_code_lines_array = GetStringsPtrArrayFromFile(input_file_name,
                                                         &asm_struct.ptrs_to_code_lines_array_size);

ON_DEBUG(printf("Strings count is %d\n\n", asm_struct.ptrs_to_code_lines_array_size));

    asm_struct.output_arr = (int *) calloc(asm_struct.ptrs_to_code_lines_array_size * 2 + SIZE_OF_HEADER_IN_BYTES,
                                     sizeof(int));

    //int command_num = 0;

    asm_struct.output_arr[0] = DEFAULT_INFORMATION.version;
    asm_struct.output_arr[1] = DEFAULT_INFORMATION.signature;

    asm_struct.output_arr += SIZE_OF_HEADER_IN_BYTES;

    for (int i = 0; i < 2; i++)
        asm_struct.command_num = FillCodeArray(&asm_struct);

    asm_struct.output_arr -= SIZE_OF_HEADER_IN_BYTES;

    *commands_count = asm_struct.command_num;

ON_DEBUG(LabelsDump(labels));

    return asm_struct.output_arr;
}

//=============================================================================

void LabelsDump(int *labels) {

    assert(labels);

    printf("------LABELS---------\n\n");

    for (int curr_num = 0; curr_num < LABELS_ARRAY_SIZE; curr_num++)
        printf("[%d] ", labels[curr_num]);

    printf("\n\n-----------------------\n");
}

//-----------------------------------------------------------------------------

void ErrorDump(int *error) {

    printf("----------ASM DUMP----------\n\n");

    if (*error & COMMAND_ASM_ERROR)
        printf("Wrong command\n");

    if (*error & REGISTER_ASM_ERROR)
        printf("Wrong Register\n");

    if (*error & NUMBER_ARGUMENT_ASM_ERROR)
        printf("Wrong Number Argument\n");

    if (*error & LABELS_ASM_ERROR)
        printf("Wrong Label Argument\n");

    if (*error & RAM_ARGUMENT_ERROR)
        printf("Wrong RAM Argument\n");

    printf("\n--------------------------\n\n");
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "buffer.h"
#include "../include/commands.h"
#include "assembler.h"

//-----------------------------------------------------------------------------

//#define input_file_name "Task6.txt"

char **GetStringsPtrArray(const char *file_name, int *ptr_array_size) {

    size_t file_size = 0;
    char *buffer = GetBuffer(file_name, &file_size);

    *ptr_array_size = StringsCount(buffer);
    char **strings_ptr_array = GetStringPtrArray(buffer, *ptr_array_size);

    return strings_ptr_array;
}

int GetCurrentOpcode(const char* command) {

    for (int

    if (strcmp(command, "PUSH") == 0)     // цикл
        return PUSH;

    if (strcmp(command, "OUT") == 0)
        return OUT;

    if (strcmp(command, "ADD") == 0)
        return ADD;

    if (strcmp(command, "SUB") == 0)
        return SUB;

    if (strcmp(command, "MUL") == 0)
        return MUL;

    if (strcmp(command, "DIV") == 0)
        return DIV;

    if (strcmp(command, "POW") == 0)
        return POW;

    if (strcmp(command, "SQRT") == 0)
        return SQRT;

    if (strcmp(command, "PUSHREG") == 0)
        return PUSHREG;

    if (strcmp(command, "POPREG") == 0)
        return POPREG;

    if (strcmp(command, "JUMP") == 0)
        return JUMP;

    if (strcmp(command, "IN") == 0)
        return IN;

    if (strcmp(command, "JB") == 0)
        return JB;

    if (strcmp(command, "JBE") == 0)
        return JBE;

    if (strcmp(command, "JA") == 0)
        return JA;

    if (strcmp(command, "JAE") == 0)
        return JAE;

    if (strcmp(command, "JE") == 0)
        return JE;

    if (strcmp(command, "JNE") == 0)
        return JNE;

    if (strcmp(command, "HLT") == 0)
        return HLT;

    if (strcmp(command, "CALL") == 0)
        return CALL;

    if (strcmp(command, "RET") == 0)
        return RET;

    if (strcmp(command, "PUSHM") == 0)
        return PUSHM;

    if (strcmp(command, "POPM") == 0)
        return POPM;

    return -1;
}

int *CreateCodeArray(const char *input_file_name, int *commands_count) {

    char command[10] = "";

    int labels[10] = {};

    int ptr_array_size = 0;
    char **text_ptr_array = GetStringsPtrArray(input_file_name, &ptr_array_size);

printf("Strings count is %d\n\n", ptr_array_size);

    int *output_arr = (int *) calloc(ptr_array_size * 2 + 2, sizeof(int));

    output_arr[0] = Version;
    output_arr[1] = Constant;

    int counter = 0;
    int command_num = 2;

    for (int i = 0; i < 2; i++) {
        counter = 0;
        command_num = 2;

        while (counter < ptr_array_size) {
            int command_size = 0;
            sscanf(text_ptr_array[counter], "%s%n", command, &command_size);

            if (command[0] == ':') {
                int label_number = 0;
                sscanf(text_ptr_array[counter], ":%d", &label_number);

                labels[label_number] = command_num - 2;
printf("Label number is %d, current command is %d\n", label_number, command_num - 2);

                counter++;
                continue;
            }

            int current_opcode = GetCurrentOpcode(command);
            output_arr[command_num++] = current_opcode;

            switch (current_opcode) {
                case PUSH:
                case JUMP:
                case JB:
                case JBE:
                case JE:
                case JNE:
                case JA:
                case JAE:
                case CALL: {
                    int curr_number = 0;

                    char label = '\0';
                    sscanf(text_ptr_array[counter] + command_size, " %c", &label);

                    if (label == ':') {
                        sscanf(text_ptr_array[counter] + command_size, " %*c%d", &curr_number);
                        output_arr[command_num++] = labels[curr_number];

                    } else {
                        sscanf(text_ptr_array[counter] + command_size, "%d", &curr_number);
                        output_arr[command_num++] = curr_number;
                    }

                }
                    break;

                case PUSHREG:
                case POPREG: {
                    char reg[2] = "";
                    sscanf(text_ptr_array[counter] + command_size, "%s", reg);  // TODO: fuction

                    if (strcmp(reg, "AX") == 0)
                        output_arr[command_num++] = AX;

                    else if (strcmp(reg, "BX") == 0)
                        output_arr[command_num++] = BX;

                    else if(strcmp(reg, "CX") == 0)
                        output_arr[command_num++] = CX;

                    else if(strcmp(reg, "DX") == 0)
                        output_arr[command_num++] = DX;

                    else if(strcmp(reg, "RV") == 0)
                        output_arr[command_num++] = RV;

                }
                    break;

                case PUSHM:
                case POPM: {
                    char reg[2] = "";
                    sscanf(text_ptr_array[counter] + command_size, "[%s]", reg);

                    if (strcmp(reg, "AX") == 0)
                        output_arr[command_num++] = AX;

                    else if (strcmp(reg, "BX") == 0)
                        output_arr[command_num++] = BX;

                    else if(strcmp(reg, "CX") == 0)
                        output_arr[command_num++] = CX;

                    else if(strcmp(reg, "DX") == 0)
                        output_arr[command_num++] = DX;

                    else if(strcmp(reg, "RV") == 0)
                        output_arr[command_num++] = RV;
                }
                    break;

                default:
                    break;
            }

            counter++;

        }
    }

    *commands_count = command_num;

    for (int i = 0; i < 10; i++) {
        printf("[%d] ", labels[i]);
    }

    return output_arr;
}

void CreateBinaryFile(const char *file_name, int commands_count, int *output_arr) {

    FILE *output_file = fopen(file_name, "wb");

    fwrite(output_arr, sizeof(int), commands_count, output_file);

    fclose(output_file);
}

void CreateNormalFile(const char *file_name, int commands_count, int *output_arr) {

    FILE *output_file = fopen(file_name, "w");

    fprintf(output_file, "FILE VERSION: %d\n", Version);
    fprintf(output_file, "CODE: %d\n\n", Constant);

    for (int curr_num = 2; curr_num < commands_count; curr_num++) {      // функция
        if (output_arr[curr_num] == POPREG || output_arr[curr_num] == PUSHREG || output_arr[curr_num] == PUSH ||
            output_arr[curr_num] == JUMP   || output_arr[curr_num] == JB      || output_arr[curr_num] == JBE  ||
            output_arr[curr_num] == JA     || output_arr[curr_num] == JAE     || output_arr[curr_num] == JE   ||
            output_arr[curr_num] == JNE    || output_arr[curr_num] == CALL    ||
            output_arr[curr_num] == PUSHM  || output_arr[curr_num] == POPM)

            fprintf(output_file, "%d ", output_arr[curr_num++]);

        fprintf(output_file, "%d\n", output_arr[curr_num]);
    }

    fclose(output_file);
}

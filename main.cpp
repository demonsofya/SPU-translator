#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "buffer.h"
#include "stack.h"
#include "commands.h"

int main()
{
    char command[10] = "";

    FILE *output_file = fopen("task_rewrite.txt", "w");

    size_t file_size = 0;
    char *buffer = GetBuffer("Task6.txt", &file_size);

    //Stack_t output_stk = {};
    //StackCtor(&output_stk, 1);


    int ptr_array_size = StringsCount(buffer);
    char **text_ptr_array = GetStringPtrArray(buffer, ptr_array_size);

    int *output_arr = (int *) calloc(ptr_array_size * 2, sizeof(int));
    FILE *output_bin_file = fopen("task_bin.txt", "wb");

    int counter = 0;
    int command_num = 0;
    while (counter < ptr_array_size) {
        int command_size = 0;
        sscanf(text_ptr_array[counter], "%s%n", &command, &command_size);

        if (strcmp(command, "PUSH") == 0) {
            output_arr[command_num++] = PUSH;
            //StackPush(&output_stk, PUSH);

            int curr_number = 0;
            sscanf(text_ptr_array[counter] + command_size, "%d", &curr_number);

            output_arr[command_num++] = curr_number;
            //StackPush(&output_stk, curr_number);

        }

        else if (strcmp(command, "JUMP") == 0) {
            output_arr[command_num++] = JUMP;
            //StackPush(&output_stk, PUSH);

            int curr_number = 0;
            sscanf(text_ptr_array[counter] + command_size, "%d", &curr_number);

            output_arr[command_num++] = curr_number;
            //StackPush(&output_stk, curr_number);

        }

        else if (strcmp(command, "JB") == 0) {
            output_arr[command_num++] = JB;
            //StackPush(&output_stk, PUSH);

            int curr_number = 0;
            sscanf(text_ptr_array[counter] + command_size, "%d", &curr_number);

            output_arr[command_num++] = curr_number;
            //StackPush(&output_stk, curr_number);

        }

        else if (strcmp(command, "JBE") == 0) {
            output_arr[command_num++] = JBE;
            //StackPush(&output_stk, PUSH);

            int curr_number = 0;
            sscanf(text_ptr_array[counter] + command_size, "%d", &curr_number);

            output_arr[command_num++] = curr_number;
            //StackPush(&output_stk, curr_number);

        }

        else if (strcmp(command, "JA") == 0) {
            output_arr[command_num++] = JAE;
            //StackPush(&output_stk, PUSH);

            int curr_number = 0;
            sscanf(text_ptr_array[counter] + command_size, "%d", &curr_number);

            output_arr[command_num++] = curr_number;
            //StackPush(&output_stk, curr_number);

        }

        else if (strcmp(command, "JAE") == 0) {
            output_arr[command_num++] = JAE;
            //StackPush(&output_stk, PUSH);

            int curr_number = 0;
            sscanf(text_ptr_array[counter] + command_size, "%d", &curr_number);

            output_arr[command_num++] = curr_number;
            //StackPush(&output_stk, curr_number);

        }

        else if (strcmp(command, "JE") == 0) {
            output_arr[command_num++] = JE;
            //StackPush(&output_stk, PUSH);

            int curr_number = 0;
            sscanf(text_ptr_array[counter] + command_size, "%d", &curr_number);

            output_arr[command_num++] = curr_number;
            //StackPush(&output_stk, curr_number);

        }

        else if (strcmp(command, "JNE") == 0) {
            output_arr[command_num++] = JNE;
            //StackPush(&output_stk, PUSH);

            int curr_number = 0;
            sscanf(text_ptr_array[counter] + command_size, "%d", &curr_number);

            output_arr[command_num++] = curr_number;
            //StackPush(&output_stk, curr_number);

        }

        else if (strcmp(command, "POPREG") == 0) {
            output_arr[command_num++] = POPREG;

            char reg[2] = "";
            sscanf(text_ptr_array[counter] + command_size, "%s", &reg);

            if (strcmp(reg, "AX") == 0)
                output_arr[command_num++] = AX;

            else if (strcmp(reg, "BX") == 0)
                output_arr[command_num++] = BX;

            else if(strcmp(reg, "CX") == 0)
                output_arr[command_num++] = CX;

            else if(strcmp(reg, "DX") == 0)
                output_arr[command_num++] = DX;

            else
                break;
        }

        else if (strcmp(command, "PUSHREG") == 0) {
            output_arr[command_num++] = PUSHREG;

            char reg[2] = "";
            sscanf(text_ptr_array[counter] + command_size, "%s", &reg);

            if (strcmp(reg, "AX") == 0)
                output_arr[command_num++] = AX;

            else if (strcmp(reg, "BX") == 0)
                output_arr[command_num++] = BX;

            else if(strcmp(reg, "CX") == 0)
                output_arr[command_num++] = CX;

            else if(strcmp(reg, "DX") == 0)
                output_arr[command_num++] = DX;

            else
                break;
        }

        else if (strcmp(command, "ADD") == 0)
            output_arr[command_num++] = ADD;

        else if (strcmp(command, "SUB") == 0)
            output_arr[command_num++] = SUB;

        else if (strcmp(command, "DIV") == 0)
            output_arr[command_num++] = DIV;

        else if (strcmp(command, "OUT") == 0)
            output_arr[command_num++] = OUT;

        else if (strcmp(command, "HLT") == 0)
            output_arr[command_num++] = HLT;

        else if (strcmp(command, "POW") == 0)
            output_arr[command_num++] = POW;

        else if (strcmp(command, "MVL") == 0)
            output_arr[command_num++] = MVL;

        else if (strcmp(command, "SQRT") == 0)
            output_arr[command_num++] = SQRT;

        else if (strcmp(command, "IN") == 0)
            output_arr[command_num++] = IN;

        else
            break;

        counter++;
    }

    /*counter = 0;
    while (counter < ptr_array_size) {
        int command_size = 0;
        sscanf(text_ptr_array[counter], "%s%n", &command, &command_size);

        if (strcmp(command, "PUSH") == 0) {
            fprintf(output_file, "%d ", PUSH);

            int curr_number = 0;
            sscanf(text_ptr_array[counter] + command_size, "%d", &curr_number);

            fprintf(output_file, "%d\n", curr_number);

        }

        else if (strcmp(command, "POPREG") == 0) {
            fprintf(output_file, "%d ", POPREG);

            char reg[2] = "";
            sscanf(text_ptr_array[counter] + command_size, "%s", &reg);

            if (strcmp(reg, "AX") == 0)
                fprintf(output_file, "%d\n", AX);

            else if (strcmp(reg, "BX") == 0)
                fprintf(output_file, "%d\n", BX);

            else if(strcmp(reg, "CX") == 0)
                fprintf(output_file, "%d\n", CX);

            else
                break;
        }

        else if (strcmp(command, "PUSHREG") == 0) {
            fprintf(output_file, "%d ", PUSHREG);

            char reg[2] = "";
            sscanf(text_ptr_array[counter] + command_size, "%s", &reg);

            if (strcmp(reg, "AX") == 0)
                fprintf(output_file, "%d\n", AX);

            else if (strcmp(reg, "BX") == 0)
                fprintf(output_file, "%d\n", BX);

            else if(strcmp(reg, "CX") == 0)
                fprintf(output_file, "%d\n", CX);

            else
                break;
        }

        else if (strcmp(command, "ADD") == 0)
            fprintf(output_file, "%d\n", ADD);

        else if (strcmp(command, "SUB") == 0)
            fprintf(output_file, "%d\n", SUB);

        else if (strcmp(command, "DIV") == 0)
            fprintf(output_file, "%d\n", DIV);

        else if (strcmp(command, "OUT") == 0)
            fprintf(output_file, "%d\n", OUT);

        else if (strcmp(command, "HLT") == 0)
            fprintf(output_file, "%d\n", HLT);

        else if (strcmp(command, "POW") == 0)
            fprintf(output_file, "%d\n", POW);

        else if (strcmp(command, "MVL") == 0)
            fprintf(output_file, "%d\n", MVL);

        else if (strcmp(command, "SQRT") == 0)
            fprintf(output_file, "%d\n", SQRT);

        else if (strcmp(command, "IN") == 0)
            fprintf(output_file, "%d\n", IN);

        else
            break;

        counter++;
    }*/

    //CreateBinFile(&output_stk, "task_bin.txt");
    fwrite(output_arr, sizeof(int), command_num, output_bin_file);

    for (int curr_num = 0; curr_num < command_num; curr_num++) {
        if (output_arr[curr_num] == POPREG || output_arr[curr_num] == PUSHREG
                                           || output_arr[curr_num] == PUSH ||
            output_arr[curr_num] == JUMP || output_arr[curr_num] == JB   || output_arr[curr_num] == JBE ||
            output_arr[curr_num] == JA   || output_arr[curr_num] ==  JAE || output_arr[curr_num] == JE ||
            output_arr[curr_num] == JNE)
            fprintf(output_file, "%d ", output_arr[curr_num++]);

        fprintf(output_file, "%d\n", output_arr[curr_num]);
    }

    fclose(output_file);
    fclose(output_bin_file);

    return 0;
}

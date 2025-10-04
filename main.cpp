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
    char *buffer = GetBuffer("task.txt", &file_size);

    Stack_t output_stk = {};
    StackCtor(&output_stk, 1);


    int ptr_array_size = StringsCount(buffer);
    char **text_ptr_array = GetStringPtrArray(buffer, ptr_array_size);

    int *output_arr = (int *) calloc(ptr_array_size * 2, sizeof(int));

    int counter = 0;
    int command_num = 0;
    while (counter < ptr_array_size) {
        int command_size = 0;
        sscanf(text_ptr_array[counter], "%s%n", &command, &command_size);

        if (strcmp(command, "PUSH") == 0) {
            //output_arr[command_num++] = PUSH;
            StackPush(&output_stk, PUSH);

            int curr_number = 0;
            sscanf(text_ptr_array[counter] + command_size, "%d", &curr_number);

            //output_arr[command_num++] = curr_number;
            StackPush(&output_stk, curr_number);

        }

        else if (strcmp(command, "POPREG") == 0) {
            StackPush(&output_stk, POPREG);

            char reg[2] = "";
            sscanf(text_ptr_array[counter] + command_size, "%s", &reg);

            if (strcmp(reg, "AX") == 0)
                StackPush(&output_stk, AX);
        }

        else if (strcmp(command, "ADD") == 0)
            StackPush(&output_stk, ADD);

        else if (strcmp(command, "SUB") == 0)
            StackPush(&output_stk, SUB);

        else if (strcmp(command, "DIV") == 0)
            StackPush(&output_stk, DIV);

        else if (strcmp(command, "OUT") == 0)
            StackPush(&output_stk, OUT);

        else if (strcmp(command, "HLT") == 0)
            StackPush(&output_stk, HLT);

        else if (strcmp(command, "POW") == 0)
            StackPush(&output_stk, POW);

        else if (strcmp(command, "MVL") == 0)
            StackPush(&output_stk, MVL);

        else if (strcmp(command, "SQRT") == 0)
            StackPush(&output_stk, SQRT);

        else if (strcmp(command, "IN") == 0)
            StackPush(&output_stk, IN);

        else
            break;

        counter++;
    }

    counter = 0;
    while (counter < ptr_array_size) {
        int command_size = 0;
        sscanf(text_ptr_array[counter], "%s%n", &command, &command_size);

        if (strcmp(command, "PUSH") == 0) {
            fprintf(output_file, "%d ", PUSH);

            int curr_number = 0;
            sscanf(text_ptr_array[counter] + command_size, "%d", &curr_number);

            fprintf(output_file, "%d\n", curr_number);

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

        else
            break;

        counter++;
    }

    CreateBinFile(&output_stk, "task_bin.txt");

    fclose(output_file);

    return 0;
}

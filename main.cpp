#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "buffer.h"
#include "../include/commands.h"
#include "assembler.h"

int main() {

    int commands_count = 0;
    int *output_arr = CreateCodeArray("Task11.txt", &commands_count);

    CreateBinaryFile("task_bin.txt", commands_count, output_arr);
    CreateNormalFile("task_rewrite.txt", commands_count, output_arr);

    return 0;
}

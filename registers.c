#include "registers.h"

void op_reg_message(double time, int i, pid_t pid, pid_t tid, int dur, int pl, char *oper) {
    char* message;
    message = (char*) malloc (128 * sizeof(char));

    sprintf(message, "%f ; %d ; %d ; %d ; %d ; %d ; %s\n", time, i, pid, tid, dur, pl, oper);
    write(STDOUT_FILENO, message, strlen(message));
}
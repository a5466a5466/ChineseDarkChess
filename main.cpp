# include <stdio.h>
# include <string.h>
# include "AIRyS.h"


// commands enumerate /ɪˈnuː mɚ reɪt/ verb [T] 列舉，枚舉
enum COMMANDS{
    PROTOCOL_VERSION = 0, // 0
    NAME, // 1
    VERSION, // 2
    KNOWN_COMMAND, // 3
    LIST_COMMANDS, // 4
    QUIT, // 5
    BOARDSIZE, // 6
    RESET_BOARD, // 7
    NUM_REPETITION, // 8
    NUM_MOVES_TO_DRAW, // 9
    MOVE, // 10
    FLIP, // 11
    GENMOVE, // 12
    GAME_OVER, // 13
    READY, // 14
    TIME_SETTINGS, // 15
    TIME_LEFT, // 16
    SHOWBOARD // 17
};

// function pointer array
static bool (AIRyS:: *functions[])(const char* [], char*) = {
    &AIRyS::protocol_version, //0
    &AIRyS::name, //1
    &AIRyS::version, //2
    &AIRyS::known_command, //3
    &AIRyS::list_commands, //4
    &AIRyS::quit, //5
    &AIRyS::boardsize, //6
    &AIRyS::reset_board, //7
    &AIRyS::num_repetition, //8
    &AIRyS::num_moves_to_draw, //9
    &AIRyS::move, //10
    &AIRyS::flip, //11
    &AIRyS::genmove, //12
    &AIRyS::game_over, //13
    &AIRyS::ready, //14
    &AIRyS::time_settings, //15
    &AIRyS::time_left, // 16
    &AIRyS::showboard //17
};



int main(){
    char read[1024]; //read command, ex: 1 for PROTOCOL_VERSION; 2 for ....
    char response[1024]; //response is for receiving the response of return function information
    char output[1024];
    char *token; // point to commands
    const char *data[10]; // data is for save command description
    int id; //record command number
    bool isFailed; // command function is failed or not
    AIRyS aiRyS;

    do{
        //read command
        if(fgets(read, 1024, stdin) == NULL){
            fprintf(stderr, "READ NULL COMMAND\n");
            exit(1);
        };
        fprintf(stderr, "Received command: %s", read); //print error command

        // remove (\n) -> replace \n to \0
        read[strlen(read) - 1] = '\0';
        // get command id
        token = strtok(read, " "); // delimit the first word of command
        sscanf(token, "%d", &id); //read the first line, first word which is command id and put it into "int id".

        // get command name which is the second word of read.
        token = strtok(NULL, " ");
        // get command data
        int i = 0;
        while((token = strtok(NULL, " ")) != NULL){
            data[i++] = token; //assign command data to data[i] pointer
        }

        // empty the char array
        response[0] = '\0';

        // TODO: Call AI Function
        isFailed = (aiRyS.*functions[id])(data, response);


        // the ? and = below is for GUI to judge that commands is successed(=) or failed(?).
        if(strlen(response) > 0){ //The command has response.
            if(isFailed){
                sprintf(output, "?%d %s\n", id, response);
            }else{
                sprintf(output, "=%d %s\n", id, response);
            }
        }else{
            if(isFailed){ //The command hasn't response.
                sprintf(output, "?%d\n", id);
            }else{
                sprintf(output, "=%d\n", id);
            }
        }

        fprintf(stdout, "%s", output);
        fprintf(stderr, "%s", output);

        // important, do not delete. This is for cleaning buffer.
        fflush(stdout);
        fflush(stderr);


    }while(id != QUIT);

    return 0;
}
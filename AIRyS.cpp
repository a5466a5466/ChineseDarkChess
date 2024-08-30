# include "AIRyS.h"






AIRyS::AIRyS(void){}

AIRyS::~AIRyS(void){}


//----------------------------------COMMAND FUNCTION------------------------------------------------------
/*
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
*/
bool AIRyS::protocol_version(const char* data[], char* response){ //0
    strcpy(response, "protocol ver 1.0.0");
    return 0;
}

bool AIRyS::name(const char* data[], char* response){ //1
    strcpy(response, "AI Name: IRySoCute");
    return 0;
}

bool AIRyS::version(const char* data[], char* response){ //2
    strcpy(response, "AI Version: 1.0.0, Update date: 2022/08/04");
    return 0;
}

bool AIRyS::known_command(const char* data[], char* response){ //3
    for(int i = 0; i < COMMAND_NUM; i++){
        if(data[0]==NULL){
            strcpy(response, "false");
            return 0;
        }else if(strcmp(data[0], commands_name[i]) != 0){
            strcpy(response, "True");
            return 0;
        }else{
            strcpy(response, "false");
        }
        return 0;
    }
}

bool AIRyS::list_commands(const char* data[], char* response){ //4
    for(int i = 0; i < COMMAND_NUM-1; i++){
        strcat(response, commands_name[i]);   //strcat ->full name: string catenate [ˋkætə͵neɪt]:to connect in a series
    }
    strcat(response, commands_name[COMMAND_NUM-1]);
    return 0;
}

bool AIRyS::quit(const char* data[], char* response){ //5
    strcpy(response, "ByRyS! Remember to subscribe IRyS ch!\n");
    return 0;
}

bool AIRyS::boardsize(const char* data[], char* response){ //6
    fprintf(stderr, "It's still non-functional.");
    return 0;
}

bool AIRyS::reset_board(const char* data[], char* response){ //7
    this->Red_time = -1; // -1 means unknown here
    this->Black_time = -1; // -1 means unknown here
    this->initBoard();
    return 0;
}

bool AIRyS::num_repetition(const char* data[], char* response){ //8
    fprintf(stderr, "Set repetition limits: %s\n", data[0]);
    return 0;
}

bool AIRyS::num_moves_to_draw(const char* data[], char* response){ //9
    fprintf(stderr, "Set moves limits to draw: %s\n", data[0]);
    return 0;
}

bool AIRyS::move(const char* data[], char* response){ //10 //ex: 10 move a1 a2\n  //data[0] = a1; data[1] = a2
    char move[6];
    int *src = new int;
    int *dst = new int;
    sprintf(move, "%s-%s", data[0], data[1]);
    this->movePreprocessing(move, src, dst);
    this->moveChess(&(this->main_chessboard), position_to_bitboard(*src), position_to_bitboard(*dst));
    delete src, dst;
    return 0;
}


bool AIRyS::flip(const char* data[], char* response){ //11 //ex: 11 flip a3 G\n  //data[0] = a3; data[1] = G
    char move[6];
    sprintf(move, "%s(%s)", data[0], data[1]);
    
}



//----------------------------------COMMAND FUNCTIONS END-------------------------------------------------


//----------------------------------GENERAL FUNCTIONS----------------------------------------------------
int bitboard_to_position(U32 bitboard){
    int tmp = (bitboard * 0x08ED2BE6) >> 27; //I don't know why it's 0x08ED2BE6; It can make perfect hashing.
    int position = position_index[tmp];
    return position;
}

U32 position_to_bitboard(int position){
    U32 bitboard = bitboard_index[position];
    return bitboard;
}


//----------------------------------AIRyS FUNCTIONS------------------------------------------------------

void AIRyS::initBoard(){
    U32 tmp[16] = {0};
    memcpy(main_chessboard.Piece, tmp, sizeof(U32)*16);
    main_chessboard.Red_Occupied = 0;
    main_chessboard.Black_Occupied = 0;
    main_chessboard.All_Occupied = 0xffffffff;

    main_chessboard.Red_chess_Num = 0;
    main_chessboard.Black_Chess_Num = 0;
    main_chessboard.NoEatFlip = 0;
    main_chessboard.HistoryCount = 0;
}



void AIRyS::movePreprocessing(const char move[], int *src, int* dst){ // ex: move[6] = c3 - b3
    *src = ('d' - move[0]) + ('8' - move[1])*4; //get src position
    *dst = ('d' - move[3]) + ('8' - move[4])*4; //get dst position
}

void AIRyS::moveChess(ChessBoard* chessboard, const U32 src, const U32 dst){
    int chesstype = -1;
    chessboard->All_Occupied = chessboard->All_Occupied^src;
    for(int i = 0; i < TOTAL_PIECES; i++){
        if((chessboard->Piece[i] & src) == src){
            chesstype = i;
            break;
        }
    }
    if((chesstype>0)&&(chesstype<9)){
        chessboard->Red_Occupied = chessboard->Red_Occupied^src;
    }else if((chesstype<8)&&(chesstype<15)){
        chessboard->Black_Occupied = chessboard->Black_Occupied^src;
    }else{
        fprintf(stderr, "chesstype error");
    }
    chessboard->Piece[chesstype] = chessboard->Piece[chesstype]^src;
    chessboard->Piece[chesstype] = chessboard->Piece[chesstype]|dst;
}



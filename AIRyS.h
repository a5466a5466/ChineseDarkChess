#ifndef AIRyS_H_
#define AIRyS_H_

# include <stdio.h>
# include <string.h>
# include <stdlib.h>



# define COMMAND_NUM 18
# define TOTAL_PIECES 16

//===================================GENERAL SETTING==================================
typedef unsigned int U32;

struct ChessBoard{
    U32 Piece[TOTAL_PIECES]; //TOTAL_PIECES == 16; Piece[0] means occupied position; Piece[1~8] save red pieces; Piece[9~14] save black pieces; Piece[15] means unflipped pieces;
    U32 Red_Occupied, Black_Occupied, All_Occupied;
    int Red_chess_Num, Black_Chess_Num;
    int NoEatFlip;
    int History[4096];
    int HistoryCount;
};

const int column_filter[4] = {0x11111111, 0x22222222, 0x44444444, 0x88888888};
const int row_filter[8] = {0x0000000F, 0x000000F0, 0x00000F00, 0x0000F000,0x000F0000, 0x00F00000, 0x0F000000, 0xF0000000};
                        // first row, ...... 

const int pMove_filter[32] = {0x00000012, 0x00000025, 0x0000004A, 0x00000084, 
                              0x00000121, 0x00000252, 0x000004A4, 0x00000848,
                              0x00001210, 0x00002520, 0x00004A40, 0x00008480,
                              0x00012100, 0x00025200, 0x0004A400, 0x00084800,
                              0x00121000, 0x00252000, 0x004A4000, 0x00848000,
                              0x01210000, 0x02520000, 0x04A40000, 0x08480000,
                              0x12100000, 0x25200000, 0x4A400000, 0x84800000,
                              0x21000000, 0x52000000, 0xA4000000, 0x48000000
                            };

//Use for changing bitboard to position
const int position_index[32] = {31, 0, 1, 5, 2, 16, 27, 6, 3, 14, 17, 19, 28, 11, 7, 21, 30, 4, 15, 26, 13, 18, 10, 20, 29, 25, 12, 9, 24, 8, 23, 22};

//Use for changing position to bitboard
const U32 bitboard_index[32] = {0x00000001, 0x00000002, 0x00000004, 0x00000008,
                                0x00000010, 0x00000020, 0x00000040, 0x00000080,
                                0x00000100, 0x00000200, 0x00000400, 0x00000800,
                                0x00001000, 0x00002000, 0x00004000, 0x00008000,
                                0x00010000, 0x00020000, 0x00040000, 0x00080000,
                                0x00100000, 0x00200000, 0x00400000, 0x00800000,
                                0x01000000, 0x02000000, 0x04000000, 0x08000000,
                                0x10000000, 0x20000000, 0x40000000, 0x80000000,
                               };



//=======================GENERAL FUNCTIONS=========================
int bitboard_to_position(U32 bitboard);
U32 position_to_bitboard(int position);


//=======================AIRyS CLASS===============================
class AIRyS{
    const char* commands_name[COMMAND_NUM] = {
        "protocol_version", // 0
        "name", // 1
        "version", // 2
        "known_command", // 3
        "list_commands", // 4
        "quit", // 5
        "boardsize", // 6
        "reset_board", // 7
        "num_repetition", // 8          //repetition (n.) /ˌrep.əˈtɪʃ.ən/ 重複；重做；重說
        "num_moves_to_draw", // 9
        "move", //10
        "flip", //11
        "genmove", //12
        "game_over", //13
        "ready", //14
        "time_settings", //15
        "time_left", // 16
        "showboard"
    };

public:
    AIRyS(void);  //Constructor
    ~AIRyS(void); //Destructor

    // commands
    bool protocol_version(const char* data[], char* response); //0
    bool name(const char* data[], char* response); //1
    bool version(const char* data[], char* response); //2
    bool known_command(const char* data[], char* response); //3
    bool list_commands(const char* data[], char* response); //4
    bool quit(const char* data[], char* response); //5
    bool boardsize(const char* data[], char* response); //6
    bool reset_board(const char* data[], char* response); //7
    bool num_repetition(const char* data[], char* response); //8
    bool num_moves_to_draw(const char* data[], char* response); //9
    bool move(const char* data[], char* response); //10
    bool flip(const char* data[], char* response); //11
    bool genmove(const char* data[], char* response); //12
    bool game_over(const char* data[], char* response); // 13
    bool ready(const char* data[], char* response); //14
    bool time_settings(const char* data[], char* response); // 15
    bool time_left(const char* data[], char* response); // 16
    bool showboard(const char* data[], char*response); //17

private:
    int Color;
    int Red_time, Black_time;
    ChessBoard main_chessboard;
    bool TimeIsUp;


    // AIRyS functions
    void initBoard();
    void movePreprocessing(const char move[], int *src, int* dst);
    void moveChess(ChessBoard* chessboard, const U32 src, const U32 dst);
    void MakeFlip(ChessBoard* chessboard, const char move[6]);





    // Display
    void Show_Chessboard();
    
};






















# endif
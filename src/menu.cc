#include "game.h"
#include <iostream>
#include "window.h"
#include "renderUtil.h"

using namespace std;

string board_setup(){
    auto win = new Xwindow{500,500};
    win->drawStringBold(90, 35, "CS246 - C++ Chess");
    vector<char> opti;
    opti.assign(64, ' ');
    char file = 'A';
    int shift = 50;
    for(int i = 0; i < 8; i++){
        win->drawStringBold(20, i*50 + 35 + shift, std::to_string(8-i));
        for(int j = 0; j < 8; j++){
            if ((j+i) % 2 != 0){
                win->fillRectangle((i*50) + shift, (j*50) + shift, 50, 50, 4); // Print Black Square
            }
        }
        std::string s(1,file);
        win->drawStringBold((i*50) + 10 + shift, 490, s);
        file++;
    }
    win->fillRectangle(shift, shift, 400, 5, 1);
    win->fillRectangle(shift, shift, 5, 400, 1);
    win->fillRectangle(400+shift, shift, 5, 405, 1);
    win->fillRectangle(shift, 400+shift, 400, 5, 1);

    string command;
    string output;
    int empty = 0;
    string turn = " w";
    vector<char> grid;
    grid.resize(64,' ');

    while(cin >> command){
        if(command == "+"){
            char piece; std::cin >> piece;
            char file; std::cin >> file;
            int row; std::cin >> row;
            grid[(8*(row-1))+mapFiles(file)] = piece;
            txt_printer(grid);
            gfx_printer(win,grid, &opti);
        } else if (command == "-"){
            char piece; std::cin >> piece;
            char file; std::cin >> file;
            int row; std::cin >> row;
            grid[(8*(row-1))+mapFiles(file)] = ' ';
            txt_printer(grid);
            gfx_printer(win,grid,&opti);
        } else if (command == "="){
            string color; std::cin >> color;
            if (color == "white"){
                turn = " w";
            } else if (color == "black"){
                turn = " b";
            }
        } else if (command == "fen"){
            std::cin >> output;
            delete win;
            return output;
        } else if (command == "done"){
            break;
        }
    }

    for(int i = 7; i >= 0; i--){
        for(int j = 0; j < 8; j++){
            if(grid[(8*i) + j] == ' '){
                empty++;
            } else {
                if(empty > 0){
                    auto s = std::to_string(empty);
                    output += s;
                    empty = 0;
                }
                output += grid[(8*i) + j];
            }
        }
        if(empty != 0){
            auto s = std::to_string(empty);
            output += s;
            empty = 0;
        }
        if(i != 0){
            output += "/";
        }
    }
    delete win;
    output += turn;
    return output;
}

int main(int argc, char const *argv[])
{
    cout << "+-----------------------------------------------------+" << endl;
    cout << "|   _____                  _____ _                    |" << endl;
    cout << "|  / ____|    _     _     / ____| |                   |" << endl;
    cout << "| | |       _| |_ _| |_  | |    | |__   ___  ___ ___  |" << endl;
    cout << "| | |      |_   _|_   _| | |    | '_ \\ / _ \\/ __/ __| |" << endl;
    cout << "| | |____    |_|   |_|   | |____| | | |  __/\\__ \\__ \\ |" << endl;
    cout << "|  \\_____|                \\_____|_| |_|\\___||___/___/ |" << endl;
    cout << "+-----------------------------------------------------+ " << endl; 
    cout << "|    BY ABDULLAH SHAHID, ARIQ ISHFAR, & SOHAM NAGI    |" << endl;
    cout << "+-----------------------------------------------------+" << endl;

    int black_wins = 0;
    int white_wins = 0;
    int draw = 0;

    string command;
    string board = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    
    while(cin >> command){
        if (command == "setup"){
            board = board_setup();
        } else if (command == "game"){
            string white;
            cin >> white;
            string black;
            cin >> black;
            Game* round = new Game(board, white, black);
            round->notifyObservers();
            round->start();
            int score = round->getResult();
            if (score == -1){
                ++black_wins;
            } else if (score == 0){
                ++draw;
            } else {
                ++white_wins;
            }
            delete round;
            board = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
        } else if (command == "quit"){
            break;
        }
    }
    cout << "Final Score:" << endl;
    cout << "White: " << white_wins << endl;
    cout << "Black: " << black_wins << endl;
}

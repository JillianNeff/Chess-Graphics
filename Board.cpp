#include "Board.h"

Board::Board() {
    resetBoard();
    for(int y = 50; y<=750; y+=100){
        for(int x = 50; x<=750; x+=100) {
            if((((x-50)/100)+(y-50)/100) % 2 == 0) {
                squares.push_back((Button({.7373, .6196,.4863}, {x, y}, 100, 100, "")));
            }
            else{
                squares.push_back(Button({.4, .314, .239}, {x, y}, 100, 100, ""));
            }
        }
    }

}

ChessPiece* Board::getPiece(int x, int y, faction team) {
    if (team == WHITE) {
        int index = (y * 8) + x;
        return whiteBoard[index];
    } else {
        int index = (y * 8) + x;
        return blackBoard[index];
    }
}

bool Board::getWhiteTurn() const {
    return whiteTurn;
}


void Board::setWhiteTurn(bool whiteTurn) {
    this->whiteTurn = whiteTurn;
}

void Board::resetBoard() {
    whiteBoard.clear();
    whiteBoard.resize(64);
    blackBoard.clear();
    blackBoard.resize(64);
    for (int i = 0; 64 > i; i++) {
        whiteBoard[i] = nullptr;
        blackBoard[i] = nullptr;
    }
    ended = false;
}

void Board::setPiece(ChessPiece* piece) {
    if(piece->getTeam() == WHITE) {
        int setIndex = (piece->getY() * 8) + piece->getX();
        whiteBoard[setIndex] = piece;
    } else {
        int setIndex = (piece->getY() * 8) + piece->getX();
        blackBoard[setIndex] = piece;
    }
}

void Board::checkCollisions() {
    for (int i = 0; 64 > i; i++) {
        if (whiteBoard[i] != nullptr && blackBoard[i] != nullptr) {
            if (whiteTurn) {
                blackBoard[i] = nullptr;
            } else {
                whiteBoard[i] = nullptr;
            }
        }
    }
}

void Board::fileToBoard(std::string filename) {
    std::ifstream fIn; // Creating in files
    fIn.open(filename); // "../boards/default.csv"
    std::vector<std::string> fileContents; // Will store string representation of pieces
    if (fIn.is_open()) { // Error handling if file doesn't open
        std::string tempPiece; // Will store a line of data from CSV
        std::string piece; // Accumulates the string representation of the piece
        char comma = ',';
        while(fIn >> tempPiece) { // Loops for each line of CSV
            // Loops through the line of the CSV
            for(unsigned i = 0; tempPiece.length() > i; i++) {
                if(tempPiece.at(i) != comma) { // Makes sure the character isn't a comma
                    if(tempPiece.at(i) == '\\' && tempPiece.at(i+1) == 'n') { // Makes sure it is not a new line
                        piece = "";
                        i++;
                    } else {
                        piece += tempPiece.at(i);
                    }
                } else { // If the char is a comma it pushes the piece to the vec and clears the accumulated chars
                    fileContents.push_back(piece);
                    piece = "";
                }
            }
            fileContents.push_back(piece);
            piece = "";
        }
    } else {
        std::cout << "ERROR: File not open." << std::endl;
    }
    // This loop will create piece objects and push them to the board based on fileContents
    for (int i = 0; fileContents.size() > i; i++) {
        if (!fileContents[i].empty()) {
            if (fileContents[i].at(0) == 'W') {
                if (fileContents[i] == "WP") {
                    setPiece(new Pawn(WHITE, true, i % 8, i / 8));
                } else if (fileContents[i] == "WB") {
                    setPiece(new Bishop(WHITE, true, i % 8, i / 8));
                } else if (fileContents[i] == "WKG") {
                    setPiece(new King(WHITE, true, i % 8, i / 8));
                } else if (fileContents[i] == "WKN") {
                    setPiece(new Knight(WHITE, true, i % 8, i / 8));
                } else if (fileContents[i] == "WQ") {
                    setPiece(new Queen(WHITE, true, i % 8, i / 8));
                } else if (fileContents[i] == "WR") {
                    setPiece(new Rook(WHITE, true, i % 8, i / 8));
                }
            } else {
                if (fileContents[i] == "BP") {
                    setPiece(new Pawn(BLACK, true, i % 8, i / 8));
                } else if (fileContents[i] == "BB") {
                    setPiece(new Bishop(BLACK, true, i % 8, i / 8));
                } else if (fileContents[i] == "BKG") {
                    setPiece(new King(BLACK, true, i % 8, i / 8));
                } else if (fileContents[i] == "BKN") {
                    setPiece(new Knight(BLACK, true, i % 8, i / 8));
                } else if (fileContents[i] == "BQ") {
                    setPiece(new Queen(BLACK, true, i % 8, i / 8));
                } else if (fileContents[i] == "BR") {
                    setPiece(new Rook(BLACK, true, i % 8, i / 8));
                }
            }
        }
    }
    fIn.close();
}

void Board::popPiece(int x, int y, faction team) {
    if (team == WHITE) {
        int setIndex = (y * 8) + x;
        whiteBoard[setIndex] = nullptr;
    } else {
        int setIndex = (y * 8) + x;
        blackBoard[setIndex] = nullptr;
    }
}

void Board::movePieceToOption(ChessPiece* piece, int choice) {
    std::vector<int> pieceOptions = piece->getValidMoves();
    int indexToMoveTo = pieceOptions[choice-1];
    faction team = piece->getTeam();
    int x = indexToMoveTo % 8;
    int y = indexToMoveTo / 8;
    popPiece(x, y, team);
    piece->setX(x);
    piece->setY(y);
    setPiece(piece);
}

std::vector<int> Board::cleanValidMoves(int x, int y, faction team) {
    pieceType type = getPiece(x, y, team)->getType();
    std::vector<int> validMoves = getPiece(x, y, team)->getValidMoves();
    std::vector<int> cleanMoves;
    std::vector<int> allPositions;
    std::vector<int> movingTeam;
    std::vector<int> enemyTeam;

    // Get a vector of team locations for each team
    for(int i = 0; i < 64; i++) {
        if (whiteBoard[i] != nullptr) {
            if (team == WHITE) {
                movingTeam.push_back(whiteBoard[i]->getBoardIndex());
            } else {
                enemyTeam.push_back(whiteBoard[i]->getBoardIndex());
            }
            allPositions.push_back(whiteBoard[i]->getBoardIndex());
        }
        if (blackBoard[i] != nullptr) {
            if (team == BLACK) {
                movingTeam.push_back(blackBoard[i]->getBoardIndex());
            } else {
                enemyTeam.push_back(blackBoard[i]->getBoardIndex());
            }
            allPositions.push_back(blackBoard[i]->getBoardIndex());
        }
    }

    for(int validMove : validMoves) {
        bool valid = true;

        // Check for team collisions
        for (int j = 0; (movingTeam.size() > j && valid); j++) {
            if (validMove == movingTeam[j]) {
                valid = false;
            }
        }

        // Check for enemy collisions
        switch (type) {
            case PAWN:
                if (validMove%8 != x) {
                    valid = false;
                    for (int j = 0; enemyTeam.size() > j; j++) {
                        if (validMove == enemyTeam[j]) {
                            valid = true;
                        }
                    }
                } else if (validMove%8 == x) {
                    for (int j = 0; enemyTeam.size() > j; j++) {
                        if (validMove == enemyTeam[j]) {
                            valid = false;
                        }
                    }
                }
                break;
        }


        // Check for blockages
        // Cases are broken up into movement types: up, down, left, right (ROOK)
        // The cases each check if there is nothing in front of the valid move.
        // If something at all blocks te valid move, it is invalid.
        // This causes the piece to take both friendly and enemy units, but the logic is already being cleaned.
        switch (type) {
            case PAWN:
                if (validMove%8 == x) {
                    if (team == WHITE) {
                        for (int i = validMove+8; i < y*8+x && valid; i+=8) {
                            if (std::count(allPositions.begin(), allPositions.end(), i)) {
                                valid = false;
                            }
                        }
                    } else if (team == BLACK) {
                        for (int i = validMove-8; i > y*8+x && valid; i-=8) {
                            if (std::count(allPositions.begin(), allPositions.end(), i)) {
                                valid = false;
                            }
                        }
                    }
                }
                break;
            case QUEEN:
                // Up x=x Case
                if (validMove%8 == x && validMove/8 < y) {
                    for (int i = validMove+8; i < y*8+x && valid; i+=8) {
                        if (std::count(allPositions.begin(), allPositions.end(), i)) {
                            valid = false;
                        }
                    }
                    // Down x=x Case
                } else if (validMove%8 == x && validMove/8 > y) {
                    for (int i = validMove-8; i > y*8+x && valid; i-=8) {
                        if (std::count(allPositions.begin(), allPositions.end(), i)) {
                            valid = false;
                        }
                    }
                    // Left y=y Case
                } else if (validMove%8 < x && validMove/8 == y) {
                    for (int i = validMove+1; i < y*8+x && valid; i++) {
                        if (std::count(allPositions.begin(), allPositions.end(), i)) {
                            valid = false;
                        }
                    }
                    // Right y=y Case
                } else if (validMove%8 > x && validMove/8 == y) {
                    for (int i = validMove - 1; i > y * 8 + x && valid; i--) {
                        if (std::count(allPositions.begin(), allPositions.end(), i)) {
                            valid = false;
                        }
                    }
                // Up Left
                } else if (validMove%8 < x && validMove/8 < y) {
                    for (int i = validMove+9; i < y*8+x && valid; i+=9) {
                        if (std::count(allPositions.begin(), allPositions.end(), i)) {
                            valid = false;
                        }
                    }
                    // Up Right
                } else if (validMove%8 > x && validMove/8 < y) {
                    for (int i = validMove+7; i < y*8+x && valid; i+=7) {
                        if (std::count(allPositions.begin(), allPositions.end(), i)) {
                            valid = false;
                        }
                    }
                    // Down Left
                } else if (validMove%8 < x && validMove/8 > y) {
                    for (int i = validMove-7; i > y*8+x && valid; i-=7) {
                        if (std::count(allPositions.begin(), allPositions.end(), i)) {
                            valid = false;
                        }
                    }
                    // Down Right
                } else if (validMove%8 > x && validMove/8 > y) {
                    for (int i = validMove-9; i > y*8+x && valid; i-=9) {
                        if (std::count(allPositions.begin(), allPositions.end(), i)) {
                            valid = false;
                        }
                    }
                }
                break;
            case ROOK:
                // Up x=x Case
                if (validMove%8 == x && validMove/8 < y) {
                    for (int i = validMove+8; i < y*8+x && valid; i+=8) {
                        if (std::count(allPositions.begin(), allPositions.end(), i)) {
                            valid = false;
                        }
                    }
                // Down x=x Case
                } else if (validMove%8 == x && validMove/8 > y) {
                    for (int i = validMove-8; i > y*8+x && valid; i-=8) {
                        if (std::count(allPositions.begin(), allPositions.end(), i)) {
                            valid = false;
                        }
                    }
                // Left y=y Case
                } else if (validMove%8 < x && validMove/8 == y) {
                    for (int i = validMove+1; i < y*8+x && valid; i++) {
                        if (std::count(allPositions.begin(), allPositions.end(), i)) {
                            valid = false;
                        }
                    }
                // Right y=y Case
                } else if (validMove%8 > x && validMove/8 == y) {
                    for (int i = validMove - 1; i > y * 8 + x && valid; i--) {
                        if (std::count(allPositions.begin(), allPositions.end(), i)) {
                            valid = false;
                        }
                    }
                }
                break;
            case BISHOP:
                // Up Left
                if (validMove%8 < x && validMove/8 < y) {
                    for (int i = validMove+9; i < y*8+x && valid; i+=9) {
                        if (std::count(allPositions.begin(), allPositions.end(), i)) {
                            valid = false;
                        }
                    }
                // Up Right
                } else if (validMove%8 > x && validMove/8 < y) {
                    for (int i = validMove+7; i < y*8+x && valid; i+=7) {
                        if (std::count(allPositions.begin(), allPositions.end(), i)) {
                            valid = false;
                        }
                    }
                // Down Left
                } else if (validMove%8 < x && validMove/8 > y) {
                    for (int i = validMove-7; i > y*8+x && valid; i-=7) {
                        if (std::count(allPositions.begin(), allPositions.end(), i)) {
                            valid = false;
                        }
                    }
                // Down Right
                } else if (validMove%8 > x && validMove/8 > y) {
                    for (int i = validMove-9; i > y*8+x && valid; i-=9) {
                        if (std::count(allPositions.begin(), allPositions.end(), i)) {
                            valid = false;
                        }
                    }
                }
                break;
        }

        if(valid) {
            cleanMoves.push_back(validMove);
        }
    }

    allPositions.clear();
    return cleanMoves;
}

std::string Board::toString() {
    int line = 0;
    std::string outString;
    for (unsigned i = 0; whiteBoard.size() > i;  i++) {
        if (whiteBoard[i] != nullptr) {
            outString += whiteBoard[i]->toStringBoardInfo();
        } else if (blackBoard[i] != nullptr) {
            outString += blackBoard[i]->toStringBoardInfo();
        } else {
            outString += "    ";
        }

        line += 1;
        if (line == 8) {
            line = 0;
            outString += "\n";
        }
    }
    return outString;
}

void Board::draw() {
    for(Button b : squares){
        b.draw();
        if(b.isPressed()){
            b.choice();
        }
    }

    if(kingInCheck(WHITE)) {
        for (ChessPiece *piece : whiteBoard) {
            if (piece != nullptr) {
                if (piece->getType() == KING) {
                    Quad check = Quad({1, 0, 0}, {(piece->getX()*100)+50, (piece->getY())*100 +50}, 75, 75);
                    check.draw();
                }
            }
        }
    }
    if (kingInCheck(BLACK)){
        for (ChessPiece *piece : blackBoard) {
            if (piece != nullptr) {
                if (piece->getType() == KING) {
                    Quad check = Quad({1, 0, 0}, {(piece->getX()*100)+50, (piece->getY())*100 +50}, 75, 75);
                    check.draw();
                }
            }
        }
    }

    for(ChessPiece *p : whiteBoard) {
        if(p != nullptr){
           p->draw();
        }
    }

    for(ChessPiece *p : blackBoard) {
        if(p != nullptr){
            p->draw();
        }
    }

}

std::vector<Button>* Board::getSquares(){
    return &squares;
}

bool Board::kingInCheck(faction team) {
    int kingIndex;
    if (team == WHITE) {
        for (int i = 0; whiteBoard.size() > i; i++) {
            if (whiteBoard[i] != nullptr) {
                if (whiteBoard[i]->getType() == KING) {
                    kingIndex = whiteBoard[i]->getBoardIndex();
                }
            }
        }
        for (int i = 0; blackBoard.size() > i; i++) {
            if (blackBoard[i] != nullptr) {
                std::vector<int> cleanValid = cleanValidMoves(blackBoard[i]->getX(), blackBoard[i]->getY(), BLACK);
                for (int move : cleanValid) {
                    if (move == kingIndex) {
                        return true;
                    }
                }
            }
        }
    } else {
        for (int i = 0; blackBoard.size() > i; i++) {
            if (blackBoard[i] != nullptr) {
                if (blackBoard[i]->getType() == KING) {
                    kingIndex = blackBoard[i]->getBoardIndex();
                }
            }
        }
        for (int i = 0; whiteBoard.size() > i; i++) {
            if (whiteBoard[i] != nullptr) {
                std::vector<int> cleanValid = cleanValidMoves(whiteBoard[i]->getX(), whiteBoard[i]->getY(), WHITE);
                for (int move : cleanValid) {
                    if (move == kingIndex) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool Board::gameOver() {
    bool white = true, black = true;
    for(ChessPiece* piece : whiteBoard) {
        if(piece != nullptr) {
            if(piece->getType() == KING) {
                white = false;
            }
        }
    }
    for(ChessPiece* piece : blackBoard) {
        if(piece != nullptr) {
            if(piece->getType() == KING) {
                black = false;
            }
        }
    }
    return white || black || ended;
}

void Board::setGameOver(bool over) {
    ended = over;
}

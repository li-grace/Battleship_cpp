/*Lab 6.4.4 Your own game
 ICS4U1-03 Mr. Saleem, January 1, 2019.
 This is a text-based version of BATTLESHIP.
 The player will use the keyboard to enter the placement
 of their next attack. The player has 50 moves
 to try and sink all of his/her opponent's ships. */
#include <iostream>
using namespace std;

class Ship{ //parent class ship
protected:
    int size;
    int *status, *pos;
public:
    Ship(){ //blank constructor
        size = 0;
        status = NULL;
        pos = NULL; }
    
    Ship(int length) { //constructor
        size = length;
        status = new int[length];
        pos = new int[length];
        for (int i = 0; i < length; i++)
            status[i] = 0; // set status to 0 for unhit.
        //place ship on board
        int align = rand() % 2+0; //0 for hor. 1 for vert.
        
        //generate random placements
        int Row = rand() % (8-size+1)+1;
        int Col = rand() % (8-size+1)+1;
        if (align == 0) { //horizontal ship
            for (int i = 0; i < size; i++) {
                pos[i] = Row*10 + Col;
                Col++;} }
        
        else { //vertical ship
            for (int i = 0; i < size; i++) {
                pos[i] = Col*10 + Row;
                Col++;} }
    } //end of constructor
    
    Ship &operator=(const Ship &other) {
        this->size = other.size; //copy assignment
        this->status = new int[size]; //operator
        this->pos = new int[size];
        for (int i = 0; i < size; i ++){
            this->status[i] = other.status[i];
            this->pos[i] = other.pos[i];}
        return *this; }
    
    Ship (const Ship &parent) { //copy constructor
        size = parent.size;
        status = new int[size];
        pos = new int[size];
        for (int i = 0; i < size; i ++){
            status[i] = parent.status[i];
            pos[i] = parent.pos[i]; } }
    
    ~Ship() { //class destructor
        delete [] status;
        delete [] pos; }
    
    void print(){ //prints location of ship
        for (int i = 0; i < size; i++)
            cout << pos[i]<<" ";
        cout <<endl; }
    
    int ifLocated(int coord) { //checks ship location
        for (int i = 0; i < size; i++){
            if (coord == pos[i]) { //if hit
                status[i] = 1; //change status
                return 1; }}
        return 0; }
    
    int getLocation(int index) { //returns one location
        if (index < size)          //of ship
            return pos[index];
        return 0; }
    
    int getSize(){ //returns the size of ship
        return size; }
    
    bool sunkStatus(){
        int sunk = 0;
        for (int n = 0; n < size; n++)
            sunk += status[n]; //count changed status
        if (sunk == size){  //if all squares hit
            cout <<"\n\n\n\n-\n!!!   You sunk a ship !!!\n-\n\n";
            return true; }
        return false; }
};//end of Ship class

class ShipSize2: public Ship{ //child class of Ship, 2 spaces long
    public: ShipSize2(int size = 2): Ship(size){} };

class ShipSize3: public Ship{ //child class of Ship, 3 spaces long
    public: ShipSize3(int size = 3): Ship(size){} };

class ShipSize5: public Ship{ //child class of Ship, 5 spaces long
    public: ShipSize5(int size = 5): Ship(size){} };

bool locationValid(Ship ship, Ship ships[5], int index){
    bool isUnique = true; //check for duplicate locations
    int shipSize = ship.getSize();
    
    for (int p = 0; p < shipSize; p++) { //for each pos in ship1
        for (int s = 0; s < index; s++) { //for other ships to check
            //for each pos in other ships
            for (int sp = 0; sp < ships[s].getSize(); sp++) {
                if (ship.getLocation(p) == ships[s].getLocation(sp))
                    isUnique = false; } } }
    return isUnique; }

int main() {
    int board[8][8]; //initialize empty board
    for (int d = 0; d < 8; d++) {
        for (int a = 0; a < 8; a++)
            board[d][a] = 0; }
    
    srand(time(NULL)); //initialize random int generator
    Ship shipsList[5]; //initialize ships array
    
    shipsList[0] = ShipSize2(); //create ships size 2
    shipsList[1] = ShipSize2();
    while (!locationValid(shipsList[1],shipsList,1))
        shipsList[1] = ShipSize2(); //prevent duplicate locations
    
    for (int n = 2; n < 4; n++){ //create 2 ships of size 3
        shipsList[n] = ShipSize3();
        while (!locationValid(shipsList[n],shipsList,n))
            shipsList[n] = ShipSize3(); }
    
    shipsList[4] = ShipSize5(); //create 1 ship of size 4
    while (!locationValid(shipsList[4],shipsList,4))
        shipsList[4] = ShipSize5();
    
    /*for (int c = 0; c < 5; c++){ //print ships location
     shipsList[c].print();
     }*/ //uncomment this block for cheat codes. 25 = b5, 31 = c1
    
    cout << "Welcome to Battleship! There are a total of 5 ships to sink\n";
    cout <<"2 are 2 squares long, 2 are 3 squares long, 1 is 5 squares long.\n";
    cout <<"_: unhit space. X: hit. o: miss." <<endl;
    
    char input[2]; //initalize variables for input
    char row;
    int col, target, n = 0;
    int shipsSunk = 0;
    
    while (n < 50){ //allow 50 turns
        //print the board
        cout <<"\n    1   2   3   4   5   6   7   8\n\n";
        char letter = 'a';
        for (int r = 0; r < 8; r++) {
            cout << letter++ << " ";
            for (int c = 0; c < 8; c++) {
                //print different char for different status
                switch(board[r][c]) {
                    case 0: cout << ". _ "; break;
                    case 1: cout << ". o "; break;
                    case 2: cout << ". X "; }
            }
            cout <<".\n\n"; } //print new line
        
        cout << shipsSunk <<"/5 ships sunk. "<<n++<<"/50 moves used.\n";
        cout <<"\nEnter 'q' to quit, or your target in the format: a1: ";
        cin >> input; //get user input and separate
        sscanf(input,"%c %d",&row,&col);
        target = (row-96)*10 + col; //reformat input for use
        
        //validate user input
        if (row == 'q') n = 52;
        else { //forbid duplicate shots
            if (board[target/10-1][target-(target/10)*10-1] < 3 &&
                board[target/10-1][target-(target/10)*10-1] > 0){
                cout <<"\nSorry, you entered a duplicate move.\n";}
            
            if (row > 96 && row < 105 && col > 0 && col < 9
                && board[target/10-1][target-(target/10)*10-1] == 0){
                //for valid input
                //mark the square where the user has attacked
                board[target/10-1][target-(target/10)*10-1] = 1;
                
                for (int i = 0; i < 5; i++) { //check if ship hit
                    if (shipsList[i].ifLocated(target)){
                        board[target/10-1][target-(target/10)*10-1] = 2;
                        shipsSunk += shipsList[i].sunkStatus(); }}}
            
            if (row > 'h' || !col || col > 8) //input out of range
                cout <<"Invalid input, enter in format a1 (eg. b5, d8)\n";
            
            if (shipsSunk == 5) { //if user is successful
                cout << "Good work! All 5 ships have been destroyed. \n";
                n = 51; } }//exit condition
    } //end of while loop
    
    //game over messages
    if (n == 52) //if user chose to quit
        cout << "User chose to quit."<<endl;
    else if (shipsSunk < 5) //if user failed
        cout <<"\n\nNice try. Sorry, you used up all 50 moves.\n";
    
    cout <<"Game over! Thanks for playing." << endl;
    return 0;
}

#include <iostream>
#include <cstring>
#include <cmath>

using namespace std;

/*
members:
Rhyle Abram Regodon
Theressa Marie Tan
Darling Cathyrene Villas


*/

typedef struct node{
    int weight; // size of the plate
    struct node *previousNode ; //pointer pointing to the previous node, NULL if it is bottom
    struct node *nextNode ; //pointer pointing to next node, NULL if it is HEAD
} node;

typedef struct stack{
    struct node *bottom ; //pointer referring to the end node of the pole
    struct node *head ; //pointer referring to the current head of the pole
} stack;
typedef struct option{
    int source;
    int destination;
} choice;
const int numberOfPoles= 3 ;
const char poleLabel = 'A';

/*
    print c character n times
*/
void printNStrings(int i, int n , char c){

    if(i < n){
        cout << c;
        printNStrings(++i , n, c);
    }else{
        return;
    }
}



/*
     a.) calculate the possible move given the current state of the nodes
     b.) main logic is to check each head, and compare to other heads,
         if head A is < head B = possible move
         if head A is > head B = not possible move
     c.) can be recursive, but not sure
*/
int calculatePossibleMoves(stack *poles[], choice *optionPointer[]){

   int options = 0;
   memset(optionPointer, NULL, sizeof(optionPointer));

   for(int i= 0 ; i < numberOfPoles ; i++){
        stack *currentPole = poles[i];
        if(currentPole->head == NULL){
            continue;
        }else{
            // loop through poles
            for(int j = 0; j < numberOfPoles  ; j++){
                stack *currentPoleInner = poles[j];
                if(currentPole != currentPoleInner ){

                    //compare head

                    if((currentPoleInner->head == NULL)
                       || (currentPole->head->weight < currentPoleInner->head->weight)){
                        cout << string(5, ' ') <<"(" << options+1 << ")" << static_cast<char>(poleLabel + i) << "->" << static_cast<char>(poleLabel + j) << " \n";

                        optionPointer[options]= (choice *)malloc(sizeof(choice) *1);
                        optionPointer[options]->source = i;
                        optionPointer[options]->destination = j;
                        options++;

                    }
                }else{
                    continue;
                }

            }
        }

   }

   return options;
}
/*
     a.) print the three poles horizontally and the values of each node belonging to each pole, start from
      BOTTOM node to HEAD node
     b.) if both BOTTOM and HEAD node is NULL, just print n*2 '=' chars
*/

void printPoles(stack *poles[3], int n){

    for(int i =0 ; i < numberOfPoles ; i++){
        stack *currentPole = poles[i];
        cout << "  |\n";
        cout << static_cast<char>(poleLabel + i) << " +";

        if(currentPole->bottom != NULL){
            int withNodes = 0;
            node *currentNode = currentPole->bottom;
            while(currentNode!= NULL){
                cout << currentNode->weight << "=";
                currentNode = currentNode->nextNode;
                withNodes++;
            }

            if(n != withNodes){
                int extraChar =(n- withNodes)*2;
                printNStrings(0,extraChar, '=');
            }
            cout << "\n";

        }else{
            printNStrings(0, n*2, '=');
            cout << "\n";

        }
        cout << "  |\n\n";

    }
}


/*
  when function is called:
  a.) create a queue of linked list type struct node, given the n number of plates
  b.) set pole2 and pole3 to NULL ,meaning HEAD and BOTTOM still points to null

*/
void initializePoles(stack *poles[] , int n){


    for(int i = 0 ; i< 3 ; i++){
        poles[i] =(stack *)malloc(sizeof(stack) * 1);
        poles[i]->bottom = NULL;
        poles[i]->head = NULL;
    }

    int i = n;
    node *previousNode = NULL;

    if(poles[0] != NULL){
        stack *pole1 = poles[0];
        while(i > 0){


            node *newNode  = (node *)malloc(sizeof(node)* 1);
            newNode->weight = i;
            newNode->previousNode = previousNode;
            newNode->nextNode = NULL;
            if(previousNode != NULL){
                previousNode->nextNode = newNode;
            }
            if(newNode->previousNode == NULL){
                pole1->bottom = newNode;
            }
            pole1->head = newNode;
            previousNode = newNode;

            i--;
        }
    }
}

void printMoves(int disk, int moves){
    //a.) print info on no. of minimum moves required
    int minimumMoves = pow(2, disk) - 1;
    cout << "Minimum moves needed: " << minimumMoves << "\n";

    // b.) print info on no. of moves already made
    cout << "Number of moves made: " << moves << "\n";
}


/*
      a.) print possible moves for the calculatePossibleMoves function
      b.) print fixed options

      such as :
      (R) Restart with N disks
      (X) Exit game
*/
int printOptions(stack *poles[], choice *optionPointer[]){

    int availableOptions = 0;
    cout << "Next moves: \n";
    availableOptions = calculatePossibleMoves(poles, optionPointer);

    return availableOptions;
}


void printDefaultOptions(int availableMoves){
    if(availableMoves == 0){
        cout << "Next moves: \n";
    }
    cout << "(R) Restart with N disks \n";
    cout << "(X) Exit game \n";
}


int getPlateValue(){
    char newPlate;
    int validChoice = 0;
    do{
        cout << "Enter new N value: ";
        cin >> &newPlate;
        if(isdigit(newPlate))
        {
            newPlate = newPlate - '0';
            if(newPlate > 0){
                validChoice = 1;
            }
        }
        if(!validChoice){
            cout << "You entered an invalid N value. Try again!\n";
        }
    }while(validChoice == 0);
    return newPlate;
}
int getOption(int possibleMoves){

    char option;
    int choice = 0;
    do{
        cout << "Your choice?: ";
        cin >> &option;

        if(option == 'R' || option == 'r')
        {
            choice = 0;
        }
        else if(option == 'X' || option == 'x')
        {
            choice = -1;
        }
        else if(isdigit(option))
        {
            choice = option - '0';
            // Check if the option of the user is in the valid option list
            if((choice <= possibleMoves && choice >= 1)){
                choice = choice;
            }else{
                // Invalid Choice
                choice = -2;
                cout << "Invalid choice! Please try again.\n";
            }
        }
    }while(choice == -2);
    return choice;
}

/*
 a.) update the state of the poles depending on the option selected
 b.) general logic:

     given the ff.
        pole1 = source pole
        pole2 = destination pole

     for pole1:
     b.1 ) check pole1.head, if pole1.head.previousNode is NULL, meaning it is the only plate on the pole
     b.2 ) if it is the only plate, set pole1.head = NULL , so no plates are left on pole1.
     b.3 ) if not set pole1.head = pole1.head.previousNode. and
      with new HEAD, pole1.head.nextnode = null ( so previousNode is now HEAD)

     for pole2:
     b.4 )  check pole2.head, if pole2.head is NULL, meaning no plates on the pole
     b.5 ) if pole has no plates , pole2.head = pole1.head
     b.6 ) if it has more plates, pole2.head.nextNode = pole1.head , and pole2.head = pole1.head
     ( so new node is now the HEAD)
*/
void executeOption(stack *poles[], choice *selectedOption){

    stack *sourcePole = poles[selectedOption->source];
    stack *destinationPole = poles[selectedOption->destination];

    node *prevNode = NULL;
    if(sourcePole->head != sourcePole->bottom){
        prevNode = sourcePole->head->previousNode;
    }else{
        sourcePole->bottom= NULL;
    }
    //destination
    if(destinationPole->bottom != NULL){
        sourcePole->head->previousNode = destinationPole->head;
        destinationPole->head->nextNode = sourcePole->head;
        destinationPole->head =sourcePole->head;

    }else{
        sourcePole->head->previousNode = destinationPole->head;
        destinationPole->bottom = sourcePole->head;
        destinationPole->head = sourcePole->head;
    }

    //source
    sourcePole->head = prevNode;
    if(sourcePole->head != NULL){
        sourcePole->head->nextNode = NULL;
    }

}

/*
     a.) gets option selected and checks if it is numeric, if numeric check if poles are in their correct states
         (meaning the game is won)
         return 1 if yes, return 0 if no.
     b.) game is solved if 1st and 2nd poles are empty AND 3rd pole is in correct order from botom to head
     c.) can use short circuiting and recursion, not totally sure
*/
int evaluateOption(stack *poles[], int n){

    if(poles[0]->bottom == NULL && poles[0]->head == NULL
    && poles[1]->bottom == NULL && poles[1]->head == NULL){
        node *currentNode = poles[2]->bottom;
        if(currentNode != NULL){

            int weight = n;
            while(currentNode != NULL){
                if(currentNode->weight == weight){
                    currentNode= currentNode->nextNode;
                    weight--;
                }else{
                    return 0;
                }
            }
            if(weight == 0){ //reached the top
                return 1;
            }else{
                return 0;
            }
        }else{
            return 0;
        }

    }else{
        return 0;
    }
}

void printGameResult(int moves){
    //  display congratulatory message
    cout << "Congratulations! You completed the game in" << moves << "move/s :D \n";
}

void printExitMessage(){
    cout << "Goodbye for now! See you again!";
}


void  printGameIntro(){
    // Print Introduction
    cout << "***********************\n";
    cout << "*  The Tower of Hanoi *\n";
    cout << "*   Interactive Game  *\n";
    cout << "*    version 2.0      *\n";
    cout << "***********************\n";
}
void freePointers(stack *poles[] , choice *optionPointer[] ){

    const int numberOfPoles= 3 ;
    for(int i = 0 ; i< 3 ; i++){
        node *currentNode = poles[i]->bottom;
        node *nextNode = NULL;
        while(currentNode != NULL){
            nextNode = currentNode->nextNode;
            free(currentNode);
            currentNode = nextNode;
        }
        free(poles[i]);
        free(optionPointer[i]);
    }


}
int main()
{
    int moves = 0; // moves done by user;
    int gameResult = 0; // default game not stopped
    int exit = 0;
    int n = 3 ; // default to 3 plates
    int option = -1; // default to exit game
    int availableOptions = 0;
    //create poles
    stack *poles[3];
    choice *optionPointer[3] ;
    initializePoles(poles, n); // function to create the initial queue for each poles, on game initalization default to 3
    printGameIntro(); // print game intro


    do{
        availableOptions = 0;
        printPoles(poles,n); // print the current state of the poles
        if(!gameResult){
            printMoves(n, moves); // print # of minimum moves possible and #moves already made
            availableOptions = printOptions(poles, optionPointer); // print possible options for the user
        }else{
            gameResult = 0; // re-initalize result
        }

        printDefaultOptions(availableOptions); // print default options only
        option = getOption(availableOptions); // get user input
        if(option > 0){ // option not X and not R
            executeOption(poles,optionPointer[option-1]);// update the current state of the poles based on the option selected
            gameResult = evaluateOption(poles,n); // evaluate option
            moves++;
            if(gameResult){
                printGameResult(moves); // print the game result
            }
        }else if(option == 0){
            n = getPlateValue(); // get the new N number
            initializePoles(poles, n); // re-initialize based on the new n number
            moves = 0;
        }else if(option == -1){
            exit = 1; //exit the game
        }
    }while(exit != 1);

    printExitMessage();


    freePointers(poles,optionPointer);
    return 0;
}
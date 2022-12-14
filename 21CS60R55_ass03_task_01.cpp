/*
 * Problem Statement #1: MasterMind Game Using SAT Solver (without Repetition of colours).
 * Author: ADARSH G KRISHNAN : 21CS60R55
 * Timestamp : 10-09-2021 
 * Time Complexity : O(n^2)
 * Language:  C++ 
 */

//Including Header Files.
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <vector>
#include "togasat.hpp"

using namespace std;

#define No_Pegs 4 //No. of Pegs or Slots.
#define No_col 8  //No. of colours.
#define Max_Iterations 20 //Maximum No. of Iterations.

//Defining a structure to represent No. of Black & White Pegs in the Feedback.
struct Score{
    int Black,White;
};


typedef struct Score Score;

vector<vector<int> > vec;        //Declaring Vector of Vectors to store clauses.
vector<int> CodeMakerSecretCode; //Declaring Vector to store Secret code Generated by CodeMaker.
vector<int> CodeBreakerGuess;    //Declaring Vector to store Guesses Made by the SAT Solver.

Score score; //Object of Structure Score.

//Utility Functions.

//Function to Sort the output from a SAT Solver to it's correct position using an Auxillary Array.
void sort(){
    int codebreaktemp[No_Pegs];
    for(int i=0;i<No_Pegs;i++){
        int temp = CodeBreakerGuess[i]%4;
        switch(temp){
            case 0:{codebreaktemp[3]=CodeBreakerGuess[i];break;}
            case 1:{codebreaktemp[0]=CodeBreakerGuess[i];break;}
            case 2:{codebreaktemp[1]=CodeBreakerGuess[i];break;}
            case 3:{codebreaktemp[2]=CodeBreakerGuess[i];break;}
        }
    }
    CodeBreakerGuess.clear();
    for(int i=0;i<No_Pegs;i++){
        CodeBreakerGuess.push_back(codebreaktemp[i]);
    }

}

//Function to get Colour associated with Each Position from 1 to 32.
char color(int code)                                          
{
    if(code>=1 && code<=4)        return 'R';
    else if(code>=5 && code<=8)   return 'G'; 
    else if(code>=9 && code<=12)  return 'B';
    else if(code>=13 && code<=16) return 'Y';
    else if(code>=17 && code<=20) return 'O';
    else if(code>=21 && code<=24) return 'P';
    else if(code>=25 && code<=28) return 'W';
    else if(code>=29 && code<=32) return 'K';                          
    return 'R';
}

//Function to get the Initial position of Each Colour.
int code_color(char color)
{
    switch(color){
        case 'R': return 1;
        case 'G': return 5;
        case 'B': return 9;
        case 'Y': return 13;
        case 'O': return 17;
        case 'P': return 21;
        case 'W': return 25;
        case 'K': return 29;
    }
}

//Function to Generate Secret Code by CodeMaker Randomly and store in the CodeMakerSecretCode Vector.
vector<int> CodeMaker(){
    srand((unsigned) time(0));
    int random=rand() % 8;
    for(int i=0;i<No_Pegs;++i){
        random+=5;
        CodeMakerSecretCode.push_back(random);
    }
    return CodeMakerSecretCode;
}

//Function to get output from SAT Solver & store in the CodeBreakerGuess Vector.
void findSAT(){
    togasat::Solver solver;
    for(int i=0; i< vec.size(); i++){
        solver.addClause(vec[i]);
    }
    solver.solve();
    vector<int> r = solver.printAnswer();
    CodeBreakerGuess.clear();
    for (auto f : r){
        CodeBreakerGuess.push_back(f);
    }
}

//Function to check if the peg inside the codebreaker guess is in secretcode generated by CodeMaker or not.
bool IsMember(int guess,int position){
    for (int i=0;i<No_Pegs;i++){
        if (CodeMakerSecretCode[i] == guess && i==position || (color(CodeMakerSecretCode[i])==color(guess)) && i==position){
            return true;
        }
    }
    return false;
}

//Function to Generate feedback based on the current guess by the code Breaker.
void FeedbackGenerator()
{
    int flag,temp;
    score.Black = 0;
    score.White = 0;
    for (int i=0;i<No_Pegs;i++){
        temp=CodeBreakerGuess[i];
        if (IsMember(temp,i))
            score.Black++;
    }
    for (auto t : CodeMakerSecretCode){
        flag=0;
        for (auto k : CodeBreakerGuess){
            if (color(t) == color(k)){
               flag=1;
            }    
        }
        if(flag)
              score.White++;    
    }
    score.White=score.White-score.Black;
    cout<<"\tB-"<<score.Black<<" and "<<"W-"<<score.White<<"\t//Guess and Score\n";

}

//Function to generate Clauses for Black & White Pegs.
void clausemaker(){
    int a = CodeBreakerGuess[0];
    int b = CodeBreakerGuess[1];
    int c = CodeBreakerGuess[2];
    int d = CodeBreakerGuess[3];
    if(score.Black==0){
        vector<int> clause{};
        for (auto k : CodeBreakerGuess)
        {   clause.clear();
            clause.push_back(-k);
            vec.push_back(clause);
        }
    } 
    if(score.Black==1){
        vector<int> clause{};
        clause.push_back(-a);
        clause.push_back(-b);
        vec.push_back(clause);
        clause.clear();
        clause.push_back(-c);
        clause.push_back(-d);
        vec.push_back(clause);
        clause.clear();
        clause.push_back(-b);
        clause.push_back(-d);
        vec.push_back(clause);
        clause.clear();
        clause.push_back(-b);
        clause.push_back(-c);
        vec.push_back(clause);
        clause.clear();
        clause.push_back(-a);
        clause.push_back(-c);
        vec.push_back(clause);
        clause.clear();
        clause.push_back(a);
        clause.push_back(b);
        clause.push_back(c);
        clause.push_back(d);
        vec.push_back(clause);
        clause.clear();
        }
    if(score.Black==2){
        vector<int> clause{};
        clause.push_back(-a);
        clause.push_back(-b);
        clause.push_back(-d);
        vec.push_back(clause);
        clause.clear();
        clause.push_back(-a);
        clause.push_back(-b);
        clause.push_back(-c);
        vec.push_back(clause);
        clause.clear();
        clause.push_back(-a);
        clause.push_back(-c);
        clause.push_back(-d);
        vec.push_back(clause);
        clause.clear();
        clause.push_back(-b);
        clause.push_back(-c);
        clause.push_back(-d);
        vec.push_back(clause);
        clause.clear();
        clause.push_back(a);
        clause.push_back(b);
        clause.push_back(d);
        vec.push_back(clause);
        clause.clear();
        clause.push_back(a);
        clause.push_back(b);
        clause.push_back(c);
        vec.push_back(clause);
        clause.clear();
        clause.push_back(a);
        clause.push_back(c);
        clause.push_back(d);
        vec.push_back(clause);
        clause.clear();
        clause.push_back(b);
        clause.push_back(c);
        clause.push_back(d);
        vec.push_back(clause);
        clause.clear();
        }
    if(score.Black==3){
        vector<int> clause{};
        clause.push_back(a);
        clause.push_back(b);
        vec.push_back(clause);
        clause.clear();
        clause.push_back(c);
        clause.push_back(d);
        vec.push_back(clause);
        clause.clear();
        clause.push_back(b);
        clause.push_back(d);
        vec.push_back(clause);
        clause.clear();
        clause.push_back(b);
        clause.push_back(c);
        vec.push_back(clause);
        clause.clear();
        clause.push_back(a);
        clause.push_back(c);
        vec.push_back(clause);
        clause.clear();
        clause.push_back(a);
        clause.push_back(d);
        vec.push_back(clause);
        clause.clear();
        clause.push_back(-b);
        clause.push_back(-d);
        clause.push_back(-a);
        clause.push_back(-c);
        vec.push_back(clause);
        clause.clear();
        }
}

//Driver Function.
int main(){

    vector<int> clause;  //Temporary vector to add athe clauses to vector of vectors.

    //Make the Secret Code.
    CodeMakerSecretCode=CodeMaker();
    
    //Printing th Secret Code Generated.
    cout<<"\t\t";
    for (auto t : CodeMakerSecretCode){
        cout << color(t) << " ";
    }
    cout<<"\t\t\t//Hidden Code\n";

    //Constraint 1 : Same colour should not be repeated at different positions.
    for(int i=1;i<=32;i++){
        if (i%4 == 1){
            clause.push_back(-i);
            clause.push_back(-(i + 1));
            vec.push_back(clause);
            clause.clear();
            clause.push_back(-i);
            clause.push_back(-(i + 2));
            vec.push_back(clause);
            clause.clear();
            clause.push_back(-i);
            clause.push_back(-(i + 3));
            vec.push_back(clause);
            clause.clear();
        }
        else if (i%4 == 2){
            clause.push_back(-i);
            clause.push_back(-(i + 1));
            vec.push_back(clause);
            clause.clear();
            clause.push_back(-i);
            clause.push_back(-(i + 2));
            vec.push_back(clause);
            clause.clear();
            clause.push_back(-i);
            clause.push_back(-(i - 1));
            vec.push_back(clause);
            clause.clear();
        }
        else if (i%4 == 3){
            clause.push_back(-i);
            clause.push_back(-(i + 1));
            vec.push_back(clause);
            clause.clear();
            clause.push_back(-i);
            clause.push_back(-(i - 1));
            vec.push_back(clause);
            clause.clear();
            clause.push_back(-i);
            clause.push_back(-(i - 2));
            vec.push_back(clause);
            clause.clear();
        }
        else{
            clause.push_back(-i);
            clause.push_back(-(i - 1));
            vec.push_back(clause);
            clause.clear();
            clause.push_back(-i);
            clause.push_back(-(i - 2));
            vec.push_back(clause);
            clause.clear();
            clause.push_back(-i);
            clause.push_back(-(i - 3));
            vec.push_back(clause);
            clause.clear();
        }
        findSAT();
    }

    //Constraint 2 : No 2 Colour should be present at the same Position.
    for(int i=1;i<=28;i++){
        for(int j=i+4;j<=32;j+=4){
            clause.push_back(-i);
            clause.push_back(-j);
            vec.push_back(clause);
            findSAT();
            clause.clear();
        }
    }

    //Constraint 3 : Atleast 4 Colour should be present in the output of SAT Solver. 
    for(int i=1;i<=4;i++){
        clause.push_back(i);
        clause.push_back(i+4);
        clause.push_back(i+8);
        clause.push_back(i+12);
        clause.push_back(i+16);
        clause.push_back(i+20);
        clause.push_back(i+24);
        clause.push_back(i+28);
        vec.push_back(clause);
        findSAT();   
        clause.clear();
    }

    //Iterator variable to check Max. Iteration to crossed or not.
    int limit=0;  

    cout<<"\nInteration "<<limit+1<<" :\t";


    //Sorting the output of SAT Solver to correct Position.
    sort(); 

    //Printing Initial Guess by SAT Solver.
    for (auto t : CodeBreakerGuess){
        cout << color(t) << " ";
    }
    
    //Loops Until Black Count is equal to 4 or Max. no. of Iterations has reached.
    while(score.Black!=4 && limit<Max_Iterations){
        FeedbackGenerator(); //Generating Feedback based on current Guess.
        clausemaker();       //Making clauses based of feedback.
        findSAT();           //Adding clauses to SAT Solver.

        //Checking if Code has been Cracked or Not.
        if(score.Black ==4){
            break;
        }

        //Sorting the output of SAT Solver to correct Position.
        sort();
        cout<<"\nInteration "<<limit+1<<" :\t";

        //Printing Current Guess by SAT Solver.
        for (auto t : CodeBreakerGuess){
            cout << color(t) << " ";
        }
        limit++;
    }

    cout<<"\nYou Won!!\n\n";
  
}
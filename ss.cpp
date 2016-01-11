#include <stdio.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include "puzzle.h"
#include <stack>
#include <list>

using namespace std;

void checkInput(char* input);
Square* findDecidableSq(Puzzle P);
bool guess(Puzzle &, Puzzle &);

int main()
{
	char input[83];
	Square* curSq;
  bool simpFound;

  list<Puzzle> solutions;
  stack<Puzzle> alternatives;
  //Square* S = new Square(0,0);
  
  try{
    checkInput(input);
    Puzzle P(input);
    
    alternatives.push(P);

    while(!alternatives.empty())
    {
      P = alternatives.top();
      alternatives.pop();
      while((curSq = findDecidableSq(P)))
        P.decide(curSq); 

      //
      //cout << "numba 1" << endl;
      //P.print();
      //      
 
      simpFound = true;
    
      while(!P.solved() && simpFound)
      {

        //
        //cout << "simp found" << endl;
        //

        while(simpFound)
        {
          simpFound = P.HiddenSingles();
      
          if(!simpFound)
          {
            
            //
            cout << "inside guess" << endl;
            //P.print();
            //
            Puzzle alt;
            if((simpFound = guess(P, alt)))
            {
              //
              //alt.print();
              //cout << endl << endl;
              //
              alternatives.push(alt);
              (alternatives.top()).print();
              cout << endl;
            }
          }

          if(simpFound)
          {
            while((curSq = findDecidableSq(P)))
              P.decide(curSq);
          }
        }
      }

      if(P.solved())
        solutions.push_back(P);
    }//while alternatives_empty

    if(solutions.empty())
      cout << "No solutions." << endl;
    else
    {
      for(list<Puzzle>::iterator it = solutions.begin(); it != solutions.end(); ++it)
        (*it).print();
    }
    //P.print();
  }
	
  catch(char n)
  {
    if(n == '1')
      cout << "ERROR: expected <value> saw \\n" << endl;
    else if(n == '2')
      cout << "ERROR: expected <value> saw <eof>" << endl;
    else
      cout << "ERROR: expected <value> saw " << n << endl;
  } //values error
  
  catch(char* n)
  {
    if(n == 0)
      cout << "ERROR: expected \\n saw <eof>" << endl;
    else
      cout << "ERROR: expected \\n saw " << *n << endl;
  } //\n error
    
  catch(string s)
  {
    if(s == "\\n")
      cout << "ERROR: expected <eof> saw \\n" << endl;
    else
      cout << "ERROR: expected <eof> saw " << s << endl;
  } //eof error
 
  catch(int n)
  {
    cout.width(2);
    cout.fill(0);
    if(n < 100){
      cout << "ERROR: expected <value> saw \\x";
      cout.width(2);
      cout.fill('0');
      cout << hex << n << endl;
    }
    else if(n < 150){
      cout << "ERROR: expected \\n saw \\x";
      cout.width(2);
      cout.fill('0');
      cout << hex << (n-100) << endl;
    }
    else{
      cout << "ERROR: expected <eof> saw \\x";
      cout.width(2);
      cout.fill('0'); 
      cout << hex << (n-200) << endl;
    }
  }
 
	return 0;
}

//checkInput()=================================================================
void checkInput(char* input)
{
  int i = 0; 

  while(i < 81)
  {
    input[i] = getchar();

    if((input[i] == '\n'))
      throw ('1');
    else if(input[i] == EOF)
      throw ('2');
    else if(input[i] < 32)
    {
      int h = input[i];
      throw(h);
    }
    else if((input[i] < '1' || input[i] > '9') && (input[i] != '.'))
      throw (input[i]);

    i++;
  }//endwhile, reading the first 81 characters

  //82nd character: \n
  input[81] = getchar();

  if(input[81] != '\n')
  {
    if(input[81] < 32)
    {
      int h = input[81] + 100;
      throw(h);
    }
    else if(input[81] == EOF)
    {
      char* n = 0;
      throw(n);
    }

    throw(&input[81]);
  }//error check for the 82nd character

  //83rd character: EOF
  input[82] = getchar();

  if(input[82] != EOF)
  {
    if(input[82] < 32)
    {
      int h = input[82] + 200;
      throw(h);
    }    

    string s;

    if(input[82] == '\n')
    {
      s = "\\n";
      throw(s);
    }

    s = input[82];
    throw(s);
  }//error check for 83rd character
}//checkInput()================================================================

//findDecidableSq(Puzzle)======================================================
Square* findDecidableSq(Puzzle P)
{
  Square* temp = P.head();
  
  for(int i = 0; i < 81; i++)
  {
    if((!P.getCleared(temp)) && (P.getSqSetSize(temp) == 1)){
       return temp;
    }

    temp = P.next(temp, 1);
  }

  return NULL; 
}
//findDecidableSq(Puzzle)======================================================

//guess may or may not work perfectly
bool guess(Puzzle &P, Puzzle &alt)
{
  Square* GG = P.head();  
  int i;

  for(i = 0; i < 82; i++)
  {
    if(GG == NULL || P.getSqSetSize(GG) == 2)
      break;
    else if(P.getSqSetSize(GG) < 2 
              || (P.next(P.head(), i) != NULL
              && P.getSqSetSize(GG) > P.getSqSetSize(P.next(P.head(), i))))
      GG = P.next(P.head(), i);
  }//for loop to find square set of size 2 or find the smallest
 
  //
  cout << "pos " << i << endl;
  GG->printSSet(); 
  //
 
  if(GG == NULL)
    return false;
  
  alt = *(new Puzzle());
  P.clone(alt);
  P.choose(alt, GG);
  return true;
}//guess(Puzzle&, Puzzle&)=====================================================

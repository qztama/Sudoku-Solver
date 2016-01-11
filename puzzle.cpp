#include "puzzle.h"

Puzzle::Puzzle(char* input)
{
  for(int i = 0; i < 81; i++)
    grid[i] = new Square(input[i], i);
  
  grid[81] = NULL;
}//puzzle constructor=========================

Puzzle::Puzzle()
{
  for(int i = 0; i < 81; i++)
    grid[i] = new Square('a', 0);
}//puzzle constructor

void Puzzle::clone(Puzzle &alt)
{
  for(int i = 0; i < 81; i++)
  {
    //cout << alt.grid[i] << " " << grid[i] << endl;
    *(alt.grid[i]) = *(grid[i]);
    //cout << alt.grid[i]->val << " " << alt.grid[i]->val << endl;
  }

  alt.grid[81] = NULL;
}//Puzzle::clone()=============================================================
/*Puzzle::Puzzle(const Puzzle &rhs)
{
  solved = rhs.solved;
  cout << "Copy";
  for(int i = 0; i < 82; i++)
  {
    grid[i] = new Square('a', 0);
  }
}//puzzle copy constructor=====================================================*/

//Puzzle::~Puzzle();

void Puzzle::clearRow(Square* S)
{
    
    char val = S->val;  

    //row==========
    Square* temp = next(S, 1);
    int curPos = (S->pos % 9) + 1;

    while(curPos < 9)
    {
      //cout << "in right "<< temp->pos << " ";
      temp->posVal.erase(val);
      temp = next(temp, 1);
      curPos++;
    }//right while
    
    temp = next(S, -1);
    curPos = (S->pos % 9) - 1;
    
    while(curPos >= 0)
    {
      temp->posVal.erase(val);
      temp = next(temp, -1);
      curPos--; 
    }//left while
}//Puzzle::clearRow()==========================================================

void Puzzle::clearCol(Square* S)
{
    char val = S->val;  
    
    //col==========
    Square* temp = next(S, ROW);
    int curPos = (S->pos/9) + 1;
    
    while(curPos < 9)
    {
      temp->posVal.erase(val);
      temp = next(temp, ROW);
      curPos++;
    }//down while

    temp = next(S, -ROW);
    curPos = (S->pos/9) - 1;

    while(curPos >= 0)
    {
      temp->posVal.erase(val);
      temp = next(temp, -ROW);
      curPos--;
    }//up while
}//Puzzle::clearCol()==========================================================

void Puzzle::clearReg(Square* S)
{
    char val = S->val;
    int curPos;
    //region=======
    //determine which region
    int x = S->pos%9;
    int y = S->pos/9;
    
    if(x < 3){
      if(y < 3){
        curPos = 0; //top left
      }
      else if(y < 6)
        curPos = 27; //middle left
      else
        curPos = 54;//bottom left
    }//left
    else if(x < 6){
      if(y < 3)
        curPos = 3; //top middle
      else if(y < 6)
        curPos = 30; //center
      else
        curPos = 57;//bottom middle
    }//middle
    else{
      if(y < 3)
        curPos = 6; //top right
      else if(y < 6)
        curPos = 33; //middle right
      else
        curPos = 60;//bottom right
    }//right

    //clear region
    for(int i = 0; i < 3;++i)
    {
      for(int j = 0;j < 3;++j)
      {
        //cout << curPos << " ";
        if((curPos + (9 * i + j)) != S->pos)
          grid[curPos + (9 * i + j)]->posVal.erase(val);
      }
    }
}//clearReg(Square* S)=========================================================

void Puzzle::clear(Square* S)
{
  //Square* S = grid[60];
  if(!S->cleared)
  {
    S->cleared = true;
    clearRow(S);
    clearCol(S);
    clearReg(S);
  }//if S is cleared
}//Puzzle::clear===============================================================

bool Puzzle::getCleared(Square* S)
{
  return S->cleared;
}//Puzzle::getCleared

bool Puzzle::solved()
{
  int i;

  for(i = 0; i < 81; i++)
  {
    if(getSqSetSize(grid[i]) != 1)
      break;
  }
  
  if(i == 81)
    return true;

  return false;
}//bool Puzzle::solved()=======================================================

bool Puzzle::HiddenSingles()
{
  int freq[9];
  bool simpFound = false;
  int j;
  set<char>::iterator it;
  Square* temp;
  
  //row
  //for going down
  for(int i = 0; i < 81; i+=9)
  { 
    //set each element in freq to 0
    for(int n = 0; n < 9; n++)
      freq[n] = 0;

    //for going across
    for(j = 0; j < 9; j++)
    {
      temp = next(head(), (i+j));
      
      for(it = temp->posVal.begin(); !(getCleared(temp)) && it != temp->posVal.end(); ++it)
        freq[(*it - '0' - 1)]++;
    }//for loop, for going across to find frequency
    
    for(j = 0; j < 9; j++)
    {
      if(freq[j] == 1)
        break;
    }//for loop to check for frequency of 1

    if(j != 9)
    {
      simpFound = true;
      j= j + '0' + 1;
      
      for(int k = 0; k < 9; k++)
      {
        temp = next(head(), (i+k));
        
        if(temp->posVal.count(j) == 1)
          break;
      }//for loop to find square with hidden single
      
      temp->posVal.clear();
      temp->posVal.insert(j);
    }//if hidden single found
  }//for loop, for going down

  //col
  //for going across
  for(int i = 0; i < 9; i++)
  { 
    //set each element in freq to 0
    for(int n = 0; n < 9; n++)
      freq[n] = 0;

    //for going down
    for(j = 0; j < 81; j+=9)
    {
      temp = next(head(), (i+j));
      
      for(it = temp->posVal.begin();!(getCleared(temp)) && it != temp->posVal.end(); ++it)
        freq[(*it - '0' - 1)]++;
    }//for loop, for going across to find frequency
    
    for(j = 0; j < 9; j++)
    {
      if(freq[j] == 1)
        break;
    }//for loop to check for frequency of 1

    if(j != 9)
    {
      simpFound = true;
      j= j + '0' + 1;
      
      for(int k = 0; k < 81; k+=9)
      {
        temp = next(head(), (i+k));
        
        if(temp->posVal.count(j) == 1)
          break;
      }//for loop to find square with hidden single
      
      temp->posVal.clear();
      temp->posVal.insert(j);
    }//if hidden single found
  }//for loop, for going across

  //region
  for(int i = 0; i < 63; i+=3)
  {
    if((i != 0) && (i % 9 == 0))
	  i+=18;
	
	  //set each element in freq to 0
	  for(int n = 0; n < 9; n++)
      freq[n] = 0;
	  
	  for(int k = 0; k < 21; k++)
	  {
	    if((k%3 == 0) && (k != 0))
		    k+=6;
		  
      temp = next(head(), (i+k));
      
      for(it = temp->posVal.begin();!(getCleared(temp)) &&  it != temp->posVal.end(); ++it)
        freq[(*it - '0' - 1)]++;
	  }//for loop for traveling the region, finds frequency

    for(j = 0; j < 9; j++)
    {
      if(freq[j] == 1)
        break;
    }

    if(j != 9)
    {
      simpFound = true;
      j = j + '0' + 1;      

      for(int k = 0; k < 21; k++)
      {
        if((k != 0) && (k%3 == 0))
          k+=6;

        temp = next(head(), (i+k));

        if(temp->posVal.count(j) == 1)
          break;
      }

      temp->posVal.clear();
      temp->posVal.insert(j);
    }//if hidden single found
  }//for loop for traveling between region

  return simpFound;
}//Puzzle::HiddenSingles()=====================================================

Square* Puzzle::head()
{
  return grid[0];
}//Puzzle::head()==============================================================

Square* Puzzle::next(Square* S, int i)
{
  if((S->pos + i) >= 0)
    return grid[S->pos + i];
  
  return NULL;
}//Puzzle::next================================================================

void Puzzle::choose(Puzzle &alt, Square* GG)
{
  int pos = GG->pos;
  char guessVal = *(GG->posVal.begin());
  
  GG->posVal.clear();
  GG->posVal.insert(guessVal);

  (alt.grid[pos])->posVal.erase(guessVal);
}//Puzzle::choose(Puzzle&, Square*)============================================

void Puzzle::print()
{
  for(int i = 0; i < 81; i++)
      cout << grid[i]->val;
 
  cout << endl;
}//puzzle::print()============================

void Puzzle::printSet()
{
  set<char>::iterator it;
  for(int i = 0; i < 81; i++)
  {
    for(it=grid[i]->posVal.begin();it!=grid[i]->posVal.end();++it)
      cout << *it << " ";
    cout << endl;
  }
}

int Puzzle::getSqSetSize(Square* S)
{
  return S->posVal.size();
}//Puzzle::getSqSetSize(Square*)===============================================

void Puzzle::decide(Square* S)
{
  set<char>::iterator iter = S->posVal.begin();
  S->val = *iter;
  //cout << S->val << " ";  

  clear(S);
}//Puzzle::decide()============================================================

Square::Square(char value, int i):cleared(false),val(value),pos(i)
{
  if(val != '.')
    posVal.insert(val);
  else{
    for(char ch = '1'; ch <= '9'; ++ch)
      posVal.insert(ch); 
  }
}//square constructor=========================

void Square::printSSet()
{
  set<char>::iterator it;  

  for(it=posVal.begin();it!=posVal.end();++it)
      cout << *it << " ";
}

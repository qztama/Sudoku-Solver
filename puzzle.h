#ifndef PUZZLE_H
  #define PUZZLE_H

  #define COL 1
  #define ROW 9
  //included dependencies
  #include <set>
  #include <iostream>

  using namespace std;

  class Square
  {
    public:
      Square(char value, int i);
      void printSSet();
      //Square(const Square &);
      //~Square();
      friend class Puzzle;
    private:
      bool cleared;
      char val;
      int pos;
      set<char> posVal;
  }; //class Square


  class Puzzle
  {
    public:
      Puzzle(char* input);
      Puzzle();
      //Puzzle(const Puzzle &obj);
      //~Puzzle();
      void printSet();
      void print();
      void decide(Square*);
      void clearRow(Square*);
      void clearCol(Square*);
      void clearReg(Square*);
      void clear(Square*);
      void clone(Puzzle &);
      void choose(Puzzle&, Square*);
      int getSqSetSize(Square* S);
      bool solved();
      bool getCleared(Square*);
      bool HiddenSingles();
      bool NakedPairs();
      Square* head();
      Square* next(Square*, int);
      Square* nextEmptySq(Square* S);
    private:
      Square* grid[82];
  };

#endif

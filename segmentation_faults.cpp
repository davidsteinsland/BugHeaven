#include <iostream>
#include "./static.h"
using namespace std;

/*
  A collection of ways to create segmentation faults
*/

void gangsterString()
{
	// an innocent char pointer ...
	char* name = (char*) "LLCoolJ"; // c-style cast to avoid warnings
	// we don't take too kindly to them rappers..
	// Lower-case that gangsta!
	while (*name)
	{
		if (*name >= 'a' && *name <= 'z')
			*name -= 32;
		name++;
	}

	cout << *name << endl;
}

void stackExplosion(){
  cout << "Exploding the stack..." << endl;
  //OK: 
  //char bytes[10000000];
  
  //NOT OK (at least not on OSX)
  char bytes[10000000]={};
}

int** pointerToPointer(){
  int a=77;
  int* b=&a;

  //We're gonna return this
  int** c=&b;
  cout << "Pointer to pointer: " << c << endl;
  cout << "Dereferenced: " << *c << endl;
  
  //..but do this too, just to prove the point
  int*** d=&c;
  int**** e=&d; 
  cout << "Quadruple deref is also OK: " << ****e << endl;

  return c;
}



void pointerToPointerOnStack(){
  cout << "Never return a pointer to the stack..." << endl;
  //OK:
  int** pp=pointerToPointer();
  cout << "pointer to pointer: " << pp << endl;
  cout << "Dereferenced: " << *pp << endl;
  cout << "De-dereferenced: " << **pp << endl;
}

int* getTodayPointer(){
  //Pretend a is the result of some important computation
  int a=3;
  // return &a does the same, but makes a (helpful) warning
  int* b=&a;
  cout << "Int: " << *b << " @ " << b << endl; 
  return b;
}


int stackScramble(int n=100){
  if(n>0)
    int i[100]={stackScramble(n-1)};
  return n;
}

string arr[]={"Monday","Tuesday","Wednesday","Thursday"};
void pointerToStack(){  
  cout << "Pointer to stack" << endl;

  int* p=getTodayPointer();
  cout << "Int: " << *p << " @ " << p << endl; 
  cout << "If today is day " << *p << " then it's " << arr[*p] << endl;
  stackScramble();
  cout << "...yesterday was " << *p << " is " << arr[*p-1] << endl;
  cout << "Not so lucky?" << endl;
}

/*
  Hmmm, wonder what's in my memory?
  Calibrated for OSX, so counting kilobytes
 */
void overTheEdge(){
  char i=0;
  char* p=&i;
  
  //Leap of Faith...
  while(p++)
    cout << (float)(p-&i)/1000 << "Kb : '" << *p << "' @ " << (void*)p << endl;
  
}

/*
  Another (disastrous) memory mapper, but upwards
  Calibrated for OSX, so counting megabytes
 */
void overTheOtherEdge(){
  long n=0;
  char i=0;
  char* p=&i;
  
  //Leap of Faith...
  while(p--)
    if(++n % 100000 == 0)
      cout << (float)(&i-p)/1000000 << "MB : '" << *p << "' @ " << (void*)p << endl;
  
}


int main(int argc, char** argv){

#define FN_COUNT 5
  void(*funcs[FN_COUNT])()={

    //Callable functions
    pointerToStack,
    pointerToPointerOnStack,
    stackExplosion,
    overTheEdge,
    overTheOtherEdge
    
  };
  
  if(argc<2)
    cout << "First fault (funny)... " << endl;
  
  if(atoi(argv[1])>=FN_COUNT)
    cout << "Another funny fault" << endl;
  
  funcs[atoi(argv[1])]();
  
}

#include <iostream>
#include <cassert>
#include <string>
#include <cmath>
#include "unit.h"
#include "block.h"
#include "jenga.h"

// ==========================================================================================
// IMPORTANT: Carefully read this file, but do not make any changes to
//            the code, except to fill in the StudentTest() function.
// ==========================================================================================

void SimpleTest() {

  // build the simple example used in the homework pdf diagram
  Jenga g(4);
  g.AddBlock(new Block(2,'a'), 1);
  g.AddBlock(new Block(2,'b'), 2);

  // by default print a small size=2 version of the board
  // and no labeling of center of mass or links between units
  g.Print();
  // print a larger version of the board with links between units labeled
  g.Print(8,false,true);

  // verify values of member variables of the Jenga object
  assert (g.getGridWidth() == 4);
  assert (g.getMaxHeight() == 2);
  assert (g.getNumBlocks() == 2);
  Block *a = g.getBlock('a');
  assert (a != NULL);
  Block *b = g.getBlock('b');
  assert (b != NULL);
  assert (g.getTop(0) == NULL);
  assert (g.getTop(1) == a->getUnit(0));
  assert (g.getTop(2) == b->getUnit(0));
  assert (g.getTop(3) == b->getUnit(1));
  assert (g.getGround(0) == NULL);
  assert (g.getGround(1) == a->getUnit(0));
  assert (g.getGround(2) == a->getUnit(1));
  assert (g.getGround(3) == NULL);

  // verify values of member variables of the Block objects
  assert (a->getLetter() == 'a');
  assert (a->getRow() == 0);
  assert (a->getWidth() == 2);
  assert (b->getLetter() == 'b');
  assert (b->getRow() == 1);
  assert (b->getWidth() == 2);

  // verify values of member variables of the Unit objects
  assert (a->getUnit(0)->getWhichUnit() == 0);
  assert (a->getUnit(0)->getColumn() == 1);
  assert (a->getUnit(0)->getBlock() == a);
  assert (a->getUnit(0)->getUp() == NULL);
  assert (a->getUnit(0)->getDown() == NULL);

  assert (a->getUnit(1)->getWhichUnit() == 1);
  assert (a->getUnit(1)->getColumn() == 2);
  assert (a->getUnit(1)->getBlock() == a);
  assert (a->getUnit(1)->getUp() == b->getUnit(0));
  assert (a->getUnit(1)->getDown() == NULL);

  assert (b->getUnit(0)->getWhichUnit() == 0);
  assert (b->getUnit(0)->getColumn() == 2);
  assert (b->getUnit(0)->getBlock() == b);
  assert (b->getUnit(0)->getUp() == NULL);
  assert (b->getUnit(0)->getDown() == a->getUnit(1));

  assert (b->getUnit(1)->getWhichUnit() == 1);
  assert (b->getUnit(1)->getColumn() == 3);
  assert (b->getUnit(1)->getBlock() == b);
  assert (b->getUnit(1)->getUp() == NULL);
  assert (b->getUnit(1)->getDown() == NULL);

  std::cout << "Completed SimpleTest!" << std::endl;
}

// ==========================================================================================

void CantileverTest() {
  Jenga g(23);

  // start with the 3 base blocks
  g.AddBlock(new Block(10,'a'), 12);
  g.AddBlock(new Block(10,'b'), 11);
  g.AddBlock(new Block(10,'c'), 10);
  // first visualize the links between Unit objects
  g.Print(4,false,true);
  // then visualize the center of mass for each brick
  g.Print(4,true,false);
  int mass;
  float center_of_mass;

  // manually check the mass and and center of mass of each block
  // (includes the mass of block resting on the block from above!)
  g.getBlock('c')->getCenterOfMass(mass,center_of_mass);
  assert (mass == 10);
  assert (fabs(center_of_mass-15.0) < 0.01);
  g.getBlock('b')->getCenterOfMass(mass,center_of_mass);
  assert (mass == 20);
  assert (fabs(center_of_mass-15.5) < 0.01);
  g.getBlock('a')->getCenterOfMass(mass,center_of_mass);
  assert (mass == 30);
  assert (fabs(center_of_mass-16.0) < 0.01);
  // StabilityAnalysis returns true if all Blocks are validly supported
  // by default, StabilityAnalysis does not print any information
  bool stable = g.StabilityAnalysis();
  assert (stable == true);

  // add 3 more blocks... stretching near optimally to the left!
  g.AddBlock(new Block(10,'d'), 8);
  g.AddBlock(new Block(10,'e'), 6);
  g.AddBlock(new Block(10,'f'), 1);
  g.Print(4,true,false);
  g.getBlock('f')->getCenterOfMass(mass,center_of_mass);
  assert (mass == 10);
  assert (fabs(center_of_mass-6.0) < 0.01);
  // When the optional argument to StabilityAnalysis is true, the function
  // prints a report on the stability of each block.
  stable = g.StabilityAnalysis(true);
  assert (stable == true);

  // Adding one tiny block makes the structure unstable!
  g.AddBlock(new Block(1,'g'), 2);
  g.Print(4,true,false);
  g.getBlock('f')->getCenterOfMass(mass,center_of_mass);
  assert (mass == 11);
  assert (fabs(center_of_mass-5.68) < 0.01);
  stable = g.StabilityAnalysis(true);
  assert (stable == false);

  std::cout << "Completed CantileverTest!" << std::endl;
}

// ==========================================================================================

void JengaTest(bool run_stability_analysis) {

  // NOTE: Correct behavior for StabilityAnalysis on this test is Extra Credit!

  Jenga g(5);

  // create a small Jenga Tower
  g.AddBlock(new Block(3,'a'), 1);
  g.AddBlock(new Block(1,'b'), 1);
  g.AddBlock(new Block(1,'c'), 2);
  g.AddBlock(new Block(1,'d'), 3);
  g.AddBlock(new Block(3,'e'), 1);
  g.AddBlock(new Block(1,'f'), 1);
  g.AddBlock(new Block(1,'g'), 2);
  g.AddBlock(new Block(1,'h'), 3);
  g.AddBlock(new Block(3,'i'), 1);
  g.Print(4,run_stability_analysis);
  assert (g.getMaxHeight() == 5);
  assert (g.getNumBlocks() == 9);
  if (run_stability_analysis) { assert (g.StabilityAnalysis(true) == true); }
  
  // move one piece from the center to the top
  g.RemoveBlock('c');
  assert (g.getNumBlocks() == 8);
  g.AddBlock(new Block(1,'c'), 2);  
  assert (g.getNumBlocks() == 9);
  g.Print(4,run_stability_analysis);
  assert (g.getMaxHeight() == 6);
  if (run_stability_analysis) { assert (g.StabilityAnalysis(true) == true); }
  
  // move two more pieces...
  g.RemoveBlock('f');
  g.AddBlock(new Block(1,'f'), 1);
  g.RemoveBlock('h');
  g.AddBlock(new Block(1,'h'), 3);
  g.Print(4,run_stability_analysis);  
  assert (g.getNumBlocks() == 9);
  assert (g.getMaxHeight() == 6);
  if (run_stability_analysis) { assert (g.StabilityAnalysis(true) == true); }

  // move a piece that breaks stability
  g.RemoveBlock('b');
  g.AddBlock(new Block(1,'b'), 3);
  assert (g.getNumBlocks() == 9);
  assert (g.getMaxHeight() == 7);
  
  g.Print(4,run_stability_analysis);
  if (run_stability_analysis) { assert (g.StabilityAnalysis(true) == false); }

  std::cout << "Completed JengaTest!" << std::endl;
}

// ==========================================================================================

void ArchTest() {

  // NOTE: Correct behavior for StabilityAnalysis on this test is Extra Credit!

  {
    // two "separate" stacks
    Jenga g(14);
    g.AddBlock(new Block(1,'a'), 5);
    g.AddBlock(new Block(1,'b'), 10);
    g.AddBlock(new Block(5,'c'), 3);
    g.AddBlock(new Block(5,'d'), 8);
    g.Print(4,true);
    bool success = g.StabilityAnalysis(true);
    assert (success);
  }
  {
    // same shape, but an arch
    Jenga g(14);
    g.AddBlock(new Block(1,'a'), 5);
    g.AddBlock(new Block(1,'b'), 10);
    g.AddBlock(new Block(10,'c'), 3);
    g.Print(4,true);
    bool success = g.StabilityAnalysis(true);
    assert (success);
  }
  {
    // wider arch
    Jenga g(14);
    g.AddBlock(new Block(1,'a'), 4);
    g.AddBlock(new Block(1,'b'), 11);
    g.AddBlock(new Block(10,'c'), 3);
    g.Print(4,true);
    bool success = g.StabilityAnalysis(true);
    assert (success);
  }
  {
    // taller arch
    Jenga g(14);
    g.AddBlock(new Block(1,'a'), 5);
    g.AddBlock(new Block(1,'b'), 10);
    g.AddBlock(new Block(1,'c'), 5);
    g.AddBlock(new Block(1,'d'), 10);
    g.AddBlock(new Block(10,'e'), 3);
    g.Print(4,true);
    bool success = g.StabilityAnalysis(true);
    assert (success);
  }
  {
    // base underneath arch
    Jenga g(14);
    g.AddBlock(new Block(10,'a'), 3);
    g.AddBlock(new Block(1,'b'), 5);
    g.AddBlock(new Block(1,'c'), 10);
    g.AddBlock(new Block(10,'d'), 3);
    g.Print(4,true);
    bool success = g.StabilityAnalysis(true);
    assert (success);
  }
  {
    // shift the top bar fully to the left
    Jenga g(14);
    g.AddBlock(new Block(1,'a'), 5);
    g.AddBlock(new Block(1,'b'), 10);
    g.AddBlock(new Block(10,'c'), 0);
    g.Print(4,true);
    bool success = g.StabilityAnalysis(true);
    assert (success);
  }
  {
    // shift the top bar mostly to the left
    Jenga g(14);
    g.AddBlock(new Block(1,'a'), 5);
    g.AddBlock(new Block(1,'b'), 10);
    g.AddBlock(new Block(10,'c'), 1);
    g.Print(4,true);
    bool success = g.StabilityAnalysis(true);
    assert (success);
    g.RemoveBlock('b');
    g.Print(4,true);
    success = g.StabilityAnalysis(true);
    assert (success);
  }

  // NOTE: this is a work-in-progress test and will be updated soon
  
  std::cout << "Completed ArchTest!" << std::endl;  
}

// ==========================================================================================

void StudentTest() {
  std::cout << "Beginning StudentTest!" << std::endl;


  //
  // FILL IN THIS FUNCTION
  //
  

  // NOTE: don't delete this line
  std::cout << "Completed StudentTest!" << std::endl;
}


// ==========================================================================================
// Helper Functions for Interactive Mode

int ReadInteger(int min, int max) {
  while (true) {
    std::string token;
    std::cin >> token;
    int answer = min-1;
    try {
      answer = std::stoi(token);
    } catch (...) {
    }
    if (answer >= min && answer <= max) return answer;
    std::cerr << "ERROR: invalid integer - please try again." << std::endl;
  }
}

char ReadLowerCaseLetter() {
  while (true) {
    char letter;
    std::cin >> letter;
    if (letter >= 'a' && letter <= 'z')
      return letter;
    else
      std::cerr << "ERROR: invalid lower case letter - please try again." << std::endl;
  }
}

void InteractiveMode() {
  std::cout << "Enter Board Width: "; fflush(stdout);
  int width = ReadInteger(1,99);
  Jenga g(width);
  g.Print(2,false,false);
  while (true) {
    std::cout << "Press 'a' to add Block, 'r' to remove Block, 'p' to print, " << std::endl;
    std::cout << "      's' for stability analysis, or 'q' to quit: "; fflush(stdout);
    char c;
    std::cin >> c;
    if (c == 'a') {
      std::cout << "What letter?  What width?  What position?"; fflush(stdout);
      char letter = ReadLowerCaseLetter();
      int w = ReadInteger(1,width);
      int pos = ReadInteger(0,width-w);
      g.AddBlock(new Block(w,letter),pos);
      g.Print(2,false,false);
    } else if (c == 'r') {
      std::cout << "What letter?  "; fflush(stdout);
      char letter = ReadLowerCaseLetter();
      g.RemoveBlock(letter);
      g.Print(2,false,false);
    } else if (c == 'p') {
      g.Print(6,true,true);
    } else if (c == 's') {
      g.StabilityAnalysis(true);
    } else if (c == 'q') {
      std::cout << "Thank you for playing!" << std::endl;
      return;
    } else {
      std::cout << "Invalid key, please try again." << std::endl;
    }
  }
}

// ==========================================================================================

int main(int argc, char* argv[]) {
  if (argc == 2) {
    if (std::string(argv[1]) == "SimpleTest") {
      SimpleTest();
    }
    else if (std::string(argv[1]) == "CantileverTest") {
      CantileverTest();    
    }
    else if (std::string(argv[1]) == "JengaTest") {
      // DON'T run the StabilityAnalysis 
      JengaTest(false);  
    }
    else if (std::string(argv[1]) == "JengaTestStability") {
      // run the Stability Analysis -- this is extra credit!
      JengaTest(true);
    }
    else if (std::string(argv[1]) == "ArchTest") {
      // this is extra credit!
      ArchTest();
    }
    else if (std::string(argv[1]) == "StudentTest") {
      StudentTest();
    }
    else {
      std::cerr << "ERROR: No test case for '" << argv[1] << "'" << std::endl;
    }
  } else {
    if (argc != 1) {
      std::cerr << "ERROR: Wrong number of arguments" << std::endl;
    }
    InteractiveMode();  
  }
}

// ==========================================================================================

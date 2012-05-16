#include <iostream>

#include <Inventor/SoDB.h>
#include <Inventor/SoInteraction.h>
#include <Inventor/nodekits/SoNodeKit.h>

#include <gtest/gtest.h>

int main(int argc, char **argv)
{
  std::cout << "Running main() from main.cpp\n";

  testing::InitGoogleTest(&argc, argv);

  // init Inventor
  SoDB::init();
  SoNodeKit::init();
  SoInteraction::init();

  return RUN_ALL_TESTS();
}

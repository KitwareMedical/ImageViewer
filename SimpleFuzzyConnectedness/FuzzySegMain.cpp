#include "FuzzySegGrayUI.h"

int main(){
  FuzzySegGrayUI * console=new FuzzySegGrayUI;
  console->Show();

  Fl::run();

  return 1;
}
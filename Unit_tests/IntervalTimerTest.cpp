int main(int argc, char const* argv[])
{
  // SDL_Init(SDL_INIT_VIDEO);
  IntervalTimer ttest;
  std::cout << "time before input one: " << ttest.UpdateAndGetInterval()
            << std::endl;
  int num { 0 };
  std::cout << "Enter first num: ";
  std::cin >> num;
  std::cout << "time after first input: " << ttest.UpdateAndGetInterval()
            << std::endl;
  std::cout << "Enter second num: ";
  std::cin >> num;

  std::cout << "time after second input: " << ttest.UpdateAndGetInterval()
            << std::endl;
  return 0;
}

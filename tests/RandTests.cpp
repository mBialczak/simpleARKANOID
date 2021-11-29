#include "RandNum.hpp"
#include <iomanip>
#include <iostream>

using namespace std;
using namespace gMath;

void RandIntegralTest()
{

  // normal int test
  cout << "static RandNum::Random<int>(int,int) test:\n";
  for (auto i = 0; i < 5; ++i) {
    cout << RandNum::Random(1, 5) << "\t";
  }
  cout << "\n##################################\n\n";

  RandNum randomizer;
  cout << "object RandNum::Random<int>(int,int) test:\n";
  for (auto i = 0; i < 5; ++i) {
    cout << randomizer.Random(1, 5) << "\t";
  }
  cout << "\n##################################\n\n";

  // long int test
  cout << "static RandNum::Random<long>(long,long) test:\n";
  for (auto i = 0; i < 5; ++i) {
    cout << RandNum::Random(1l, 55555555555555555l) << "\t";
  }
  cout << "\n##################################\n\n";
}

void RandFloatingTest()
{
  // float test
  cout << "static RandNum::Random<float>(float,float) test:\n";
  cout << setprecision(90) << RandNum::Random(30.0f, 30.5f) << "\t";
  cout << "\n##################################\n\n";

  RandNum randomizer;
  cout << "object RandNum::Random<float>(float,float) test:\n";
  cout << setprecision(90) << randomizer.Random(30.0f, 30.5f) << "\t";
  cout << "\n##################################\n\n";

  // double test
  cout << "object RandNum::Random<double>(double,double) test:\n";
  cout << setprecision(90) << RandNum::Random<double>(30.0, 30.5) << "\t";
  cout << "\n##################################\n\n";

  // long double test
  cout
      << "object RandNum::Random<long double>(long double,long double) test:\n";
  cout << setprecision(90) << RandNum::Random<long double>(30.0l, 30.5l)
       << "\t";
  cout << "\n##################################\n\n";
}

void OperatorIntegralTest()
{
  RandNum randomizer;
  cout << "operator() for int:\t" << randomizer(3, 7) << endl;

  cout << "operator() for long:\t"
       << randomizer(30'000'000'00l, 70'000'000'000l) << endl;

  cout << "operator() for long long:\t"
       << randomizer(5'000'000'000'000'000'000ll, 5'001'000'000'000'000'000ll)
       << "\n=======================" << endl;
}

void OperatorDoubleTest()
{
  RandNum randomizer;
  cout << "operator() for float:\t\t" << setprecision(100)
       << randomizer(30.0f, 30.5f) << endl;

  cout << "operator() for double:\t\t" << setprecision(100)
       << randomizer(30.0, 30.5) << endl;

  cout << "operator() for long double:\t" << setprecision(100)
       << randomizer(30.0l, 30.5l) << "\n=======================" << endl;
}

int main(int argc, char const* argv[])
{
  RandIntegralTest();
  RandFloatingTest();

  OperatorIntegralTest();
  OperatorDoubleTest();

  return 0;
}

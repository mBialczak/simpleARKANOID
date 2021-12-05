#include "LevelData.hpp"
// #include <fstream> //VERIFY
// #include <iostream> //VERIFY
#include <exception>
#include <sstream>
#include <string>

// constructor taking the path to levels to load
// throws if unable to load level data
LevelData::LevelData(const std::string& path, unsigned levelNumber)
    : _level(levelNumber)
    , _ball_speed(0.0f)
    , _paddle_speed(0.0f)
    , _lives(0)
    , _points_per_block(0)
{ // TODO: revise
  // create full path to the level file
  std::string full_path
      = path + "Level_" + std::to_string(levelNumber) + ".txt";
  // read level numerical data and throw if it was unsuccessful
  if (!ReadNumericalData(full_path)) {
    throw std::runtime_error("Unable to load level data");
  }
}

// COMMENT all
// reads single value described by key from the file
template <typename T>
T LevelData::ReadDataItem(
    const std::string& filePath, const std::string& keyword) const
{
  // single word read and checked against keyword for match
  std::string key;
  // single line of file to be parsed
  std::string line;
  // returned value
  T value {};
  // open file for reading
  std::ifstream file_stream(filePath);
  // check if the stream is open and in the correct reading state
  if (file_stream) {
    // read entire file line by line
    while (std::getline(file_stream, line)) {
      // create stream for stripping single line
      std::istringstream line_stream { line };
      // check the line for value
      while (line_stream >> key >> value) {
        // if found the key, return corresponding value
        if (key == keyword) {
          return value;
        }
      }
    }
  }
  // if value for the given key was not found returns type's default value
  return value;
}

// reads all level numerical data from the level file
// returns true if succesfull, false - otherwise
// VERIFY return type
bool LevelData::ReadNumericalData(const std::string& filePath)
{
  _ball_speed = ReadDataItem<float>(filePath, "ball_speed");
  _paddle_speed = ReadDataItem<float>(filePath, "paddle_speed");
  _lives = ReadDataItem<unsigned>(filePath, "lives");
  _points_per_block = ReadDataItem<unsigned>(filePath, "points_per_block");
  // check if the read values make sense
  if (_ball_speed <= 0.0 || _paddle_speed <= 0.0 || _lives == 0 || _lives > 5
      || _points_per_block == 0) {
    return false;
  }
  // every value makes sense
  return true;
}

// TODO: implement
// reads the block composition table from file
// returns true if succesfull, false otherwise
bool LevelData::ReadBlocksData(const std::string& filePath) { }
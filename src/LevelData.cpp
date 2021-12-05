#include "LevelData.hpp"
// #include <fstream> //VERIFY
// #include <iostream> //VERIFY
#include <exception>
#include <sstream>
#include <string>

// for operator""s usage // VERIFY if used in this file
using std::string_literals::operator""s;

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
  // read level numerical data and throw if unsuccessful
  if (!ReadNumericalData(full_path)) {
    throw std::runtime_error("Unable to load level numerical data");
  }
  // read the sprite table representing block composition in the level and throw
  // if something went wrong during creation
  if (!ReadSpriteTable(full_path)) {
    throw std::runtime_error(
        "Error while reading sprite table data from the file"s + full_path);
  }
}

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
  // check if opening file for reading was successful and report error if not
  if (!file_stream) {
    throw std::runtime_error(
        "Unable to open file with the level data!"s + filePath);
  }

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
  // if value for the given key was not found returns type's default value
  return T {};
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

// Parses the given file and tries to create and fill member sprite table,
// where each object of enum type Sprite corresponds to one block. In the file
// however, each block is represented by single character, so the function
// maps the char into Sprite while reading. returns true if sprite table is
// created succesfully, false otherwise
bool LevelData::ReadSpriteTable(const std::string& filePath)
{
  // reserve enough space for the rows in member sprite table
  _sprite_table.reserve(_max_rows);
  // open the file
  std::ifstream file_stream(filePath);
  // check if opening file for reading was successful
  if (!file_stream) {
    throw std::runtime_error(
        "Unable to open file with the level data!"s + filePath);
  }
  // single line of file data to be read
  std::string file_line;
  // // string stream for stripping each line into characters
  // std::istringstream line_stream; // REMOVE
  // counter to control the number of read rows
  unsigned row_count { 0 };
  // read the file line by line until end of file or maximum number of
  // rows predicted for displaying is reached
  while (std::getline(file_stream, file_line) && row_count < _max_rows) {
    // // load the the line data into string stream
    // line_stream.str(file_line); // REMOVE
    // single character read from file to be mapped into sprite
    // char cell; // REMOVE
    // a row of sprite cells to be composed into the final table of sprites
    // std::vector<Sprite> sprite_row; // REMOVE
    // reserve enough space for entire row of sprites
    // sprite_row.reserve(_row_size); // REMOVE
    // strip the line data into cells //REMOVE
    // while (line_stream >> cell) {
    //   sprite_row.emplace_back(CharToSprite(cell));
    // }

    // parse single file line and create a single sprite_row
    std::vector<Sprite> sprite_row { ReadSpriteRow(file_line) };
    // check for correct number of sprites in the signle row
    if (sprite_row.size() != _row_size) {
      return false;
    }
    // put the created row into sprite table
    _sprite_table.emplace_back(std::move(sprite_row));
    row_count++;
  }
  // check if the created table has correct size
  if (_sprite_table.size() != _max_rows) {
    return false;
  }
  // the sprite table was succesfully read
  return true;
}

// loads single row of sprites; helper for ReadSpriteTable()
std::vector<Sprite> LevelData::ReadSpriteRow(const std::string& fileLine) const
{
  // string stream for stripping each line into characters
  std::istringstream line_stream { fileLine };
  // single character  read from file line and later mapped into sprite
  char cell;
  // a row of sprite cells to be composed into the final table of sprites
  std::vector<Sprite> sprite_row;
  // reserve enough space for entire row of sprites
  sprite_row.reserve(_row_size);
  // strip the line char by char
  while (line_stream >> cell) {
    // Create Sprite and put into row
    sprite_row.emplace_back(CharToSprite(cell));
  }

  return sprite_row;
}

// maps a single char (read from file) into sprite type
// helper for ReadSpriteRow
Sprite LevelData::CharToSprite(char spriteCharRepresentation) const
{
  switch (spriteCharRepresentation) {
    case '0':
      return Sprite::None;
    case 'g':
      return Sprite::BlockGreen;
    default:
      return Sprite::None;
  }
}
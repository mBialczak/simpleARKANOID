#include "LevelData.hpp"
#include <exception>
#include <sstream>
#include <string>

// for operator""s usage
using namespace std::string_literals;

// Static member initialization:

// the keyword marking the ball speed in the level file
const std::string LevelData::ball_speed_key { "ball_speed" };
// the keyword marking the paddle speed in the parsed level file
const std::string LevelData::paddle_speed_key { "paddle_speed" };
// the keyword marking the number of lives in the parsed level file
const std::string LevelData::lives_key { "lives" };
// the keyword marking the points per block destroyed in the parsed level file
const std::string LevelData::points_per_block_key { "points_per_block" };

// constructor taking the path to levels to load and level number
// throws std::runtime_error if unable to load level data
LevelData::LevelData(const std::string& path, unsigned levelNumber)
    : _level(levelNumber)
    , _ball_speed(0.0f)
    , _paddle_speed(0.0f)
    , _lives(0)
    , _points_per_block(0)
{
  // create full path to the level file
  std::string full_path
      = path + "Level_" + std::to_string(levelNumber) + ".txt";

  // read level numerical data and the sprite table representing block
  // composition or throw exception if unsuccessful
  if (!ReadNumericalData(full_path) || !ReadSpriteTable(full_path)) {
    throw std::runtime_error(
        "Error: wrong format in level file: "s + full_path);
  }
}

// returns the sprite table representing
// the composition of blocks in the level
const std::vector<std::vector<Sprite>>& LevelData::SpriteTable() const
{
  return _sprite_table;
}

// Reads and returns single value described by key from the file.
// If the value wasn't found returns default value for the type.
// Throws std::runtime_error if was unable to open file
template <typename T>
T LevelData::ReadDataItem(
    const std::string& filePath, const std::string& keyword) const
{
  // single word read and checked against keyword for match
  std::string word;
  // single line of file to be parsed
  std::string line;
  // returned value
  T value {};
  // open file for reading
  std::ifstream file_stream(filePath);
  // check if opening file for reading was successful
  if (!file_stream) {
    throw std::runtime_error(
        "Unable to open file with the level data!"s + filePath);
  }

  // read entire file line by line
  while (std::getline(file_stream, line)) {
    // create stream for stripping single line
    std::istringstream line_stream { line };
    // check the line for value
    while (line_stream >> word >> value) {
      // if found the key, return corresponding value
      if (word == keyword) {
        return value;
      }
    }
  }
  // if value for the given keyword was not found returns type's default value
  return T {};
}

// reads all level numerical data from the level file
// returns true if succesfull, false - otherwise
bool LevelData::ReadNumericalData(const std::string& filePath)
{
  _ball_speed = ReadDataItem<float>(filePath, ball_speed_key);
  _paddle_speed = ReadDataItem<float>(filePath, paddle_speed_key);
  _lives = ReadDataItem<unsigned>(filePath, lives_key);
  _points_per_block = ReadDataItem<unsigned>(filePath, points_per_block_key);
  // check if the read values make sense
  if (_ball_speed <= 0.0 || _paddle_speed <= 0.0 || _lives == 0 || _lives > 5
      || _points_per_block == 0) {
    // returned values make no sense to use them
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
// Throws std::runtime_error if was unable to open file
bool LevelData::ReadSpriteTable(const std::string& filePath)
{
  // open the file
  std::ifstream file_stream(filePath);
  // check if opening file for reading was successful
  if (!file_stream) {
    throw std::runtime_error(
        "Unable to open file with the level data!"s + filePath);
  }

  // single line of file data to be read
  std::string file_line;

  // counter to control the number of read rows
  unsigned row_count { 0 };

  // reserve enough space for the rows in member sprite table
  _sprite_table.reserve(max_rows);

  // read the file line by line until  maximum number of
  // rows predicted for displaying is reached
  while (std::getline(file_stream, file_line) && row_count < max_rows) {

    // parse single file line and create a single sprite_row
    std::vector<Sprite> sprite_row { ReadSpriteRow(file_line) };

    // check for correct number of sprites in the signle row
    if (sprite_row.size() != row_size) {
      return false;
    }
    // put the created row into sprite table
    _sprite_table.emplace_back(std::move(sprite_row));
    row_count++;
  }

  // check if the created table has correct size
  if (_sprite_table.size() != max_rows) {
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
  // single character read from file line and later mapped into sprite
  char cell;
  // a row of sprite cells to be composed into the final table of sprites
  std::vector<Sprite> sprite_row;
  // reserve enough space for entire row of sprites
  sprite_row.reserve(row_size);
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
  // single char put while level creation in the level file is coverted to
  // corresponding enum sprite type
  switch (spriteCharRepresentation) {
    case static_cast<char>(BlockCharCode::None):
      return Sprite::None;

    case static_cast<char>(BlockCharCode::BlockGreen):
      return Sprite::BlockGreen;

    case static_cast<char>(BlockCharCode::BlockBlue):
      return Sprite::BlockBlue;

    case static_cast<char>(BlockCharCode::BlockOrange):
      return Sprite::BlockOrange;

    case static_cast<char>(BlockCharCode::BlockPurple):
      return Sprite::BlockPurple;

    case static_cast<char>(BlockCharCode::BlockRed):
      return Sprite::BlockRed;

    case static_cast<char>(BlockCharCode::BlockSilver):
      return Sprite::BlockSilver;

    case static_cast<char>(BlockCharCode::BlockTeal):
      return Sprite::BlockTeal;

    case static_cast<char>(BlockCharCode::BlockYellow):
      return Sprite::BlockYellow;

    default:
      return Sprite::None;
  }
}
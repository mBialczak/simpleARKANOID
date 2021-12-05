#ifndef LevelData_HPP
#define LevelData_HPP
#include "Enums.hpp"
#include <fstream>
#include <string>
#include <vector> // REMOVE INU

// TODO: THINK if destructor and copy/move operations are needed
class LevelData
{
  public:
  // constructor taking the path to levels to load
  // throws if unable to load level data
  LevelData(const std::string& path, unsigned levelNumber = 1);
  // returns current scalar speed of the ball (pixels/second)
  float BallSpeed() const { return _ball_speed; }
  // returns current paddle scalar speed;
  float PaddleSpeed() const { return _paddle_speed; }
  // returns lives assigned to the current level // VERIFY if needed
  unsigned Lives() const { return _lives; }
  // returns point value assigned to blocks on the current level
  unsigned PointsPerBlock() const { return _points_per_block; }
  // returns current level // VERIFY if needed
  unsigned Level() const { return _level; }
  // returns the sprite table representing
  // the composition of blocks in the level
  const std::vector<std::vector<Sprite>>& SpriteTable() const
  {
    return _sprite_table;
  }

  private:
  // reads single value described by key from the file
  template <typename T>
  T ReadDataItem(const std::string& filePath, const std::string& keyword) const;
  // reads all level numerical data from the level file
  // returns true if succesfull, false - otherwise
  // VERIFY return type
  bool ReadNumericalData(const std::string& filePath);
  // Parses the given file and tries to create and fill member sprite table,
  // where each object of enum type Sprite corresponds to one block. In the file
  // however, each block is represented by single character, so the function
  // maps the char into Sprite while reading.
  // Returns true if sprite table is created succesfully, false otherwise
  bool ReadSpriteTable(const std::string& filePath);
  // loads single row of sprites; helper for ReadSpriteTable()
  std::vector<Sprite> ReadSpriteRow(const std::string& fileLine) const;
  // maps a single char (read from file) into sprite type
  // helper for ReadSpriteRow
  Sprite CharToSprite(char spriteCharRepresentation) const;

  // VERIFY  maximum size of the blocks row on the screen
  static const std::size_t _row_size = 20;
  // VERIFY maximum number of block rows on the screen
  static const std::size_t _max_rows = 15;
  // VERIFY if needed current game level
  std::size_t _level;
  // ball starting scalar speed (pixels per second)
  float _ball_speed;
  // paddle scalar moving speed
  float _paddle_speed;
  // current remaining lives
  unsigned _lives;
  // points value of each block in the level
  unsigned _points_per_block;
  // table of block sprites describing block composition in the level
  std::vector<std::vector<Sprite>> _sprite_table;
};
#endif // !Level_HPP

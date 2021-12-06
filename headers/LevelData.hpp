#ifndef LevelData_HPP
#define LevelData_HPP
#include "Enums.hpp"
#include <fstream>
#include <string>
#include <vector>

// loads and represents level data: ball speed, paddle speed, player lives
// assinged to the level, level number, point value of each block destroyed and
// the table of enum Sprites representing the block layout in the game
class LevelData
{
  public:
  // constructor taking the path to levels to load and level number
  // throws std::runtime_error if unable to load level data
  LevelData(const std::string& path, unsigned levelNumber = 1);
  // returns minimum scalar speed of the ball (pixels/second) for the level
  float BallSpeed() const { return _ball_speed; }
  // returns paddle scalar speed for the level;
  float PaddleSpeed() const { return _paddle_speed; }
  // returns lives available in the start of the current level
  unsigned Lives() const { return _lives; }
  // returns point value assigned to destroyed block in the current level
  unsigned PointsPerBlock() const { return _points_per_block; }
  // returns current level to which stored data corresponds // VERIFY if needed
  unsigned Level() const { return _level; }
  // REMOVE if not used - most likely static consts will be ok
  // returns the maximum number of maximum rows
  static std::size_t MaxRows() { return _max_rows; };
  // REMOVE if not used - most likely static consts will be ok
  // returns the maximum block row size
  static std::size_t RowSize() { return _row_size; }

  // returns the sprite table representing
  // the composition of blocks in the level
  const std::vector<std::vector<Sprite>>& SpriteTable() const
  {
    return _sprite_table;
  }

  // REVIEW: if should be private
  // public static consts (const so no risk of undesired access):
  // static consts publicly accessible (const so no risk of undesired access)
  // maximum number of the blocks in the row on the screen
  static const std::size_t _row_size = 20;
  // maximum number of block rows on the screen
  static const std::size_t _max_rows = 15;
  // block width in pixels
  static constexpr float _block_width = 50.0f;
  // block height in pixels
  static constexpr float _block_height = 25.0f;

  private:
  // reads single value described by key from the file
  template <typename T>
  T ReadDataItem(const std::string& filePath, const std::string& keyword) const;
  // reads all level numerical data from the level file
  // returns true if succesfull, false - otherwise
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
  // helper for ReadSpriteRow()
  Sprite CharToSprite(char spriteCharRepresentation) const;

  // static private consts - no other class needs them
  // the keyword marking the ball speed in the parsed level file
  static const std::string _ball_speed_key;
  // the keyword marking the paddle speed in the parsed level file
  static const std::string _paddle_speed_key;
  // the keyword marking the number of lives in the parsed level file
  static const std::string _lives_key;
  // the keyword marking the points per block destroyed in the parsed level file
  static const std::string _points_per_block_key;

  // VERIFY if needed current game level
  std::size_t _level;
  // ball starting scalar speed (pixels per second) assigned to the level
  float _ball_speed;
  // paddle scalar moving speed assigned to the level
  float _paddle_speed;
  // number of startingg lives assigned to the level
  unsigned _lives;
  // points value of each block destroyed in the level
  unsigned _points_per_block;
  // table of block sprites describing block composition in the level
  std::vector<std::vector<Sprite>> _sprite_table;
};
#endif // !Level_HPP

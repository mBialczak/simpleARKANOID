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
  float BallSpeed() const { return ball_speed_; }
  // returns paddle scalar speed for the level;
  float PaddleSpeed() const { return paddle_speed_; }
  // returns lives available in the start of the current level
  unsigned Lives() const { return lives_; }
  // returns point value assigned to destroyed block in the current level
  unsigned PointsPerBlock() const { return points_per_block_; }
  // returns current level to which stored data corresponds
  unsigned Level() const { return level_; }
  // returns the sprite table representing
  // the composition of blocks in the level
  const std::vector<std::vector<Sprite>>& SpriteTable() const;

  // public static consts (const so no risk of undesired access):
  // maximum number of the blocks in the row on the screen
  static const std::size_t row_size = 20;
  // maximum number of block rows on the screen
  static const std::size_t max_rows = 15;
  // block width in pixels
  static constexpr float block_width = 50.0f;
  // block height in pixels
  static constexpr float block_height = 25.0f;

  private:
  // Reads and returns single value described by key from the file.
  // If the value wasn't found returns default value for the type.
  // Throws std::runtime_error if was unable to open file
  template <typename T>
  T ReadDataItem(const std::string& filePath, const std::string& keyword) const;
  // reads all level numerical data from the level file
  // returns true if succesfull, false - otherwise
  bool ReadNumericalData(const std::string& filePath);
  // Parses the given file and tries to create and fill member sprite table,
  // where each object of enum type Sprite corresponds to one block. In the file
  // however, each block is represented by single character, so the function
  // maps the char into Sprite while reading.
  // Returns true if sprite table is created succesfully, false otherwise.
  // Throws std::runtime_error if was unable to open file
  bool ReadSpriteTable(const std::string& filePath);
  // loads single row of sprites; helper for ReadSpriteTable()
  std::vector<Sprite> ReadSpriteRow(const std::string& fileLine) const;
  // maps a single char (read from file) into sprite type
  // helper for ReadSpriteRow()
  Sprite CharToSprite(char spriteCharRepresentation) const;

  // static private consts - no other class needs them:
  // the keyword marking the ball speed in the parsed level file
  static const std::string ball_speed_key;
  // the keyword marking the paddle speed in the parsed level file
  static const std::string paddle_speed_key;
  // the keyword marking the number of lives in the parsed level file
  static const std::string lives_key;
  // the keyword marking the points per block destroyed in the parsed level file
  static const std::string points_per_block_key;

  // level to which the object corresponds to
  std::size_t level_;
  // ball starting scalar speed (pixels per second) assigned to the level
  float ball_speed_;
  // paddle scalar moving speed assigned to the level
  float paddle_speed_;
  // number of starting lives assigned to the level
  unsigned lives_;
  // points value of each block destroyed in the level
  unsigned points_per_block_;
  // table of block sprites describing block composition in the level
  std::vector<std::vector<Sprite>> sprite_table_;
};
#endif // !Level_HPP

#ifndef LevelData_HPP
#define LevelData_HPP
#include <fstream>
#include <string>
#include <vector> // REMOVE INU

// TODO: COMMENTS

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
  // returns lives assigned to the current level
  unsigned Lives() const { return _lives; }
  // returns point value assigned to blocks on the current level
  unsigned PointsPerBlock() const { return _points_per_block; }
  // returns current level // VERIFY if needed
  unsigned Level() const { return _level; };

  private:
  // reads single value described by key from the file
  template <typename T>
  T ReadDataItem(const std::string& filePath, const std::string& keyword) const;

  // reads all level numerical data from the level file
  // returns true if succesfull, false - otherwise
  // VERIFY return type
  bool ReadNumericalData(const std::string& filePath);
  // reads the block composition table from file
  // returns true if succesfull, false otherwise
  bool ReadBlocksData(const std::string& filePath);

  // VERIFY  maximum size of the blocks row on the screen
  const std::size_t _row_size = 20;
  // VERIFY maximum number of block rows on the screen
  const std::size_t _max_rows = 15;
  // VERIFY current game level
  std::size_t _level;
  // ball starting scalar speed (pixels per second)
  float _ball_speed;
  // paddle scalar moving speed
  float _paddle_speed;
  // current remaining lives
  unsigned _lives;
  // REVIEW: points value of each block in the level
  unsigned _points_per_block;
  // table of chars describing block composition in the level
  std::vector<std::vector<char>> _block_table;
};
#endif // !Level_HPP

#ifndef Paths_HPP
#define Paths_HPP
#include <string>

// namespace with deffined paths to files used in the game
namespace Paths {
// path to the image used for displaying the ball
const std::string pBallImage { "../assets/ball.png" };
// path to the image used for displaying the paddle
const std::string pPadleImage { "../assets/paddle.png" };
// path to the image used for displaying the top wall
const std::string pHorizontalWallImage { "../assets/horizontal_wall.png" };
// path to the image used for displaying the right and left walls
const std::string pVerticalWallImage { "../assets/vertical_wall.png" };
// path to the image used for displaying the green block
const std::string pBlockGreenImage { "../assets/block_green.png" };
// VERIFY name path to levels data
const std::string pLevels { "../assets/Levels/" };
}
#endif // !Paths_HPP

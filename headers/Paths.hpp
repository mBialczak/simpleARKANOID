#ifndef Paths_HPP
#define Paths_HPP
#include <string>

// namespace with deffined paths to files used in the game
namespace Paths {
// path to the image used for displaying the ball
const std::string pBallImage { "../assets/images/ball.png" };
// path to the image used for displaying the paddle
const std::string pPadleImage { "../assets/images/paddle.png" };
// path to the image used for displaying the top wall
const std::string pHorizontalWallImage {
  "../assets/images/horizontal_wall.png"
};
// path to the image used for displaying the right and left walls
const std::string pVerticalWallImage { "../assets/images/vertical_wall.png" };
// path to the image used for displaying the green block
const std::string pBlockGreenImage { "../assets/images/block_green.png" };
// VERIFY path to levels data
const std::string pLevels { "../assets/Levels/" };
// VERIFY path to roboto bold font
const std::string pFontRobotoBold { "../assets/fonts/Roboto-Bold.ttf" };
}
#endif // !Paths_HPP

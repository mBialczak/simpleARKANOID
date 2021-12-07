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
// VERIFY paths to roboto fonts
const std::string pFontRobotoBold { "../assets/fonts/Roboto-Bold.ttf" };
const std::string pFontRobotoRegular { "../assets/fonts/Roboto-Regular.ttf" };
const std::string pFontRobotoBoldItalic {
  "../assets/fonts/Roboto-BoldItalic.ttf"
};

// path to Lexia Bold font // REMOVE
// const std::string pFontLexiaBold { "../assets/fonts/Lexia-Bold.ttf" };

// REMOVE
// const std::string pLooneyTunes { "../assets/fonts/Looney-Tunes-Tilt-BT.ttf"
// };

}
#endif // !Paths_HPP

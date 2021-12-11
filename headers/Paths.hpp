#ifndef Paths_HPP
#define Paths_HPP
#include <string>

// namespace with deffined paths to files used in the game
namespace Paths {
// ------ IMAGES paths ------//
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

// ------ FONTS paths ------//
const std::string pFontRobotoBold { "../assets/fonts/Roboto-Bold.ttf" };
const std::string pFontRobotoRegular { "../assets/fonts/Roboto-Regular.ttf" };
const std::string pFontRobotoBoldItalic {
  "../assets/fonts/Roboto-BoldItalic.ttf"
};

// ------ Sound effects paths ------//

// sound to be emitted when the ball hits the paddle
const std::string pSoundBallBouncePaddle {
  "../assets/sounds/mixkit-basketball-ball-hard-hit-2093.wav"
};
// sound to be emitted when the ball hits a wall
const std::string pSoundBallBounceWall {
  "../assets/sounds/mixkit-quick-hit-of-a-basketball-2087.wav"
};

// sound to be emitted when a block is destroyed
const std::string pSoundBlockDestroyed {
  "../assets/sounds/mixkit-video-game-mystery-alert-234.wav"
};

// sound to be emitted when a ball leaves the screen
const std::string pSoundBallLost {
  "../assets/sounds/mixkit-arcade-retro-game-over-213.wav"
};

// sound to be emitted when a level is completed
const std::string pSoundLevelCompleted {
  "../assets/sounds/mixkit-completion-of-a-level-2063.wav"
};

// sound to be emitted when a game is won
const std::string pSoundGameWon {
  "../assets/sounds/mixkit-game-level-completed-2059.wav"
};
}
#endif // !Paths_HPP

# simple Arkanoid - a capstone project for C++ Nanodegree Program

As a capstone project for the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213) I have chosen to create a simple remake of classic game Arkanoid.
Therefore I have called the project simpleArkanoid.

In this single player game, the player tries to destroy all the blocks on the current level with the ball. The player needs to prevent the ball from escaping the bottom border of the game window. To this aim, the player uses the paddle, which can be moved left, right (within wall limits) and up and down (within invisible rectangle limiting the area of movement).

The game is controlled via keyboard. The detail instructions are displayed on the welcome/pause screen, which can be displayed whenever player wishes, by simply pausing the game.

On each level the ball speed is increased. However, the player can increase the ball speed in steps to unlimited value at will (and own responsibility) each time the ball hits the paddle by holding the correct key. The speed can also be decreased when hitting the paddle, but no more then to the minimum speed determined by the game level.

When the ball hits the paddle, it's direction can be altered slightly to the left or right (again - by holding the correct key in the correct moment).

Currently the game has only 10 levels implemented, but introducing new levels is fairly easy and fast, as each level data is stored in a text-file. Therefore, almost unlimited number of levels can be introduced by creating new text files. Important thing to remember is to keep the structure of the level file and naming convention.
Furthermore, for convenience, an empty level pattern is enclosed in the level folder. To help create the level, a calculation sheet is contained in the level folder as well. It doesn't have direct correspondence to creation of a level, but helps create the visual layout of a level first, which can later be entered into the level file manually.

Tthe game supports textures and basic sound effects. There is plenty of space for enhancing the game features.

Obviously, the game is programmed in the C++. The framework used is SDL (with additional SDL_image, SDL_ttf and SDL_mixer libraries).

All the sound effects files are downloaded from the [https://mixkit.co/free-sound-effects/](https://mixkit.co/free-sound-effects/) as free to use. All the textures are simple hand drawings created personaly (which can easily by seen as I have no drawing skills at all...).

Thank you for trying out my capstone project and thank you in advance for understanding, that it's not a fully featured top notch version of the original Arkanoid game (at this stage at least).

Have fun!

REMOVE
<img src="snake_game.gif"/>

## Dependencies for Running Locally

- cmake >= 3.16 (it is likely that the project should build using previous versions)
  - All OSes: [click here for installation instructions](https://cmake.org/install/)
- make >= 4.1 (Linux, Mac), 3.81 (Windows)
  - Linux: make is installed by default on most Linux distros
  - Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  - Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
- SDL2 >= 2.0
  - All installation instructions can be found [here](https://wiki.libsdl.org/Installation) > Note that for Linux, an `apt` or `apt-get` installation is preferred to building from source. > IMPORTANT NOTE: accept the main SDL library also SDL_Image, SDL_Mixer and SDL_ttf suport needs to be installed. Some additional help can be found
    Some additional information on installation can be found [here](https://lazyfoo.net/tutorials/SDL/06_extension_libraries_and_loading_other_image_formats/index.php).
    And on many places on the Internet.
- gcc/g++ >= 9.3 (it is likely that the project should build using previous versions)
  - Linux: gcc / g++ is installed by default on most Linux distros
  - Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  - Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./simpleArkanoid`.

## Overview of the code structure REVIEW:

The project is structured into classes, seperated into header files (contained in "headers" folder) and source files (src folder).

There are two shallow inheritence hierarchies:

1.  The base class StaticObject is an abstract base class for non movable game objects displayed on the screen.
    The class is inherited by the folling clasess:

    - the class SideWall - which is displayed on the right, left and top side of the screen and is responsible for limiting the ball movement area,
    - the class Block - which represents a block to be destroyed by the ball,
    - the class TextElement - which is represents a static rendered text to be displayed on the screen (currently it is heavily used on the pause screen, game over screen, the screen displayed after level completion and after the game is won).

2.  The base clas of MovableObject, which is an abstract base class for objects which can move on the game screen.
    The class is inherited by the following classes:
    - the Ball class - which represents the ball moving on the screen,
    - the Paddle class - represents a paddle used for preventing the ball from falling down the bottom limit of the screen and for directing the ball towards the blocks to be destroyed.

Appart the above mentioned inheritence hierarchies, the code is organised into following non-iheritance classes:

1.  the class Game - which obviously represents the main game object.
    In a seperate header (SDLinitializer.hpp) and source file (SDLinitializer.cpp) there are grouped 3 helper classes for the Game class:

    - SDLinitializer class - for SDL main subsystems RAII menagement,
    - ImageInitializer class - for RAII menagement of SDL_image support,
    - TTFinitializer class - for RAII menagement of SDL_ttf (true type font) support.
      Objects of above classes are used as components of the Game class, which initialized in the correct order provide convenient and exeption safe way to intialize and close SDL framework features.

2.  the class Controller - a component of the Game class, responsible for handling the keyboard input / stearing.

3.  the class Renderer - a component of the Game class, responsible for displaying the game objects onto the screen.

4.  the class LevelData - a component of the Game class, which loads and stores all data specific to the level from a level file.

5.  the class RandNum - a lightweight random number generating class (currently a component of the Ball class, but due to the usage of static functions can be accessed even whithout object instatiation in other places of the code); the class is enclosed into gMath namespace (as for game math).

6.  the class AudioMixer - which is responsible for providing sound effects support, namely loading and replaying .wav files.
    Together (defined with same header file and implemented in the other source file) with the AudioMixer class are the following classes:

    - the MixInitializer class
    - the MixOpener class.
      Both are helper classes for the AudioMixer class. Their sole purpose is to provide correct initialization, acquiring resources, realeasing resources and closing of SDL Mixer engine in a RAII way.

7.  the class Texture - used as an enhanced wrapper for SDL_Texture, image file loading into texture and generating textures from text. It is used as a component of StaticObject and Movable object-iheriting classes (Block, SideWall, Ball, Paddle and TextElement).

8.  the class TextElement - derived publicly from StaticObject and representing a rendered true type font text to be displayed.

9.  the class Vector2d - a basic component class for all StaticObject and MovableObject - inherited classes. In the project it is used to represent a position or velocity vector. Vector2d class is enclosed into gMath namespace together with RandNum class (although in seperate files) and some helper standalone functions.

10. IntervalTimer class - a simple component class allowing measuring intervals of time, which are used for game state update calculations

11. LimitTimer class - a simple class used for incorporating frame rate limiting policy (in the implementation of the Game::Run function) and for FPS calculations inside of Renderer::UpdateTitleBar function.

12. SDLexception class - a custom made exception class used for error handling policy related to all SDL related problems.

There are 3 standalone header files (not directly related to any class) used for convenient grouping of:

- all the enum classes used in the project - the Enum.hpp file,
- constants with color definitions - the Colors.hpp file,
- all the constants with paths to assets used in the project.

## C++ CAPSTONE PROJECT RUBRICS SATISFACTION

According to my knowledge the project should pass all the README and Compiling and Testing criteria.

## Additional rubrics satisfaction

I suppose, the project should satisfy the following rubrics:

### Loops, Functions, I/O

1. CRITERION: The project demonstrates an understanding of C++ functions and control structures
   NOTE: A variety of control structures are used in the project.
   NOTE: The project code is clearly organized into functions.

   The code is organised in a fairly reasonable way into classes. Apart from a couple of classes organised into shallow inheritence hierarchies, there are a few standalone component and helper classes. The classes Vector2d and RandNum are enclosed into namespace gMath. There are some enum classes as well. All the entities, which are not classes themselves, are enclosed into namespaces (namespace Colors, namespace Paths)

   WHERE IN THE CODE: classes maintain the most part of the project, clearly seperated into files. Some files have helper classes grouped together in one header-source file pairs. In standalone files are: namespace Colors (Colors.hpp), namespace Paths( Paths.hpp), enum classes grouped together (Enum.hpp).

2. CRITERION: The project reads data from a file and process the data, or the program writes data to a file.
   NOTE: The project reads data from an external file or writes data to a file as part of the necessary operation of the program.

   The project reads level files, parses them and uses the read information for retrieving level specifics. In particular, the layout of objects of class Block for each level is composed based on the level files data.

   WHERE IN THE CODE: the class LevelData loads and parses level files. The Game::LoadNewLevel function retrieves the information read from the file in the class LevelData for level construction and adjusting game state.

3. CRITERION: The project accepts user input and processes the input.
   NOTE: The project accepts input from a user as part of the necessary operation of the program.

   All the game control is based on the input from the keyboard.

   WHERE IN THE CODE: the class Controler

### Object Oriented Programming

1. CRITERION: The project uses Object Oriented Programming techniques.
   NOTE: The project code is organized into classes with class attributes to hold the data, and class methods to perform tasks.

   In my opinion the code is clearly organised into classes, with attributes to hold the data and member functions performing the most of the operations.

   WHERE IN THE CODE: virtually everywhere

2. CRITERION: Classes use appropriate access specifiers for class members.
   NOTE: All class data members are explicitly specified as public, protected, or private.

   Every class data or function member was exlipicitly specified as public, proteced or private.

   WHERE IN THE CODE: virtaully everywhere

3. CRITERION: Class constructors utilize member initialization lists.
   NOTE: All class members that are set to argument values are initialized through member initialization lists.

   As can be seen, class members that are set to argument values are initialized through initialization lists.

   WHERE IN THE CODE: virtually every class accepting arguments

4. CRITERION: Classes abstract implementation details from their interfaces.
   NOTE: All class member functions document their effects, either through function names, comments, or formal documentation. Member functions do not change program state in undocumented ways.

   The function names clearly resemble their effects. Moreover, the source code is heavily commented.
   To the best of my knowledge, the functions do not change the program state in undocumented ways.

   WHERE IN THE CODE: virtaully everywhere

5. CRITERION: Classes encapsulate behavior.
   NOTE: Appropriate data and functions are grouped into classes. Member data that is subject to an invariant is hidden from the user. State is accessed via member functions.

   All the appropriate data and functions are grouped into classes. State is accessed only via member functions. The access to data members being subject to an invariant is properly protected (in my humble opinion).

   WHERE IN THE CODE: virtaully everywhere

6. CRITERION: Classes follow an appropriate inheritance hierarchy.
   NOTE: Inheritance hierarchies are logical. Composition is used instead of inheritance when appropriate. Abstract classes are composed of pure virtual functions. Override functions are specified.

   Although there are only 2 inheritence hierarchies (StaticObject and MovableObject related), the use of them seems logical. Both StaticObject and MovableObject are abstract classes. The classes inheritting from them use virtual inheritence in case that the hierarchies might grow in the future. I imagine, that even if this is the case, this approach might prevent diamond inheritance related problems.

   The size of the project did not suggested many obvious hierchies, so composition was heavily used instead.

   WHERE IN THE CODE:

   - StaticObject is inherited by: SideWall, Block and TextElement
   - MovableObject is inherited by: Ball and Paddle.

   In my opinion there is plenty of space for the project enhancement and introducing for example: falling debris from the block, some moving obstacles (as MovableObjects) or some static obstacles inside the game screen (as StaticObjects).

7. CRITERION: Overloaded functions allow the same function to operate on different parameters.
   NOTE: One function is overloaded with different signatures for the same function name.

   In class Renderer there are two versions of DisplayScreen function:

   - void DisplayScreen(const std::vector<const StaticObject*>& staticObjects,
     const std::vector<const MovableObject*>& movableObjects) const
   - void DisplayScreen(const std::vector<const StaticObject\*>& staticObjects) const;

   In class Texture the constructor is overloaded in 2 versions:

   - Texture(const std::string& imagePath, SDL_Renderer\* gameRenderer)
   - Texture(const std::string& fontPath, SDL_Color color, int textSize,
     SDL_Renderer\* gameRenderer, const std::string& text)

   In class Vector2d there are 3 overloaded versions of constructor:

   - Vector2d(),
   - explicit Vector2d(float angle),
   - Vector2d(float X, float Y)

   In class MovableObject constructor is overloaded:

   - MovableObject(),
   - MovableObject(float x, float y, float speed)

   In class StaticObject constructor is overloaded:

   - StaticObject(),
   - StaticObject(float x, float y).

8. CRITERION: Derived class functions override virtual base class functions.
   NOTE: One member function in an inherited class overrides a virtual base class member function.

   Class Ball overrides pure virtual functions: Draw() and Update(). Additionally it overrides non-pure virtual function SetPositon().

   Class Paddle overrides pure virtual functions: Draw() and Update().

   Both class SideWall and TextElement override pure virtual function Draw().

9. CRITERION: Templates generalize functions in the project.
   NOTE: One function is declared with a template that allows it to accept a generic parameter.

   In the RandNum there are two templated functions: Random<T>() and operator<T>()().

   In the LevelData there is one templated function: ReadDataItem<T>(const std::string& filePath, const std::string& keyword);

### Memory Management

1. CRITERION: The project makes use of references in function declarations.
   NOTE: At least two variables are defined as references, or two functions use pass-by-reference in the project code.

   In class AudioMixer: AudioMixer(const std::unordered_map<Sound, std::string>& soundsAndPaths),

   In class Ball:

   - Ball(float speed, const Texture& texture, Paddle& paddle, float screenBottomY,
     Game& game, const std::vector<SideWall>& sideWalls,
     std::vector<Block>& blocks);
   - bool HasHitWall(const SideWall& wall) const,
   - void BounceWall(const SideWall& wall),
   - bool HasHitBlock(const Block& block) const,
   - void BounceBlock(const Block& block)
   - data member: Paddle& \_paddle
   - data member: Game& \_game,
   - data member: const std::vector<SideWall>& \_side_walls,
   - data member: std::vector<Block>& \_blocks.

   In class Block:

   - Block(float x, float y, const Texture& texture, unsigned pointValue);
   - data member: const Texture& \_texture

   In class Controller:

   - Controller(Game& game),
   - void HandleInput(bool& running, Paddle& paddle, Ball& ball) const
   - void HandleRoutineEvents(Paddle& paddle, Ball& ball) const

   In class Game: void HandleBlockHit(Block& block),

   In class LevelData:

   - LevelData(const std::string& path, unsigned levelNumber = 1),
   - T ReadDataItem(const std::string& filePath, const std::string& keyword) const,
   - bool ReadNumericalData(const std::string& filePath),
   - bool ReadSpriteTable(const std::string& filePath),
   - std::vector<Sprite> ReadSpriteRow(const std::string& fileLine) const,

   In class Paddle:

   - Paddle(float x, float y, float speed, SDL_Rect moveLimits, const Texture& texture),
   - data member: const Texture& \_texture

   In class Renderer:

   - void DisplayScreen(const std::vector<const StaticObject*>& staticObjects,
     const std::vector<const MovableObject*>& movableObjects) const,
   - void DisplayScreen(
     const std::vector<const StaticObject\*>& staticObjects) const

   In class SDLexception - SDLexception(const std::string& firstPart, const char* errorCodePart,
   const char* file, int line).

   In class SideWall:

   - SideWall(float x, float y, ScreenSide sideOfScreen, const Texture& texture,
     float halfTickness),
   - data member: const Texture& \_texture

   In class TextElement: TextElement(float x, float y, const std::string& fontPath, SDL_Color color,
   int textSize, SDL_Renderer\* gameRenderer, const std::string& text).

   In class Texture:

   - Texture(const std::string& imagePath, SDL_Renderer\* gameRenderer),
   - Texture(const std::string& fontPath, SDL_Color color, int textSize,
     SDL_Renderer\* gameRenderer, const std::string& text)

   In class Vector2d: Vector2d& operator+=(const Vector2d& other).

   Additionaly functions outside any class in file Vector2d.hpp:

   - inline float VerticalDistance(const Vector2d& A, const Vector2d& B),
   - inline float HorizontalDistance(const Vector2d& A, const Vector2d& B)

2. CRITERION: The project uses destructors appropriately.
   NOTE: At least one class that uses unmanaged dynamically allocated memory, along with any class that otherwise needs to modify state upon the termination of an object, uses a destructor.

   In file SDLinitializers.hpp:

   - class SDLinitializer,
   - class ImageInitializer,
   - class TTFinitializer.

   In file AudioMixer.hpp:

   - class MixInitializer,
   - class MixOpener

3. CRITERION: The project uses scope / Resource Acquisition Is Initialization (RAII) where appropriate.
   NOTE: The project follows the Resource Acquisition Is Initialization pattern where appropriate, by allocating objects at compile-time, initializing objects when they are declared, and utilizing scope to ensure their automatic destruction.

   In class AudioMixer:

   - usage of helper RAII classes MixInitializer and MixOpener,
   - usage of std::unique_ptr with custom deleter stored in data member \_sound_effects (the unique_ptr's are initialized in the constructor and stored in the member unordered map).

   In class Game:

   - usage of helper RAII classes: SDLinitializer, ImageInitializer, TTFinitializer (from the file. SDLinitializers.hpp),
   - data member: std::unique_ptr<Renderer> \_renderer;
   - data member: std::unordered_map<Sprite, Texture> \_images,
   - data member: std::vector<TextElement> \_texts,
   - data member: std::unique_ptr<AudioMixer> \_audio,
   - data member: std::unique_ptr<Controller> \_controller
   - data member: std::vector<SideWall> \_side_walls,
   - data member: std::unique_ptr<Ball> \_ball,
   - data member: std::unique_ptr<Paddle> \_paddle,
   - data member: std::vector<Block> \_blocks
   - data member: std::unique_ptr<LevelData> \_level_data
     All of the above are initialized in the constructor (often through helper private functions).

   In class Renderer:

   - data member: std::unique_ptr<SDL_Window, std::function<void(SDL_Window\*)>> \_sdl_window,
   - data member: std::unique_ptr<SDL_Renderer, std::function<void(SDL_Renderer\*)>>
     \_sdl_renderer.
     Both data members are intialized in the constructor. Both are unique_ptr with custom deleters.

   In class TextElement- data member: std::unique_ptr<Texture> \_texture - initialized in the constructor.

   In class Texture:

   - std::unique_ptr<SDL_Texture, std::function<void(SDL_Texture\*)>> \_sdl_texture - in the end parts of both the constructors ; unique_ptr uses custom deleter.
   - in the constructor Texture(const std::string& imagePath, SDL_Renderer\* gameRenderer):
     Before \_sdl_texture is created, additional local unique pointer with custom deleter is used:
     - std::unique_ptr<SDL_Surface, std::function<void(SDL_Surface\*)>> img_surface,
   - in the constructor Texture::Texture(const std::string& fontPath, SDL_Color color, int textSize,
     SDL_Renderer\* gameRenderer, const std::string& text), before \_sdl_texture is created, additonal local unique_ptrs with custom deleters are used:
     - std::unique_ptr<TTF_Font, std::function<void(TTF_Font\*)>> font,
     - std::unique_ptr<SDL_Surface, std::function<void(SDL_Surface\*)>> text_surface

4. CRITERION: The project follows the Rule of 5.
   NOTE: For all classes, if any one of the copy constructor, copy assignment operator, move constructor, move assignment operator, and destructor are defined, then all of these functions are defined.

   In general, the project is organised in a way ensuring that no custom destructors, copy or move operations are needed, due to the careful use of RAAI mechanisms, unique_ptrs, containers etc.

   I am not sure, if it satisfies the criterion in such case.

5. CRITERION: The project uses smart pointers instead of raw pointers.
   NOTE: The project uses at least one smart pointer: unique_ptr, shared_ptr, or weak_ptr. The project does not use raw pointers.

   The project doesn't use raw pointers for menaging resources. The only cases were raw pointers are used, is for passing raw pointers to interal SDL library function.

   In class AudioMixer: data member: std::unordered_map<Sound, std::unique_ptr<Mix_Chunk, std::function<void(Mix_Chunk\*)>>> \_sound_effects;

   In class Game:

   - std::unique_ptr<Renderer> \_renderer,
   - std::unique_ptr<AudioMixer> \_audio,
   - std::unique_ptr<Controller> \_controller,
   - std::unique_ptr<Ball> \_ball,
   - std::unique_ptr<Paddle> \_paddle,
   - std::unique_ptr<LevelData> \_level_data

   In class Renderer:

   - std::unique_ptr<SDL_Window, std::function<void(SDL_Window\*)>> \_sdl_window;
   - std::unique_ptr<SDL_Renderer, std::function<void(SDL_Renderer\*)>>
     \_sdl_renderer.

   In class TextElement: std::unique_ptr<Texture> \_texture;

   In class Texture:

   - std::unique_ptr<SDL_Texture, std::function<void(SDL_Texture\*)>> \_sdl_texture;
   - additonaly local unique pointers with custom deleters in constructors:
     - std::unique_ptr<SDL_Surface, std::function<void(SDL_Surface\*)>> img_surface
     - std::unique_ptr<TTF_Font, std::function<void(TTF_Font\*)>> font
     - std::unique_ptr<SDL_Surface, std::function<void(SDL_Surface\*)>> text_surface

## CC Attribution-ShareAlike 4.0 International

Shield: [![CC BY-SA 4.0][cc-by-sa-shield]][cc-by-sa]

This work is licensed under a
[Creative Commons Attribution-ShareAlike 4.0 International License][cc-by-sa].

[![CC BY-SA 4.0][cc-by-sa-image]][cc-by-sa]

[cc-by-sa]: http://creativecommons.org/licenses/by-sa/4.0/
[cc-by-sa-image]: https://licensebuttons.net/l/by-sa/4.0/88x31.png
[cc-by-sa-shield]: https://img.shields.io/badge/License-CC%20BY--SA%204.0-lightgrey.svg

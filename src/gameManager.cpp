#include "../include/gameManager.hpp"

// Definição dos membros estáticos
const int GameManager::HEIGHT;
const int GameManager::WIDTH;
const int GameManager::CELL_SIZE;
const int GameManager::H_RES;
const int GameManager::W_RES;
const int GameManager::NUM_TETRO;
const int GameManager::NUM_COLORS;
const char GameManager::types[NUM_TETRO] = {'I', 'O', 'T', 'L', 'J', 'S', 'Z'};
const int GameManager::gravityTimeMs;

//All the colors for the cells
const std::vector<sf::Color> GameManager::tetromino_colors= {
    sf::Color::Red,
    sf::Color::Blue,
    sf::Color::Yellow,
    sf::Color::Green,
    sf::Color::Cyan,
    sf::Color::Magenta
};

GameManager::GameManager()
    : board(WIDTH, HEIGHT, CELL_SIZE), // Standard Tetris board size
      isRunning(false),                // Window with 400x800 resolution
      window(sf::VideoMode(W_RES, H_RES), "Tetris"),
      gravityTime(sf::milliseconds(GameManager::gravityTimeMs))
{
}

// Ta bom por enquanto
void GameManager::runGameLoop()
{
    this->isRunning = true;

    // Init timer
    gameClock.restart();
    spawnTetromino();

    while (window.isOpen())
    {
        handleInput();
        update();
        render();
    }
}

void GameManager::renderTetromino()
{
    // Se o ponteiro é nulo, não há Tetromino para renderizar
    if (!currentTetromino)
    {
        std::cout << "ERROR: No Tetromino generated!" << std::endl;
        return;
    }

    // Obter o shape e a posição do Tetromino
    const auto &shape = currentTetromino->getShape();
    int tetrominoX = currentTetromino->getX();
    int tetrominoY = currentTetromino->getY();

    // Configurar um bloco representando cada célula do Tetromino
    sf::RectangleShape block(sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE));
    block.setFillColor(currentTetromino->getColor()); // Usar a cor do Tetromino
    block.setOutlineThickness(1);
    block.setOutlineColor(sf::Color::Black); // Adicionar um contorno para clareza

    // Percorrer a matriz do Tetromino (shape)
    for (size_t i = 0; i < shape.size(); ++i)
    {
        for (size_t j = 0; j < shape[i].size(); ++j)
        {
            if (shape[i][j] > 0)
            { // Se for uma célula válida (1,2,3... no shape)
                // Calcular a posição do bloco na grid
                block.setPosition((tetrominoX + j) * BLOCK_SIZE, (tetrominoY + i) * BLOCK_SIZE);

                // Desenhar o bloco
                window.draw(block);
            }
        }
    }
}

void GameManager::spawnTetromino()
{
    // Example of spawning a random tetromino (you can enhance this with a better randomizer)
    char randomType = types[rand() % NUM_TETRO]; // Randomly select one tetromino
    char randomColor = tetromino_colors[rand() % NUM_COLORS];
    currentTetromino = TetrominoFactory::createTetromino(randomType, randomColor);
    // random placement as well
}

/* ---------- Game Loop functions --------- */

void GameManager::handleInput()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        switch (event.type)
        {

        case sf::Event::Closed:
            window.close();
            break;

        case sf::Event::KeyPressed:
            switch (event.key.code)
            {
            case sf::Keyboard::Left: // Move the Tetromino left
                // currentTetromino->moveLeft();
                break;

            case sf::Keyboard::Right: // Move the Tetromino right
                // currentTetromino->moveRight();
                break;

            case sf::Keyboard::Up: // Rotate the Tetromino
                // currentTetromino->rotateRight();
                break;

            case sf::Keyboard::Down: // Drop the Tetromino faster
                // currentTetromino->dropFast();
                break;

            case sf::Keyboard::Escape: // Exit the game
                window.close();
                break;

            default:
                break;
            }
            break;

        default:
            break;
        }
    }
}

void GameManager::update()
{

    // if (gameClock.getElapsedTime() > gravityTime)
    // {
    // currentTetromino->dropSlow();
    // gameClock.restart();
    // }

    // Handle collision with the bottom of the board or locked pieces
    // if (board.checkCollision(*currentTetromino))
    // {
    // board.placeTetromino(*currentTetromino); // Place tetromino on the board
    // spawnTetromino();                        // Spawn a new tetromino
    // }
}
void GameManager::render()
{
    window.clear(); // Clear the screen before drawing

    // Total rendering

    board.render(window); // Render the board (grid), with all it's elements (locked tetrominos, UI elems -> score, level, next piece, etc)
    renderTetromino();    // Render the current tetromino

    window.display(); // Display everything we just drew
}

/* ---------- ---------- ---------- --------- */

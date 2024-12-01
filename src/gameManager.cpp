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

// All the colors for the cells
const std::vector<sf::Color> GameManager::tetromino_colors = {
    sf::Color::Red,
    sf::Color::Blue,
    sf::Color::Yellow,
    sf::Color::Green,
    sf::Color::Cyan,
    sf::Color::Magenta};

GameManager::GameManager()
    : board(WIDTH, HEIGHT, CELL_SIZE), // Standard Tetris board size
      isRunning(false),                // Window with 400x800 resolution
      window(sf::VideoMode(W_RES, H_RES), "Tetris"),
      gravityTime(sf::milliseconds(GameManager::gravityTimeMs))
{
}

void GameManager::printStatus()
{
    system("clear");

    // Imprimir status da board
    std::cout << "\nEstado da Board:" << std::endl;
    for (int x = 0; x < HEIGHT; x++)
    {
        for (int y = 0; y < WIDTH; y++)
        {
            auto currCell = board.getGrid()[x][y];
            std::cout << (currCell.isFalling() ? " # " : (currCell.isEmpty()) ? " * "
                                                                              : " $ ");
        }
        std::cout << std::endl;
    }
    std::cout << std::string(40, '-') << std::endl; // Separador visual
}

// Ta bom por enquanto
void GameManager::runGameLoop()
{
    this->isRunning = true;

    // Init timer
    srand((int)time(0));
    // srand(0);
    gameClock.restart();
    spawnTetromino();

    while (window.isOpen())
    {
        handleInput();
        update();
        render();
    }
}

void GameManager::spawnTetromino()
{
    // Example of spawning a random tetromino (you can enhance this with a better randomizer)
    char randomType = types[rand() % NUM_TETRO]; // Randomly select one tetromino
    sf::Color randomColor = tetromino_colors.at(rand() % NUM_COLORS);
    int col = rand() % WIDTH;
    currentTetromino = TetrominoFactory::createTetromino(randomType, 0, col, randomColor);
    // std::cout << "Tetromino " << randomType << " spawnado em " << col << "Com a cor numero " << num_color << std::endl;
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
                currentTetromino->action(LEFT);
                break;

            case sf::Keyboard::Right: // Move the Tetromino right
                currentTetromino->action(RIGHT);
                break;

            case sf::Keyboard::Up: // Rotate the Tetromino right
                currentTetromino->action(ROT_RIGHT);
                break;

            case sf::Keyboard::Down: // Rotate the Tetromino right
                currentTetromino->action(ROT_LEFT);
                break;

            case sf::Keyboard::Space: // Drop the Tetromino
                currentTetromino->action(DOWN_FASTER);
                break;

            // case sf::Keyboard::Enter: // Drop it to the ground !
            //     currentTetromino->dropToTheGround();
            //     break;

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
    // Já foi atualizado o estado interno no passo anterior, pelas teclas

    // Apagar os tetrominos anteriores
    board.clearFallingTetrominos();

    // Gravidade -> Checar mais tarde pra condições de se já ta embaixo
    if (gameClock.getElapsedTime() > gravityTime)
    {
        currentTetromino->dropGravity();
        gameClock.restart();
    }

    int lastMove = currentTetromino->getLastMove();

    // Só deve dar updateStates depois de checar contra a board
    if (board.checkCollision(*currentTetromino) )
    {

        if(lastMove == LEFT || lastMove == RIGHT)
            std::cout << "Dectectei colisao lateral " << std::endl;
        // std::cout << "Check collision : last move = " << lastMove << std::endl;
        // na vdd seria board.placeFalledTetromino()
        if(lastMove == DOWN_FASTER || currentTetromino->getGravity()){

            // Place tetromino on the board
            board.placeTetromino(*currentTetromino, true); 
            currentTetromino->updateStates();
            currentTetromino.reset();
            std::cout << "Saiu um novo do forno" << std::endl;
            spawnTetromino(); // Spawn a new tetromino
        }else{ // Just invalid, just clean it's action
            board.placeTetromino(*currentTetromino, false); 
            currentTetromino->updateStates();
        }
    }else{
        // Atualiza o Tetromino ativo no jogo (movimenta e reposiciona)
        board.placeTetromino(*currentTetromino, false);
        currentTetromino->updateStates();
    }
}

void GameManager::render()
{
    // printStatus();
    window.clear(); // Clear the screen before drawing

    // Total rendering
    board.render(window); // Render the board (grid), with all it's elements (locked tetrominos, UI elems -> score, level, next piece, etc)

    window.display(); // Display everything we just drew
}

/* ---------- ---------- ---------- --------- */

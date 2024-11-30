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
    // Imprimir status do Tetromino atual

    const auto &shape = currentTetromino->getShape();
    for (const auto &row : shape)
    {
        for (int cell : row)
        {
            std::cout << (cell ? "#" : ".") << " ";
        }
        std::cout << std::endl;
    }

    // Imprimir status da board
    std::cout << "\nEstado da Board:" << std::endl;
    for (int x=0; x < HEIGHT; x++)
    {
        for (int y = 0 ; y < WIDTH; y++)
        {
            auto currCell = board.getGrid()[x][y];
            std::cout << (currCell.isFalling() ? " # " : (currCell.isEmpty()) ? " * " : " $ ");
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
    // std::cout << "Tetromino " << randomType << " spawnado em " << col << std::endl;
}

/*
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
    sf::RectangleShape block(sf::Vector2f(CELL_SIZE, CELL_SIZE));
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
                block.setPosition((tetrominoX + j) * CELL_SIZE, (tetrominoY + i) * CELL_SIZE);

                // Desenhar o bloco
                window.draw(block);
            }
        }
    }
}
*/

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
                currentTetromino->moveLeft();
                break;

            case sf::Keyboard::Right: // Move the Tetromino right
                currentTetromino->moveRight();
                break;

            case sf::Keyboard::Up: // Rotate the Tetromino right
                currentTetromino->rotateRight();
                break;

            case sf::Keyboard::Down: // Rotate the Tetromino right
                currentTetromino->rotateLeft();
                break;

            case sf::Keyboard::Space: // Drop the Tetromino
                currentTetromino->dropSlow();
                break;

            case sf::Keyboard::Enter: // Drop the Tetromino faster
                currentTetromino->dropFast();
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
    // Já foi atualizado o estado interno no passo anterior, pelas teclas

    // Apagar os tetrominos anteriores
    board.clearFallingTetrominos();

    // Gravidade -> Checar mais tarde pra condições de se já ta embaixo
    if (gameClock.getElapsedTime() > gravityTime)
    {
        currentTetromino->dropSlow();
        gameClock.restart();
    }

    // Handle collision with the bottom of the board or locked pieces
    if (board.checkCollision(*currentTetromino))
    {
        // std::cout << "Entrei no fundo " << std::endl;
        // na vdd seria board.placeFalledTetromino()
        board.placeTetromino(*currentTetromino, true); // Place tetromino on the board
        // board.
        currentTetromino.reset();
        spawnTetromino(); // Spawn a new tetromino
    } // else { place tetromino}

    // Atualiza o Tetromino ativo no jogo (movimenta e reposiciona)
    board.placeTetromino(*currentTetromino, false);
}
void GameManager::render()
{
    printStatus();
    window.clear(); // Clear the screen before drawing

    // Total rendering
    board.render(window, *currentTetromino); // Render the board (grid), with all it's elements (locked tetrominos, UI elems -> score, level, next piece, etc)

    window.display(); // Display everything we just drew
}

/* ---------- ---------- ---------- --------- */

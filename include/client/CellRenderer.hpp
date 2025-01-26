// CellRenderer.hpp
#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <functional>
#include "Cell.hpp"
#include <iostream> // Para std::cerr

/**
 * Define os modos de renderização de cada célula.
 */
enum class CellRenderMode
{
    None,               // Sem gradiente, cor única
    DiagonalGradient,   // Diagonal escuro->claro
    VerticalGradient,   // Vertical escuro->claro
    HorizontalGradient, // Horizontal escuro->claro
    CentralGradient     // Gradiente circular claro->escuro
};

class CellRenderer : public sf::Drawable, public sf::Transformable
{
private:
    static int cells_count;

    std::shared_ptr<Cell> cell;
    std::function<void()> onClickCallback;

    // Vertex array principal (preenchimento para modos não-shader)
    sf::VertexArray fillVertices;

    // Outline (borda grossa)
    sf::RectangleShape outlineShape;

    // RectangleShape para CentralGradient com shader
    sf::RectangleShape shaderShape;
    sf::Shader shader;

    // Tamanho “interno” da célula (sem considerar outline)
    sf::Vector2f size;

    // Modo de render escolhido pelo construtor
    CellRenderMode renderMode;

    // Textura branca para shader
    static sf::Texture whiteTexture;
    static bool whiteTextureInitialized;

    //------------------------------------------------------
    // Inicialização da Textura Branca
    //------------------------------------------------------
    static bool initializeWhiteTexture();

    //------------------------------------------------------
    // Reconstrói a geometria do fill e do outline
    //------------------------------------------------------
    void updateGeometry();

    //------------------------------------------------------
    // Monta a parte interna (fillVertices) para modos não-shader
    //------------------------------------------------------
    void rebuildFill();

    //------------------------------------------------------
    // Monta o outline usando RectangleShape
    //------------------------------------------------------
    void rebuildOutline();

    //------------------------------------------------------
    // Configura o shape para CentralGradient
    //------------------------------------------------------
    void rebuildShaderShape();

    //------------------------------------------------------
    // Desenho final
    //------------------------------------------------------
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

public:
    //------------------------------------------------------
    // Inicialização estática da textura branca
    //------------------------------------------------------
    static bool initStatic();

    //------------------------------------------------------
    // Construtor
    //------------------------------------------------------
    CellRenderer(sf::Vector2f size,
                 sf::Vector2f position,
                 CellRenderMode mode,
                 std::shared_ptr<Cell> c);

    //------------------------------------------------------
    // Destructor
    //------------------------------------------------------
    ~CellRenderer();

    //------------------------------------------------------
    // Ajuste de posição/tamanho (se a célula for redimensionada)
    //------------------------------------------------------
    void refreshPosition(sf::Vector2f newSize, sf::Vector2f newPosition);

    //------------------------------------------------------
    // Outline (se quiser trocar em tempo de execução)
    //------------------------------------------------------
    void setOutlineThickness(float thickness);
    void setOutlineColor(const sf::Color &color);

    //------------------------------------------------------
    // Callback de clique (opcional)
    //------------------------------------------------------
    void setOnClick(const std::function<void()> &callback);

    void handleEvent(const sf::Event &event, const sf::RenderWindow &window);

    //------------------------------------------------------
    // updateData() — define as cores de acordo com CellColor
    //------------------------------------------------------
    void updateData();
};

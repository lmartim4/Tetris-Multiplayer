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
    HorizontalGradient // Horizontal escuro->claro
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

    // Tamanho “interno” da célula (sem considerar outline)
    sf::Vector2f size;

    // Modo de render escolhido pelo construtor
    CellRenderMode renderMode;

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
    // Desenho final
    //------------------------------------------------------
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

public:
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

    std::shared_ptr<Cell> getCell();
};

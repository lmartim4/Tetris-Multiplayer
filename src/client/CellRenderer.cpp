// CellRenderer.cpp
#include "CellRenderer.hpp"

int CellRenderer::cells_count = 0;

// Definições estáticas
sf::Texture CellRenderer::whiteTexture;
bool CellRenderer::whiteTextureInitialized = CellRenderer::initStatic();

// Implementação da inicialização da textura branca
bool CellRenderer::initializeWhiteTexture()
{
    if (!whiteTexture.create(1, 1))
    {
        std::cerr << "Erro ao criar a textura branca." << std::endl;
        return false;
    }

    // Define o pixel branco
    unsigned char whitePixel[4] = {255, 255, 255, 255};
    whiteTexture.update(whitePixel);
    return true;
}

// Implementação dos métodos da classe
CellRenderer::CellRenderer(sf::Vector2f size,
                           sf::Vector2f position,
                           CellRenderMode mode,
                           std::shared_ptr<Cell> c)
    : cell(c),
      size(size),
      renderMode(mode)
{
    std::cout << "CellRender Count = " << ++cells_count << "\n";

    // Assegura que a textura branca está inicializada
    if (!whiteTextureInitialized)
    {
        std::cerr << "Falha na inicialização da textura branca." << std::endl;
    }

    // Carrega o shader para o gradiente central
    if (!shader.loadFromFile("../assets/shaders/central_gradient.frag", sf::Shader::Fragment))
    {
        std::cerr << "Erro ao carregar o shader central_gradient.frag" << std::endl;
    }

    // Configura o fillVertices e outlineShape
    if (renderMode != CellRenderMode::CentralGradient)
    {
        rebuildFill();
    }
    else
    {
        rebuildShaderShape();
    }

    rebuildOutline();

    // Ajusta a posição global via Transformable
    setPosition(position);
}

CellRenderer::~CellRenderer()
{
    std::cout << "Deleting a CellRenderer (Count = " << --cells_count << ")\n";
}

bool CellRenderer::initStatic()
{
    return initializeWhiteTexture();
}

void CellRenderer::updateGeometry()
{
    // Atualiza o outline
    rebuildOutline();

    if (renderMode != CellRenderMode::CentralGradient)
    {
        // Atualiza o fillVertices para modos baseados em cor
        rebuildFill();
    }
    else
    {
        // Atualiza o shaderShape para CentralGradient
        rebuildShaderShape();
    }
}

void CellRenderer::rebuildFill()
{
    // Configura os vértices para preenchimento, dependendo do modo
    fillVertices.setPrimitiveType(sf::Quads);
    fillVertices.resize(4);

    // Posições do Quad
    fillVertices[0].position = sf::Vector2f(0.f, 0.f);       // Top-Left
    fillVertices[1].position = sf::Vector2f(size.x, 0.f);    // Top-Right
    fillVertices[2].position = sf::Vector2f(size.x, size.y); // Bottom-Right
    fillVertices[3].position = sf::Vector2f(0.f, size.y);    // Bottom-Left
}

void CellRenderer::rebuildOutline()
{
    outlineShape.setSize(size);
    outlineShape.setPosition(0.f, 0.f);                // posição local
    outlineShape.setFillColor(sf::Color::Transparent); // Apenas a borda
    outlineShape.setOutlineThickness(10.f);            // Espessura da borda
    outlineShape.setOutlineColor(sf::Color::Black);
}

void CellRenderer::rebuildShaderShape()
{
    shaderShape.setSize(size);
    shaderShape.setPosition(0.f, 0.f);
    shaderShape.setFillColor(sf::Color::White); // Cor base, será sobrescrita pelo shader
    shaderShape.setOutlineThickness(0.f);       // Usaremos outlineShape para a borda

    // Assegura que a textura está atribuída para passar coordenadas de textura
    shaderShape.setTexture(&whiteTexture);
    shaderShape.setTextureRect(sf::IntRect(0, 0, static_cast<int>(size.x), static_cast<int>(size.y)));
}

void CellRenderer::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    // Aplica a transformação do próprio objeto
    states.transform *= getTransform();

    if (renderMode == CellRenderMode::CentralGradient)
    {
        // Configura o shader
        sf::RenderStates shaderStates = states;
        shaderStates.shader = &shader;
        target.draw(shaderShape, shaderStates);
    }
    else
    {
        // Desenha o fillVertices com cores interpoladas
        target.draw(fillVertices, states);
    }

    // Desenha o outline
    target.draw(outlineShape, states);
}

void CellRenderer::refreshPosition(sf::Vector2f newSize, sf::Vector2f newPosition)
{
    size = newSize;
    setPosition(newPosition);

    if (renderMode != CellRenderMode::CentralGradient)
    {
        rebuildFill();
    }
    else
    {
        rebuildShaderShape();
    }

    rebuildOutline();
}

void CellRenderer::setOutlineThickness(float thickness)
{
    outlineShape.setOutlineThickness(thickness);
}

void CellRenderer::setOutlineColor(const sf::Color &color)
{
    outlineShape.setOutlineColor(color);
}

void CellRenderer::setOnClick(const std::function<void()> &callback)
{
    onClickCallback = callback;
}

void CellRenderer::handleEvent(const sf::Event &event, const sf::RenderWindow &window)
{
    if (event.type == sf::Event::MouseButtonPressed)
    {
        sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x),
                              static_cast<float>(event.mouseButton.y));

        // Transforma para coords locais deste objeto
        sf::Transform inverse = getInverseTransform();
        sf::Vector2f localPos = inverse.transformPoint(mousePos);

        // Testa se está dentro da área [0,0]..[size.x, size.y]
        if (localPos.x >= 0.f && localPos.y >= 0.f &&
            localPos.x <= size.x && localPos.y <= size.y)
        {
            if (onClickCallback)
                onClickCallback();
        }
    }
}

void CellRenderer::updateData()
{
    // Pega a cor do Cell
    CellColor ctype = cell->getColor();

    // Escolhe as cores "claras" e "escuras" para cada modo
    sf::Color dark, light;
    switch (ctype)
    {
    case CellColor::Empty:
        dark = sf::Color(80, 80, 80);
        light = sf::Color(50, 50, 50);
        break;
    case CellColor::Red:
        dark = sf::Color(255, 100, 100);
        light = sf::Color::Red;
        break;
    case CellColor::Green:
        dark = sf::Color(100, 255, 100);
        light = sf::Color::Green;
        break;
    case CellColor::Blue:
        dark = sf::Color(100, 100, 255);
        light = sf::Color::Blue;
        break;
    case CellColor::Yellow:
        dark = sf::Color(255, 255, 128);
        light = sf::Color::Yellow;
        break;
    default:
        dark = sf::Color(255, 100, 255);
        light = sf::Color::Magenta;
        break;
    }

    if (renderMode == CellRenderMode::None)
    {
        // Sem gradiente, cor única
        for (size_t i = 0; i < fillVertices.getVertexCount(); i++)
            fillVertices[i].color = light;
    }
    else if (renderMode == CellRenderMode::DiagonalGradient)
    {
        // Diagonal escuro->claro
        fillVertices[0].color = dark;  // Top-Left
        fillVertices[1].color = light; // Top-Right
        fillVertices[2].color = dark;  // Bottom-Right
        fillVertices[3].color = light; // Bottom-Left
    }
    else if (renderMode == CellRenderMode::VerticalGradient)
    {
        // Vertical escuro->claro
        fillVertices[0].color = light; // Top-Left
        fillVertices[1].color = light; // Top-Right
        fillVertices[2].color = dark;  // Bottom-Right
        fillVertices[3].color = dark;  // Bottom-Left
    }
    else if (renderMode == CellRenderMode::HorizontalGradient)
    {
        // Horizontal escuro->claro
        fillVertices[0].color = dark;  // Top-Left
        fillVertices[1].color = light; // Top-Right
        fillVertices[2].color = light; // Bottom-Right
        fillVertices[3].color = dark;  // Bottom-Left
    }
    else if (renderMode == CellRenderMode::CentralGradient)
    {
        // CentralGradient: atualiza uniforms do shader
        shader.setUniform("u_resolution", sf::Glsl::Vec2(size));
        shader.setUniform("u_centerColor", sf::Glsl::Vec4(dark));
        shader.setUniform("u_edgeColor", sf::Glsl::Vec4(light));
    }
}

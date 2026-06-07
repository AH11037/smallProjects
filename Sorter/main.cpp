#include "robotoCondensed.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <algorithm>
#include <cstdlib>
#include <vector>

constexpr float size = 900;
constexpr float amount = 100;
constexpr float baseY = size - 150;
constexpr float barWidth = (size - 200.0f) / amount;

int i = 0;
int j = 0;
bool sorting = false;
bool completed = false;

std::vector<std::pair<int, int>> stack;
int low, high, p;
bool partitioning = false;

long long operations = 0;
int x = 0;

void bubbleSort(std::vector<sf::RectangleShape> &shapeVector) {
  if (!sorting)
    return;
  if (i < shapeVector.size()) {
    if (j < shapeVector.size() - 1 - i) {
      shapeVector[j].setFillColor(sf::Color::Red);
      shapeVector[j + 1].setFillColor(sf::Color::Red);

      operations++;
      if (shapeVector[j].getSize().y > shapeVector[j + 1].getSize().y) {
        float temp = shapeVector[j].getSize().y;

        shapeVector[j].setSize(
            {shapeVector[j].getSize().x, shapeVector[j + 1].getSize().y});
        shapeVector[j].setPosition(shapeVector[j].getPosition().x,
                                   baseY - shapeVector[j].getSize().y);

        shapeVector[j + 1].setSize({shapeVector[j + 1].getSize().x, temp});
        shapeVector[j + 1].setPosition(shapeVector[j + 1].getPosition().x,
                                       baseY - shapeVector[j + 1].getSize().y);
      }

      j++;
    } else {
      j = 0;
      i++;
    }
  } else {
    sorting = false;
    completed = true;
  }
}

void quickSort(std::vector<sf::RectangleShape> &a) {
  if (!sorting)
    return;

  if (stack.empty() && !partitioning) {
    sorting = false;
    completed = true;
    return;
  }

  if (!partitioning) {
    auto range = stack.back();
    stack.pop_back();

    low = range.first;
    high = range.second;

    p = high;
    i = low - 1;
    j = low;

    partitioning = true;
    return;
  }

  if (j < high) {
    a[j].setFillColor(sf::Color::Red);
    a[p].setFillColor(sf::Color::Blue);

    if (a[j].getSize().y < a[p].getSize().y) {
      i++;
      std::swap(a[i], a[j]);

      a[i].setPosition(100.f + i * barWidth, baseY - a[i].getSize().y);
      a[j].setPosition(100.f + j * barWidth, baseY - a[j].getSize().y);
    }

    j++;
    operations++;
    return;
  }

  int pivotIndex = i + 1;

  std::swap(a[pivotIndex], a[p]);
  a[pivotIndex].setPosition(100.f + pivotIndex * barWidth,
                            baseY - a[pivotIndex].getSize().y);
  a[p].setPosition(100.f + p * barWidth, baseY - a[p].getSize().y);

  if (pivotIndex - 1 > low)
    stack.push_back({low, pivotIndex - 1});

  if (pivotIndex + 1 < high)
    stack.push_back({pivotIndex + 1, high});

  partitioning = false;
}

std::vector<sf::RectangleShape> render() {
  std::vector<int> SorterVictim = {};
  std::vector<sf::RectangleShape> temp = {};

  for (int i = 0; i < amount; i++) {
    int a = 1 + (rand() % 51);
    SorterVictim.push_back(a);
  }

  for (int i = 0; i < SorterVictim.size(); i++) {
    float rectH = static_cast<float>(SorterVictim[i] * 10);
    sf::RectangleShape tempS(sf::Vector2f(barWidth, rectH));
    tempS.setPosition(sf::Vector2f(100.0f + i * barWidth, baseY - rectH));
    tempS.setFillColor(sf::Color::White);
    temp.push_back(tempS);
  }

  return temp;
}

void reset(std::vector<sf::RectangleShape> &shapeVector) {
  shapeVector = render();
  operations = 0;
  completed = false;
}

int main() {
  sf::RenderWindow window(sf::VideoMode({static_cast<unsigned int>(size),
                                         static_cast<unsigned int>(size)}),
                          "Sorter", sf::Style::Titlebar | sf::Style::Close);

  std::vector<sf::RectangleShape> shapeVector = render();

  sf::Font font;
  font.loadFromMemory(RobotoCondensed_VariableFont_wght, 370940);

  std::vector<std::string> sortingType = {"Bubble Sort", "QuickSort"};

  sf::Text screenText;
  screenText.setFont(font);
  screenText.setCharacterSize(35);
  screenText.setPosition(sf::Vector2f(50.f, 37.5));
  screenText.setFillColor(sf::Color::Red);
  screenText.setString("Press Space");

  sf::Text winText;
  winText.setFont(font);
  winText.setCharacterSize(35);
  winText.setPosition(sf::Vector2f(50.f, (size - 87.5)));
  winText.setFillColor(sf::Color::Green);
  winText.setString("Done!");

  sf::Text typeText;
  typeText.setFont(font);
  typeText.setCharacterSize(35);
  typeText.setPosition(sf::Vector2f(50.f, (size - 137.5)));
  typeText.setFillColor(sf::Color::White);
  typeText.setString(sortingType[x]);

  srand(time(0));

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
      if (event.type == sf::Event::KeyPressed && !sorting) {
        if (event.key.code == sf::Keyboard::R)
          reset(shapeVector);

        else if (event.key.code == sf::Keyboard::Space) {
          i = 0;
          j = 0;
          sorting = true;
          completed = false;

          for (int i = 0; i < shapeVector.size(); i++)
            shapeVector[i].setPosition(sf::Vector2f(
                100.0f + (i * barWidth), baseY - shapeVector[i].getSize().y));

          if (x == 1) {
            stack.clear();
            stack.push_back({0, static_cast<int>(shapeVector.size() - 1)});
            partitioning = false;
          }
        } else if (event.key.code == sf::Keyboard::N) {
          x = (x + 1) % sortingType.size();
          typeText.setString(sortingType[x]);
          reset(shapeVector);
        }
      };
    }

    window.clear();
    window.draw(screenText);
    window.draw(typeText);
    for (auto &shape : shapeVector)
      window.draw(shape);
    for (auto &shape : shapeVector)
      shape.setFillColor(sf::Color::White);

    switch (x) {
    case 0:
      bubbleSort(shapeVector);
      break;
    case 1:
      quickSort(shapeVector);
      break;
    }

    screenText.setString("Current amount of operations: " +
                         std::to_string(operations));
    if (completed)
      window.draw(winText);
    window.display();
  }
}
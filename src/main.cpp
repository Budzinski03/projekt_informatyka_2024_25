#include <iostream>
#include <SFML\Graphics.hpp>
#include <vector>

class Menu
{
public:
    Menu(float, float,const sf::Font &);
    void rysuj(sf::RenderWindow &window);
    void przesunWGore();
    void przesunWDol();
    int pobierzWybranaOpcje() const;

private:
    int wybranaOpcja;
    sf::Font czcionka;
    std::vector<sf::Text> opcje;
};

Menu::Menu(float width, float height,const sf::Font &czcionka) : wybranaOpcja(0)
{   
    std::string tekstOpcji[] = {"Nowa gra", "Opcje", "Wyjscie"};

    for (int i = 0; i < 3; i++)
    {
        sf::Text tekst;
        tekst.setFont(czcionka);
        //jesli i = wybranaOpcja to kolor czerowny, jesli nie to bialy
        tekst.setFillColor(i == wybranaOpcja ? sf::Color::Red : sf::Color::White);
        tekst.setString(tekstOpcji[i]);
        tekst.setPosition(sf::Vector2f(800 / 10, 600 / (6) * (i + 1)));
        opcje.push_back(tekst);
    }
}

void Menu::rysuj(sf::RenderWindow &window)
{
    for (const auto &opcja : opcje)
    {
        window.draw(opcja);
    }
}

void Menu::przesunWGore()
{
    if (wybranaOpcja - 1 >= 0)
    {
        opcje[wybranaOpcja].setFillColor(sf::Color::White);
        wybranaOpcja--;
        opcje[wybranaOpcja].setFillColor(sf::Color::Red);
    }
}

void Menu::przesunWDol()
{
    //sprawdzenie czy mozna przesunac w dol
    if (wybranaOpcja + 1 < opcje.size())
    {
        opcje[wybranaOpcja].setFillColor(sf::Color::White);
        wybranaOpcja++;
        opcje[wybranaOpcja].setFillColor(sf::Color::Red);
    }
}

int Menu::pobierzWybranaOpcje() const
{
    return wybranaOpcja;
}

class Gracz
{
public:
    Gracz();
    void steruj(float);
    void rysuj(sf::RenderWindow &);
    sf::Vector2f pobierzPozycje() const;

private:
    sf::RectangleShape ksztalt;
    float predkosc;
};

Gracz::Gracz() : predkosc(200.f)
{
    ksztalt.setSize(sf::Vector2f(50, 20));
    ksztalt.setFillColor(sf::Color::Green);
    ksztalt.setPosition(375, 550);
}

// obsługa wejścia
void Gracz::steruj(float deltaTime)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        ksztalt.move(-predkosc * deltaTime, 0);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        ksztalt.move(predkosc * deltaTime, 0);

    // ograniczenie ruchu w oknie
    if (ksztalt.getPosition().x < 0)
    {
        ksztalt.setPosition(0, ksztalt.getPosition().y);
    }
    if (ksztalt.getPosition().x > 750)
    {
        ksztalt.setPosition(750, ksztalt.getPosition().y);
    }
}

void Gracz::rysuj(sf::RenderWindow &window)
{
    window.draw(ksztalt);
}

sf::Vector2f Gracz::pobierzPozycje() const
{
    return ksztalt.getPosition();
}

class Wrog
{
public:
    Wrog(float x, float y);
    void rysuj(sf::RenderWindow &window);
    sf::Vector2f pobierzPozycje() const;
    void ustawPozycje(float x, float y);
    void przesun(float dx, float dy);

private:
    sf::RectangleShape ksztalt;
    float predkosc;
};

Wrog::Wrog(float x, float y) : predkosc(50.f)
{
    ksztalt.setSize(sf::Vector2f(40, 20));
    ksztalt.setFillColor(sf::Color::Red);
    ksztalt.setPosition(x, y);
}

void Wrog::rysuj(sf::RenderWindow &window)
{
    window.draw(ksztalt);
}

sf::Vector2f Wrog::pobierzPozycje() const
{
    return ksztalt.getPosition();
}

void Wrog::ustawPozycje(float x, float y)
{
    ksztalt.setPosition(x, y);
}

void Wrog::przesun(float dx, float dy)
{
    ksztalt.move(dx, dy);
}

class Pocisk
{
public:
    Pocisk(float x, float y);
    void ruszaj(float deltaTime);
    void rysuj(sf::RenderWindow &window);
    sf::FloatRect pobierzObszar() const;

private:
    sf::RectangleShape ksztalt;
    float predkosc;
};

Pocisk::Pocisk(float x, float y) : predkosc(-300.f)
{
    ksztalt.setSize(sf::Vector2f(5, 10));
    ksztalt.setFillColor(sf::Color::White);
    ksztalt.setPosition(x, y);
}

// ruch pocisków
void Pocisk::ruszaj(float deltaTime)
{
    ksztalt.move(0, predkosc * deltaTime);
}

void Pocisk::rysuj(sf::RenderWindow &window)
{
    window.draw(ksztalt);
}

sf::FloatRect Pocisk::pobierzObszar() const
{
    return ksztalt.getGlobalBounds();
}

class Punkty
{
private:
    int punkty;
    sf::Text tekstPunkty;

public:
    Punkty();
    void dodaj(int punkty);
    void rysuj(sf::RenderWindow &window, const sf::Font &czcionka);
    void ustawPunkty(int);
};

Punkty::Punkty() : punkty(0)
{
    // inicjalizacja tekstu
    tekstPunkty.setCharacterSize(30);
    tekstPunkty.setFillColor(sf::Color::Blue);
}

void Punkty::dodaj(int nowyPunkt)
{
    punkty += nowyPunkt;
}

void Punkty::rysuj(sf::RenderWindow &window, const sf::Font &czcionka)
{
    tekstPunkty.setFont(czcionka);
    tekstPunkty.setPosition(600, 20);
    tekstPunkty.setString("Punkty: " + std::to_string(punkty));
    window.draw(tekstPunkty);
}

void Punkty::ustawPunkty(int nowePunkty) 
{
    punkty = nowePunkty;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Space invaders");

    bool gra = false;

    bool przegrana = false;
    sf::Font czcionka;
    if (!czcionka.loadFromFile("Chunk Five Print.otf"))
    {
        std::cout << "Nie udalo sie zaladowac czcionki!" << std::endl;
        return 0;
    }
    Menu menu(800, 600, czcionka);

    sf::Text tekstPrzegrana;
    tekstPrzegrana.setFont(czcionka);
    tekstPrzegrana.setString("Przegrales! Wcisnij ESC, aby opuscic gre.");
    tekstPrzegrana.setCharacterSize(30);
    tekstPrzegrana.setFillColor(sf::Color::Red);
    tekstPrzegrana.setPosition(100, 250);

    Gracz gracz;
    std::vector<Wrog> wrogowie;
    std::vector<Pocisk> pociski;
    Punkty punkty;

    // Dodanie przeciwnikow
    int kolumny = 10;
    int wiersze = 4;
    float odstepK = 60.f;
    float odstepW = 40.f;

    // kierunek i szybkość ruchu przeciwników
    bool ruchWPrawo = true;     // czy grupa porusza sie w prawo
    float predkoscRuchu = 50.f; // predkosc ruchu w poziomie

    for (int rzad = 0; rzad < wiersze; ++rzad)
    {
        for (int kolumna = 0; kolumna < kolumny; ++kolumna)
        {
            float x = kolumna * odstepK + 20; // odstep miedzy kolumnami
            float y = rzad * odstepW + 80;    // odstep miedzy rzedami
            wrogowie.emplace_back(x, y);
        }
    }

    // czas do obliczania płynności ruchu
    sf::Clock clock;

    // do ograniczenie czasu miedzy kolejnymi strzalami
    float czasOdOstatniegoStrzalu = 0.f;
    float minimalnyCzasStrzalu = 0.3f;

    // aby naprawić blad zwiazany z wielokrotnym dotykaniem krawedzi
    bool dotknietoKrawedziWczesniej = false;

    // pętla gry
    while (window.isOpen())
    {
        // obsługa zdarzeń
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            //     window.close();
            if (gra)
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                {
                    gra = false;
                }
                // po nacisnieciu spacji wystrzal oraz czas miedzy strzalami = minimalnyCzasStrzalu
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && czasOdOstatniegoStrzalu >= minimalnyCzasStrzalu)
                {
                    pociski.emplace_back(gracz.pobierzPozycje().x + 22.5f, gracz.pobierzPozycje().y);
                    czasOdOstatniegoStrzalu = 0.0f;
                }
            }
            else
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                    menu.przesunWGore();
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                    menu.przesunWDol();
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
                {
                    int opcja = menu.pobierzWybranaOpcje();
                    if (opcja == 0)
                    {
                        gra = true;
                        // reset stanu gracza
                        gracz = Gracz();
                        wrogowie.clear();
                        pociski.clear();
                        punkty.ustawPunkty(0);

                        for (int rzad = 0; rzad < wiersze; ++rzad)
                        {
                            for (int kolumna = 0; kolumna < kolumny; ++kolumna)
                            {
                                float x = kolumna * odstepK + 20;
                                float y = rzad * odstepW + 80;
                                wrogowie.emplace_back(x, y);
                            }
                        }
                        clock.restart();
                        czasOdOstatniegoStrzalu = 0.f;
                        dotknietoKrawedziWczesniej = false;
                        przegrana = false;
                    }
                    else if (opcja == 1)
                    {
                    }
                    else if (opcja == 2)
                    {
                        window.close();
                    }
                }
            }
        }
        // aktualizacja stanu gry
        if (gra)
        {
            sf::Time deltaTime = clock.restart();
            gracz.steruj(deltaTime.asSeconds());

            czasOdOstatniegoStrzalu += deltaTime.asSeconds();

            bool dotknietoKrawedzi = false;
            for (auto &wrog : wrogowie)
            {
                // ruch w poziomie
                if (ruchWPrawo)
                {
                    wrog.przesun(predkoscRuchu * deltaTime.asSeconds(), 0);
                }
                else
                {
                    wrog.przesun(-predkoscRuchu * deltaTime.asSeconds(), 0);
                }

                // sprawdzenie czy wrog dotknal krawedzi ekranu
                if (!dotknietoKrawedzi && (wrog.pobierzPozycje().x <= 0 || wrog.pobierzPozycje().x + 40 >= 800))
                {
                    dotknietoKrawedzi = true; // zmiana kierunku
                }

                // std::cout<<"Pozycja wroga Y: "<<wrog.pobierzPozycje().y<<std::endl;

                // sprawdzenie czy ktorys wrog dotarl do dolnej krawedzi
                if (wrog.pobierzPozycje().y + 20 >= 600)
                {
                    przegrana = true;
                }
            }

            // jesli dotykają krawędzi to zmiana kierunku
            if (dotknietoKrawedzi && !dotknietoKrawedziWczesniej)
            {
                ruchWPrawo = !ruchWPrawo; // zmiana kierunku
                for (auto &wrog : wrogowie)
                {
                    wrog.przesun(0, odstepW); // przesun wrogow w dol
                }
                dotknietoKrawedziWczesniej = true;
            }
            else if (!dotknietoKrawedzi)
            {
                dotknietoKrawedziWczesniej = false;
            }

            // komunikat o przegranej
            if (przegrana)
            {
                window.clear();
                window.draw(tekstPrzegrana);
                window.display();

                while (true)
                {
                    sf::Event event;
                    while (window.pollEvent(event))
                    {
                        if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                        {
                            window.close();
                            break;
                        }
                    }
                    if (!window.isOpen())
                        break;
                }
            }

            for (auto it = pociski.begin(); it != pociski.end();)
            {
                it->ruszaj(deltaTime.asSeconds());

                // usuwanie pociskow ktore wyszly poza zakres
                if (it->pobierzObszar().top + it->pobierzObszar().height < 0)
                {
                    it = pociski.erase(it);
                }
                else
                {
                    ++it;
                }
            }

            // Sprawdzanie kolizji między pociskami a przeciwnikami
            for (auto wrogIt = wrogowie.begin(); wrogIt != wrogowie.end();)
            {
                bool wrogZniszczony = false;

                for (auto pociskIt = pociski.begin(); pociskIt != pociski.end();)
                {
                    if (pociskIt->pobierzObszar().intersects(sf::FloatRect(wrogIt->pobierzPozycje(), sf::Vector2f(40, 20))))
                    {
                        // Jeśli kolizja, usuń pocisk i oznacz przeciwnika do usunięcia
                        pociskIt = pociski.erase(pociskIt);
                        wrogZniszczony = true;
                        break;
                    }
                    else
                    {
                        ++pociskIt;
                    }
                }

                // Usuń przeciwnika, jeśli został trafiony
                if (wrogZniszczony)
                {
                    wrogIt = wrogowie.erase(wrogIt);
                    punkty.dodaj(10);
                }
                else
                {
                    ++wrogIt;
                }
            }
        }

        // renderowanie
        window.clear();
        if (gra)
        {
            gracz.rysuj(window);

            for (auto &wrog : wrogowie)
            {
                wrog.rysuj(window);
            }

            for (auto &pocisk : pociski)
            {
                pocisk.rysuj(window);
            }
            punkty.rysuj(window, czcionka);
        }
        else
        {
            menu.rysuj(window);
        }
        window.display();
    }

    return 0;
}
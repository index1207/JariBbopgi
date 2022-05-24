#include <SFML/Graphics.hpp>
#include <windows.h>

#include <iostream>
#include <numeric>
#include <random>
#include <thread>
#include <fstream>
#include <ostream>

using namespace sf;
using namespace std;

class Desk {
public:
    Desk(String name, Vector2f size) noexcept {
        font.loadFromFile("./resource/fonts/noto.otf");

        desk.setSize(size);
        desk.setOutlineThickness(3);
        desk.setOutlineColor(Color::Black);
        desk.setPosition({0,0});

        txt.setFillColor(Color::Black);
        txt.setFont(font);
        txt.setString(name);
        txt.setCharacterSize(24);
        txt.setPosition({0,0});
    }
    void setTextPosition(Vector2f pos) {
        txt.setPosition(pos);
    }
    void setPosition(Vector2f pos) {
        txt.setPosition({pos.x + 26, pos.y + 20});
        desk.setPosition(pos);
    }
    void draw(RenderWindow& w) {
        w.draw(desk);
        w.draw(txt);
    }
private:
    Font font;
    RectangleShape desk;
    Text txt;
};

void readList(std::map<int, String>& m) {
    wfstream f;
    f.open("resource/list.txt");

    int i = 0;
    if(f.is_open()) {
        while(!f.eof()) {
            wchar_t s[16];
            f.getline(s, 16);

            m.insert({++i, String(s)});
        }
    }
}

int main() {
    setlocale(LC_ALL, "Korean");

    HWND hWnd = GetConsoleWindow();

    char ans;
    cout << "Would you like to use your seat option? (Y/N)\n>> ";
    cin >> ans;
    if(islower(ans)) ans = toupper(ans);

    int ori;
    int resv;
    bool isSel = false;
    if(ans == 'Y') {
        isSel = true;
        cout << "Input your number\n>> ";
        cin >> ori;

        cout << "    [    List    ]\n";
        for(int i = 1; i <= 18; ++i) {
            printf("%3d", i);
            if(!(i % 2)) {
                cout << "  ";
            }
            if(i % 6==0) {
                printf("\n");
            }
        }
        cout << "\nInput number to choose.\n>> ";
        cin >> resv;
    }
    else {
        ShowWindow(hWnd, HIDE_WINDOW);
    }

    std::map<int, String> name;
    readList(name);

    std::array<unsigned, 18> number;
    iota(number.begin(), number.end(), 1);

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(number.begin(), number.end(), g);

    if (isSel) {
        int i = 0;
        for (; i < 18; ++i) {
            if (number[i] == ori) {
                break;
            }
        }

        swap(number[i], number[resv - 1]);
        cout << "\n\nSELECTED!!";
        Sleep(500);

        ShowWindow(hWnd, HIDE_WINDOW);
    }
    sf::ContextSettings setting;
    setting.antialiasingLevel = 8;
    RenderWindow app(VideoMode(840, 540), L"자리뽑기", Style::Default, setting);
    app.setFramerateLimit(60);

    sf::Texture tex;
    tex.create(app.getSize().x, app.getSize().y);

    Texture logo;
    logo.loadFromFile("resource/gsm.png");

    Sprite s_logo(logo);
    s_logo.setScale({0.1f, 0.1f});
    s_logo.setPosition({30,10});

    Font cf;
    cf.loadFromFile("resource/fonts/noto.otf");

    Text ct;
    ct.setFont(cf);
    ct.setCharacterSize(36);
    ct.setFillColor(Color::Black);
    ct.setPosition({385, 20});
    ct.setString("1 - 2");

    Desk gyotak(L"교 탁", {243, 70});
    gyotak.setPosition({297, 420});
    gyotak.setTextPosition({393, 440});

    while(app.isOpen()) {
        Event e;
        while(app.pollEvent(e)) {
            if(e.type == Event::Closed) {
                app.close();
            }
            else if(Keyboard::isKeyPressed(Keyboard::Enter)) {
                tex.update(app);
                sf::Image img = tex.copyToImage();

                std::thread t([img]() {
                    img.saveToFile("Jari.png");
                });
                t.detach();
            }
        }
        app.clear(Color::White);

        Vector2f pos = { 25, 90 };
        for(int i = 0; i < 18; ++i) {
            Desk d(name[number[i]], {120, 70});
            if(i > 0 && i % 6 == 0) {
                pos.y += 100;
                pos.x = 25;
            }
            else if(i != 0) {
                pos.x += 120;
                if(i % 2 == 0) {
                    pos.x += 30;
                }
                else {
                    pos.x += 3;
                }
            }
            d.setPosition(pos);
            gyotak.draw(app);
            d.draw(app);
        }
        app.draw(s_logo);
        app.draw(ct);
        app.display();
    }
    return 0;
}
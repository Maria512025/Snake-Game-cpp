/*
 * ========================================
 *   SNAKE GAME - C++ OOP Implementation
 *   Author: Maria Ramzan
 *   Data Structures: Linked List (Snake Body)
 *   Concepts: OOP, Collision Detection, Game Loop
 * ========================================
 */

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <conio.h>    // Windows: _kbhit(), _getch()
#include <windows.h>  // Windows: Sleep(), system()

using namespace std;

// ─────────────────────────────────────────
// Constants
// ─────────────────────────────────────────
const int WIDTH  = 40;
const int HEIGHT = 20;
const int INITIAL_SPEED = 150; // milliseconds

// ─────────────────────────────────────────
// Direction Enum
// ─────────────────────────────────────────
enum Direction { UP, DOWN, LEFT, RIGHT, NONE };

// ─────────────────────────────────────────
// Node — Linked List node for snake body
// ─────────────────────────────────────────
struct Node {
    int x, y;
    Node* next;

    Node(int x, int y) : x(x), y(y), next(nullptr) {}
};

// ─────────────────────────────────────────
// Food Class
// ─────────────────────────────────────────
class Food {
public:
    int x, y;

    Food() { spawn(); }

    void spawn() {
        x = rand() % (WIDTH  - 2) + 1;
        y = rand() % (HEIGHT - 2) + 1;
    }

    void draw() const {
        setCursor(x, y);
        cout << "\033[31m@\033[0m"; // Red food
    }

    static void setCursor(int x, int y) {
        COORD coord = { (SHORT)x, (SHORT)y };
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }
};

// ─────────────────────────────────────────
// Snake Class — uses Linked List
// ─────────────────────────────────────────
class Snake {
private:
    Node* head;
    Node* tail;
    int   length;
    Direction dir;

public:
    Snake(int startX, int startY) {
        head   = new Node(startX, startY);
        tail   = head;
        length = 1;
        dir    = RIGHT;
    }

    ~Snake() {
        Node* curr = head;
        while (curr) {
            Node* temp = curr;
            curr = curr->next;
            delete temp;
        }
    }

    Direction getDirection() const { return dir; }
    int       getLength()    const { return length; }
    int       headX()        const { return head->x; }
    int       headY()        const { return head->y; }

    void setDirection(Direction newDir) {
        // Prevent 180-degree turn
        if ((dir == UP    && newDir == DOWN)  ||
            (dir == DOWN  && newDir == UP)    ||
            (dir == LEFT  && newDir == RIGHT) ||
            (dir == RIGHT && newDir == LEFT)) return;
        dir = newDir;
    }

    // Move snake: add new head, remove tail (unless growing)
    bool move(bool grow) {
        int nx = head->x;
        int ny = head->y;

        switch (dir) {
            case UP:    ny--; break;
            case DOWN:  ny++; break;
            case LEFT:  nx--; break;
            case RIGHT: nx++; break;
            default: break;
        }

        // Wall collision
        if (nx <= 0 || nx >= WIDTH - 1 || ny <= 0 || ny >= HEIGHT - 1)
            return false;

        // Self collision
        if (checkSelfCollision(nx, ny))
            return false;

        // Add new head
        Node* newHead = new Node(nx, ny);
        newHead->next = head;
        head = newHead;
        length++;

        if (!grow) {
            // Remove tail
            Node* curr = head;
            while (curr->next != tail) curr = curr->next;

            // Erase tail from screen
            Food::setCursor(tail->x, tail->y);
            cout << ' ';

            delete tail;
            tail = curr;
            tail->next = nullptr;
            length--;
        }

        return true;
    }

    bool checkSelfCollision(int nx, int ny) const {
        Node* curr = head;
        while (curr) {
            if (curr->x == nx && curr->y == ny) return true;
            curr = curr->next;
        }
        return false;
    }

    void draw() const {
        Node* curr = head;
        bool  isHead = true;
        while (curr) {
            Food::setCursor(curr->x, curr->y);
            if (isHead) {
                cout << "\033[32mO\033[0m"; // Green head
                isHead = false;
            } else {
                cout << "\033[92mo\033[0m"; // Light green body
            }
            curr = curr->next;
        }
    }
};

// ─────────────────────────────────────────
// Game Class
// ─────────────────────────────────────────
class Game {
private:
    Snake*  snake;
    Food*   food;
    int     score;
    int     highScore;
    int     level;
    int     speed;
    bool    running;

    void hideCursor() {
        CONSOLE_CURSOR_INFO ci = { 1, FALSE };
        SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ci);
    }

    void drawBorder() {
        system("cls");
        // Top & Bottom walls
        for (int x = 0; x < WIDTH; x++) {
            Food::setCursor(x, 0);        cout << "\033[33m#\033[0m";
            Food::setCursor(x, HEIGHT-1); cout << "\033[33m#\033[0m";
        }
        // Left & Right walls
        for (int y = 1; y < HEIGHT - 1; y++) {
            Food::setCursor(0,        y); cout << "\033[33m#\033[0m";
            Food::setCursor(WIDTH-1,  y); cout << "\033[33m#\033[0m";
        }
    }

    void drawHUD() {
        Food::setCursor(0, HEIGHT + 1);
        cout << "\033[36m Score: \033[97m" << score
             << "  \033[36mLevel: \033[97m" << level
             << "  \033[36mHigh Score: \033[97m" << highScore
             << "  \033[36mLength: \033[97m" << snake->getLength()
             << "  \033[90m[WASD/Arrows to move | Q to quit]\033[0m   ";
    }

    void processInput() {
        if (!_kbhit()) return;
        int key = _getch();

        if (key == 0 || key == 224) { // Arrow keys
            key = _getch();
            switch (key) {
                case 72: snake->setDirection(UP);    break;
                case 80: snake->setDirection(DOWN);  break;
                case 75: snake->setDirection(LEFT);  break;
                case 77: snake->setDirection(RIGHT); break;
            }
        } else {
            switch (tolower(key)) {
                case 'w': snake->setDirection(UP);    break;
                case 's': snake->setDirection(DOWN);  break;
                case 'a': snake->setDirection(LEFT);  break;
                case 'd': snake->setDirection(RIGHT); break;
                case 'q': running = false;             break;
            }
        }
    }

    void updateLevel() {
        int newLevel = score / 50 + 1;
        if (newLevel != level) {
            level = newLevel;
            speed = max(60, INITIAL_SPEED - (level - 1) * 15);
        }
    }

public:
    Game() : score(0), highScore(0), level(1), speed(INITIAL_SPEED), running(true) {
        srand((unsigned)time(nullptr));
        snake = new Snake(WIDTH / 2, HEIGHT / 2);
        food  = new Food();
    }

    ~Game() {
        delete snake;
        delete food;
    }

    void run() {
        hideCursor();
        drawBorder();

        while (running) {
            processInput();

            // Check if ate food
            bool ate = (snake->headX() == food->x && snake->headY() == food->y);

            if (!snake->move(ate)) {
                gameOver();
                return;
            }

            if (ate) {
                score += 10;
                if (score > highScore) highScore = score;
                food->spawn();
                food->draw();
                updateLevel();
            }

            snake->draw();
            food->draw();
            drawHUD();

            Sleep(speed);
        }

        gameOver();
    }

    void gameOver() {
        system("cls");
        cout << "\n\n";
        cout << "  ╔══════════════════════════════════╗\n";
        cout << "  ║         \033[31mGAME OVER!\033[0m              ║\n";
        cout << "  ╠══════════════════════════════════╣\n";
        cout << "  ║  Final Score  : \033[97m" << score      << "\033[0m\n";
        cout << "  ║  High Score   : \033[93m" << highScore  << "\033[0m\n";
        cout << "  ║  Level Reached: \033[96m" << level      << "\033[0m\n";
        cout << "  ║  Snake Length : \033[92m" << snake->getLength() << "\033[0m\n";
        cout << "  ╠══════════════════════════════════╣\n";
        cout << "  ║  Press R to Restart | Q to Quit  ║\n";
        cout << "  ╚══════════════════════════════════╝\n\n";

        while (true) {
            char ch = _getch();
            if (tolower(ch) == 'r') {
                delete snake;
                delete food;
                score   = 0;
                level   = 1;
                speed   = INITIAL_SPEED;
                running = true;
                snake   = new Snake(WIDTH / 2, HEIGHT / 2);
                food    = new Food();
                drawBorder();
                run();
                return;
            }
            if (tolower(ch) == 'q') return;
        }
    }
};

// ─────────────────────────────────────────
// Main
// ─────────────────────────────────────────
int main() {
    // Title screen
    system("cls");
    cout << "\n\n";
    cout << "  ╔══════════════════════════════════════╗\n";
    cout << "  ║   \033[32m  ___  _  _   ___  _  _  ____   \033[0m  ║\n";
    cout << "  ║   \033[32m / __|| \\| | / _ \\| |/ /| ___| \033[0m  ║\n";
    cout << "  ║   \033[32m \\__ \\| .  || (_) | ' < | _|   \033[0m  ║\n";
    cout << "  ║   \033[32m |___/|_|\\_| \\__\\_\\_|\\_\\|___| \033[0m  ║\n";
    cout << "  ╠══════════════════════════════════════╣\n";
    cout << "  ║  Controls: WASD or Arrow Keys        ║\n";
    cout << "  ║  Eat \033[31m@\033[0m to grow | Avoid walls & self ║\n";
    cout << "  ║  Speed increases every 50 points     ║\n";
    cout << "  ╠══════════════════════════════════════╣\n";
    cout << "  ║       Press ENTER to Start...        ║\n";
    cout << "  ╚══════════════════════════════════════╝\n\n";

    cin.get();

    Game game;
    game.run();

    return 0;
}

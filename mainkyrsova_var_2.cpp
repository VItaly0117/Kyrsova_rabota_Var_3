#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>
#include <thread>      // Для создания задержки (sleep)
#include <chrono>      // Для задания длительности задержки

using namespace std;

class MemoryGame {
private:
    vector<vector<int>> board;         // Поле с картами
    vector<vector<bool>> revealed;     // Статус карт (открыты/закрыты)
    int rows, cols;                    // Размеры поля
    int moves;                         // Количество ходов
    time_t startTime;                  // Время начала игры
    int lives;                         // Количество жизней

    // Генерация случайного размещения карт
    void shuffleBoard() {
        vector<int> cards;
        for (int i = 0; i < (rows * cols) / 2; ++i) {
            cards.push_back(i);
            cards.push_back(i);
        }
        random_device rd;
        mt19937 g(rd());
        shuffle(cards.begin(), cards.end(), g);

        int index = 0;
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                board[i][j] = cards[index++];
            }
        }
    }

public:
    MemoryGame(int r, int c, int l) : rows(r), cols(c), moves(0), lives(l) {
        board.resize(rows, vector<int>(cols));
        revealed.resize(rows, vector<bool>(cols, false));
        shuffleBoard();
        startTime = time(0);
    }

    // Отображение поля на экране
    void printBoard() {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (revealed[i][j]) {
                    cout << board[i][j] << " ";
                } else {
                    cout << "* ";
                }
            }
            cout << endl;
        }
    }

    // Эффект открытия карты
    void revealEffect(int r, int c) {
        cout << "Открытие карты на позиции (" << r + 1 << ", " << c + 1 << ")..." << endl;
        revealed[r][c] = true;
        printBoard();
        this_thread::sleep_for(chrono::milliseconds(500));  // Задержка в 500 мс для имитации анимации
    }

    // Открытие двух карт
    bool revealCards(int r1, int c1, int r2, int c2) {
        moves++;
        revealEffect(r1, c1);  // Показ первой карты с эффектом
        revealEffect(r2, c2);  // Показ второй карты с эффектом

        if (board[r1][c1] == board[r2][c2]) {
            return true;
        } else {
            revealed[r1][c1] = false;
            revealed[r2][c2] = false;
            lives--;
            return false;
        }
    }

    // Основной игровой процесс
    void play() {
        int r1, c1, r2, c2;
        bool gameOver = false;

        while (!gameOver && lives > 0) {
            system("cls");
            printBoard();
            cout << "Осталось жизней: " << lives << endl;

            cout << "Введите координаты первой карты (строка и столбец, начиная с 1): ";
            cin >> r1 >> c1;
            cout << "Введите координаты второй карты (строка и столбец, начиная с 1): ";
            cin >> r2 >> c2;

            r1--; c1--;
            r2--; c2--;

            if (revealCards(r1, c1, r2, c2)) {
                cout << "Карты совпали! Вы можете играть дальше." << endl;
            } else {
                cout << "Карты не совпали. Попробуйте снова." << endl;
                this_thread::sleep_for(chrono::milliseconds(1000));  // Короткая пауза перед продолжением
            }

            gameOver = true;
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    if (!revealed[i][j]) {
                        gameOver = false;
                    }
                }
            }
        }

        time_t endTime = time(0);
        int gameTime = difftime(endTime, startTime);

        if (lives == 0) {
            cout << "Игра окончена! У вас закончились жизни." << endl;
        } else {
            cout << "Игра окончена! Вы нашли все пары." << endl;
        }

        cout << "Время игры: " << gameTime << " секунд." << endl;
        cout << "Количество переворотов карт: " << moves << endl;
    }
};

int main() {
    srand(time(0));
    int rows = 4, cols = 4;
    int levelChoice;
    int lives;

    cout << "Выберите уровень сложности (1 - Лёгкий, 2 - Средний, 3 - Сложный): ";
    cin >> levelChoice;

    if (levelChoice == 1) {
        lives = 10;
    } else if (levelChoice == 2) {
        lives = 6;
    } else {
        lives = 3;
    }

    MemoryGame game(rows, cols, lives);
    game.play();

    return 0;
}

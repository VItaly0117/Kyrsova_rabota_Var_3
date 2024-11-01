#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>  // Для std::shuffle
#include <random>     // Для генератора случайных чисел

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
            cards.push_back(i);         // Создаём пары карт
            cards.push_back(i);
        }

        // Используем std::shuffle с генератором случайных чисел
        random_device rd;               // Источник случайных чисел
        mt19937 g(rd());                // Генератор случайных чисел
        shuffle(cards.begin(), cards.end(), g);  // Перемешиваем карты

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
        startTime = time(0); // Запускаем таймер
    }

    // Отображение поля на экране
    void printBoard() {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (revealed[i][j]) {
                    cout << board[i][j] << " ";  // Показываем карту, если она открыта
                } else {
                    cout << "* ";               // Закрытая карта
                }
            }
            cout << endl;
        }
    }

    // Открытие двух карт
    bool revealCards(int r1, int c1, int r2, int c2) {
        moves++;  // Увеличиваем счётчик ходов
        if (board[r1][c1] == board[r2][c2]) {
            revealed[r1][c1] = true;
            revealed[r2][c2] = true;
            return true;  // Карты совпали
        } else {
            lives--;  // Уменьшаем количество жизней при несовпадении
            return false;  // Карты не совпали
        }
    }

    // Основной игровой процесс
    void play() {
        int r1, c1, r2, c2;
        bool gameOver = false;

        while (!gameOver && lives > 0) {
            system("cls");  // Очистка экрана
            printBoard();   // Вывод текущего состояния поля
            cout << "Осталось жизней: " << lives << endl;

            cout << "Введите координаты первой карты (строка и столбец, начиная с 1): ";
            cin >> r1 >> c1;
            cout << "Введите координаты второй карты (строка и столбец, начиная с 1): ";
            cin >> r2 >> c2;

            // Корректируем индексы, так как пользователь вводит их с 1, а не с 0
            r1--; c1--;
            r2--; c2--;

            if (revealCards(r1, c1, r2, c2)) {
                cout << "Карты совпали! Вы можете играть дальше." << endl;
            } else {
                cout << "Карты не совпали. Попробуйте снова." << endl;
            }
            
            // Проверяем, остались ли ещё закрытые карты
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
        int gameTime = difftime(endTime, startTime);  // Подсчёт времени игры

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
    srand(time(0));  // Инициализация генератора случайных чисел
    int rows = 4, cols = 4;  // Поле 4x4 по умолчанию
    int levelChoice;
    int lives;

    cout << "Выберите уровень сложности (1 - Лёгкий, 2 - Средний, 3 - Сложный): ";
    cin >> levelChoice;

    if (levelChoice == 1) {
        lives = 10;  // Лёгкий уровень
    } else if (levelChoice == 2) {
        lives = 6;   // Средний уровень
    } else {
        lives = 3;   // Сложный уровень
    }

    MemoryGame game(rows, cols, lives);
    game.play();

    return 0;
}

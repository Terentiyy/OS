#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

// Функция для генерации случайного числа в диапазоне [min, max]
int rand_range(int min, int max) {
    return min + rand() % (max - min + 1);
}

// Структура для передачи параметров каждому потоку
typedef struct {
    int *arrays[K]; // Указатели на массивы
    int start_index; // Начальный индекс обработки
    int end_index;   // Конечный индекс обработки
    int result_idx;  // Индекс результирующего массива
} thread_args_t;

// Функция потока
void* sum_arrays(void* args) {
    thread_args_t* targs = (thread_args_t*) args;
    
    for (int i = targs->start_index; i <= targs->end_index; ++i) {
        for (int j = 0; j < K; ++j) {
            targs->arrays[targs->result_idx][i] += targs->arrays[j][i];
        }
    }
    
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s num_threads array_length\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));

    const int num_threads = atoi(argv[1]); // Количество потоков
    const int array_len = atoi(argv[2]);   // Длина одного массива

    // Проверяем корректность ввода
    if (num_threads <= 0 || array_len <= 0) {
        fprintf(stderr, "Invalid input: number of threads or array length must be positive.\n");
        exit(EXIT_FAILURE);
    }

    // Массивы для хранения значений
    int arrays[K][array_len];
    memset(arrays, 0, sizeof(arrays)); // Обнуляем массивы

    // Генерация случайных чисел для массивов
    for (int i = 0; i < K; ++i) {
        for (int j = 0; j < array_len; ++j) {
            arrays[i][j] = rand_range(-100, 100); // Случайные значения от -100 до 100
        }
    }

    // Результирующий массив
    int result[array_len];
    memset(result, 0, sizeof(result));

    // Распределяем работу между потоками
    pthread_t threads[num_threads];
    thread_args_t thread_args[num_threads];

    int chunk_size = array_len / num_threads; // Размер порции для каждого потока
    int remainder = array_len % num_threads;  // Остаток

    int current_start = 0;
    for (int i = 0; i < num_threads; ++i) {
        thread_args[i].start_index = current_start;
        thread_args[i].end_index = current_start + chunk_size - 1;
        
        if (remainder > 0) {
            thread_args[i].end_index++;
            remainder--;
        }

        current_start = thread_args[i].end_index + 1;

        for (int j = 0; j < K; ++j) {
            thread_args[i].arrays[j] = arrays[j];
        }
        thread_args[i].result_idx = i;

        // Запускаем поток
        int rc = pthread_create(&threads[i], NULL, sum_arrays, &thread_args[i]);
        if (rc) {
            fprintf(stderr, "Error creating thread: %d\n", rc);
            exit(EXIT_FAILURE);
        }
    }

    // Ожидаем завершение всех потоков
    for (int i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], NULL);
    }

    // Суммируем результаты
    for (int i = 0; i < num_threads; ++i) {
        memcpy(result, thread_args[i].arrays[thread_args[i].result_idx], sizeof(result));
    }

    // Выводим результат
    printf("Result:\n");
    for (int i = 0; i < array_len; ++i) {
        printf("%d ", result[i]);
    }
    printf("\n");

    return 0;
}
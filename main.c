#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

// === РАЗРАБОТЧИК АЛГОРИТМОВ: Трошин Олег ===
// === РАЗРАБОТЧИК ИНТЕРФЕЙСА: Ковтун Алексей ===
// === ТЕСТИРОВЩИК: Долгов Лев ===
// Версия: 1.4 (29.06.2026) - добавлена обработка ошибок

// Глобальные счётчики для статистики
long long comparison_count = 0;
long long swap_count = 0;

// Функция сортировки Шелла
void shell_sort(int arr[], int n) {
    comparison_count = 0;
    swap_count = 0;
    
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            int temp = arr[i];
            int j;
            for (j = i; j >= gap; j -= gap) {
                comparison_count++;
                if (arr[j - gap] > temp) {
                    arr[j] = arr[j - gap];
                    swap_count++;
                } else {
                    break;
                }
            }
            arr[j] = temp;
        }
    }
}

// Функция вывода массива
void print_array(int arr[], int n, int max_show) {
    int show = (n < max_show) ? n : max_show;
    for (int i = 0; i < show; i++) {
        printf("%7d ", arr[i]);
        if ((i + 1) % 10 == 0) printf("\n");
    }
    if (n > max_show) {
        printf("\n... и еще %d элементов\n", n - max_show);
    }
    printf("\n");
}

// Функция генерации случайного массива
void generate_random_array(int arr[], int n) {
    srand((unsigned int)time(NULL));
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 2001 - 1000;
    }
}

// Функция сохранения массива в файл
void save_array_to_file(int arr[], int n, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Ошибка: не удалось открыть файл %s для записи!\n", filename);
        return;
    }
    for (int i = 0; i < n; i++) {
        fprintf(file, "%d", arr[i]);
        if (i < n - 1) fprintf(file, ", ");
        if ((i + 1) % 20 == 0) fprintf(file, "\n");
    }
    fclose(file);
    printf("Массив сохранён в файл: %s\n", filename);
}

// Функция загрузки массива из файла
int load_array_from_file(int arr[], int max_size, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Ошибка: не удалось открыть файл %s для чтения!\n", filename);
        return 0;
    }
    int count = 0;
    int value;
    while (fscanf(file, "%d", &value) == 1 && count < max_size) {
        arr[count++] = value;
        int ch = fgetc(file);
        while (ch == ',' || ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t') {
            ch = fgetc(file);
        }
        if (ch != EOF) ungetc(ch, file);
    }
    fclose(file);
    return count;
}

// Генерация отсортированного массива (лучший случай)
void generate_sorted_array(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = i + 1;
    }
}

// Генерация инвертированного массива (худший случай)
void generate_reverse_array(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = n - i;
    }
}

// Функция сравнения для qsort
int cmp_int(const void* a, const void* b) {
    int va = *(const int*)a;
    int vb = *(const int*)b;
    return (va > vb) - (va < vb);
}

// Функция автоматического тестирования
void run_tests() {
    printf("\n========================================\n");
    printf("   ЗАПУСК ТЕСТОВ\n");
    printf("========================================\n\n");
    
    int test_sizes[] = { 100, 500, 1000, 5000, 10000 };
    int num_tests = 5;
    
    FILE* report = fopen("test_results.csv", "w");
    if (report == NULL) {
        printf("Ошибка: не удалось создать файл отчёта!\n");
        return;
    }
    
    fprintf(report, "Размер,Тип данных,Сравнений,Перестановок,Время(сек)\n");
    
    printf("%-10s | %-15s | %-15s | %-15s | %-15s\n",
        "Размер", "Тип данных", "Сравнений", "Перестановок", "Время(сек)");
    printf("----------+-----------------+-----------------+-----------------+-----------------\n");
    
    for (int t = 0; t < num_tests; t++) {
        int n = test_sizes[t];
        int* arr = (int*)malloc(n * sizeof(int));
        if (arr == NULL) {
            printf("Ошибка выделения памяти для n=%d\n", n);
            continue;
        }
        
        generate_sorted_array(arr, n);
        clock_t start = clock();
        shell_sort(arr, n);
        clock_t end = clock();
        double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
        fprintf(report, "%d,Отсортированный,%lld,%lld,%.6f\n",
            n, comparison_count, swap_count, time_spent);
        printf("%-10d | %-15s | %-15lld | %-15lld | %-15.6f\n",
            n, "Отсортированный", comparison_count, swap_count, time_spent);
        
        generate_random_array(arr, n);
        start = clock();
        shell_sort(arr, n);
        end = clock();
        time_spent = (double)(end - start) / CLOCKS_PER_SEC;
        fprintf(report, "%d,Случайный,%lld,%lld,%.6f\n",
            n, comparison_count, swap_count, time_spent);
        printf("%-10d | %-15s | %-15lld | %-15lld | %-15.6f\n",
            n, "Случайный", comparison_count, swap_count, time_spent);
        
        generate_reverse_array(arr, n);
        start = clock();
        shell_sort(arr, n);
        end = clock();
        time_spent = (double)(end - start) / CLOCKS_PER_SEC;
        fprintf(report, "%d,Инвертированный,%lld,%lld,%.6f\n",
            n, comparison_count, swap_count, time_spent);
        printf("%-10d | %-15s | %-15lld | %-15lld | %-15.6f\n",
            n, "Инвертированный", comparison_count, swap_count, time_spent);
        
        printf("----------+-----------------+-----------------+-----------------+-----------------\n");
        free(arr);
    }
    
    fclose(report);
    printf("\nРезультаты тестов сохранены в файл: test_results.csv\n");
    printf("========================================\n");
}

// ============================================================
// ГЛАВНАЯ ФУНКЦИЯ (С ОБРАБОТКОЙ ОШИБОК)
// ============================================================

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    
    int choice = 0;
    int size = 0;
    int* arr = NULL;
    
    printf("========================================\n");
    printf("   ПРОГРАММА: СОРТИРОВКА ШЕЛЛА\n");
    printf("   (Shell Sort Algorithm)\n");
    printf("========================================\n\n");
    
    do {
        printf("\n--- МЕНЮ ---\n");
        printf("1. Сгенерировать случайный массив\n");
        printf("2. Загрузить массив из файла\n");
        printf("3. Выполнить сортировку\n");
        printf("4. Показать массив\n");
        printf("5. Сохранить массив в файл\n");
        printf("6. Запустить тесты\n");
        printf("0. Выход\n");
        printf("\nВыберите действие: ");
        
        // ===== ОБРАБОТКА ОШИБОК ВВОДА =====
        if (scanf("%d", &choice) != 1) {
            printf("Ошибка: введён некорректный символ!\n");
            while (getchar() != '\n');
            continue;
        }
        
        switch (choice) {
            case 1: {
                printf("Введите размер массива: ");
                // ===== ПРОВЕРКА РАЗМЕРА =====
                if (scanf("%d", &size) != 1) {
                    printf("Ошибка: введён некорректный символ!\n");
                    while (getchar() != '\n');
                    break;
                }
                if (size < 2) {
                    printf("Ошибка: размер должен быть больше 1!\n");
                    break;
                }
                // ===== ОСВОБОЖДЕНИЕ ПАМЯТИ =====
                if (arr != NULL) {
                    free(arr);
                }
                arr = (int*)malloc(size * sizeof(int));
                if (arr == NULL) {
                    printf("Ошибка выделения памяти!\n");
                    break;
                }
                generate_random_array(arr, size);
                printf("Сгенерирован массив из %d элементов\n", size);
                break;
            }
            
            case 2: {
                char filename[256];
                printf("Введите имя файла (например, numbers.txt): ");
                scanf("%s", filename);
                int* temp_arr = (int*)malloc(10000 * sizeof(int));
                if (temp_arr == NULL) {
                    printf("Ошибка выделения памяти!\n");
                    break;
                }
                int loaded = load_array_from_file(temp_arr, 10000, filename);
                if (loaded > 0) {
                    if (arr != NULL) {
                        free(arr);
                    }
                    arr = temp_arr;
                    size = loaded;
                    printf("Загружено %d элементов из файла %s\n", size, filename);
                } else {
                    free(temp_arr);
                    printf("Файл пуст или содержит некорректные данные!\n");
                }
                break;
            }
            
            case 3: {
                if (arr == NULL) {
                    printf("Ошибка: сначала создайте или загрузите массив!\n");
                    break;
                }
                printf("Сортировка массива из %d элементов...\n", size);
                
                int* sorted_arr = (int*)malloc(size * sizeof(int));
                for (int i = 0; i < size; i++) {
                    sorted_arr[i] = arr[i];
                }
                
                clock_t start = clock();
                shell_sort(sorted_arr, size);
                clock_t end = clock();
                double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
                
                printf("\n=== РЕЗУЛЬТАТЫ СОРТИРОВКИ ===\n");
                printf("Время выполнения: %.6f секунд\n", time_spent);
                printf("Количество сравнений: %lld\n", comparison_count);
                printf("Количество перестановок: %lld\n", swap_count);
                
                printf("\nОтсортированный массив (первые 20 элементов):\n");
                print_array(sorted_arr, size, 20);
                
                save_array_to_file(sorted_arr, size, "sorted.txt");
                free(sorted_arr);
                break;
            }
            
            case 4: {
                if (arr == NULL) {
                    printf("Ошибка: массив пуст!\n");
                    break;
                }
                printf("Массив из %d элементов:\n", size);
                print_array(arr, size, 50);
                break;
            }
            
            case 5: {
                if (arr == NULL) {
                    printf("Ошибка: массив пуст!\n");
                    break;
                }
                char filename[256];
                printf("Введите имя файла для сохранения: ");
                scanf("%s", filename);
                save_array_to_file(arr, size, filename);
                break;
            }
            
            case 6: {
                run_tests();
                break;
            }
            
            case 0: {
                printf("Завершение программы...\n");
                break;
            }
            
            default: {
                printf("Неверный выбор! Попробуйте снова.\n");
                break;
            }
        }
    } while (choice != 0);
    
    // ===== ОСВОБОЖДЕНИЕ ПАМЯТИ =====
    if (arr != NULL) {
        free(arr);
    }
    
    printf("\nСпасибо за использование программы!\n");
    system("pause");
    return 0;
}

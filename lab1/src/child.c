#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define MAX_INPUT 512
#define MAX_NUMBERS 100

int main() {
  char filename[MAX_INPUT];
  char data[MAX_INPUT];

  float numbers[MAX_NUMBERS];

  int count = 0;
  float result = 0.0;

  ssize_t filename_len = read(STDIN_FILENO, filename, MAX_INPUT);
  read(STDIN_FILENO, data, MAX_INPUT);

  filename[filename_len] = '\0';

  char *newline = strchr(data, '\n');
  if (newline) {
    *newline = '\0';
  }

  char *ptr = data;
  float num;
  while (count < MAX_NUMBERS && sscanf(ptr, "%f", &num) == 1) {
    numbers[count++] = num;

    while (*ptr != ' ' && *ptr != '\0') ptr++;
    while (*ptr == ' ') ptr++;
  }

  for (int i = 0; i < count; i++) { result += numbers[i]; }

  FILE *file = fopen(filename, "w");
  if (file == NULL) {
    perror("Ошибка при открытии файла");
    return 1;
  }

  fprintf(file, "%.2f\n", result);

  fclose(file);

  printf("Данные успешно записаны в файл: %s\n", filename);

  return 0;
}
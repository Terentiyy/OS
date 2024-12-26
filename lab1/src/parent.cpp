#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_INPUT 512

int main(void) {
  char filepath[MAX_INPUT];
  char line[MAX_INPUT];

  printf("Путь для файла вывода:\n");
  scanf("%s", filepath);

  printf("Введите числа, разделенные пробелами:\n");

  //Очищаем буфер ввода перед считыванием новой строки
  int c;
  while ((c = getchar()) != '\n' && c != EOF);

  if (fgets(line, sizeof(line), stdin) == NULL) {
    fprintf(stderr, "Ошибка при чтении ввода\n");
    return 1;
  }

  int pipefd[2];
  if (pipe(pipefd) == -1) {
    perror("pipe");
    exit(1);
  }

  switch (fork()) {
    case -1:perror("fork");
      exit(1);
    case 0:close(pipefd[1]);
      dup2(pipefd[0], STDIN_FILENO);
      close(pipefd[0]);
      execlp("./build/child", "child", NULL);

      perror("execlp");
      exit(1);

    default:close(pipefd[0]);

      write(pipefd[1], filepath, MAX_INPUT);
      write(pipefd[1], line, MAX_INPUT);

      close(pipefd[1]);

      wait(NULL);
  }

  return 0;
}
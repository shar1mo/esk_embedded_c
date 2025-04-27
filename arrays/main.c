#include <stdio.h>

#define N 6

void print_matrix(void) {
  int matrix[N][N];
  int value = 1;

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      matrix[i][j] = value++;
      printf("%3d", matrix[i][j]);
    }
    printf("\n");
  }
}

void reverse_array(void) {
  int arr[N] = {1, 2, 3, 4, 5, 6};
  int buffer;

  for (int i = 0; i < N / 2; i++) {
    buffer = arr[i];
    arr[i] = arr[N - 1 - i];
    arr[N - 1 - i] = buffer;
  }

  for (int i = 0; i < N; i++) {
    printf("%2d", arr[i]);
  }
  printf("\n");
}

void triangle_matrix(void) {
  int matrix[N][N];

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      matrix[i][j] = (i + j >= N - 1) ? 1 : 0;
      printf("%2d", matrix[i][j]);
    }
    printf("\n");
  }
}

void snail_matrix(void) {
  int matrix[N][N];
  int value = 1;
  int top = 0, bottom = N - 1;
  int left = 0, right = N - 1;

  while (top <= bottom && left <= right) {
    for (int j = left; j <= right; j++) {
      matrix[top][j] = value++;
    }
    top++;

    for (int i = top; i <= bottom; i++) {
      matrix[i][right] = value++;
    }
    right--;

    if (top <= bottom) {
      for (int j = right; j >= left; j--) {
        matrix[bottom][j] = value++;
      }
      bottom--;
    }

    if (left <= right) {
      for (int i = bottom; i >= top; i--) {
        matrix[i][left] = value++;
      }
      left++;
    }
  }

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      printf("%3d", matrix[i][j]);
    }
    printf("\n");
  }
}

int main() {
  print_matrix();
  putchar('\n');

  reverse_array();
  putchar('\n');

  triangle_matrix();
  putchar('\n');

  snail_matrix();
  putchar('\n');

  return 0;
}
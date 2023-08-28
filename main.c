#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Matrix
{
    int line;
    int column;
    float value;
    struct Matrix *right;
    struct Matrix *below;
} Matrix;

Matrix *createHeaderNodes(int numRows, int numColumns)
{
    Matrix *header = (Matrix *)malloc(sizeof(Matrix));

    header->column = -1;
    header->line = -1;
    header->value = -1;
    header->right = header;
    header->below = header;

    for (int rowIdx = numRows; rowIdx > 0; rowIdx--)
    {
        Matrix *rowHeader = (Matrix *)malloc(sizeof(Matrix));
        rowHeader->value = rowIdx;
        rowHeader->line = -1;
        rowHeader->column = 0;
        rowHeader->right = rowHeader;

        rowHeader->below = header->below;
        header->below = rowHeader;
    }

    for (int colIdx = numColumns; colIdx > 0; colIdx--)
    {
        Matrix *colHeader = (Matrix *)malloc(sizeof(Matrix));
        colHeader->value = colIdx;
        colHeader->line = 0;
        colHeader->column = -1;
        colHeader->below = colHeader;

        colHeader->right = header->right;
        header->right = colHeader;
    }
    return header;
}

void insertElement(Matrix *matrix, int numRows, int numColumns, int currentRow,
                   int currentCol, float value)
{
    Matrix *element = (Matrix *)malloc(sizeof(Matrix));
    Matrix *aux = matrix;
    Matrix *previous = matrix;

    element->value = value;
    element->column = currentCol;
    element->line = currentRow;

    for (int i = 0; i < currentCol; i++)
    {
        previous = matrix;
        matrix = matrix->right;
    }

    for (int i = 0; i < numRows; i++)
    {
        if (matrix->below == matrix)
        {
            matrix->below = element;
            element->below = matrix;
            break;
        }
        if (matrix->line > element->line)
        {
            element->below = matrix;
            previous->below = element;
            break;
        }
        if (matrix->below->column == -1)
        {
            previous = matrix;
            matrix = matrix->below;
            element->below = matrix;
            previous->below = element;
            break;
        }

        previous = matrix;
        matrix = matrix->below;
    }

    previous = NULL;
    matrix = aux;

    for (int i = 0; i < currentRow; i++)
    {
        previous = matrix;
        matrix = matrix->below;
    }

    for (int i = 0; i < numColumns; i++)
    {
        if (matrix->right == matrix)
        {
            matrix->right = element;
            element->right = matrix;
            break;
        }
        if (matrix->column > element->column)
        {
            previous->right = element;
            element->right = matrix;
            break;
        }
        if (matrix->right->line == -1)
        {
            previous = matrix;
            matrix = matrix->right;

            previous->right = element;
            element->right = matrix;
            break;
        }

        previous = matrix;
        matrix = matrix->right;
    }

    previous = NULL;
    aux = NULL;
    free(aux);
    free(previous);
}

Matrix *createMatrix() {
  int lines = 0;
  int columns = 0;

  scanf("%d %d", &lines, &columns);

  int line = -1, column = -1, scanCount = 0;
  float value = 0;
  Matrix *head;

  head = createHeaderNodes(lines, columns);

  while (1) {
    scanf("%d", &line);
    if (line == 0)
      return head;
    scanf(" %d %f", &column, &value);

    if (lines < line || columns < column) {
      printf("\nO número de linhas e colunas deve ser menor ou igual ao número especificado!\n\n");
      exit(0);
    } else if (value != 0) {
      insertElement(head, lines, columns, line, column, value);
      scanCount++;

      if (scanCount == (lines * columns)) {
        return head;
      }
    } else {
      scanCount++;
      if (scanCount == (lines * columns)) {
        return head;
      }
    }
  }

  return head;
}

int countRows(Matrix *matrix);
int countColumns(Matrix *matrix);

void printMatrix(Matrix *matrix) {
  if (matrix == NULL) {
    printf("A matriz está vazia!\n");
    return;
  } else if (matrix->line != -1 && matrix->column != -1) {
    printf("A matriz está vazia!\n");
    return;
  } else {
    int numRows = countRows(matrix);
    int numColumns = countColumns(matrix);

    printf("%d %d <=tamanho\n", numRows, numColumns);

    Matrix *rowHeader = matrix->below;
    while (rowHeader != matrix) {
      Matrix *current = rowHeader->right;
      while (current != rowHeader) {
        printf("%d, %d, %.2f \n", current->line, current->column, current->value);
        current = current->right;
      }
      rowHeader = rowHeader->below;
    }
  }
}


float getElement(Matrix *matrix, int row, int col)
{
    Matrix *current = matrix->below;
    while (current != matrix)
    {
        if (current->line == row)
        {
            while (current->right->column <= col)
            {
                current = current->right;
            }
            return (current->column == col) ? current->value : 0;
        }
        current = current->below;
    }
    return 0;
}

void setElement(Matrix *matrix, int row, int col, float elem)
{
    Matrix *aux = matrix;
    int numRows = 0, numColumns = 0;
    numRows = countRows(matrix);
    numColumns = countColumns(matrix);

    for (int i = 1; i <= col; i++)
    {
        matrix = matrix->right;
    }
    for (int i = 0; i <= row; i++)
    {
        if (matrix->line == row && matrix->column == col)
        {
            matrix->value = elem;
        }
        else if (matrix->below->column == -1)
        {
            insertElement(aux, numRows, numColumns, row, col, elem);
        }

        matrix = matrix->below;
    }
}

void destroyMatrix(Matrix *matrix)
{
    if (matrix == NULL)
        return;
    int rowCount = countRows(matrix), cont = 0;

    Matrix *header;
    Matrix *next;

    while (1)
    {
        header = matrix;
        matrix = matrix->right;
        while (1)
        {
            if (matrix->line != -1)
            {
                next = matrix->right;
                free(matrix);
                matrix = next;
            }
            else if (matrix->line == -1 && matrix == header)
            {
                next = matrix->below;
                free(matrix);
                matrix = next;
                cont++;
                break;
            }
            else
            {
                matrix = matrix->right;
            }
        }

        if (cont == rowCount + 1)
            break;
    }

    next = NULL;
    matrix = NULL;
}

Matrix *transposeMatrix(Matrix *matrix)
{
    Matrix *transposedMatrix;
    int numRows = 0, numColumns = 0;
    numRows = countColumns(matrix);
    numColumns = countRows(matrix);

    transposedMatrix = createHeaderNodes(numRows, numColumns);

    while (1)
    {
        matrix = matrix->below;
        while (1)
        {
            matrix = matrix->right;
            if (matrix->line != -1 && matrix->column != -1)
            {
                insertElement(transposedMatrix, numRows, numColumns, matrix->column, matrix->line, matrix->value);
            }
            else if ((matrix->line == -1 && matrix->column == 0) ||
                     (matrix->line == -1 && matrix->column == -1))
            {
                break;
            }
        }
        if (matrix->line == -1 && matrix->column == -1)
            break;
    }

    return transposedMatrix;
}

Matrix *addMatrices(Matrix *m, Matrix *n) {
  Matrix *result;
  int numRowsInM = 0, numColumnsInM = 0, numRowsInN = 0, numColumnsInN = 0;
  float sum = 0;
  numRowsInM = countRows(m);
  numColumnsInM = countColumns(m);
  numRowsInN = countRows(n);
  numColumnsInN = countColumns(n);

  if ((numRowsInM == numRowsInN) && (numColumnsInM == numColumnsInN)) {
    result = createHeaderNodes(numRowsInM, numColumnsInM);

    for (int i = 1; i <= numRowsInM; i++) {
      for (int j = 1; j <= numColumnsInM; j++) {
        sum = getElement(m, i, j) + getElement(n, i, j);

        if (sum != 0) {
          insertElement(result, numRowsInM, numColumnsInM, i, j, sum);
        }
      }
    }
  } else {
    printf("\nNão foi possível realizar a adição\n");
    result = NULL;
  }

  return result;
}


Matrix *multiplyMatrices(Matrix *m, Matrix *n)
{
    int numRowsInM = countRows(m), numColumnsInM = countColumns(m),
        numRowsInN = countRows(n), numColumnsInN = countColumns(n);

    if (numColumnsInM != numRowsInN)
    {
        printf("\nNão foi possível multiplicar as matrizes.\n");
        return NULL;
    }

    Matrix *result = createHeaderNodes(numRowsInM, numColumnsInN);

    for (int i = 1; i <= numRowsInM; i++)
    {
        for (int j = 1; j <= numColumnsInN; j++)
        {
            float sum = 0;

            float a = getElement(m, i, j);
            float b = getElement(n, i, j);
            sum += a * b;

            if (sum != 0.0)
            {
                insertElement(result, numRowsInM, numColumnsInN, i, j, sum);
            }
        }
    }

    return result;
}

Matrix *regularMatrixMultiply(Matrix *m, Matrix *n)
{
    int numRowsInM = countRows(m), numColumnsInM = countColumns(m),
        numRowsInN = countRows(n), numColumnsInN = countColumns(n);

    if (numColumnsInM != numRowsInN)
    {
        printf("\nNão foi possível multiplicar as matrizes.\n");
        return NULL;
    }

    Matrix *result = createHeaderNodes(numRowsInM, numColumnsInN);

    for (int i = 1; i <= numRowsInM; i++)
    {
        for (int j = 1; j <= numColumnsInN; j++)
        {
            float sum = 0;

            float a = getElement(m, i, j);
            float b = getElement(n, i, j);
            sum += a * b;

            insertElement(result, numRowsInM, numColumnsInN, i, j, sum);
        }
    }

    return result;
}

int countRows(Matrix *matrix)
{
    int numRows = 0;
    Matrix *aux = matrix;
    while (1)
    {
        matrix = matrix->below;
        if (matrix->column != -1)
        {
            numRows++;
        }
        else
        {
            break;
        }
    }
    matrix = aux;
    aux = NULL;
    return numRows;
}

int countColumns(Matrix *matrix)
{
    int numColumns = 0;
    Matrix *aux = matrix;
    while (1)
    {
        matrix = matrix->right;
        if (matrix->line != -1)
        {
            numColumns++;
        }
        else
        {
            break;
        }
    }
    matrix = aux;
    aux = NULL;
    return numColumns;
}

Matrix *createBigMatrix(unsigned int size)
{
    Matrix *header;

    header = createHeaderNodes(size, size);

    srand(time(NULL));

    for (unsigned i = 1; i <= size; i++)
    {
        for (unsigned j = 1; j <= size; j++)
        {
            insertElement(header, size, size, i, j, rand() % 2);
        }
    }

    return header;
}

int main(void)
{
    Matrix *A = createMatrix();
    printf("Matrix A:\n");
    printMatrix(A);

    Matrix *B = createMatrix();
    printf("Matrix B:\n");
    printMatrix(B);

    Matrix *C = addMatrices(A, B);
    printf("Matrix C (A + B):\n");
    printMatrix(C);
    destroyMatrix(C);

    C = multiplyMatrices(A, B);
    printf("Matrix C (A * B):\n");
    printMatrix(C);
    destroyMatrix(C);

    C = transposeMatrix(A);
    printf("Transposed Matrix A:\n");
    printMatrix(C);
    destroyMatrix(C);

    destroyMatrix(A);
    destroyMatrix(B);

    return 0;
}

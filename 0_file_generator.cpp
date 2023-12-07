#include <iostream>       // Librería estándar para operaciones de entrada/salida.
#include <string>         // Clase para trabajar con cadenas de caracteres.
#include <fstream>        // Librería para operaciones de archivo.
using namespace std;


// meter bitacora com primer argumento y otro nombre para sacar el file a usar
void readNwrite(string inputFileName, string outputFileName) {
  ifstream inputFile(inputFileName);
  ofstream outputFile(outputFileName);

  if (inputFile.is_open() && outputFile.is_open()) {
    string line;
    int lineCount = 0;

    while (getline(inputFile, line) && lineCount < 100) {
      if (lineCount % 10 == 0){
        int limit = lineCount/10;
        for (int i=0; i<limit; i++){
            outputFile << line << '\n';
        }
      }

      else outputFile << line << '\n';

      lineCount++;
    }

    inputFile.close();
    outputFile.close();
  }
}






int main() {

    readNwrite("bitacora.txt", "prueba.txt");

    return 0;
}
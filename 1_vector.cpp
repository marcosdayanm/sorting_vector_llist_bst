#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

// Clase llamada Data para guardar las fechas de cada registro
class Data {
public:
  int month;
  int day;
  int hour;
  int minute;
  int second;
  string text;

  Data(int mo, int d, int h, int m, int s, string t) { // constructor de la clase
    month = mo;
    day = d;
    hour = h;
    minute = m;
    second = s;
    text = t;
  }
};

vector<Data> database = {}; // se declaran los vectores como globales
vector<Data> aux = {};

// Mapa para convertir meses en string a su formato en int
unordered_map<string, int> monthToNumber = {
    {"Jan", 1}, {"Feb", 2}, {"Mar", 3}, {"Apr", 4},  {"May", 5},  {"Jun", 6},
    {"Jul", 7}, {"Aug", 8}, {"Sep", 9}, {"Oct", 10}, {"Nov", 11}, {"Dec", 12}};

// Leer cada uno de los registros del archivo y guardarlos en el vector database
// en forma de objeto
void read_data(string path) {
  int dato;

  ifstream file(path);

  if (file.is_open()) {
    string line;

    while (getline(file, line)) {
      istringstream iss(line);
      string word;
      vector<string> words;

      while (iss >> word) {
        words.push_back(word);
      }

      string month = words[0];
      int monthNum = monthToNumber[month];
      string day = words[1];
      string time = words[2];

      string hour = "";
      hour += time[0];
      hour += time[1];

      string minute = "";
      minute += time[3];
      minute += time[4];

      string second = "";
      second += time[6];
      second += time[7];

      // Llamar al constructor de Data para guardar los datos         de la
      // línea y  meter el objeto en un vector
      database.push_back(Data(monthNum, stoi(day), stoi(hour), stoi(minute),
                              stoi(second), line));
    }

    file.close();
  } else {
    cout << "No se encontro el archivo\n";
  }
}

void print_data(const vector<string> &data) {
  for (int i = 0; i < data.size(); i++) {
    cout << data[i] << '\n';
  }
}

bool compare_date(int i, int j) {

  if (database[i].month < database[j].month)
    return true;
  else if (database[i].month > database[j].month)
    return false;

  else if (database[i].day < database[j].day)
    return true;
  else if (database[i].day > database[j].day)
    return false;

  else if (database[i].hour < database[j].hour)
    return true;
  else if (database[i].hour > database[j].hour)
    return false;

  else if (database[i].minute < database[j].minute)
    return true;
  else if (database[i].minute > database[j].minute)
    return false;

  return database[i].second < database[j].second;
}

void insertion_sort() {
  for (int i = 1; i < database.size(); i++) {
    Data obj = database[i];

    int j = i;

    for (int j = i; j > 0; j--) {
      if (compare_date(j, j - 1)) {
        swap(database[j], database[j - 1]);
      } else {
        break;
      }
    }
  }
}

int comparebinary(int month, int day, int i) {
  if (month < database[i].month)
    return 1;
  else if (month > database[i].month)
    return 0;

  else if (day < database[i].day)
    return 1;
  else if (day > database[i].day)
    return 0;

  return 2;
}

int binary_search(int month, int day, int type) {
  int min = 0;
  int max = database.size() - 1;

  while (true) {
    if ((min > max)) {
      return -1;
    }

    int middle = min + (max - min) / 2;
    if (comparebinary(month, day, middle) == 2) {
      int aum;
      if (type == 0)
        aum = -1;
      else
        aum = 1;

      while (comparebinary(month, day, middle) == 2) {
        middle += aum;
      }
      middle -= aum;
      return middle;
    } else if (comparebinary(month, day, middle) == 1) {
      max = middle - 1;
    } else {
      min = middle + 1;
    }
  }
}

void save_data(string file) {
  ofstream output(file);

  if (output.is_open()) {
    int dbsize = database.size();
    for (int i = 0; i < dbsize; i++) {
      output << database[i].text << '\n';
    }

    output.close();
  } else {
    cout << "Error al abrir " << file << " para guardar los datos\n";
  }
}

int main() {

  cout << "\nFavor de esperar a que se solicite el rango de fechas para realizar una búsqueda\n\n";

  read_data("bitacora.txt");
  insertion_sort();

  int inf_lim, sup_lim;

  // 5 chances para introducir un intervalo de facha válido
  for (int i = 0; i < 5; i++) {

    string fecha = "", mes1 = "", dia1 = "", mes2 = "", dia2 = "";

    // Input
    cout << "Inserte el intervalo de tiempo en el que quiere realizar su "
            "búsqueda en éste formato (MM/DD-MM/DD): ";
    cin >> fecha;

    mes1 += fecha[0];
    mes1 += fecha[1];

    dia1 += fecha[3];
    dia1 += fecha[4];

    mes2 += fecha[6];
    mes2 += fecha[7];

    dia2 += fecha[9];
    dia2 += fecha[10];

    inf_lim = binary_search(stoi(mes1), stoi(dia1), 0);
    sup_lim = binary_search(stoi(mes2), stoi(dia2), 1);

    if (inf_lim == -1 || sup_lim == -1) {
      cout << "\n\nIntervalo de fecha no válido\n\n";
      continue;
    }
    break;
  }

  for (int i = inf_lim; i < sup_lim + 1; i++) {

    cout << database[i].text << "\n";
  }

  save_data("sorted.txt");

  return 0;
}
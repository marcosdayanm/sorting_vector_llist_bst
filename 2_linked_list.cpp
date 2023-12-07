#include <fstream>
#include <iostream>
#include <list>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// Clase llamada Data para guardar los datos de cada registro
class Data {
public:
  int oct4;
  int oct3;
  int oct2;
  int oct1;
  int port;
  string text;

  Data() : oct4(0), oct3(0), oct2(0), oct1(0), port(0), text("") {}

  // Constructor para insertar los datos a cada objeto
  Data(int o4, int o3, int o2, int o1, int p,
       string t) { 
    oct4 = o4;
    oct3 = o3;
    oct2 = o2;
    oct1 = o1;
    port = p;
    text = t;
  }
};

// La lista enlazada que va a contener todos los datos
list<Data> database = {};

// Leer cada uno de los registros del archivo y guardarlos en el vector database
// en forma de objeto, se dividirán las direcciones IP con el fin de sortearlas de mejor manera, se hará mediante a una
// Regular Expression
void read_data(string path) {
  ifstream file(path);

  if (file.is_open()) {

    string line;
    regex ip_pattern(R"((\d+\.\d+\.\d+\.\d+):(\d+))");

    while (getline(file, line)) {
      smatch match;
      if (regex_search(line, match, ip_pattern)) {
        if (match.size() == 3) {
          string ip = match[1].str();
          int port = stoi(match[2].str());

          // Formattear los componentes del IP
          vector<int> tokens;
          istringstream stream(ip);
          string token;
          while (getline(stream, token, '.')) {
            tokens.push_back(stoi(token));
          }

          // Construir el objeto con los datos del IP
          database.push_back(
              Data(tokens[0], tokens[1], tokens[2], tokens[3], port, line));
        }
      }
    }

    file.close();
  } else {
    cout << "No se encontro el archivo\n";
  }
}

// Ésta función se ayudará de un iterador para imprimir todos los elementos de la lista pasada como argumento
void print_data(list<Data> &interval = database) {
  list<Data>::iterator it = interval.begin();
  for (int i = 0; i < interval.size(); i++) {
    cout << it++->text << '\n';
  }
}

// Función para imprimir solamente las direcciones IP con fines de comprobar fácilmente si están sorteadas
void print_all_ip() {
  list<Data>::iterator it = database.begin();
  for (int i = 0; i < database.size(); i++) {
    cout << it->oct4 << " " << it->oct3 << " " << it->oct2 << " " << it->oct1
         << " " << it++->port << " "
         << "\n";
  }
}

// Función que creamos para comparar dos direcciones IP que servirá para ayudar
// a la función de sort que viene con las listas ligadas
bool compare_ip(const Data &d1, const Data &d2) {

  if (d1.oct4 < d2.oct4)
    return true;
  else if (d1.oct4 > d2.oct4)
    return false;

  else if (d1.oct3 < d2.oct3)
    return true;
  else if (d1.oct3 > d2.oct3)
    return false;

  else if (d1.oct2 < d2.oct2)
    return true;
  else if (d1.oct2 > d2.oct2)
    return false;

  else if (d1.oct1 < d2.oct1)
    return true;
  else if (d1.oct1 > d2.oct1)
    return false;

  return d1.port < d2.port;
}


// Ésta es una función que devuelve una lista de objetos de la clase Data, que usa los objetos pasados como intervalos
// Primero posiciona al iterador en el primer intervalo, para después añadir a todos los elementos de la lista database
// a la lista que se regresará para imprimir el intervalo seleccionado por el usuario
list<Data> limits(const Data &inflim, const Data &suplim) {

  list<Data> intervals = {};

  if (compare_ip(inflim, suplim) == false)
    return intervals;

  list<Data>::iterator it = database.begin();

  // llevar el iterador al límite inferior
  while (it != database.end() && compare_ip(*it, inflim)) {
    it++;
  }

  while (it != database.end() && compare_ip(*it, suplim)) {
    intervals.push_back(*it++);
  }
  intervals.push_back(*it); // insertar el límite superior de esta forma ya que
                            // no entra en la excepción del while

  return intervals;
}

// Función que sirve para guardar los datos ordenados por IP en un nuevo archivo
void save_data(string file) {
  ofstream output(file);

  if (output.is_open()) {

    int dbsize = database.size();
    list<Data>::iterator it = database.begin();

    for (int i = 0; i < dbsize; i++) {
      output << it++->text << '\n';
    }
    output.close();
  }

  else {
    cout << "Error al abrir " << file << " para guardar los datos\n";
  }
}

int main() {

  cout << "\nFavor de esperar a que se solicite el rango de fechas para "
          "realizar una búsqueda\n\n";

  read_data("bitacora.txt");

  // Se usa la función sort de la librería list de STL, y se pasa como argumento
  // una función que hicimos que sirve para comparar direcciones IP
  database.sort(compare_ip);

  string start_ip, end_ip;

  // Tomar el rango de IPs del usuario
  cout << "Inserte la IP de inicio del rango (e.g., 1.6.378.65:6772): ";
  cin >> start_ip;

  cout << "Inserte la IP de fin del rango (e.g., 1.61.436.96:4201): ";
  cin >> end_ip;


  // Objeto de Regular Expressons para poder seccionar el input del usuario e identificar los diferentes datos de la IP proporcionada
  smatch start_match, end_match;
  regex_search(start_ip, start_match,
               regex(R"((\d+)\.(\d+)\.(\d+)\.(\d+):(\d+))"));
  regex_search(end_ip, end_match, regex(R"((\d+)\.(\d+)\.(\d+)\.(\d+):(\d+))"));


  // Comprobar si el input de los usuarios fue correcto para crear objetos tipo Data con el input y poder obtener el intervalo
  if (start_match.size() == 6 && end_match.size() == 6) {
    Data start_data(stoi(start_match[1]), stoi(start_match[2]),
                    stoi(start_match[3]), stoi(start_match[4]),
                    stoi(start_match[5]), "");
    Data end_data(stoi(end_match[1]), stoi(end_match[2]), stoi(end_match[3]),
                  stoi(end_match[4]), stoi(end_match[5]), "");

    // Obtener intervalos en base a el input del usuario
    list<Data> user_intervals = limits(start_data, end_data);

    // Imprimir los datos para los IPs del rango especificado
    print_data(user_intervals);
  } 
  
  else {
    cout << "Entrada inválida para las direcciones IP.\n";
  }



  save_data("sorted.txt");

  return 0;
}
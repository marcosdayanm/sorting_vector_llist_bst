#include <fstream>
#include <iostream>
#include <queue>
#include <regex>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

// Clase para guardar los datos de cada registro
class Data {
public:
  int oct4;
  int oct3;
  int oct2;
  int oct1;
  int port;
  string text;
  int accesos = 1;

  Data() : oct4(0), oct3(0), oct2(0), oct1(0), port(0), text("") {}

  // Constructor para insertar los datos a cada objeto
  Data(int o4, int o3, int o2, int o1, int p, string t) {
    oct4 = o4;
    oct3 = o3;
    oct2 = o2;
    oct1 = o1;
    port = p;
    text = t;
  }

  // Constructor para insertar los datos a cada objeto con accesos
  Data(int o4, int o3, int o2, int o1, int p, string t, int a) {
    oct4 = o4;
    oct3 = o3;
    oct2 = o2;
    oct1 = o1;
    port = p;
    text = t;
    accesos = a;
  }
};

// Función para comparar IPs entre objetos
int compare_ip(const Data &d1, const Data &d2) {

  if (d1.oct4 < d2.oct4)
    return 1;
  else if (d1.oct4 > d2.oct4)
    return 0;

  else if (d1.oct3 < d2.oct3)
    return 1;
  else if (d1.oct3 > d2.oct3)
    return 0;

  else if (d1.oct2 < d2.oct2)
    return 1;
  else if (d1.oct2 > d2.oct2)
    return 0;

  else if (d1.oct1 < d2.oct1)
    return 1;
  else if (d1.oct1 > d2.oct1)
    return 0;

  else if (d1.port < d2.port)
    return 1;
  else if (d1.oct1 > d2.oct1)
    return 0;

  return 2; // Son iguales
}

// Clase del nodo del árbol
class Node {
public:
  Data dato;
  Node *left = nullptr;
  Node *right = nullptr;

  Node(Data n) { dato = n; }
};

// Clase del +arbol de búsqueda binaria
class BST {
public:
  Node *root = nullptr;

  // Wrappers de funcionalidad de BST
  Node *buscar(Data dato) { return buscarRecursiva(root, dato); }

  void insertar(Data dato) { root = insertarRecursivo(root, dato); }

  void insertarAcceso(Data dato) { root = insertarRecursivoAcceso(root, dato); }

  int height() { return heightRecursivo(root) - 1; }

  void saveInorden(string file) {
    ofstream output(file);

    saveInordenHelper(root, output);
  }

  void saveReverseInorden(string file) {
    ofstream output2(file);

    saveReverseInordenHelper(root, output2);
  }

  void destroyTree() {
    destroyTreeHelper(root);
    root = nullptr;
  }

  // Éste método desplegará el árbol de difentes maneras dependiento
  // del número pasado como argumento
  bool visit(int n) {
    if (n < 1 || n > 4) {
      return false;
    }
    switch (n) {
    case 1:
      preOrden(root);
      break;

    case 2:
      inOrden(root);
      break;

    case 3:
      posOrden(root);
      break;

    case 4:
      levelByLevel();
      break;
    }
    cout << "\n\n";
  }

private:
  // Busca un nodo en el árbol con búsqueda binaria recursiva en base al IP
  Node *buscarRecursiva(Node *r, Data dato) {
    if (r == nullptr) // Si el nodo raíz de la función es nullptr se regresa
                      // significando que no se encontró el dato
      return nullptr;

    if (compare_ip(dato, r->dato) == 1)
      return buscarRecursiva(r->left, dato);

    else if (compare_ip(dato, r->dato) == 0)
      return buscarRecursiva(r->right, dato);

    return r;
  }

  // Inserta cualquier nodo en su lugar de forma recursiva en base al IP
  Node *insertarRecursivo(Node *r, Data dato) {
    if (r == nullptr) // Acá se inserta ya que es que ya se llegóa una hoja
      return new Node(dato);

    if (compare_ip(dato, r->dato) == 1)
      r->left = insertarRecursivo(r->left, dato);

    else if (compare_ip(dato, r->dato) == 0)
      r->right = insertarRecursivo(r->right, dato);

    else if (compare_ip(dato, r->dato) == 2) {
      r->dato.accesos++;
    }

    return r;
  }

  // Inserta cualquier nodo en su lugar de forma recursiva utilizando el número
  // de accesos como referencia
  Node *insertarRecursivoAcceso(Node *r, Data dato) {
    if (r == nullptr) // Acá se inserta ya que es que ya se llegóa una hoja
      return new Node(dato);

    if (dato.accesos < r->dato.accesos)
      r->left = insertarRecursivoAcceso(r->left, dato);

    else if (dato.accesos > r->dato.accesos)
      r->right = insertarRecursivoAcceso(r->right, dato);

    return r;
  }

  // El recorrido del arbol con el orden preorden
  void preOrden(Node *r) {
    if (r == nullptr)
      return;
    cout << r->dato.text << "\n";
    preOrden(r->left);
    preOrden(r->right);
  }

  // El recorrido del arbol con el orden inorden
  void inOrden(Node *r) {
    if (r == nullptr)
      return;
    inOrden(r->left);
    cout << r->dato.text << "\n";
    inOrden(r->right);
  }

  // El recorrido del arbol con el orden posorden
  void posOrden(Node *r) {
    if (r == nullptr)
      return;
    posOrden(r->left);
    posOrden(r->right);
    cout << r->dato.text << "\n";
  }

  // El recorrido level by level del árbol, éste método pushea a la raíz del BST
  // a una queue, para después iterar sobre el tamaño de la queue, y si es que
  // los nodos que están adentro tienen left y right, se añade éstos mismos a la
  // queue, y se vuelve a ciclar en el loop
  void levelByLevel() {
    if (root == nullptr)
      return;

    queue<Node *> q;
    q.push(root);

    while (!q.empty()) {
      int nodosNivel = q.size();

      for (int i = 0; i < nodosNivel; i++) {
        Node *actual = q.front();
        q.pop();

        cout << actual->dato.text << "\n";

        if (actual->left != nullptr)
          q.push(actual->left);

        if (actual->right != nullptr)
          q.push(actual->right);
      }

      cout << "\n";
    }
  }

  // Encuentra el nivel del nodo proporcionado
  int levelRecursiva(Node *r, Data dato, int level = 0) {
    if (r == nullptr) // Si el nodo raíz de la función es nullptr se regresa
                      // significando que no se encontró el dato
      return -1;

    if (compare_ip(dato, r->dato) == 1)
      return levelRecursiva(r->left, dato, ++level);

    else if (compare_ip(dato, r->dato) == 0)
      return levelRecursiva(r->right, dato, ++level);

    return level;
  }

  // Encuentra la altura del árbol de forma recursiva recorriendo todos los
  // nodos
  int heightRecursivo(Node *r) {
    if (r == nullptr)
      return 0;

    int hleft = heightRecursivo(r->left);
    int hright = heightRecursivo(r->right);

    return 1 + max(hleft, hright);
  }

  // Guarda el árbol a un archivo ordenado, añade el númeo de accesos al final
  // de cada línea
  void saveInordenHelper(Node *r, ofstream &output) {
    if (r == nullptr)
      return;
    saveInordenHelper(r->left, output);
    output << r->dato.text << " " << r->dato.accesos << '\n';
    saveInordenHelper(r->right, output);
  }

  // Guarda el árbol a un archivo ordenado al revés
  void saveReverseInordenHelper(Node *r, ofstream &output2) {
    if (r == nullptr)
      return;
    saveReverseInordenHelper(r->right, output2);
    output2 << r->dato.text << '\n';
    saveReverseInordenHelper(r->left, output2);
  }

  // Borra todos los nodos del árbol
  void destroyTreeHelper(Node *r) {
    if (r != nullptr) {
      destroyTreeHelper(r->left);
      destroyTreeHelper(r->right);
      delete r;
    }
  }
};

// La lista enlazada que va a contener todos los datos
BST database;

// Función para leer el archivo con los dato, utiliza regular expressions para
// extraer el ip y el número de accesos según sea el caso
void read_data(string path, bool mode) {
  ifstream file(path);

  if (file.is_open()) {

    // Expresiones regulares para obtener IP y número de accesos
    string line;
    regex ip_pattern(R"((\d+\.\d+\.\d+\.\d+):(\d+))");
    regex last_digits_pattern("\\b(\\d+)\\b(?!.*\\d.*)");

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

          if (!mode) {
            // Construir el objeto con los datos del IP
            database.insertar(
                Data(tokens[0], tokens[1], tokens[2], tokens[3], port, line));
          }

          else {
            smatch last_digits_match;
            regex_search(line, last_digits_match, last_digits_pattern);
            // Construir el objeto con los datos del IP
            database.insertarAcceso(Data(tokens[0], tokens[1], tokens[2],
                                         tokens[3], port, line,
                                         stoi(last_digits_match[1])));
          }
        }
      }
    }

    file.close();
  } else {
    cout << "No se encontro el archivo\n";
  }
}

// Obtiene los 5 datos con más accesos
void topFive(string inputFileName, string outputFileName) {
  ifstream inputFile(inputFileName);
  ofstream outputFile(outputFileName);

  if (inputFile.is_open() && outputFile.is_open()) {
    string line;
    int lineCount = 0;

    while (getline(inputFile, line) && lineCount < 5) {
      outputFile << line << '\n';
      lineCount++;
    }

    inputFile.close();
    outputFile.close();
  }
}

int main() {
  // Lee el archivo inicial al árbol
  read_data("bitrep.txt", false);

  // Guarda los datos de manera ordenada a otro archivo
  database.saveInorden("sorted_ip.txt");

  // Reinicia el árbol
  database.destroyTree();

  // Lee el archivo con los datos ordenados y los mete al árbol
  read_data("sorted_ip.txt", true);

  // Guarda los datos del árbol en otro archivo ordenados de mayor a menor
  database.saveReverseInorden("tempfile.txt");

  // Toma los primeros cinco elementos de ese archivo y los guarda en uno nuevo
  topFive("tempfile.txt", "top_five_access.txt");

  return 0;
}

#include <iostream>
#include <cstdint>  // std::size_t
#include <stdexcept>  // C++ exceptions
#include <string>
#include <fstream>
#include <vector>

//implementacao da pilha e fila feitas em aula:

template<typename T>
//! CLASSE PILHA
class ArrayStack {
 public:
    //! construtor simples
    ArrayStack() {
        max_size_ = DEFAULT_SIZE;
        contents = new T[max_size_];
        top_ = -1;
    }
    //! construtor com parametro tamanho
    explicit ArrayStack(std::size_t max) {
        max_size_ = max;
        contents = new T[max_size_];
        top_ = -1;
    }
    //! destrutor
    ~ArrayStack() {
        delete [] contents;
    }
    //! metodo empilha
    void push(const T& data) {
        if (full()) {
            throw std::out_of_range("pilha cheia");
        } else {
            top_++;
            contents[top_] = data;
        }
    }
    //! metodo desempilha
    T pop() {
        if (empty())
            throw std::out_of_range("pilha vazia");
        T aux;
        aux = contents[top_];
        top_--;
        return aux;
    }
    //! metodo retorna o topo
    T& top() {
        if (empty())
            throw std::out_of_range("pilha vazia");
        return contents[top_];
    }
    //! metodo limpa pilha
    void clear() {
        top_ = -1;
    }
    //! metodo retorna tamanho
    std::size_t size() {
        return (top_ + 1);
    }
    //! metodo retorna capacidade maxima
    std::size_t max_size() {
        return(max_size_);
    }
    //! verifica se esta vazia
    bool empty() {
        return (top_ == -1);
    }
    //! verifica se esta cheia
    bool full() {
        return (static_cast<size_t>(top_) == max_size()-1);
    }

 private:
    T* contents;
    int top_;
    std::size_t max_size_;

    static const auto DEFAULT_SIZE = 10u;
};

template<typename T>
//! classe ArrayQueue
class ArrayQueue {
 public:
    //! construtor padrao
    ArrayQueue() {
        max_size_ = DEFAULT_SIZE;
        contents = new T[max_size_];
        size_ = 0;
        begin_ = 0;
        end_ = -1;
    }
    //! construtor com parametro
    explicit ArrayQueue(std::size_t max) {
        max_size_ = max;
        contents = new T[max_size_];
        size_ = 0;
        begin_ = 0;
        end_ = -1;
    }
    //! destrutor padrao
    ~ArrayQueue() {
        delete [] contents;
    }
    //! metodo enfileirar
    void enqueue(const T& data) {
        if (full()) {
            throw std::out_of_range("Fila cheia");
        } else {
            end_ = (end_ + 1) % max_size();
            size_++;
            contents[end_] = data;
        }
    }

    //! metodo desenfileirar
    T dequeue() {
        if (empty()) {
            throw std::out_of_range("Fila vazia");
        } else {
            T aux;
            aux = contents[begin_];
            begin_ = (begin_ + 1) % max_size();
            size_--;
            return (aux);
        }
    }
    //! metodo retorna o ultimo
    T& back() {
        if (empty()) {
            throw std::out_of_range("Fila vazia");
        } else {
            return (contents[end_]);
        }
    }
    //! metodo limpa a fila
    void clear() {
        begin_ = 0;
        end_ = -1;
        size_ = 0;
    }
    //! metodo retorna tamanho atual
    std::size_t size() {
        return (size_);
    }
    //! metodo retorna tamanho maximo
    std::size_t max_size() {
        return(max_size_);
    }
    //! metodo verifica se vazio
    bool empty() {
        return (size_ == 0);
    }
    //! metodo verifica se esta cheio
    bool full() {
        return (static_cast<size_t>(size_) == max_size());
    }

 private:
    T* contents;
    std::size_t size_;
    std::size_t max_size_;
    int begin_;  // indice do inicio (para fila circular)
    int end_;  // indice do fim (para fila circular)
    static const auto DEFAULT_SIZE = 1000u;
};

// ID 2- explicacao no relatorio.
// Funcao para verificar a validade do arquivo XML
bool validarXML(const std::string& xml) {
    
     // Criação da pilha para armazenar as marcações
    ArrayStack<std::string> pilha;
    
    std::size_t pos = 0;
    while (pos < xml.size()) {
        // ID 2.1:
        if (xml[pos] == '<') {
            std::size_t fechamento = xml.find('>', pos);
            if (fechamento == std::string::npos) {
                return false;
            }
            std::string identificador = xml.substr(pos + 1, fechamento - pos - 1);
            if (identificador[0] == '/') {
                if (pilha.empty() || pilha.top() != identificador.substr(1)) {
                    return false;
                }
                pilha.pop();
            } else {
                pilha.push(identificador);
            }
            pos = fechamento + 1;
        } else {
            pos++;
        }
    }

    if (!pilha.empty()) {
        return false;
    }

    return true;
}

// ID 3- explicacao no relatorio.
// Divide o xml em substrings contendo cada cenario e coloca em uma fila.
ArrayQueue<std::string> processaXML(const std::string& xml) {
    ArrayQueue<std::string> xmlDiv;
    std::size_t pos = 0;
    std::size_t abertura = 0;
    std::size_t fechamento = 0;
    std::string cenario;
    std::string aberturaTag = "<cenario>";
    std::string fechamentoTag = "</cenario>";
    
    while ((abertura = xml.find(aberturaTag, pos)) != std::string::npos) {
        abertura += aberturaTag.length();
        fechamento = xml.find(fechamentoTag, abertura);
        cenario = xml.substr(abertura, fechamento - abertura);
        pos = fechamento + fechamentoTag.length();
        xmlDiv.enqueue(cenario);
    }
    
    return xmlDiv;
}

// ID 3.1
// Copia de processaXML que extrai os dados do cenario.
int getData(const std::string& xml, const std::string& aberturaTag, const std::string& fechamentoTag) {
    std::size_t pos = 0;
    std::size_t abertura = 0;
    std::size_t fechamento = 0;
    int data;
   
    while ((abertura = xml.find(aberturaTag, pos)) != std::string::npos) {
        abertura += aberturaTag.length();
        fechamento = xml.find(fechamentoTag, abertura);
        data = stoi(xml.substr(abertura, fechamento - abertura));
        pos = fechamento + fechamentoTag.length();
    }
    
    return data;
}

// ID 3.2
// Versao de getData que retorna string da matriz E e nome
std::string getDataS(const std::string& xml, const std::string& aberturaTag, const std::string& fechamentoTag) {
    std::size_t pos = 0;
    std::size_t abertura = 0;
    std::size_t fechamento = 0;
    std::string data;
   
    while ((abertura = xml.find(aberturaTag, pos)) != std::string::npos) {
        abertura += aberturaTag.length();
        fechamento = xml.find(fechamentoTag, abertura);
        data = xml.substr(abertura, fechamento - abertura);
        pos = fechamento + fechamentoTag.length();
    }
    
    return data;
}

// ID 4
// Coleta de dados e rotina executada pelo robo
std::string rotina(const std::string& cenario) {
    std::string resultado;
    std::string nome;
    
    // Coleta os dados do cenario.
    // ID 3.1, 3.2-
    std::string a = "<x>";
    std::string b = "</x>";
    int x = getData(cenario, a, b);
    
    a = "<y>";
    b = "</y>";
    int y = getData(cenario, a, b);
    
    a = "<altura>";
    b = "</altura>";
    int altura = getData(cenario, a, b);
    
    a = "<largura>";
    b = "</largura>";
    int largura = getData(cenario, a, b);
    
    a = "<nome>";
    b = "</nome>";
    nome = getDataS(cenario, a, b);
    
    // Cria as matrizes R e E
    
    std::vector<std::vector<int>> R(altura, std::vector<int>(largura, 0));
    std::vector<std::vector<int>> E(altura, std::vector<int>(largura, 0));
    
    for (int i = 0; i < altura; i++) {
        for (int j = 0; j < largura; j++){
            R[i][j] = 0;
        }
    }
    
    // Preenche matriz E 
    a = "<matriz>";
    b = "</matriz>";
    std::string mE = getDataS(cenario, a, b);
    int cont = 0;
    std::string aux;
    for (int i = 0; i < altura; i++) {
        for (int j = 0; j < largura; j++){
            aux = mE[cont];
            E[i][j] = stoi(aux);
            cont++;
            //std::cout << E[i][j] << "";
        }
        //std::cout << std::endl;
    }
    
    // Rotina do robo
    // Preferi nao criar outra funcao para nao ter que lidar com os problemas gerados em passar as matrizes para ela
    
    // If para checar se E[x][y] = 0, caso contrario executa a rotina
    int area;
    if(E[x][y] == 0) {
        area = 0;
    } else {
        
        ArrayQueue<std::pair<int, int>> queue;
        queue.enqueue({x, y});
        R[x][y] = 1;
        while (!queue.empty()) {
            std::pair<int, int> temp = queue.dequeue();
            int currentX = temp.first;
            int currentY = temp.second;
    
            int dx[] = {-1, 1, 0, 0};
            int dy[] = {0, 0, -1, 1};
    
            for (int i = 0; i < 4; i++) {
                int nx = currentX + dx[i];
                int ny = currentY + dy[i];
    
                if (nx >= 0 && nx < altura && ny >= 0 && ny < largura && E[nx][ny] == 1 && R[nx][ny] == 0) {
                    queue.enqueue({nx, ny});
                    R[nx][ny] = 1;
                }
            }
        }
    
        area = 0;
        for (int i = 0; i < altura; i++) {
            for (int j = 0; j < largura; j++) {
                if (R[i][j] == 1) {
                    area++;
                }
            }
        }
    }

    resultado = nome + " " + std::to_string(area);
    return resultado;
}

int main() {
    char xmlfilename[100];
    std::cin >> xmlfilename;  // entrada

// ID 1- explicacao no relatorio.

    // Abrir o arquivo XML
    std::ifstream arquivo(xmlfilename);
    
    // Ler o conteúdo do arquivo XML
    std::string linha;
    std::string xml;
    while (std::getline(arquivo, linha)) {
        xml += linha;
    }

    // Valida o XML e executa a funcao do robo para o numero de cenarios.
    if(validarXML(xml)) {
        
    // ID 3-
    ArrayQueue<std::string> xmlDiv = processaXML(xml);
    int size = xmlDiv.size();
    std::string cenario;
    std::string resultado;
    for (int i = 0; i < size; i++) {
        cenario = xmlDiv.dequeue();
        // ID 4-
        resultado = rotina(cenario);
        std::cout << resultado << std::endl;
    }
        
    } else {
        std::cout << "erro" << std::endl;
    };
    
    return 0;
}



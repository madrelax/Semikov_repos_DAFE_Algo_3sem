#ifndef DSF_GRAPH_H //Это нужно чтобы заголовочный файл не включался больше 1 раза в какой либо файл
#define DSF_GRAPH_H
#include <vector>
#include <algorithm>
#include <queue>
#include <iostream>

enum Color {
    WHITE,
    GRAY,
    BLACK
};

class IGraph {  //Интерфейс графа
public:
    virtual ~IGraph() {}    //Виртуальный деструктор чтобы при удалении по указателю не было утечки памяти

    //Добавить ребро выходящее из вершины from и ведущее в вершину to
    virtual void AddEdge(size_t from, size_t to) = 0;   //чисто виртуальная функция (не имеет реализации в этом классе

    //получить кол-во вершин в графе
    virtual size_t VerticesCount() const  = 0;

    //Получить вектор всех вершин в которые можно попасть по ребру из вершины vertex
    virtual std::vector<size_t> GetVertices(size_t vertex) const = 0;
};

//невзвешенный неорграф
class ListGraph : public IGraph {
public:
    ListGraph(size_t verticesNumber) : vertices(verticesNumber){}

    void AddEdge(size_t from, size_t to) override {
        vertices.at(from).push_back(to);
        vertices.at(to).push_back(from);
    }

    size_t VerticesCount() const override{
        return vertices.size();
    }

    std::vector<size_t> GetVertices(size_t vertex) const override {
        return vertices.at(vertex);
    }
private:
    std::vector<std::vector<size_t>> vertices;
};

//Ориентированный граф на основе матрицы инцендентности, реализующий интерфейс IGraph
class MatrixGraph : public IGraph {
public:
    //Конструктор принимает кол-во вершин в графе
    MatrixGraph(size_t verticesCount) : vertices(verticesCount, std::vector<bool>(verticesCount, false)) { //задаём матрицу verticesCount x verticesCount заполненую false
    }

    void AddEdge(size_t from, size_t to) override {
        vertices.at(from).at(to) = true;
        //Если бы граф был неориентированным то также было бы vertices.at(to).at(from) = true;
    }

    size_t VerticesCount() const override {
        return vertices.size();
    }

    //Внимание! эта функция работает за линейное от кол-ва вершин время
    std::vector<size_t> GetVertices(size_t vertex) const override {
        std::vector<size_t> result;
        for(size_t i = 0, n = vertices.size(); i < n; ++i) {
            if(vertices.at(vertex)[i]) {    //если в нашей матрице на позиции (vertex, i) стоит true то из vertex по ребру можно попасть в i
                result.push_back(i);        //добавляем i в результат. Амартизированное время работы будет константным
            }
        }
        return result;
    }
private:
    std::vector<std::vector<bool>> vertices;   //Матрицу будем хранить в виде вектора векторов, кол-во столбцов равео кол-ву строк  равно  кол-ву вершин
};

// Функция принимает указатель на интерфкйс графа, вершину, и ссылку на вектор цветов
bool dfs(const IGraph* const graph, size_t vertex, std::vector<Color>& colors) {
    colors[vertex] = GRAY;  //когда зашли в вершину закрасили её в серый цвет
    for(auto nextVertex : graph->GetVertices(vertex)) { //Перебираем все вершины в которые можно попасть по ребру из данной
        if(colors[nextVertex] == GRAY) {
            return true;    //Нашли что есть цикл, дальше можно не искать
        } else if(colors[nextVertex] == WHITE)  //надо проверить что цвет белый чтобы идти дальше, т к в орграфе можно попасть и в чёрную
            if(dfs(graph, nextVertex, colors)) {
                return true;
        }
    }
    colors[vertex] = BLACK; //когда все соседние вершмны обрабртаны закрашиваем данную вершину в чёрный
    return false;
}

bool hasCycle(const IGraph* const graph) {
    auto verticesColors = std::vector<Color>(graph->VerticesCount(), WHITE);    //Задаём вектор цветов, индекс - номер вершины
    //Ищим первую белую вершину, здесь тоже намеренное усложнение чтобы показать работу с find, быстрее было просто один раз пройти for
    auto it = std::find(verticesColors.begin(), verticesColors.end(), WHITE);
    while (it != verticesColors.end()) {    //В случае если белых вершин нет, find вернёт vertices.end()
        if(dfs(graph, it - verticesColors.begin(), verticesColors)) {   //запускаем dfs для белой вершины, индекс находим как разницу найденого итератора и begin
            return true;    //Нашли цикл дальше можно не идти
        }
        it = std::find(verticesColors.begin(), verticesColors.end(), WHITE);    //Ищем есть ли ещё белые вершины, если граф не очень связан
    }
    return false;
}


int bfs(const IGraph* const graph, size_t vertex) {
    std::queue<size_t> q;
    q.push (vertex);
    std::vector<bool> used (graph->VerticesCount(), false);
    std::vector<int> length(graph->VerticesCount()), parent(graph->VerticesCount());
    used[vertex] = true;
    parent[vertex] = -1;
    while (!q.empty()) {
        size_t nextVertex = q.front();
        q.pop();
        for (size_t i=0; i<graph->GetVertices(nextVertex).size(); ++i) {
            size_t to = graph->GetVertices(nextVertex)[i];

            if (!used[to]) {
            used[to] = true;
            q.push (to);
            length[to] = length[nextVertex] + 1;
            parent[to] = static_cast<int>(nextVertex);
            }
            else {
            }
        }
    }
    return 0;
}

void minCycleConteiningVertex(const IGraph* const graph, size_t vertex, int& cicle_len) {
    std::queue<size_t> q;
    q.push (vertex);
    std::vector<bool> used (graph->VerticesCount(), false);
    std::vector<int> length(graph->VerticesCount());
    used[vertex] = true;
    while (!q.empty()) {
        size_t from = q.front();
        q.pop();
        for (auto to : graph->GetVertices(from)) {
            if (!used[to]) {
            used[to] = true;
            q.push (to);
            length[to] = length[from] + 1;
            }
            else
                if(length[to] == length[from]) {
                    cicle_len = (cicle_len == -1 ? 2*length[to] + 1 : std::min(cicle_len, 2*length[to] + 1));
                    return;
                }
                else if(length[to] == length[from] + 1) {
                    cicle_len = (cicle_len == -1 ? 2*length[to]: std::min(cicle_len, 2*length[to]));
                    return;
            }
        }
    }
}

int minCycle(const IGraph* const graph) {
    int minCycle = -1;
    for(size_t i = 0; i < graph->VerticesCount(); i++)
        minCycleConteiningVertex(graph, i, minCycle);

    return minCycle;
}

size_t kShortestPaths(const IGraph* const graph, size_t start, size_t finish){
    std::vector<size_t> shortest_paths(graph->VerticesCount());
    std::vector<bool> next_ring(graph->VerticesCount());
    std::queue<size_t> q;
    q.push (start);
    shortest_paths[start] = 1;
    while (!q.empty()) {
        size_t curVertex = q.front();
        q.pop();
        if(next_ring[curVertex] == true){
            if(shortest_paths[finish] != 0){
                return shortest_paths[finish];
            }
            next_ring.assign(next_ring.size(), false);
        }
        for (size_t nextVertex : graph->GetVertices(curVertex)) {

            if (shortest_paths[nextVertex] == 0) {
                shortest_paths[nextVertex] = shortest_paths[curVertex];
                q.push(nextVertex);
                next_ring[nextVertex] = true;
            }
            else if(next_ring[nextVertex]) {
                shortest_paths[nextVertex] += shortest_paths[curVertex];
            }
        }
    }
}

enum Type {
    NONE,
    FIRST,
    SECOND
};

Type t_rev(Type a){
    if(a == FIRST) return SECOND;
    if(a == SECOND) return FIRST;
}

std::string isBipartite(const IGraph* const graph) {
    std::queue<size_t> q;
    std::vector<bool> used (graph->VerticesCount(), false);
    std::vector<Type> Part(graph->VerticesCount(), NONE);
    size_t vertex = 0;
    q.push(vertex);
    used[vertex] = true;
    Part[vertex] = FIRST;
    while (!q.empty()) {
        size_t curVertex = q.front();
        q.pop();
        for (size_t nextVertex : graph->GetVertices(curVertex)) {
            if (!used[nextVertex]) {
                if(Part[nextVertex] == NONE)
                    Part[nextVertex] = t_rev(Part[curVertex]);

                else if(Part[nextVertex] != Part[curVertex])
                    return "NO";

                used[nextVertex] = true;
                q.push(nextVertex);
            }
            else {
                if(Part[nextVertex] == Part[curVertex])
                    return "NO";

            }
        }
    }
    return "YES";
}
#endif //DSF_GRAPH_H

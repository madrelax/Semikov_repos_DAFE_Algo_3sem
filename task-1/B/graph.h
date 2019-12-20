#ifndef DSF_GRAPH_H //Это нужно чтобы заголовочный файл не включался больше 1 раза в какой либо файл
#define DSF_GRAPH_H
#include <vector>
#include <algorithm>
#include <queue>

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

int minCycleConteiningVertex(const IGraph* const graph, size_t vertex) {
    int minCycle = -1;
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
                if(length[nextVertex] + length[to] < minCycle || minCycle == -1) {
                    minCycle = length[nextVertex] + length[to];
                    if(minCycle == 3){
                        return minCycle;
                    }
                }
            }
        }
    }
    return minCycle;
}

int minCycle(const IGraph* const graph) {
    int minCycle = -1;
    for(size_t nextVertex = 0; nextVertex < graph->VerticesCount(); nextVertex++){
        int VertexCycle = minCycleConteiningVertex(graph, nextVertex);
        if(VertexCycle < minCycle || minCycle == -1) {
            minCycle = VertexCycle;
            if(minCycle == 3){
                return minCycle;
            }
        }
    }
    return minCycle;
}
#endif //DSF_GRAPH_H

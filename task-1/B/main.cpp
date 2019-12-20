#include <iostream>
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

int main()
{
    size_t N;
    size_t numOfVertices;
    std::cin >> N >> numOfVertices;

    ListGraph graph(N);
    for(size_t i = 0; i < numOfVertices; i++){
        size_t A;
        size_t B;
        std::cin >> A >> B;

        graph.AddEdge(A,B);
    }
    std::cout << minCycle(&graph);
    return 0;
}

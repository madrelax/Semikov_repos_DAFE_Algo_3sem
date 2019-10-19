#include <vector>
#include <algorithm>
#include <queue>
#include <iostream>

enum Type {
    NONE,
    FIRST,
    SECOND
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

int main() {
    size_t V, N;
    std::cin >> V;
    std::cin >> N;
    ListGraph graph(V);
    for(size_t i = 0; i < N; i++) {
        size_t A, B;
        std::cin >> A >> B;
        graph.AddEdge(A, B);
    }
    std::cout << isBipartite(&graph);
}

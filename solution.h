class Solution {
public:
void bfs(const int &n, vector<int> &ans, unordered_map<int, vector<int>> &red,  unordered_map<int, vector<int>> &blue, int prev){
        int path = 1;
        vector<vector<pair<bool, bool>>> visit(n, vector<pair<bool, bool>>(n, make_pair(false, false)));
        
        queue<int> q; q.push(0);
        while(!q.empty()){
            queue<int> tmp;
            
            while(!q.empty()){
                int f = q.front(); q.pop();
                
				
                for(auto v: (!prev ? red[f] : blue[f]))
                    if(prev ? !visit[f][v].first : !visit[f][v].second){
                        prev ? visit[f][v].first = true : visit[f][v].second = true;
                        ans[v] = min(ans[v], path), tmp.push(v);
                    }          
                
            }
            prev ^= 1;
            path++;
            q = tmp;
        }
    }
    vector<int> shortestAlternatingPaths(int n, vector<vector<int>>& red_edges, vector<vector<int>>& blue_edges) {

        unordered_map<int, vector<int>> red;
        unordered_map<int, vector<int>> blue;
        
        for(auto e: red_edges)
            red[e[0]].push_back(e[1]);
        
        for(auto e: blue_edges)
            blue[e[0]].push_back(e[1]);
        
        vector<int> ans(n, INT_MAX);
        ans[0] = 0;
        
        bfs(n, ans, red, blue, 0);
        bfs(n, ans, red, blue, 1);
        
        for(int i=0; i<n; i++) ans[i] = (ans[i] == INT_MAX ? -1 : ans[i]);
        
        return ans;
        
    }
};

#include<iostream>
#include<vector>
using namespace std;

#define endl '\n'

vector<pair<int, int>> vertex[10001]; //입력받는 정점 및 그의 정보들
vector<pair<int, int>> fringe; //fringe set
vector<pair<int, int>> tree; //tree set

int find_min() { //fringe set 중 가장 작은 우선 순위 찾기
    int min_idx = 0;
    for (int i = 1; i < fringe.size(); i++) {
        if (fringe[i].second == fringe[min_idx].second) { //cost가 같을 때
            if (fringe[i].first < fringe[min_idx].first) { //건너편 정점 번호 비교 
                min_idx = i; //min_idx 갱신
            }
        }
        else if (fringe[i].second < fringe[min_idx].second) { //fringe[i] 가 더 작을 때
            min_idx = i;  //min_idx 갱신
        }
    }
    return min_idx; //min_idx 반환
}

int find_idx(int id) { //fringe의 destination과 같은 값 찾기
    for (int i = 0; i < fringe.size(); i++) {
        if (fringe[i].first == id) {
            return i; //같은 값이 있을 경우 해당 값 반환
        }
    }
    return -1; //같은 값이 없을 경우 -1 반환
}

void prim(int _start) {
    bool visited[10001] = { false, }; //tree set에 포함이 됐는지 판별하는 배열

    int min_num = -1;

    tree.push_back(make_pair(_start, 0)); //제일 첫 노드를 tree set에 입력
    visited[_start] = true; //첫 노드는 visited = true

    for (int i = 0; i < vertex[_start].size(); i++) {
        fringe.push_back(vertex[_start][i]); //fringe set에 첫 노드에 연결된 노드들을 입력
    }

    while (!fringe.empty()) { //fringe set이 비지 않을 때까지
        min_num = find_min(); //min_num은 fringe set 중 우선순위가 가장 높은 노드의 번호

        tree.push_back(fringe[min_num]); //tree set에 min_num에 해당하는 노드 push
        visited[fringe[min_num].first] = true; //tree set에 들어갔음(visited)를 표시
        int vertex_destination_idx = fringe[min_num].first;  //fringe[min_num]과 동일한 노드
        fringe.erase(fringe.begin() + min_num); //fringe set에서 min_num에 해당되는 노드 제거

        for (int i = 0; i < vertex[vertex_destination_idx].size(); i++) {
            int id = find_idx(vertex[vertex_destination_idx][i].first); 
            if (id == -1) { //새로 fringe set에 넣을 노드가 fringe set에 존재하지 않을 경우
                if (visited[vertex[vertex_destination_idx][i].first] == false) { //새로 fringe set에 넣을 노드가 이미 tree set에 존재할 경우
                    fringe.push_back(vertex[vertex_destination_idx][i]); //fringe set에 해당 노드들 추가
                }
            }
            else { //새로 fringe set에 넣을 노드가 이미 fringe set에 존재 할 경우 
                fringe[id].second = min(vertex[vertex_destination_idx][i].second, fringe[id].second); //해당 fringe노드의 cost를 vertex에 있는것과 비교하여 cost가 더 작은 값으로 갱신
            }
        }
    }
    //출력하기
    int sum = 0;
    for (int i = 0; i < tree.size(); i++) {
        sum += tree[i].second; //비용 합
    }
    cout << sum << " ";

    for (int i = 0; i < tree.size(); i++) {
        cout << tree[i].first << " "; //지나온 경로 출력
    }
    cout << endl;

    tree.clear(); //tree set 초기화
}

int main() {
    int n, m, q = 0; //n:vertex 수, m:edge 수, q:질의 수
    char p;
    cin >> n >> m >> q;

    while (m--) {
        int f1, f2, cost = 0; //f1:조형물1, f2:조형물2, cost:디자인 비용

        cin >> f1 >> f2 >> cost;

        //adjacency list graph / connected, undirected graph
        vertex[f1].push_back(make_pair(f2, cost));
        vertex[f2].push_back(make_pair(f1, cost));
    }

    while (q--) {
        int start = 0; //출발 조형물 번호
        cin >> p >> start; //질의

        prim(start); //prim algorithm 출력
    }
}
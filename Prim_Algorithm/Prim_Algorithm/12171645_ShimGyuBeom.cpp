#include<iostream>
#include<vector>
using namespace std;

#define endl '\n'

vector<pair<int, int>> vertex[10001]; //�Է¹޴� ���� �� ���� ������
vector<pair<int, int>> fringe; //fringe set
vector<pair<int, int>> tree; //tree set

int find_min() { //fringe set �� ���� ���� �켱 ���� ã��
    int min_idx = 0;
    for (int i = 1; i < fringe.size(); i++) {
        if (fringe[i].second == fringe[min_idx].second) { //cost�� ���� ��
            if (fringe[i].first < fringe[min_idx].first) { //�ǳ��� ���� ��ȣ �� 
                min_idx = i; //min_idx ����
            }
        }
        else if (fringe[i].second < fringe[min_idx].second) { //fringe[i] �� �� ���� ��
            min_idx = i;  //min_idx ����
        }
    }
    return min_idx; //min_idx ��ȯ
}

int find_idx(int id) { //fringe�� destination�� ���� �� ã��
    for (int i = 0; i < fringe.size(); i++) {
        if (fringe[i].first == id) {
            return i; //���� ���� ���� ��� �ش� �� ��ȯ
        }
    }
    return -1; //���� ���� ���� ��� -1 ��ȯ
}

void prim(int _start) {
    bool visited[10001] = { false, }; //tree set�� ������ �ƴ��� �Ǻ��ϴ� �迭

    int min_num = -1;

    tree.push_back(make_pair(_start, 0)); //���� ù ��带 tree set�� �Է�
    visited[_start] = true; //ù ���� visited = true

    for (int i = 0; i < vertex[_start].size(); i++) {
        fringe.push_back(vertex[_start][i]); //fringe set�� ù ��忡 ����� ������ �Է�
    }

    while (!fringe.empty()) { //fringe set�� ���� ���� ������
        min_num = find_min(); //min_num�� fringe set �� �켱������ ���� ���� ����� ��ȣ

        tree.push_back(fringe[min_num]); //tree set�� min_num�� �ش��ϴ� ��� push
        visited[fringe[min_num].first] = true; //tree set�� ����(visited)�� ǥ��
        int vertex_destination_idx = fringe[min_num].first;  //fringe[min_num]�� ������ ���
        fringe.erase(fringe.begin() + min_num); //fringe set���� min_num�� �ش�Ǵ� ��� ����

        for (int i = 0; i < vertex[vertex_destination_idx].size(); i++) {
            int id = find_idx(vertex[vertex_destination_idx][i].first); 
            if (id == -1) { //���� fringe set�� ���� ��尡 fringe set�� �������� ���� ���
                if (visited[vertex[vertex_destination_idx][i].first] == false) { //���� fringe set�� ���� ��尡 �̹� tree set�� ������ ���
                    fringe.push_back(vertex[vertex_destination_idx][i]); //fringe set�� �ش� ���� �߰�
                }
            }
            else { //���� fringe set�� ���� ��尡 �̹� fringe set�� ���� �� ��� 
                fringe[id].second = min(vertex[vertex_destination_idx][i].second, fringe[id].second); //�ش� fringe����� cost�� vertex�� �ִ°Ͱ� ���Ͽ� cost�� �� ���� ������ ����
            }
        }
    }
    //����ϱ�
    int sum = 0;
    for (int i = 0; i < tree.size(); i++) {
        sum += tree[i].second; //��� ��
    }
    cout << sum << " ";

    for (int i = 0; i < tree.size(); i++) {
        cout << tree[i].first << " "; //������ ��� ���
    }
    cout << endl;

    tree.clear(); //tree set �ʱ�ȭ
}

int main() {
    int n, m, q = 0; //n:vertex ��, m:edge ��, q:���� ��
    char p;
    cin >> n >> m >> q;

    while (m--) {
        int f1, f2, cost = 0; //f1:������1, f2:������2, cost:������ ���

        cin >> f1 >> f2 >> cost;

        //adjacency list graph / connected, undirected graph
        vertex[f1].push_back(make_pair(f2, cost));
        vertex[f2].push_back(make_pair(f1, cost));
    }

    while (q--) {
        int start = 0; //��� ������ ��ȣ
        cin >> p >> start; //����

        prim(start); //prim algorithm ���
    }
}
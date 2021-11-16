#include <iostream>
#include <string>
#include <vector>
using namespace std;

#define black true
#define red false
#define endl '\n'

class Node {
public:
	int id; //ID(필드1)			[A]
	string name; //이름(필드2)	[N] 
	int volume; //용량(필드3)	[S]
	int price;  //가격(필드4)	[P]
	bool color; //false=red, true=black

	Node* leftChildNode; //왼쪽자식노드
	Node* rightChildNode; //오른쪽자식노드
	Node* parentNode; //부모노드
	

	Node(int id, string name, int volume, int price, bool color) { //초기화
		this->id = id;
		this->name = name;
		this->volume = volume;
		this->price = price;
		this->color = color;

		this->leftChildNode = nullptr;
		this->rightChildNode = nullptr;
		this->parentNode = nullptr;
	}
};

class RedBlackTree {
public:
	int size; //insert하면서 증가
	int rangeX;
	int rangeY;
	double discountRate;

	Node* rootNode;

	RedBlackTree() { //초기화
		this->size = 0;
		this->rangeX = 0;
		this->rangeY = 0;
		this->discountRate = 0;

		this->rootNode = nullptr;
	}

	void inorderPrint(Node* currentNode) {
		if (currentNode == nullptr) {
			return;
		}
		inorderPrint(currentNode->leftChildNode);
		cout << currentNode->id << "(" << getDepth(currentNode->id) << ") ";
		inorderPrint(currentNode->rightChildNode);
	}

	int getDepth(int _id) { //id에 해당하는 node의 depth 찾기. depth of root = 0
		Node* tempNode = rootNode;
		int depth = 0; //깊이 카운터

		while (tempNode != nullptr) {
			if (tempNode->id == _id) {
				return depth; //현재 깊이 반환!
			}
			else if (tempNode->id > _id) {
				tempNode = tempNode->leftChildNode; //왼쪽노드로 이동
			}
			else if (tempNode->id < _id) {
			tempNode = tempNode->rightChildNode; //오른쪽 노드로 이동
			}

			depth++; //노드 이동할 때마다 깊이 증가
		}

		return depth;
	}

	Node* findNode(int _id) { //node가 존재하는지 찾기
		Node* tempNode = rootNode;

		while (tempNode != nullptr) {
			if (tempNode->id == _id) {
				return tempNode; //현재 노드 반환!
			}
			else if (tempNode->id > _id) {
				tempNode = tempNode->leftChildNode; //왼쪽 노드로 이동
			}
			else if (tempNode->id < _id) {
				tempNode = tempNode->rightChildNode; //오른쪽 노드로 이동
			}
		}

		return NULL; //존재하지 않을 경우 NULL반환
	}


	bool getUncleColor(Node* currentNode) {
		Node* dadNode = currentNode->parentNode;
		Node* grandpaNode = dadNode->parentNode;
		bool uncleColor = black; //external property를 대비해서 black으로 설정

		if (grandpaNode->leftChildNode == dadNode) { //uncle이 조부모 기준 오른쪽에 있을 경우
			if (grandpaNode->rightChildNode != nullptr) { //uncle이 없지 않으면
				uncleColor = grandpaNode->rightChildNode->color;
			}
		}
		else if (grandpaNode->rightChildNode == dadNode) { //uncle이 조부모 기준 왼쪽에 있을 경우
			if (grandpaNode->leftChildNode != nullptr) { //uncle이 없지 않으면
				uncleColor = grandpaNode->leftChildNode->color;
			}
		}

		return uncleColor;
	}

	void reStructure(Node* currentNode) { //restructuring->4가지 경우
		Node* dadNode = currentNode->parentNode;
		Node* grandpaNode = dadNode->parentNode;
		Node* greatgrandpaNode = grandpaNode->parentNode;

		if (grandpaNode->id < dadNode->id) {
			if (dadNode->id > currentNode->id) { //ppt 1번 케이스
				grandpaNode->rightChildNode = currentNode->leftChildNode;
				if (currentNode->leftChildNode != nullptr) { //current노드의 왼자식이 있을 때에만
					currentNode->leftChildNode->parentNode = grandpaNode; //관계 연결시키기
				}
				dadNode->leftChildNode = currentNode->rightChildNode;
				if (currentNode->rightChildNode != nullptr) { //current노드의 오른자식이 있을 때에만
					currentNode->rightChildNode->parentNode = dadNode; //관계 연결시키기
				}

				currentNode->leftChildNode = grandpaNode;
				grandpaNode->parentNode = currentNode;
				currentNode->rightChildNode = dadNode;
				dadNode->parentNode = currentNode;

				if (greatgrandpaNode != nullptr) { //증조부가 존재하면
					if (currentNode->id < greatgrandpaNode->id) { //증조부보다 작을 때
						greatgrandpaNode->leftChildNode = currentNode;
						currentNode->parentNode = greatgrandpaNode;
					}
					else if (currentNode->id > greatgrandpaNode->id) { //증조부 보다 클 때
						greatgrandpaNode->rightChildNode = currentNode;
						currentNode->parentNode = greatgrandpaNode;
					}
				}
				else { //증조부가 존재하지 않으면
					rootNode = currentNode; //current노드가 root노드가 됨
					currentNode->parentNode = nullptr; //current노드의 부모노드는 없음
				}
				
				grandpaNode->color = red;
				dadNode->color = red;
				currentNode->color = black;

				return;
			}
			else if (dadNode->id < currentNode->id) { //ppt 4번 케이스
				grandpaNode->rightChildNode = dadNode->leftChildNode;
				if (dadNode->leftChildNode != nullptr) {
					dadNode->leftChildNode->parentNode = grandpaNode;
				}
				dadNode->leftChildNode = grandpaNode;
				grandpaNode->parentNode = dadNode;

				if (greatgrandpaNode != nullptr) {
					if (dadNode->id < greatgrandpaNode->id) {
						greatgrandpaNode->leftChildNode = dadNode;
						dadNode->parentNode = greatgrandpaNode;
					}
					else if (dadNode->id > greatgrandpaNode->id) {
						greatgrandpaNode->rightChildNode = dadNode;
						dadNode->parentNode = greatgrandpaNode;
					}
				}
				else {
					rootNode = dadNode;
					dadNode->parentNode = nullptr;
				}
				
				grandpaNode->color = red;
				dadNode->color = black;
				currentNode->color = red;

				return;
			}
		}
		else if (grandpaNode->id > dadNode->id) {
			if (dadNode->id > currentNode->id) { //ppt 3번 케이스
				grandpaNode->leftChildNode = dadNode->rightChildNode;
				if (dadNode->rightChildNode != nullptr) {
					dadNode->rightChildNode->parentNode = grandpaNode;
				}
				dadNode->rightChildNode = grandpaNode;
				grandpaNode->parentNode = dadNode;

				if (greatgrandpaNode != nullptr) {
					if (dadNode->id < greatgrandpaNode->id) {
						greatgrandpaNode->leftChildNode = dadNode;
						dadNode->parentNode = greatgrandpaNode;
					}
					else if (dadNode->id > greatgrandpaNode->id){
						greatgrandpaNode->rightChildNode = dadNode;
						dadNode->parentNode = greatgrandpaNode;
					}
				}
				else {
					rootNode = dadNode;
					dadNode->parentNode = nullptr;
				}
				
				currentNode->color = red;
				dadNode->color = black;
				grandpaNode->color = red;

				return;
			}
			else if (dadNode->id < currentNode->id) { //ppt 2번 케이스
				dadNode->rightChildNode = currentNode->leftChildNode;
				if (currentNode->leftChildNode != nullptr) {
					currentNode->leftChildNode->parentNode = dadNode;
				}
				grandpaNode->leftChildNode = currentNode->rightChildNode;
				if (currentNode->rightChildNode != nullptr) {
					currentNode->rightChildNode->parentNode = grandpaNode;
				}
				currentNode->leftChildNode = dadNode;
				dadNode->parentNode = currentNode;
				currentNode->rightChildNode = grandpaNode;
				grandpaNode->parentNode = currentNode;

				if (greatgrandpaNode != nullptr) {
					if (currentNode->id < greatgrandpaNode->id) {
						greatgrandpaNode->leftChildNode = currentNode;
						currentNode->parentNode = greatgrandpaNode;
					}
					else if(currentNode->id > greatgrandpaNode->id) {
						greatgrandpaNode->rightChildNode = currentNode;
						currentNode->parentNode = greatgrandpaNode;
					}
				}
				else {
					rootNode = currentNode;
					currentNode->parentNode = nullptr;
				}

				dadNode->color = red;
				currentNode->color = black;
				grandpaNode->color = red;

				return;
			}
		}
	}

	Node* reColor(Node* currentNode) { //recoloring
		Node* grandpaNode = currentNode->parentNode->parentNode;

		grandpaNode->color = red; //조부모는 red
		grandpaNode->leftChildNode->color = black;
		grandpaNode->rightChildNode->color = black; //부모와 uncle 전부 black

		if (grandpaNode == rootNode) { //조부모가 rootnode 일 때
			grandpaNode->color = black;
		}

		return grandpaNode;
	}

	void doubleRed(Node* currentNode) {
		Node* tempNode = nullptr;
		//double red 일 수 없는 조건
		if (currentNode == rootNode) { //root인 경우
			return;
		}
		if (currentNode->color == black) { //해당 노드가 black일 때
			return;
		}
		if (currentNode->parentNode->color == black) { //해당 노드의 부모노드가 black일 때
			return;
		}
		//double red일 조건
		if (getUncleColor(currentNode) == black) { 
			reStructure(currentNode);

			return; //restructure후
		}
		else if (getUncleColor(currentNode) == red) {
			tempNode = reColor(currentNode);
			//recolor후에도 부모, 조부에서 double red 발생 가능
		}
		
		doubleRed(tempNode); //recoloring할 때 위로도 double red가 생기는지 확인하기
	}

	void Insert(int _id, string _name, int _volume, int _price) { //(1) 어플리케이션 등록
		Node* newNode = new Node(_id, _name, _volume, _price, red); //새로 입력된 노드는 항상 red
		
		if (size == 0) { //isEmpty()인 경우
			rootNode = newNode; //root입력
			rootNode->color = black; //root property
			
			cout << 0 << endl;
			this->size++;
		}
		else { //isEmpty()가 아닌 경우
			Node* tempNode = findNode(_id); //이미 존재하는 지 검색
			
			if (tempNode != NULL) { //이미 존재
				cout << getDepth(tempNode->id) << endl; //존재하는 노드의 depth 출력
			}
			//--------------------------------------------------------------DOUBLE RED POSSIBILITY-----
			else { //이미 존재하지 않음
				Node* currentNode = rootNode; 
				Node* saveNode = nullptr;
				while (currentNode != nullptr) {
					saveNode = currentNode;

					if (newNode->id < currentNode->id) { //_id가 현재 id보다 작을 때
						currentNode = currentNode->leftChildNode; //현재 노드의 왼쪽으로
					}
					else if (newNode->id > currentNode->id) { //_id가 현재 id보다 클 때
						currentNode = currentNode->rightChildNode;//현재 노드의 오른쪽으로
					}
				}
				
				if (newNode->id < saveNode->id) { //leftchild로 지정
					saveNode->leftChildNode = newNode;
					newNode->parentNode = saveNode;
				}
				else if(saveNode->id < newNode->id) { //rightchild로 지정
					saveNode->rightChildNode = newNode;
					newNode->parentNode = saveNode;
				} 

				doubleRed(newNode);

				cout <<  getDepth(newNode->id) << endl;
				this->size++;
			}
			//-----------------------------------------------------------------------------------------
		}
	}

	void Search(int _id) { //(2) 어플리케이션 검색
		Node* currentNode = findNode(_id);
		if (currentNode == NULL) {
			cout << "NULL" << endl;
		}
		else {
			cout << getDepth(currentNode->id) << " " << currentNode->name << " " << currentNode->volume << " " << currentNode->price << endl;
		}
	}

	void Update(int _id, string _name, int _volume, int _price) { //(3) 어플리케이션 업데이트
		Node* tempNode = findNode(_id); //이미 존재하는 지 검색

		if (tempNode == NULL) { //이미 존재하지 않음
			cout << "NULL" << endl;
		}
		else { //이미 존재함
			tempNode->name = _name;
			tempNode->volume = _volume;
			tempNode->price = _price;

			cout << getDepth(tempNode->id) << endl;
		}
	}

	void Discount(int _rangeX, int _rangeY, int _discountRate) { //(4) 어플리케이션 탐색
		this->rangeX = _rangeX;
		this->rangeY = _rangeY;
		this->discountRate = (double)(100 - _discountRate) / 100;

		inorderDiscount(rootNode); //root노드부터 검색 시작해서 해당구간 discount 진행
	}

	void disCount(Node* currentNode) { //node들 할인하기
		if (rangeX <= currentNode->id && currentNode->id <= rangeY) {
			currentNode->price = (currentNode->price) * discountRate;
		}
	}

	void inorderDiscount(Node* currentNode) { //중위순회
		if (currentNode->leftChildNode != nullptr) {
			inorderDiscount(currentNode->leftChildNode);
		}
		
		disCount(currentNode);

		if (currentNode->rightChildNode != nullptr) {
			inorderDiscount(currentNode->rightChildNode);
		}
	}

};

//==============================================================================================================================================
int main() {
	RedBlackTree APP;

	int iter = 0;
	char order;
	int A; //id
	string N; //name
	int S; //volume
	int P; //price
	int x; //rangeX
	int y; //rangeY
	int d; //discount rate

	cin >> iter;

	while (iter--) {
		cin >> order;

		if (order == 'I') { //Insert
			cin >> A >> N >> S >> P;

			APP.Insert(A, N, S, P);
		}
		else if (order == 'F') { //Search
			cin >> A;

			APP.Search(A);
		}
		else if (order == 'R') { //Update
			cin >> A >> N >> S >> P;

			APP.Update(A, N, S, P);
		}
		else if (order == 'D') { //Discount
			cin >> x >> y >> d;

			APP.Discount(x, y, d);
		}
	}
	

	return 0;
}
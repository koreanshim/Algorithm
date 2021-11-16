#include <iostream>
#include <string>
#include <vector>
using namespace std;

#define black true
#define red false
#define endl '\n'

class Node {
public:
	int id; //ID(�ʵ�1)			[A]
	string name; //�̸�(�ʵ�2)	[N] 
	int volume; //�뷮(�ʵ�3)	[S]
	int price;  //����(�ʵ�4)	[P]
	bool color; //false=red, true=black

	Node* leftChildNode; //�����ڽĳ��
	Node* rightChildNode; //�������ڽĳ��
	Node* parentNode; //�θ���
	

	Node(int id, string name, int volume, int price, bool color) { //�ʱ�ȭ
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
	int size; //insert�ϸ鼭 ����
	int rangeX;
	int rangeY;
	double discountRate;

	Node* rootNode;

	RedBlackTree() { //�ʱ�ȭ
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

	int getDepth(int _id) { //id�� �ش��ϴ� node�� depth ã��. depth of root = 0
		Node* tempNode = rootNode;
		int depth = 0; //���� ī����

		while (tempNode != nullptr) {
			if (tempNode->id == _id) {
				return depth; //���� ���� ��ȯ!
			}
			else if (tempNode->id > _id) {
				tempNode = tempNode->leftChildNode; //���ʳ��� �̵�
			}
			else if (tempNode->id < _id) {
			tempNode = tempNode->rightChildNode; //������ ���� �̵�
			}

			depth++; //��� �̵��� ������ ���� ����
		}

		return depth;
	}

	Node* findNode(int _id) { //node�� �����ϴ��� ã��
		Node* tempNode = rootNode;

		while (tempNode != nullptr) {
			if (tempNode->id == _id) {
				return tempNode; //���� ��� ��ȯ!
			}
			else if (tempNode->id > _id) {
				tempNode = tempNode->leftChildNode; //���� ���� �̵�
			}
			else if (tempNode->id < _id) {
				tempNode = tempNode->rightChildNode; //������ ���� �̵�
			}
		}

		return NULL; //�������� ���� ��� NULL��ȯ
	}


	bool getUncleColor(Node* currentNode) {
		Node* dadNode = currentNode->parentNode;
		Node* grandpaNode = dadNode->parentNode;
		bool uncleColor = black; //external property�� ����ؼ� black���� ����

		if (grandpaNode->leftChildNode == dadNode) { //uncle�� ���θ� ���� �����ʿ� ���� ���
			if (grandpaNode->rightChildNode != nullptr) { //uncle�� ���� ������
				uncleColor = grandpaNode->rightChildNode->color;
			}
		}
		else if (grandpaNode->rightChildNode == dadNode) { //uncle�� ���θ� ���� ���ʿ� ���� ���
			if (grandpaNode->leftChildNode != nullptr) { //uncle�� ���� ������
				uncleColor = grandpaNode->leftChildNode->color;
			}
		}

		return uncleColor;
	}

	void reStructure(Node* currentNode) { //restructuring->4���� ���
		Node* dadNode = currentNode->parentNode;
		Node* grandpaNode = dadNode->parentNode;
		Node* greatgrandpaNode = grandpaNode->parentNode;

		if (grandpaNode->id < dadNode->id) {
			if (dadNode->id > currentNode->id) { //ppt 1�� ���̽�
				grandpaNode->rightChildNode = currentNode->leftChildNode;
				if (currentNode->leftChildNode != nullptr) { //current����� ���ڽ��� ���� ������
					currentNode->leftChildNode->parentNode = grandpaNode; //���� �����Ű��
				}
				dadNode->leftChildNode = currentNode->rightChildNode;
				if (currentNode->rightChildNode != nullptr) { //current����� �����ڽ��� ���� ������
					currentNode->rightChildNode->parentNode = dadNode; //���� �����Ű��
				}

				currentNode->leftChildNode = grandpaNode;
				grandpaNode->parentNode = currentNode;
				currentNode->rightChildNode = dadNode;
				dadNode->parentNode = currentNode;

				if (greatgrandpaNode != nullptr) { //�����ΰ� �����ϸ�
					if (currentNode->id < greatgrandpaNode->id) { //�����κ��� ���� ��
						greatgrandpaNode->leftChildNode = currentNode;
						currentNode->parentNode = greatgrandpaNode;
					}
					else if (currentNode->id > greatgrandpaNode->id) { //������ ���� Ŭ ��
						greatgrandpaNode->rightChildNode = currentNode;
						currentNode->parentNode = greatgrandpaNode;
					}
				}
				else { //�����ΰ� �������� ������
					rootNode = currentNode; //current��尡 root��尡 ��
					currentNode->parentNode = nullptr; //current����� �θ���� ����
				}
				
				grandpaNode->color = red;
				dadNode->color = red;
				currentNode->color = black;

				return;
			}
			else if (dadNode->id < currentNode->id) { //ppt 4�� ���̽�
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
			if (dadNode->id > currentNode->id) { //ppt 3�� ���̽�
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
			else if (dadNode->id < currentNode->id) { //ppt 2�� ���̽�
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

		grandpaNode->color = red; //���θ�� red
		grandpaNode->leftChildNode->color = black;
		grandpaNode->rightChildNode->color = black; //�θ�� uncle ���� black

		if (grandpaNode == rootNode) { //���θ� rootnode �� ��
			grandpaNode->color = black;
		}

		return grandpaNode;
	}

	void doubleRed(Node* currentNode) {
		Node* tempNode = nullptr;
		//double red �� �� ���� ����
		if (currentNode == rootNode) { //root�� ���
			return;
		}
		if (currentNode->color == black) { //�ش� ��尡 black�� ��
			return;
		}
		if (currentNode->parentNode->color == black) { //�ش� ����� �θ��尡 black�� ��
			return;
		}
		//double red�� ����
		if (getUncleColor(currentNode) == black) { 
			reStructure(currentNode);

			return; //restructure��
		}
		else if (getUncleColor(currentNode) == red) {
			tempNode = reColor(currentNode);
			//recolor�Ŀ��� �θ�, ���ο��� double red �߻� ����
		}
		
		doubleRed(tempNode); //recoloring�� �� ���ε� double red�� ������� Ȯ���ϱ�
	}

	void Insert(int _id, string _name, int _volume, int _price) { //(1) ���ø����̼� ���
		Node* newNode = new Node(_id, _name, _volume, _price, red); //���� �Էµ� ���� �׻� red
		
		if (size == 0) { //isEmpty()�� ���
			rootNode = newNode; //root�Է�
			rootNode->color = black; //root property
			
			cout << 0 << endl;
			this->size++;
		}
		else { //isEmpty()�� �ƴ� ���
			Node* tempNode = findNode(_id); //�̹� �����ϴ� �� �˻�
			
			if (tempNode != NULL) { //�̹� ����
				cout << getDepth(tempNode->id) << endl; //�����ϴ� ����� depth ���
			}
			//--------------------------------------------------------------DOUBLE RED POSSIBILITY-----
			else { //�̹� �������� ����
				Node* currentNode = rootNode; 
				Node* saveNode = nullptr;
				while (currentNode != nullptr) {
					saveNode = currentNode;

					if (newNode->id < currentNode->id) { //_id�� ���� id���� ���� ��
						currentNode = currentNode->leftChildNode; //���� ����� ��������
					}
					else if (newNode->id > currentNode->id) { //_id�� ���� id���� Ŭ ��
						currentNode = currentNode->rightChildNode;//���� ����� ����������
					}
				}
				
				if (newNode->id < saveNode->id) { //leftchild�� ����
					saveNode->leftChildNode = newNode;
					newNode->parentNode = saveNode;
				}
				else if(saveNode->id < newNode->id) { //rightchild�� ����
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

	void Search(int _id) { //(2) ���ø����̼� �˻�
		Node* currentNode = findNode(_id);
		if (currentNode == NULL) {
			cout << "NULL" << endl;
		}
		else {
			cout << getDepth(currentNode->id) << " " << currentNode->name << " " << currentNode->volume << " " << currentNode->price << endl;
		}
	}

	void Update(int _id, string _name, int _volume, int _price) { //(3) ���ø����̼� ������Ʈ
		Node* tempNode = findNode(_id); //�̹� �����ϴ� �� �˻�

		if (tempNode == NULL) { //�̹� �������� ����
			cout << "NULL" << endl;
		}
		else { //�̹� ������
			tempNode->name = _name;
			tempNode->volume = _volume;
			tempNode->price = _price;

			cout << getDepth(tempNode->id) << endl;
		}
	}

	void Discount(int _rangeX, int _rangeY, int _discountRate) { //(4) ���ø����̼� Ž��
		this->rangeX = _rangeX;
		this->rangeY = _rangeY;
		this->discountRate = (double)(100 - _discountRate) / 100;

		inorderDiscount(rootNode); //root������ �˻� �����ؼ� �ش籸�� discount ����
	}

	void disCount(Node* currentNode) { //node�� �����ϱ�
		if (rangeX <= currentNode->id && currentNode->id <= rangeY) {
			currentNode->price = (currentNode->price) * discountRate;
		}
	}

	void inorderDiscount(Node* currentNode) { //������ȸ
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
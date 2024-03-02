#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;

//Task1
bool isEven(int n) {
    int tmp = n;
    tmp = tmp >> 1;
    tmp = tmp << 1;
    return tmp == n;
}
//Ваш вариант я считаю предпочтительнее, так как он более понятный. Тут я использую побитовый сдвиг. Это быстро.

//Task2
//Это мое решение задачи с лит кода по реализации кеша по принципу LRU. Работает как кольцевой буффер на двухсвязном списке.
//Но еще хранит информацию о том как часто используется запись. Т.е. при привышении размера будет перезаписываться
//наименее используемая запись.
class Node {
public:
    int key;
    int value;
    Node* next = nullptr;
    Node* prev = nullptr;
    Node(int key,int value) {
        this->key = key;
        this->value = value;
    }
};
class LRUCache {
    int capacity;
    Node* mru = nullptr;
    Node* lru = nullptr;
    unordered_map <int,Node*> u_map;
public:
    LRUCache(int capacity) {
        this->capacity = capacity;
        u_map.reserve(capacity);
    }

    int get(int key) {
        auto it = u_map.find(key);
        if (it != u_map.end()) {
            Node* tmp_ptr = it->second;
            if (tmp_ptr != mru) {
                if (tmp_ptr == lru) {
                    tmp_ptr->next->prev = nullptr;
                    lru = tmp_ptr->next;
                    tmp_ptr->next = nullptr;
                    tmp_ptr->prev = mru;
                    mru->next = tmp_ptr;
                    mru = tmp_ptr;
                } else {
                    tmp_ptr->prev->next = tmp_ptr->next;
                    tmp_ptr->next->prev = tmp_ptr->prev;
                    mru->next = tmp_ptr;
                    tmp_ptr->prev = mru;
                    tmp_ptr->next = nullptr;
                    mru = mru->next;
                }
            }
            return tmp_ptr->value;
        }
        return -1;
    }

    void put(int key, int value) {
        bool found = false;
        auto it = u_map.find(key);
        if (it != u_map.end()) {
            found = true;
            Node* tmp_ptr = it->second;
            if (tmp_ptr != mru) {
                if (tmp_ptr == lru) {
                    tmp_ptr->next->prev = nullptr;
                    lru = tmp_ptr->next;
                    tmp_ptr->next = nullptr;
                    tmp_ptr->prev = mru;
                    mru->next = tmp_ptr;
                    mru = tmp_ptr;
                } else {
                    tmp_ptr->prev->next = tmp_ptr->next;
                    tmp_ptr->next->prev = tmp_ptr->prev;
                    mru->next = tmp_ptr;
                    tmp_ptr->prev = mru;
                    tmp_ptr->next = nullptr;
                    mru = mru->next;
                }
            }
            tmp_ptr->value = value;
        }
        if (!found) {
            Node* ptr_node = new Node(key,value);
            if (u_map.size() == capacity) {
                u_map.erase(lru->key);
                Node* temp = lru;
                mru->next = ptr_node;
                ptr_node->prev = mru;
                mru = ptr_node;
                lru = lru->next;
                delete temp;
                lru->prev = nullptr;
                u_map.emplace(key,ptr_node);
            } else {
                u_map.emplace(key,ptr_node);
                ptr_node->prev = mru;
                if (mru) {
                    mru->next = ptr_node;
                }
                mru = ptr_node;
                if (!lru) {
                    lru = ptr_node;
                }
            }
        }
    }
};


//Далее очень простой цикличный буффер на векторе. Из+ простота и скорость.
template <typename T>
class CycleBuffer {
    int capacity;
    int current_index;
    vector <T> vec;
public:
    CycleBuffer(int cap) {
        capacity = cap;
        vec.reserve(capacity);
        current_index = 0;
    }
    void add(T t) {
        if (vec.size() < capacity) {
            vec.push_back(t);
        } else {
            vec[current_index] = t;
            if(++current_index == capacity) {
                current_index = 0;
            }
        }
    }
};

//Task 3
// На Яндекс Алгоритмах 4.0 писал свои сортировки. Быструю, Слиянием и по Разрядную. У всех свои плюсы.
//Быстрая мне понравилась больше всего. Могу рассказать как каждая работает на интерьвью.)
//Быстрая
void quickSort(int arr[],int l, int n) {
    if(n-l < 2) return;
    int p = rand() % (n - l) + l;
    int x = arr[p];
    int e = l;
    int g = l;
    int temp;
    for (int i = l; i < n; ++i) {
        if(arr[i] < x) {
            temp = arr[i];
            arr[i] = arr[g];
            arr[g] = arr[e];
            arr[e] = temp;
            ++e;
            ++g;
        } else if(arr[i]==x) {
            temp = arr[i];
            arr[i] = arr[g];
            arr[g] = temp;
            ++g;
        }
    }


    quickSort(arr,l,e);
    quickSort(arr,g,n);
}

//Слиянием
void merge(int*arr,int start,int end,int*buffer) {
    int mid = start + (end - start)/ 2;

    for (int i = start,i1 = start,i2 = mid; i < end;) {
        if(i1 == mid) arr[i++] = buffer[i2++];
        else if(i2 == end) arr[i++] = buffer[i1++];
        else if(buffer[i1] < buffer[i2]) arr[i++] = buffer[i1++];
        else arr[i++] = buffer[i2++];
    }
    for (int i = start; i < end; ++i) {
        buffer[i] = arr[i];
    }
}

void mergeSort(int *arr,int start, int end,int*buffer) {
    if(end-start <= 1) return;
    int mid = start + (end - start)/ 2;
    mergeSort(arr,start,mid,buffer);
    mergeSort(arr,mid,end,buffer);
    merge(arr,start,end,buffer);
}

//И поразрядная.
void bit_sort() {
    int n,m;
    cin >> n;
    vector<string> vec;
    vector<vector<string>> bucket{10};
    string temp;
    for (int i = 0; i < n; ++i) {
        cin >> temp;
        vec.push_back(temp);
    }
    m = vec[0].size()-1;
    cout << "Initial array:\n";
    for (int i = 0; i < n; ++i) {
        if(i) {
            cout << ", ";
        }
        cout << vec[i];
    }
    cout << "\n**********\n";
    for (int i = m; i >= 0; --i) {
        for (int j = 0; j < n; ++j) {
            bucket[vec[j][i]-48].push_back(vec[j]);
        }
        cout << "Phase " << m-i+1;

        for (int j = 0; j < 10; ++j) {
            if(bucket[j].empty()) {
                cout << "\nBucket " << j << ": empty";
            } else {
                cout << "\nBucket " << j << ": ";
                for (int k = 0; k < bucket[j].size(); ++k) {
                    if(k) {
                        cout << ", ";
                    }
                    cout << bucket[j][k];
                }
            }
        }
        cout << "\n**********\n";
        vec.clear();
        for (int j = 0; j < 10; ++j) {
            for (const auto & k : bucket[j]) {
                vec.push_back(k);
            }
        }
        for (int j = 0; j < 10; ++j) {
            bucket[j].clear();
        }
    }
    cout << "Sorted array:\n";
    for (int i = 0; i < n; ++i) {
        if(i) {
            cout << ", ";
        }
        cout << vec[i];
    }
}

int main() {

    return 0;
}

#include <iostream>
#include <vector>

template <typename K, typename T>
class HashTable {
	struct Pair {
		K _key;
		T _val;
		enum class status {
			EMPTY, OCCUPIED, DELETED
		};
		status _status;
		Pair() : _key(0), _val(0), _status(status::EMPTY) {};
	};
	std::vector<Pair> _data;
public:
	HashTable();
	int hash_function(K key);
	HashTable(size_t size);
	//Constructor 2 
	HashTable(const HashTable& other);
	~HashTable();
	HashTable& operator=(const HashTable& other);
	void print();
	void insert(K key, T value);
	void insert_or_assign(K key, T value);
	bool contains(T value);
	T* search(K key);
	bool erase(K key);
	int count(K key);
};

template <typename K, typename T>
int HashTable<K, T>::hash_function(K key) {
	int w = 64;
	int a = 97;
	double p = (static_cast<double>(a) / w * double(key));
	int hash = int(_data.size() * (p - static_cast<int>(p)));
	return hash;
}

template <typename K, typename T>
HashTable<K, T>::HashTable() {
	_data.resize(0);
}

template <typename K, typename T>
HashTable<K, T>::HashTable(size_t size) {
	if (size == 0) throw std::invalid_argument("Size = 0!");
		_data.resize(size);
}

template <typename K, typename T>
HashTable<K, T>::HashTable(const HashTable& other) {
	_data = other._data;
}

template <typename K, typename T>
HashTable<K, T>::~HashTable(){
	_data.clear();
	_data.resize(0);
}

template <typename K, typename T>
HashTable<K, T>& HashTable<K, T>::operator=(const HashTable& other) {
	if (this != &other) {
		_data = other._data;
	}
	return *this;
}

template <typename K, typename T>
void HashTable<K, T>::print() {
	if (_data.size() == 0) throw std::out_of_range("Hash table is empty!");
	for (size_t i = 0; i < _data.size(); ++i) {
		std::cout << "{" << _data[i]._key << " " << _data[i]._val << "}";
	}
	std::cout << std:: endl;
}

template <typename K, typename T>
void HashTable<K, T>::insert(K key, T value) {
	int index = hash_function(key);
	while (_data[index]._status == Pair::status::OCCUPIED) {
		index = (index + 1) % _data.size();
	}
	_data[index]._key = key;
	_data[index]._val = value;
	_data[index]._status = Pair::status::OCCUPIED;
}

template <typename K, typename T>
void HashTable<K, T>::insert_or_assign(K key, T value) {
	int index = hash_function(key);
	if (_data[index]._status == Pair::status::EMPTY) {
		_data[index]._key = key;
		_data[index]._val = value;
		_data[index]._status = Pair::status::OCCUPIED;
	}
	else {
		while (_data[index]._status == Pair::status::OCCUPIED || _data[index]._status == Pair::status::DELETED) {
			if (_data[index]._key == key) {
				_data[index]._val = value;
			}
			index = (index + 1) % _data.size();
		}
	}
}

template <typename K, typename T>
bool HashTable<K, T>::contains(T value) {
	for (auto& pair : _data) {
		if (pair._val == value) return true;
	}
	return false;
}

template <typename K, typename T>
T* HashTable<K, T>::search(K key) {
	int index = hash_function(key);
	while (_data[index]._status != Pair::status::EMPTY) {
		if (_data[index]._key == key && _data[index]._status == Pair::status::OCCUPIED) return &_data[index]._val;
		index = (index + 1) % _data.size();
	}
	return nullptr;
}

template <typename K, typename T>
bool HashTable<K, T>::erase(K key) {
	int index = hash_function(key);
	while (_data[index]._status != Pair::status::EMPTY) {
		if (_data[index]._key == key && _data[index]._status == Pair::status::OCCUPIED) {
			_data[index]._status = Pair::status::DELETED;
			return true;
		}
		index = (index + 1) % _data.size();
	}
	return false;
}

template <typename K, typename T>
int HashTable<K, T>::count(K key) {
	int index = hash_function(key);
	int count = 0;
	while (_data[index]._status != Pair::status::EMPTY) {
		if (_data[index]._key == key && _data[index]._status == Pair::status::OCCUPIED) count++;
		index = (index + 1) % _data.size();
	}
	return count;
}
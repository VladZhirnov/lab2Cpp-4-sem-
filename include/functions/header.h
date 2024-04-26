#include <iostream>
#include <vector>
#include <random>

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
	HashTable(const HashTable& other);
	~HashTable();
	HashTable& operator=(const HashTable& other);
	void print();
	void insert(K key, T value, int& count);
	void insert_or_assign(K key, T value);
	bool contains(T value);
	T* search(K key);
	bool erase(K key);
	int count(K key);
	double load_factor() const;
	void rehash();
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
double HashTable<K, T>::load_factor() const {
	size_t count = 0;
	for (auto& pair : _data)
	{
		if (pair._status == Pair::status::OCCUPIED)
			count++;
	}
	return (double)count / _data.size();
}

template <typename K, typename T>
void HashTable<K, T>::rehash() {
	std::vector<Pair> new_data(_data.size() * 2);
	for (auto& pair : _data)
	{
		if (pair._status == Pair::status::OCCUPIED)
		{
			size_t index = hash_function(pair._key);
			while (new_data[index]._status != Pair::status::EMPTY)
				index = (index + 1) % new_data.size();
			new_data[index] = pair;
		}
	}
	_data = new_data;
}

template <typename K, typename T>
void HashTable<K, T>::insert(K key, T value, int& count) {
	if (search(key) != nullptr) return;
	if (load_factor() >= 0.75) rehash();
	int index = hash_function(key);
	if (_data[index]._status == Pair::status::OCCUPIED) count++;
	while (_data[index]._status == Pair::status::OCCUPIED) {
		index = (index + 1) % _data.size();
	}
	_data[index]._key = key;
	_data[index]._val = value;
	_data[index]._status = Pair::status::OCCUPIED;
}

template <typename K, typename T>
void HashTable<K, T>::insert_or_assign(K key, T value) {
	if (search(key) != nullptr) return;
	if (load_factor() >= 0.75) rehash();
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

void simulate_and_plot_collisions(int num_experiments, int num_people_in_group, int min_table_size, int max_table_size, int step_size) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dist(1, 1000);
	std::vector<int> table_sizes;
	std::vector<double> collision_probabilities;

	for (int table_size = min_table_size; table_size <= max_table_size; table_size += step_size) {
		int total_collisions = 0;
		for (int exp = 0; exp < num_experiments; ++exp) {
			HashTable<int, int> hash_table(table_size);
			int count = 0;
			for (int i = 0; i < num_people_in_group; ++i) {
				int key = dist(gen);
				hash_table.insert(key, i, count);
			}

			total_collisions += count;
		}
		double collision_probability = static_cast<double>(total_collisions) / (num_experiments * num_people_in_group);
		table_sizes.push_back(table_size);
		collision_probabilities.push_back(collision_probability * 100);
	}

	int table_size_final = -1;
	for (size_t i = 0; i < collision_probabilities.size(); ++i) {
		if (collision_probabilities[i] == *std::min_element(begin(collision_probabilities), end(collision_probabilities))) {
			table_size_final = table_sizes[i];
			break;
		}
	}

	std::cout << "Table Size\tCollision Probability" << std::endl;
	for (size_t i = 0; i < table_sizes.size(); ++i) {
		std::cout << table_sizes[i] << "\t\t" << collision_probabilities[i] << std::endl;
	}
	std::cout << std::endl;
	std::cout << "Table size with min probability: " << table_size_final << std::endl;
}

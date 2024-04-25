#include <functions/header.h>

int main()
{
    HashTable<int, int> a(5);
    a.insert(106, 3);
    a.insert(106, 9);
    a.insert(106, 20);
    a.print();
    a.erase(106);
    a.print();
    a.insert(106, 99);
    a.print();
    //std::cout << a.count(106);
    //a.insert_or_assign(106, 1);
    //a.print();
    //int hash = a.hash_function(104);
}
#include <functions/header.h>

int main()
{
    //int count;
    //HashTable<int, int> a(5);
    //a.insert(106, 1, count);
    //a.insert(106, 2, count);
    //a.insert(106, 3, count);
    //a.print();
    int num_experiments = 100;
    int num_people_in_group = 26;
    int min_table_size = 25;
    int max_table_size = 475;
    int step_size = 50;
    simulate_and_plot_collisions(num_experiments, num_people_in_group, min_table_size, max_table_size, step_size);
}
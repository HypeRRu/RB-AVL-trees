#include "../avl/avl_tree.h"
#include "../rb/rb_tree.h"
#include "printers.h"
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <random>
#include <fstream>
#include <string>


const std::string avl_insert_file = "";
const std::string avl_delete_file = "";
const std::string rb_insert_file = "";
const std::string rb_delete_file = "";
const int nodes_count = 100000;


std::vector<int> generateData(int count = 10000);
template<class tree_type>
std::vector<std::pair<int, double>> insert_time(tree_type& tree, std::vector<int> data);
template<class tree_type>
std::vector<std::pair<int, double>> delete_time(tree_type& tree, std::vector<int> data);


int main()
{
	std::random_device rd;
    std::mt19937 g(rd());

	AVLTree<int> avl;
	RBTree<int> rb;

	std::ofstream avl_insert_fs(avl_insert_file);
	std::ofstream avl_delete_fs(avl_delete_file);
	std::ofstream rb_insert_fs(rb_insert_file);
	std::ofstream rb_delete_fs(rb_delete_file);

	std::vector<int> insert_data = generateData(nodes_count);
	std::vector<int> delete_data(insert_data);
	std::shuffle(delete_data.begin(), delete_data.end(), g);

	std::vector<std::pair<int, double>> avl_insertion, avl_deletion;
	std::vector<std::pair<int, double>> rb_insertion, rb_deletion;

	avl_insertion = insert_time<AVLTree<int>>(avl, insert_data);
	if (avl_insert_fs.is_open())
		for (auto& p: avl_insertion)
			avl_insert_fs << "(" << p.first << "; " << p.second << ")" << std::endl;
	avl_insert_fs.close();

	rb_insertion = insert_time<RBTree<int>>(rb, insert_data);
	if (rb_insert_fs.is_open())
		for (auto& p: rb_insertion)
			rb_insert_fs << "(" << p.first << "; " << p.second << ")" << std::endl;
	rb_insert_fs.close();

	avl_deletion = delete_time<AVLTree<int>>(avl, delete_data);
	if (avl_delete_fs.is_open())
		for (auto& p: avl_deletion)
			avl_delete_fs << "(" << p.first << "; " << p.second << ")" << std::endl;
	avl_delete_fs.close();

	rb_deletion = delete_time<RBTree<int>>(rb, delete_data);
	if (rb_delete_fs.is_open())
		for (auto& p: rb_deletion)
			rb_delete_fs << "(" << p.first << "; " << p.second << ")" << std::endl;
	rb_delete_fs.close();

	return 0;
}

std::vector<int> generateData(int count)
{
	std::srand(std::time(NULL));
	std::vector<int> data;
	data.reserve(count);
	int to_add;
	for (int i = 0; i < count; ++i)
	{
		to_add = std::rand() % (count * 10 + 1) - (count * 5);
		if(std::find(data.begin(), data.end(), to_add) == data.end())
			data.push_back(to_add);
	}
	return data;
}

template<class tree_type>
std::vector<std::pair<int, double>> insert_time(tree_type& tree, std::vector<int> data)
{
	int repeats = 20;
	double multiplier = 1000.0 / static_cast<double>(repeats);
	double avg_time = 0;
	std::vector<std::pair<int, double>> time_elapsed_for_op;
	for (int i = 0; i < data.size(); ++i)
	{
		if (i % 1000 == 0)
		{	
			avg_time = 0;
			for (int j = 0; j < repeats; ++j)
			{
				tree_type copy(tree);
				auto start = std::clock();
				copy.insertNode(data.at(i));
				auto end = std::clock();
				avg_time += (end - start) / static_cast<double>(CLOCKS_PER_SEC); // time in sec
			}
			time_elapsed_for_op.push_back(
			{
				i,
				multiplier * avg_time // average time in ms
			});
		}
		std::cout << "inserting element № " << i << std::endl;
		tree.insertNode(data.at(i));
	}
	return time_elapsed_for_op;
}

template<class tree_type>
std::vector<std::pair<int, double>> delete_time(tree_type& tree, std::vector<int> data)
{
	int repeats = 20;
	double multiplier = 1000.0 / static_cast<double>(repeats);
	double avg_time = 0;
	std::vector<std::pair<int, double>> time_elapsed_for_op;
	for (int i = 0; i < data.size(); ++i)
	{
		if (i % 1000 == 0)
		{	
			avg_time = 0;
			for (int j = 0; j < repeats; ++j)
			{
				tree_type copy(tree);
				auto start = std::clock();
				copy.deleteNode(data.at(i));
				auto end = std::clock();
				avg_time += (end - start) / static_cast<double>(CLOCKS_PER_SEC); // time in sec
			}
			time_elapsed_for_op.push_back(
			{
				data.size() - i,
				multiplier * avg_time // average time in ms
			});
		}
		std::cout << "deleting element № " << i << std::endl;
		tree.deleteNode(data.at(i));
	}
	return time_elapsed_for_op;
}

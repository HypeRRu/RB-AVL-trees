#include <vector>
#include <fstream>
#include <iostream>

const std::string avl_insert_ifile = "/home/hyper/C++/alg-cw/tests_results/avl_insert.txt";
const std::string avl_delete_ifile = "/home/hyper/C++/alg-cw/tests_results/avl_delete.txt";
const std::string rb_insert_ifile = "/home/hyper/C++/alg-cw/tests_results/rb_insert.txt";
const std::string rb_delete_ifile = "/home/hyper/C++/alg-cw/tests_results/rb_delete.txt";

const std::string avl_insert_ofile = "/home/hyper/C++/alg-cw/tests_results/avl_insert_processed.txt";
const std::string avl_delete_ofile = "/home/hyper/C++/alg-cw/tests_results/avl_delete_processed.txt";
const std::string rb_insert_ofile = "/home/hyper/C++/alg-cw/tests_results/rb_insert_processed.txt";
const std::string rb_delete_ofile = "/home/hyper/C++/alg-cw/tests_results/rb_delete_processed.txt";

std::vector<std::pair<int, double>> process_data(
	const std::vector<std::pair<int, double>>& data, 
	int window_size = 5
);

int main()
{
	char symbol;
	int n;
	double t;
	std::vector<std::pair<int, double>> data;
	std::vector<std::pair<int, double>> processed_data;

	std::vector<std::ifstream> input_files;
	std::vector<std::ofstream> output_files;

	input_files.push_back(std::ifstream(avl_insert_ifile));
	input_files.push_back(std::ifstream(avl_delete_ifile));
	input_files.push_back(std::ifstream(rb_insert_ifile));
	input_files.push_back(std::ifstream(rb_delete_ifile));
	
	output_files.push_back(std::ofstream(avl_insert_ofile));
	output_files.push_back(std::ofstream(avl_delete_ofile));
	output_files.push_back(std::ofstream(rb_insert_ofile));
	output_files.push_back(std::ofstream(rb_delete_ofile));

	for (int i = 0; i < input_files.size(); ++i)
	{
		if (input_files.at(i).is_open() && output_files.at(i).is_open())
		{
			while (input_files.at(i) >> symbol >> n >> symbol >> symbol >> t >> symbol)
			{
				data.push_back({n, t});	
			}
			processed_data = process_data(data);
			for (auto& r: processed_data)
				output_files.at(i) << "(" << r.first << "; " << r.second << ")" << std::endl;
			data.clear();
		}
	}

	return 0;
}

std::vector<std::pair<int, double>> process_data(
	const std::vector<std::pair<int, double>>& data, 
	int window_size
)
{
	std::vector<std::pair<int, double>> result;
	double window_data = 0;
	if (window_size > data.size())
		return result;
	else
		result.reserve(data.size() - window_size + 1);

	for (int i = 0; i < window_size; ++i)
		window_data += data.at(i).second;
	result.push_back({
		data.at((window_size - 1) + 1 - window_size + window_size / 2).first,
		window_data / static_cast<double>(window_size)
	});
	for (int i = window_size; i < data.size(); ++i)
	{
		window_data += data.at(i).second;
		window_data -= data.at(i - window_size).second;
		result.push_back({
			data.at(i + 1 - window_size + window_size / 2).first,
			window_data / static_cast<double>(window_size)
		});
	}
	return result;
}

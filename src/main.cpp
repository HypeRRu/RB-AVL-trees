#include <vector>
#include <iostream>

#include "avl/avl_tree.h"
#include "rb/rb_tree.h"
#include "utils/printers.h"
#include "utils/merge.h"

int main()
{
	std::vector<int> data = {
		5, 3, 4, 1, 7, 8, 2, 6, 0, 9
	};
	std::vector<int> _data = {
		180, 175, 200, 1, 195, 250, 4
	};
	AVLTree<int> avl(_data);
	std::cout << "avl" << std::endl;
	printBT(avl.getRoot());
	std::cout << std::endl;
	RBTree<int> rb(data);
	std::cout << "rb" << std::endl;
	printBT(rb.getRoot());
	std::cout << std::endl;

	mergeTrees(rb, avl);
	std::cout << "after merge" << std::endl;
	printBT(avl.getRoot());
	std::cout << std::endl;
	printBT(rb.getRoot());
	std::cout << std::endl;
	return 0;
}

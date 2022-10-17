#include "../avl/avl_tree.h"
#include "../rb/rb_tree.h"
#include "printers.h"
#include <vector>

int main()
{
	std::vector<int> data = {
		5, 3, 4, 1, 7, 8, 2, 6, 0, 9
	};
	std::vector<int> _data = {
		180, 175, 200, 1, 195, 250, 4
	};
	AVLTree<int> avl;
	RBTree<int> rb;

	std::cout << "AVL insert" << std::endl;
	for (int el: _data)
	{
		std::cout << "AVL insert node " << el << std::endl;
		avl.insertNode(el);
		printBT(avl.getRoot());
		std::cout << std::endl;
	}

	std::cout << "AVL delete" << std::endl;
	std::cout << "AVL deleteNode 4" << std::endl;
	avl.deleteNode(4);
	printBT(avl.getRoot());
	std::cout << std::endl;
	std::cout << "AVL deleteNode 180" << std::endl;
	avl.deleteNode(180);
	printBT(avl.getRoot());
	std::cout << std::endl;

	std::cout << "RB insert" << std::endl;
	for (int el: data)
	{
		std::cout << "RB insert node " << el << std::endl;
		rb.insertNode(el);
		printBT(rb.getRoot());
		std::cout << std::endl;
	}

	std::cout << "RB delete" << std::endl;
	std::cout << "RB deleteNode 7" << std::endl;
	rb.deleteNode(7);
	printBT(rb.getRoot());
	std::cout << std::endl;
	std::cout << "RB deleteNode 6" << std::endl;
	rb.deleteNode(6);
	printBT(rb.getRoot());
	std::cout << std::endl;
	std::cout << "RB deleteNode 8" << std::endl;
	rb.deleteNode(8);
	printBT(rb.getRoot());
	std::cout << std::endl;
	std::cout << "RB deleteNode 5" << std::endl;
	rb.deleteNode(5);
	printBT(rb.getRoot());
	std::cout << std::endl;
	std::cout << "RB deleteNode 9" << std::endl;
	rb.deleteNode(9);
	printBT(rb.getRoot());
	std::cout << std::endl;
	return 0;
}

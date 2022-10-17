#ifndef PRINTERS_H
#define PRINTERS_H

#include <iostream>
#include "../avl/avl_node.h"
#include "../rb/rb_node.h"

/* AVL tree print */
template<typename data_type>
void printBT(
	const std::string& prefix, 
	std::shared_ptr<AVLNode<data_type>> node, 
	bool isLeft
)
{
    if(node.get())
    {
        std::cout << prefix;

        std::cout << (isLeft ? "├── L " : "└── R " );

        // print the value of the node
        std::cout << node->getData() << std::endl;

        // enter the next tree level - left and right branch
        printBT( prefix + (isLeft ? "│   " : "    "), node->getLeftChild(), true);
        printBT( prefix + (isLeft ? "│   " : "    "), node->getRightChild(), false);
    }
}

template<typename data_type>
void printBT(std::shared_ptr<AVLNode<data_type>> node)
{
    printBT("", node, false);    
}


/* RB tree print */
template<typename data_type>
void printBT(
	const std::string& prefix, 
	std::shared_ptr<RBNode<data_type>> node, 
	bool isLeft
)
{
    if(node.get())
    {
        std::cout << prefix;

        std::cout << (isLeft ? "├── L " : "└── R " );

        // print the value of the node
        std::string clr = "black: ";
        if (node->getColor() == RBTreeColor::Red)
        	clr = "red: ";
        std::cout << clr;
        std::cout << node->getData() << std::endl;

        // enter the next tree level - left and right branch
        printBT( prefix + (isLeft ? "│   " : "    "), node->getLeftChild(), true);
        printBT( prefix + (isLeft ? "│   " : "    "), node->getRightChild(), false);
    }
}

template<typename data_type>
void printBT(std::shared_ptr<RBNode<data_type>> node)
{
    printBT("", node, false);    
}

#endif
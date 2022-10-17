#ifndef MERGE_H
#define MERGE_H

#include "../rb/rb_tree.h"
#include "../avl/avl_tree.h"

template<class tree1_type, class tree2_type>
void mergeTrees(tree1_type& tree1, tree2_type& tree2)
{
	while (tree2.getRoot().get())
	{
		auto element = tree2.getRoot()->getData();
		if (!tree1.find(element).get())
			tree1.insertNode(element);
		tree2.deleteNode(element);
	}
}

#endif
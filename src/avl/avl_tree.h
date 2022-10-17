#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <vector>
#include "avl_node.h"

template<typename data_type>
class AVLTree
{
public:
	AVLTree(): root{nullptr}
	{}

	AVLTree(const std::vector<data_type>& values)
	{
		this->root = nullptr;
		for (const data_type value: values)
			this->root = this->insertNode(value);
	}

	AVLTree(const AVLTree& other)
	{
		this->root = this->copyTreeFromNode(other.getRoot());
	}

	AVLTree(AVLTree&& other)
	{
		std::swap(this->root, other.root);
	}

	std::shared_ptr<AVLNode<data_type>> copyTreeFromNode(
		std::shared_ptr<AVLNode<data_type>> node
	)
	{
		if (node)
		{
			std::shared_ptr<AVLNode<data_type>> new_node = 
				std::make_shared<AVLNode<data_type>>(
					node->getData()
				);
			new_node->setRightChild(
				this->copyTreeFromNode(node->getRightChild())
			);
			new_node->setLeftChild(
				this->copyTreeFromNode(node->getLeftChild())
			);
			return new_node;
		} else
			return nullptr;
	}

	AVLTree<data_type>& operator =(const AVLTree<data_type>& other)
	{
		if (&other == this)
			return *this;

		this->root = this->copyTreeFromNode(other.getRoot());
		return *this;
	}

	AVLTree<data_type>& operator =(AVLTree<data_type>&& other)
	{
		if (&other == this)
			return *this;

		std::swap(this->root, other.root);
		return *this;
	}

	std::shared_ptr<AVLNode<data_type>> getRoot() const
	{
		return this->root;
	}

	std::shared_ptr<AVLNode<data_type>> find(const data_type& _data) const
	{
		std::shared_ptr<AVLNode<data_type>> node = this->root;
		while (node.get())
		{
			if (node->getData() == _data)
				break;
			node = node->getData() < _data ? 
				node->getRightChild() : 
				node->getLeftChild();
		}
		return node;
	}

	std::shared_ptr<AVLNode<data_type>> insertNode(const data_type& _data)
	{
		this->root = this->insertNodeRecursive(this->root, _data);
		return this->root;
	}

	std::shared_ptr<AVLNode<data_type>> deleteNode(const data_type& _data)
	{
		std::shared_ptr<AVLNode<data_type>> node = this->find(_data);
		if (!node.get())
			return this->root;
		this->root = this->deleteNodeRecursive(this->root, _data);
		return this->root;
	}

	std::shared_ptr<AVLNode<data_type>> getMinNode(
		std::shared_ptr<AVLNode<data_type>> node = nullptr
	) const
	{
		if (!node.get())
			node = this->root;
		while (node->getLeftChild().get())
			node = node->getLeftChild();
		return node;
	}

	std::shared_ptr<AVLNode<data_type>> getMaxNode(
		std::shared_ptr<AVLNode<data_type>> node = nullptr
	) const
	{
		if (!node.get())
			node = this->root;
		while (node->getRightChild().get())
			node = node->getRightChild();
		return node;
	}

	int getHeight() const
	{
		return this->root.get() ? this->root->getHeight() : 0;
	}
protected:
	std::shared_ptr<AVLNode<data_type>> rebalance(
		std::shared_ptr<AVLNode<data_type>> node
	)
	{
		node->updateHeight();
		int balance = node->getBalanceFactor();

		if (balance > 1)
		{
			if (
				node->getRightChild()->getRightChild()->getHeight() >
				node->getRightChild()->getLeftChild()->getHeight()
			)
			{
				node = this->singleRotateLeft(node);
			} else
			{
				node = this->doubleRotateLeft(node);
			}
		} else if (balance < -1)
		{
			if (
				node->getLeftChild()->getLeftChild()->getHeight() >
				node->getLeftChild()->getRightChild()->getHeight()
			)
			{
				node = this->singleRotateRight(node);
			} else
			{
				node = this->doubleRotateRight(node);
			}
		}
		return node;
	}

	/* rotates */
	std::shared_ptr<AVLNode<data_type>> singleRotateLeft(
		std::shared_ptr<AVLNode<data_type>> node
	)
	{
		std::shared_ptr<AVLNode<data_type>> right_child = 
			node->getRightChild();
		node->setRightChild(
			right_child->getLeftChild()
		);
		right_child->setLeftChild(
			node
		);

		node->updateHeight();
		right_child->updateHeight();

		node = right_child;
		return node;
	}

	std::shared_ptr<AVLNode<data_type>> singleRotateRight(
		std::shared_ptr<AVLNode<data_type>> node
	)
	{
		std::shared_ptr<AVLNode<data_type>> left_child = 
			node->getLeftChild();
		node->setLeftChild(
			left_child->getRightChild()
		);
		left_child->setRightChild(
			node
		);

		node->updateHeight();
		left_child->updateHeight();

		node = left_child;
		return node;
	}

	std::shared_ptr<AVLNode<data_type>> doubleRotateLeft(
		std::shared_ptr<AVLNode<data_type>> node
	)
	{
		std::shared_ptr<AVLNode<data_type>> node_right_child = 
			node->getRightChild(); 
		std::shared_ptr<AVLNode<data_type>> rc_left_child = 
			node_right_child->getLeftChild();

		node_right_child->setLeftChild(
			rc_left_child->getRightChild()
		);
		rc_left_child->setRightChild(
			node_right_child
		);
		node->setRightChild(
			rc_left_child->getLeftChild()
		);
		rc_left_child->setLeftChild(
			node
		);

		node->updateHeight();
		node_right_child->updateHeight();
		rc_left_child->updateHeight();

		node = rc_left_child;
		return node;
	}

	std::shared_ptr<AVLNode<data_type>> doubleRotateRight(
		std::shared_ptr<AVLNode<data_type>> node
	)
	{
		std::shared_ptr<AVLNode<data_type>> node_left_child = 
			node->getLeftChild(); 
		std::shared_ptr<AVLNode<data_type>> lc_right_child = 
			node_left_child->getRightChild();

		node_left_child->setRightChild(
			lc_right_child->getLeftChild()
		);
		lc_right_child->setLeftChild(
			node_left_child
		);
		node->setLeftChild(
			lc_right_child->getRightChild()
		);
		lc_right_child->setRightChild(
			node
		);

		node->updateHeight();
		node_left_child->updateHeight();
		lc_right_child->updateHeight();

		node = lc_right_child;
		return node;
	}

	/* special methods */
	std::shared_ptr<AVLNode<data_type>> insertNodeRecursive(
		std::shared_ptr<AVLNode<data_type>> node,
		const data_type& _data
	)
	{
		if (!node.get())
		{
			return std::make_shared<AVLNode<data_type>>(_data);
		}
		else if (node->getData() > _data)
		{
			node->setLeftChild(
				this->insertNodeRecursive(
					node->getLeftChild(), _data
				)
			);
		}
		else if (node->getData() < _data)
		{
			node->setRightChild(
				this->insertNodeRecursive(
					node->getRightChild(), _data
				)
			);
		}
		else
		{
			// data has to be unique 
			throw std::runtime_error{"Data has to be unique!"};
		}
		node = this->rebalance(node);
		return node;
	}

	std::shared_ptr<AVLNode<data_type>> deleteNodeRecursive(
		std::shared_ptr<AVLNode<data_type>> node, 
		const data_type& _data
	)
	{
		if (!node.get())
			return node;
		else if (node->getData() > _data)
		{
			node->setLeftChild(
				this->deleteNodeRecursive(
					node->getLeftChild(),
					_data
				)
			);
		} else if (node->getData() < _data)
		{
			node->setRightChild(
				this->deleteNodeRecursive(
					node->getRightChild(),
					_data
				)
			);
		} else
		{
			if (
				!node->getLeftChild().get() || 
				!node->getRightChild().get()
			)
			{
				 // node has just one child (or zero)
				node = !node->getLeftChild() ? 
					node->getRightChild() :
					node->getLeftChild();
			} else
			{
				 // node has two childs 
				/*std::shared_ptr<AVLNode<data_type>> mostLeftChild = 
					this->getMinNode(node->getRightChild());
				node->setData(
					mostLeftChild->getData()
				);
				node->setRightChild(
					this->deleteNodeRecursive(
						node->getRightChild(), 
						node->getData()
					)
				);*/
				std::shared_ptr<AVLNode<data_type>> mostRightChild = 
					this->getMaxNode(node->getLeftChild());
				node->setData(
					mostRightChild->getData()
				);
				node->setLeftChild(
					this->deleteNodeRecursive(
						node->getLeftChild(), 
						node->getData()
					)
				);
			}
		}

		if (node.get())
			node = this->rebalance(node);
		return node;
	}

	std::shared_ptr<AVLNode<data_type>> root;
};

#endif
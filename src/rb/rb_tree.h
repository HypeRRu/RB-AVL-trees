#ifndef RB_TREE_H
#define RB_TREE_H

#include <vector>
#include "rb_node.h"

template<typename data_type>
class RBTree
{
public:
	RBTree(): root{nullptr}
	{}

	RBTree(const std::vector<data_type>& values)
	{
		this->root = nullptr;
		for (const data_type value: values)
		{
			this->root = this->insertNode(value);
		}
	}

	RBTree(const RBTree& other)
	{
		this->root = this->copyTreeFromNode(other.getRoot());
	}

	RBTree(RBTree&& other)
	{
		std::swap(this->root, other.root);
	}

	std::shared_ptr<RBNode<data_type>> copyTreeFromNode(
		std::shared_ptr<RBNode<data_type>> node, 
		std::shared_ptr<RBNode<data_type>> copied_parent = nullptr
	)
	{
		if (node)
		{
			std::shared_ptr<RBNode<data_type>> new_node = 
				std::make_shared<RBNode<data_type>>(
					node->getData(),
					node->getColor()
				);
			new_node->setParent(copied_parent);
			new_node->setRightChild(
				this->copyTreeFromNode(node->getRightChild(), new_node)
			);
			new_node->setLeftChild(
				this->copyTreeFromNode(node->getLeftChild(), new_node)
			);
			return new_node;
		} else
			return nullptr;
	}

	RBTree<data_type>& operator =(const RBTree<data_type>& other)
	{
		if (&other == this)
			return *this;

		this->root = this->copyTreeFromNode(other.getRoot());
		return *this;
	}
	RBTree<data_type>& operator =(RBTree<data_type>&& other)
	{
		if (&other == this)
			return *this;

		std::swap(this->root, other.root);
		return *this;
	}

	std::shared_ptr<RBNode<data_type>> getRoot() const
	{
		return this->root;
	}

	std::shared_ptr<RBNode<data_type>> find(const data_type& _data) const
	{
		/* find */
		std::shared_ptr<RBNode<data_type>> node = this->root;
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

	std::shared_ptr<RBNode<data_type>> insertNode(const data_type& _data)
	{
		/* insertion */
		std::shared_ptr<RBNode<data_type>> node = 
			std::make_shared<RBNode<data_type>>(_data);
		this->insertNode(node);
		return this->root;
	}

	std::shared_ptr<RBNode<data_type>> deleteNode(const data_type& _data)
	{
		/* deletion */
		this->findAndRemove(_data);
		return this->root;
	}
protected:
	std::shared_ptr<RBNode<data_type>> getGrandParent(
		std::shared_ptr<RBNode<data_type>> node
	) const
	{
		if (node.get() && node->getParent().lock().get())
			return node->getParent().lock()->getParent().lock();
		else
			return nullptr;
	}

	std::shared_ptr<RBNode<data_type>> getUncle(
		std::shared_ptr<RBNode<data_type>> node
	) const
	{
		std::shared_ptr<RBNode<data_type>> grandparent = 
			this->getGrandParent(node);

		if (!grandparent.get())
			return nullptr;
		if (node->getParent().lock() == grandparent->getLeftChild())
			return grandparent->getRightChild();
		else
			return grandparent->getLeftChild();
	}

	/* rotates */
	std::shared_ptr<RBNode<data_type>> rotateLeft(
		std::shared_ptr<RBNode<data_type>> node
	)
	{
		std::shared_ptr<RBNode<data_type>> pivot = node->getRightChild();

		pivot->setParent(node->getParent().lock());
		if (node->getParent().lock().get())
		{
			if (node->getParent().lock()->getLeftChild() == node)
				node->getParent().lock()->setLeftChild(pivot);
			else
				node->getParent().lock()->setRightChild(pivot);
		}

		node->setRightChild(pivot->getLeftChild());
		if (pivot->getLeftChild().get())
			pivot->getLeftChild()->setParent(node);
		node->setParent(pivot);
		pivot->setLeftChild(node);
		if (!pivot->getParent().lock().get())
			this->root = pivot;
		
		return node;
	}

	std::shared_ptr<RBNode<data_type>> rotateRight(
		std::shared_ptr<RBNode<data_type>> node
	)
	{	
		std::shared_ptr<RBNode<data_type>> pivot = node->getLeftChild();

		pivot->setParent(node->getParent().lock());
		if (node->getParent().lock().get())
		{
			if (node->getParent().lock()->getLeftChild() == node)
				node->getParent().lock()->setLeftChild(pivot);
			else
				node->getParent().lock()->setRightChild(pivot);
		}

		node->setLeftChild(pivot->getRightChild());
		if (pivot->getRightChild().get())
			pivot->getRightChild()->setParent(node);
		node->setParent(pivot);
		pivot->setRightChild(node);
		if (!pivot->getParent().lock().get())
			this->root = pivot;

		return node;
	}

	/* special methods */
	bool insertNode(
		std::shared_ptr<RBNode<data_type>> node
	)
	{
		if (!this->root.get())
			this->root = node;
		else
		{
			std::shared_ptr<RBNode<data_type>> current = this->root;
			std::shared_ptr<RBNode<data_type>> previous = nullptr;
			while (current.get())
			{
				previous = current;
				if (current->getData() < node->getData())
					current = current->getRightChild();
				else if (current->getData() > node->getData())
					current = current->getLeftChild();
				else
				{
					// data has to be unique 
					throw std::runtime_error{"Data has to be unique!"};
					return false;
				}
			}
			node->setParent(previous);
			if (previous->getData() < node->getData())
				previous->setRightChild(node);
			else
				previous->setLeftChild(node);
		}
		this->fixBalanceCase1(node);

		return true;
	}

	bool findAndRemove(const data_type& _data)
	{
		if (!this->root.get())
			return false;
		std::shared_ptr<RBNode<data_type>> node = 
			this->find(_data);
		if (!node.get()) // node doesn't exists
			return false;

		if (
			node->getLeftChild().get() && 
			node->getRightChild().get()
		) // node has two childs
		{
			node = this->deleteCase1(node);
		}

		if (node->getColor() == RBTreeColor::Red)
			// node is red and has two NIL childs
			this->deleteCase2(node);
		// node is black
		else if (
			(
				node->getLeftChild().get() && 
				node->getLeftChild()->getColor() == RBTreeColor::Red
			) ||
			(
				node->getRightChild().get() && 
				node->getRightChild()->getColor() == RBTreeColor::Red
			)
		)
			// node is black and has one red child and one NIL child
			this->deleteCase3(node);
		else
		{
			// node is black and has two NIL childs
			this->deleteCase4(node); // rebalance tree before deleting
			// replace node with NIL
			if (
				node->getParent().lock().get() &&
				node->getParent().lock()->getLeftChild() == node
			)
				node->getParent().lock()->setLeftChild(nullptr);
			else if (node->getParent().lock().get())
				node->getParent().lock()->setRightChild(nullptr);
			else
				this->root = nullptr;
		}
		return true;
	}

	std::shared_ptr<RBNode<data_type>> deleteCase1(
		std::shared_ptr<RBNode<data_type>> node
	)
	{
		std::shared_ptr<RBNode<data_type>> descendant;
		descendant = node->getLeftChild();
		while (descendant->getRightChild().get())
			descendant = descendant->getRightChild(); // most right child of the node left child
		const data_type& nd = node->getData();
		node->setData(descendant->getData());

		return descendant;
	}

	void deleteCase2(
		std::shared_ptr<RBNode<data_type>> node
	)
	{
		if (node->getParent().lock()->getLeftChild() == node)
			node->getParent().lock()->setLeftChild(nullptr);
		else
			node->getParent().lock()->setRightChild(nullptr);
	}

	void deleteCase3(
		std::shared_ptr<RBNode<data_type>> node
	)
	{
		std::shared_ptr<RBNode<data_type>> child;
		if (
			node->getLeftChild().get() &&
			node->getLeftChild()->getColor() == RBTreeColor::Red
		)
			child = node->getLeftChild();
		else
			child = node->getRightChild();

		if (
			node->getParent().lock().get() &&
			node->getParent().lock()->getLeftChild() == node
		)
			node->getParent().lock()->setLeftChild(child);
		else if (
			node->getParent().lock().get() &&
			node->getParent().lock()->getRightChild() == node
		)
			node->getParent().lock()->setRightChild(child);

		child->setParent(node->getParent().lock());
		child->setColor(RBTreeColor::Black);
		if (!node->getParent().lock().get())
			this->root = child;
	}

	void deleteCase4(
		std::shared_ptr<RBNode<data_type>> node
	) 
	{
		if (!node->getParent().lock().get())
			return;
	  
		this->deleteCase4_1(node);
		this->deleteCase4_2(node);
		this->deleteCase4_3(node);
		this->deleteCase4_4(node);
	}

	void deleteCase4_1(
		std::shared_ptr<RBNode<data_type>> node
	)
	{
		std::shared_ptr<RBNode<data_type>> sibling;
		if (node->getParent().lock()->getLeftChild() == node)
		{
			sibling = node->getParent().lock()->getRightChild();
			if (sibling->getColor() == RBTreeColor::Red)
			{
				this->rotateLeft(node->getParent().lock());
				sibling->setColor(RBTreeColor::Black);
				node->getParent().lock()->setColor(RBTreeColor::Red);
			} else return;
		} else
		{
			sibling = node->getParent().lock()->getLeftChild();
			if (sibling->getColor() == RBTreeColor::Red)
			{
				this->rotateRight(node->getParent().lock());
				sibling->setColor(RBTreeColor::Black);
				node->getParent().lock()->setColor(RBTreeColor::Red);
			} else return;
		}
	}

	void deleteCase4_2(
		std::shared_ptr<RBNode<data_type>> node
	)
	{
		std::shared_ptr<RBNode<data_type>> sibling;
		if (node->getParent().lock()->getLeftChild() == node)
			sibling = node->getParent().lock()->getRightChild();
		else
			sibling = node->getParent().lock()->getLeftChild();
		if (
			// parent, sibling and two his childs are black
			node->getParent().lock()->getColor() == RBTreeColor::Black &&
			sibling->getColor() == RBTreeColor::Black &&
			(
				!sibling->getLeftChild().get() ||
				sibling->getLeftChild()->getColor() == RBTreeColor::Black
			) &&
			(
				!sibling->getRightChild().get() ||
				sibling->getRightChild()->getColor() == RBTreeColor::Black
			)
		)
		{
			sibling->setColor(RBTreeColor::Red);
			this->deleteCase4(node->getParent().lock());
		}

		if (node->getParent().lock()->getLeftChild() == node)
			sibling = node->getParent().lock()->getRightChild();
		else
			sibling = node->getParent().lock()->getLeftChild();

		if (
			// parent is red, sibling and two his childs are black
			node->getParent().lock()->getColor() == RBTreeColor::Red &&
			sibling->getColor() == RBTreeColor::Black &&
			(
				!sibling->getLeftChild().get() ||
				sibling->getLeftChild()->getColor() == RBTreeColor::Black
			) &&
			sibling->getColor() == RBTreeColor::Black &&
			(
				!sibling->getRightChild().get() ||
				sibling->getRightChild()->getColor() == RBTreeColor::Black
			)
		)
		{
			sibling->setColor(RBTreeColor::Red);
			node->getParent().lock()->setColor(RBTreeColor::Black);
		}
	}

	void deleteCase4_3(
		std::shared_ptr<RBNode<data_type>> node
	)
	{
		std::shared_ptr<RBNode<data_type>> sibling;
		if (node->getParent().lock()->getLeftChild() == node)
		{
			sibling = node->getParent().lock()->getRightChild();
			if (
				// brother is black, child on the same side with node is black, other is red
				sibling->getColor() == RBTreeColor::Black &&
				(
					sibling->getLeftChild().get() &&
					sibling->getLeftChild()->getColor() == RBTreeColor::Red
				) &&
				(
					!sibling->getRightChild().get() ||
					sibling->getRightChild()->getColor() == RBTreeColor::Black
				)
			)
			{
				sibling->setColor(RBTreeColor::Red);
				sibling->getLeftChild()->setColor(RBTreeColor::Black);
				this->rotateRight(sibling);
			}
		} else
		{
			sibling = node->getParent().lock()->getLeftChild();
			if (
				// brother is black, child on the same side with node is black, other is red
				sibling->getColor() == RBTreeColor::Black &&
				(
					sibling->getRightChild().get() &&
					sibling->getRightChild()->getColor() == RBTreeColor::Red
				) &&
				(
					!sibling->getLeftChild().get() ||
					sibling->getLeftChild()->getColor() == RBTreeColor::Black
				)
			)
			{
				sibling->setColor(RBTreeColor::Red);
				sibling->getRightChild()->setColor(RBTreeColor::Black);
				this->rotateLeft(sibling);
			}
		}
	}

	void deleteCase4_4(
		std::shared_ptr<RBNode<data_type>> node
	)
	{
		std::shared_ptr<RBNode<data_type>> sibling;
		if (node->getParent().lock()->getLeftChild() == node)
		{
			sibling = node->getParent().lock()->getRightChild();
			if (
				// brother is black, child on the opposite side of node is red
				sibling->getColor() == RBTreeColor::Black &&
				(
					sibling->getRightChild().get() &&
					sibling->getRightChild()->getColor() == RBTreeColor::Red
				)
			)
			{
				sibling->setColor(
					node->getParent().lock()->getColor()
				);
				node->getParent().lock()->setColor(RBTreeColor::Black);
				sibling->getRightChild()->setColor(RBTreeColor::Black);
				this->rotateLeft(node->getParent().lock());
			}
		} else
		{
			sibling = node->getParent().lock()->getLeftChild();
			if (
				// brother is black, child on the opposite side of node is red
				sibling->getColor() == RBTreeColor::Black &&
				(
					sibling->getLeftChild().get() &&
					sibling->getLeftChild()->getColor() == RBTreeColor::Red
				)
			)
			{
				sibling->setColor(
					node->getParent().lock()->getColor()
				);
				node->getParent().lock()->setColor(RBTreeColor::Black);
				sibling->getLeftChild()->setColor(RBTreeColor::Black);
				this->rotateRight(node->getParent().lock());
			}
		}
	}

	/* insertion fix methods */
	void fixBalanceCase1(
		std::shared_ptr<RBNode<data_type>> node
	)
	{
		if (!node->getParent().lock().get())
			node->setColor(RBTreeColor::Black);
		else
			this->fixBalanceCase2(node);
	}

	void fixBalanceCase2(
		std::shared_ptr<RBNode<data_type>> node
	)
	{
		if (node->getParent().lock()->getColor() == RBTreeColor::Black)
			return; // tree is still valid 
		else
			this->fixBalanceCase3(node);
	}

	void fixBalanceCase3(
		std::shared_ptr<RBNode<data_type>> node
	)
	{
		std::shared_ptr<RBNode<data_type>> uncle = 
			this->getUncle(node);
		std::shared_ptr<RBNode<data_type>> grandparent;
		if (uncle.get() && uncle->getColor() == RBTreeColor::Red)
		{
			node->getParent().lock()->setColor(
				RBTreeColor::Black
			);
			uncle->setColor(
				RBTreeColor::Black
			);
			grandparent = this->getGrandParent(node);

			grandparent->setColor(
				RBTreeColor::Red
			);
			this->fixBalanceCase1(grandparent);
		} else
			this->fixBalanceCase4(node);
	}

	void fixBalanceCase4(
		std::shared_ptr<RBNode<data_type>> node
	)
	{
		std::shared_ptr<RBNode<data_type>> grandparent = 
			this->getGrandParent(node);
		if (
			node == node->getParent().lock()->getRightChild() && 
			node->getParent().lock() == grandparent->getLeftChild()
		)
		{
			this->rotateLeft(node->getParent().lock());
			node = node->getLeftChild();
		} else if (
			node == node->getParent().lock()->getLeftChild() &&
			node->getParent().lock() == grandparent->getRightChild()
		)
		{
			this->rotateRight(node->getParent().lock());
			node = node->getRightChild();
		}
		this->fixBalanceCase5(node);
	}

	void fixBalanceCase5(
		std::shared_ptr<RBNode<data_type>> node
	)
	{
		std::shared_ptr<RBNode<data_type>> grandparent = 
			this->getGrandParent(node);
		node->getParent().lock()->setColor(RBTreeColor::Black);
		grandparent->setColor(RBTreeColor::Red);
		if (
			node == node->getParent().lock()->getLeftChild() &&
			node->getParent().lock() == grandparent->getLeftChild()
		)
			this->rotateRight(grandparent);
		else
			this->rotateLeft(grandparent);
	}

	std::shared_ptr<RBNode<data_type>> root;
};

#endif
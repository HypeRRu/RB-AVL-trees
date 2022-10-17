#ifndef AVL_NODE_H
#define AVL_NODE_H

#include <memory>
#include <iostream>

template<typename data_type>
class AVLNode
{
public:
	AVLNode(
		const data_type& _data,
		std::shared_ptr<AVLNode<data_type>> _left = nullptr,
		std::shared_ptr<AVLNode<data_type>> _right = nullptr
	): left{_left}, right{_right}
	{
		this->setData(_data);
		this->updateHeight();
	}

	~AVLNode() = default;

	std::shared_ptr<AVLNode<data_type>> getLeftChild() const
	{
		return this->left;
	}

	std::shared_ptr<AVLNode<data_type>> getRightChild() const
	{
		return this->right;
	}

	const data_type& getData() const
	{
		return *this->data.get();
	}

	void setData(const data_type& _data)
	{
		this->data = std::make_shared<data_type>(_data);
	}

	void updateHeight()
	{
		int left_height = this->left.get() ? this->left->getHeight() : 0;
		int right_height = this->right.get() ? this->right->getHeight() : 0;
		this->height = std::max(left_height, right_height) + 1;
	}

	void setLeftChild(std::shared_ptr<AVLNode<data_type>> _left)
	{
		this->left = _left;
		this->updateHeight();
	}

	void setRightChild(std::shared_ptr<AVLNode<data_type>> _right)
	{
		this->right = _right;
		this->updateHeight();
	}

	int getBalanceFactor() const
	{
		int left_height = this->left.get() ? this->left->getHeight() : 0;
		int right_height = this->right.get() ? this->right->getHeight() : 0;
		return right_height - left_height;
	}

	int getHeight() const
	{
		if (!this)
			return 0;
		return this->height;
	}

protected:
	std::shared_ptr<data_type> data;
	std::shared_ptr<AVLNode<data_type>> left;
	std::shared_ptr<AVLNode<data_type>> right;
	int height;
};

#endif
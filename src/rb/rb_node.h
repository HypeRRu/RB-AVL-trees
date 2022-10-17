#ifndef RB_NODE_H
#define RB_NODE_H

#include <memory>
#include <iostream>

enum class RBTreeColor
{
	Black,
	Red
};

template<typename data_type>
class RBNode
{
public:
	RBNode(
		const data_type& _data,
		RBTreeColor _color = RBTreeColor::Red
	):	left{nullptr}, right{nullptr}
	{
		this->setData(_data);
		this->setColor(_color);
	}

	~RBNode() = default;

	std::shared_ptr<RBNode<data_type>> getLeftChild() const
	{
		if (!this)
			return nullptr;
		return this->left;
	}

	std::shared_ptr<RBNode<data_type>> getRightChild() const
	{
		if (!this)
			return nullptr;
		return this->right;
	}

	std::weak_ptr<RBNode<data_type>> getParent() const
	{
		return this->parent;
	}
	
	const data_type& getData() const
	{
		return *this->data.get();
	}

	void setData(const data_type& _data)
	{
		this->data = std::make_shared<data_type>(_data);
	}

	void setLeftChild(std::shared_ptr<RBNode<data_type>> _left)
	{
		this->left = _left;
	}

	void setRightChild(std::shared_ptr<RBNode<data_type>> _right)
	{
		this->right = _right;
	}

	void setParent(std::shared_ptr<RBNode<data_type>> _parent)
	{
		this->parent = _parent;
	}

	RBTreeColor getColor() const
	{
		if (!this)
			return RBTreeColor::Black;
		return this->color;
	}

	void setColor(RBTreeColor _color)
	{
		this->color = _color;
	}
protected:
	std::shared_ptr<RBNode<data_type>> left;
	std::shared_ptr<RBNode<data_type>> right;
	std::weak_ptr<RBNode<data_type>> parent;
	std::shared_ptr<data_type> data;
	RBTreeColor color;
};

#endif
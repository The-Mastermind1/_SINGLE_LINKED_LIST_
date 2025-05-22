#pragma once
#include"Header1.h"
#include<iostream>
#include<utility>
#include<cstdlib>
#include<type_traits>
#include<new>
#include<initializer_list>
#include<queue>
#include<vector>
_PANAGIOTIS_BEGIN
#if __cplusplus > 202002L
template<typename _Ty>
class SingleLinkedList {
private:
	class ListNode {
	private:
		class secretClass {};
		template<class ..._Valty>
		ListNode(secretClass, _Valty&& ..._Val) :data{ std::forward<_Valty>(_Val)... }, next{ nullptr }
		{
			//constructs the object in place
		}
	public:
		_Ty data;
		ListNode* next;
		ListNode(const _Ty& item)noexcept(noexcept(data = item))
			:next{ nullptr }
		{

			data = item;
		}
		ListNode(_Ty&& item) noexcept(noexcept(data = std::move(item)))
			:next{ nullptr }
		{

			data = std::move(item);
		}
		template<class ..._Valty>
		static ListNode* craft(_Valty&& ..._Val) {

			ListNode* ptr = new ListNode{ secretClass{},std::forward<_Valty>(_Val)... };
			return ptr;

		}
	};
	std::size_t count;
	ListNode* head;
	ListNode* tail;
public:
	static_assert(std::is_object_v<_Ty>, "The C++ Standard forbids container adaptors of non-object types "
		"because of [container.requirements].");
	static_assert(!std::is_reference_v<_Ty>, "no references allowed");
	static_assert(!std::is_const_v<_Ty>, "no const types are allowed");
	static_assert(!std::is_volatile_v<_Ty>, "no volatile types are allowed");

	SingleLinkedList()noexcept :count{ 0 }, head{ nullptr }, tail{ nullptr } {

	}
	SingleLinkedList(const std::initializer_list<_Ty>& l) :head{ nullptr }, tail{ nullptr }, count{ 0 }
	{
		const _Ty* b = l.begin();
		for (std::size_t i = 0; i < l.size(); i++) {
			if (push_back(*b)) {
				b++;
			}
			else {
				this->~SingleLinkedList();
				break;
			}

		}
	}
	SingleLinkedList(const SingleLinkedList<_Ty>& other) :count{ 0 }, head{ nullptr }, tail{ nullptr }
	{
		if (this != &other) {
			ListNode* ptr1{ other.head };

			while (ptr1 != nullptr) {

				if (!this->emplace_back(ptr1->data)) {
					this->clear();
					break;
				}
				ptr1 = ptr1->next;

			}
		}
	}
	SingleLinkedList(SingleLinkedList<_Ty>&& other)noexcept :count{ 0 }, head{ nullptr }, tail{ nullptr }
	{
		if (this != &other) {
			head = other.head;
			tail = other.tail;
			count = other.count;
			other.head = other.tail = nullptr;
			other.count = 0;
		}
	}

	bool push_back(const _Ty& data)
	{
		ListNode* ptr = new (std::nothrow)ListNode(data);
		if (count != 0) {
			if (ptr != nullptr) {
				tail->next = ptr;
				tail = tail->next;
				count++;
				return true;
			}
		}
		else {
			if (ptr != nullptr) {
				head = tail = ptr;
				count++;
				return true;
			}
		}
		return false;
	}
	bool push_back(_Ty&& data)
	{
		ListNode* ptr = new (std::nothrow)ListNode(std::move(data));
		if (count != 0) {
			if (ptr != nullptr) {
				tail->next = ptr;
				tail = tail->next;
				count++;
				return true;
			}
		}
		else {
			if (ptr != nullptr) {
				head = tail = ptr;
				count++;
				return true;
			}
		}
		return false;
	}
	bool push_front(const _Ty& data) {
		ListNode* ptr = new (std::nothrow)ListNode(data);
		if (count != 0) {
			if (ptr != nullptr) {
				ptr->next = head;
				head = ptr;
				count++;
				return true;
			}
		}
		else {
			if (ptr != nullptr) {
				head = tail = ptr;
				count++;
				return true;
			}
		}
		return false;
	}
	bool push_front(_Ty&& data) {
		ListNode* ptr = new (std::nothrow)ListNode(std::move(data));
		if (count != 0) {
			if (ptr != nullptr) {
				ptr->next = head;
				head = ptr;
				count++;
				return true;
			}
		}
		else {
			if (ptr != nullptr) {
				head = tail = ptr;
				count++;
				return true;
			}
		}
		return false;
	}

	void pop_front()
	{
		if (count != 0) {
			ListNode* ptr = head;
			head = head->next;
			if (count == 1) {
				tail = nullptr;
			}
			count--;
			delete ptr;
		}
		else {
			throw pop_from_an_empty_list{ "tried to pop from an empty list" };
		}
	}
	void pop_back()
	{
		if (count != 0) {
			delete tail;
			count--;
			if (count == 0) {
				tail = nullptr;
				head = nullptr;
				return;
			}
			tail = head;


			for (std::size_t i = 0; i < count - 1; i++) {
				tail = tail->next;
			}
			tail->next = nullptr;
		}
		else {
			throw pop_from_an_empty_list{ "tried to pop from an empty list" };
		}
	}

	_NODISCARD std::size_t size()const noexcept {
		return count;
	}
	_NODISCARD bool empty()const noexcept {
		return count == 0;
	}

	_NODISCARD _Ty back()&& {
		if (count == 0) {
			throw tried_to_access_an_empty_list_{ "tried to access  an empty list" };
		}
		return std::move(tail->data);

	}
	_NODISCARD const _Ty back()const&& {
		if (count == 0) {
			throw tried_to_access_an_empty_list_{ "tried to access  an empty list" };
		}
		return std::move(tail->data);

	}
	_NODISCARD const _Ty& back()const& {
		if (count == 0) {
			throw tried_to_access_an_empty_list_{ "tried to access  an empty list" };
		}
		return tail->data;

	}
	_NODISCARD _Ty& back()& {
		if (count == 0) {
			throw tried_to_access_an_empty_list_{ "tried to access  an empty list" };
		}
		return tail->data;

	}

	_NODISCARD const _Ty& front()const& {
		if (count == 0) {
			throw tried_to_access_an_empty_list_{ "tried to access  an empty list" };
		}
		return head->data;

	}
	_NODISCARD _Ty& front()& {
		if (count == 0) {
			throw tried_to_access_an_empty_list_{ "tried to access  an empty list" };
		}
		return head->data;

	}
	_NODISCARD _Ty front()&& {
		if (count == 0) {
			throw tried_to_access_an_empty_list_{ "tried to access  an empty list" };
		}
		return std::move(head->data);

	}
	_NODISCARD _Ty front()const&& {
		if (count == 0) {
			throw tried_to_access_an_empty_list_{ "tried to access  an empty list" };
		}
		return std::move(head->data);

	}

	void show() {
		ListNode* ptr = head;
		while (ptr != nullptr) {
			std::cout << ptr->data << '\n';
			ptr = ptr->next;
		}
	}
	~SingleLinkedList()noexcept {
		this->clear();
	}
	void clear()noexcept {
		ListNode* ptr;
		for (std::size_t i = 0; i < count; i++) {
			ptr = head;
			head = head->next;
			delete ptr;
		}
		head = tail = nullptr;
		count = 0;
	}
	void swap(SingleLinkedList& other)noexcept {
		std::swap(head, other.head);
		std::swap(tail, other.tail);
		std::swap(count, other.count);
	}
	SingleLinkedList<_Ty>& operator =(const SingleLinkedList<_Ty>& other)& {
		if (this != &other) {
			ListNode* curr1{ head }, * prev1{ nullptr };
			ListNode* curr2{ other.head }, * prev2{ nullptr };

			while (curr1 != nullptr && curr2 != nullptr) {

				curr1->data = curr2->data;
				prev1 = curr1;
				prev2 = curr2;
				curr1 = curr1->next;
				curr2 = curr2->next;

			}

			if (prev1 == nullptr && curr2 != nullptr) {//o allos exei na mas dosei

				while (curr2 != nullptr) {

					if (!this->emplace_back(curr2->data)) {
						break;
					}
					curr2 = curr2->next;
				}
				return *this;

			}
			if (prev2 == nullptr && curr1 != nullptr) { //exoume perrita

				this->clear();
				return *this;
			}
			if (curr1 != nullptr && curr2 == nullptr) {

				tail = prev1;
				tail->next = nullptr;
				while (curr1 != nullptr) {
					ListNode* ptr{ curr1 };
					curr1 = curr1->next;
					delete ptr;
					count--;

				}
				return *this;
			}
			if (curr2 != nullptr && curr1 == nullptr) {
				while (curr2 != nullptr) {

					if (!this->emplace_back(curr2->data)) {
						this->clear();
						break;
					}
					curr2 = curr2->next;
				}
				return *this;
			}



		}
		return *this;
	}

	SingleLinkedList<_Ty>& operator =(SingleLinkedList<_Ty>&& other) & noexcept
	{
		this->clear();
		if (other.count != 0) {
			head = other.head;
			tail = other.tail;
			count = other.count;
			other.head = other.tail = nullptr;
			other.count = 0;
		}
		return *this;
	}

	//std::size_t remove(const _Ty& val) {
	//	static_assert(Comparable<_Ty>, "type must support == operator");

	//	std::size_t nodeCount = 0;
	//	if (count == 1) {
	//		if (head->data == val)this->~SingleLinkedList();
	//		return 1;
	//	}//polla nodes ara
	//	while (head->data == val) {
	//		nodeCount++;
	//		this->pop_front();
	//		if (head == nullptr)break;
	//	}
	//	if (tail == nullptr)return nodeCount;
	//	while (tail->data == val) {
	//		nodeCount++;
	//		this->pop_back();
	//	}


	//	ListNode* ptr = head;

	//	while (ptr != nullptr) {
	//		while (ptr->next != nullptr) {

	//			if (ptr->next->data == val) {

	//				nodeCount++;
	//				ListNode* ptr2 = ptr->next;
	//				ptr->next = ptr->next->next;

	//				delete ptr2;
	//				count--;
	//			}
	//			else {
	//				ptr = ptr->next;
	//			}
	//		}
	//		break;
	//	}


	//	return nodeCount;
	//}
	void reverse()noexcept {
		if (count > 1) {
			ListNode* prev = nullptr;
			ListNode* curr = head;
			tail = head;

			while (curr != nullptr) {
				ListNode* next = curr->next;
				curr->next = prev;
				prev = curr;
				curr = next;
			}

			head = prev;
		}



	}


	template<class ..._Valty>
	bool emplace_back(_Valty&&..._Val) {

		ListNode* ptr = ListNode::craft(std::forward<_Valty>(_Val)...);

		if (count != 0) {
			if (ptr != nullptr) {
				tail->next = ptr;
				tail = tail->next;
				count++;
				return true;
			}
		}
		else {
			if (ptr != nullptr) {
				head = tail = ptr;
				count++;
				return true;
			}
		}
		return false;
	}
	template<class ..._Valty>
	bool emplace_front(_Valty&&..._Val) {

		ListNode* ptr = ListNode::craft(std::forward<_Valty>(_Val)...);

		if (count != 0) {
			if (ptr != nullptr) {
				ptr->next = head;
				head = ptr;
				count++;
				return true;
			}
		}
		else {
			if (ptr != nullptr) {
				head = tail = ptr;
				count++;
				return true;
			}
		}
		return false;
	}

	bool insert(const _Ty& val) {
		ListNode* curr{ head };
		ListNode* prev{ nullptr };
	
		while (curr != nullptr && curr->data < val) {
			prev = curr;
			curr = curr->next;
	
		}
		ListNode* ptr = new(std::nothrow)ListNode(val);
		if (ptr == nullptr)return false;
		count++;
		if (prev == nullptr) {
			head = ptr;
			head->next = curr;
			if (curr == nullptr) { tail = ptr; }
			}
		else {
			prev->next = ptr;
			ptr->next = curr;
			if (curr == nullptr) {
				tail = ptr;
			}

		}

		return true;
	}
	bool add_unique(const _Ty& val) {
		ListNode* curr{ head };
		ListNode* prev{ nullptr };

		while (curr != nullptr && curr->data < val) {
			prev = curr;
			curr = curr->next;

		}
		if (curr != nullptr && curr->data == val) { return false; }
		ListNode* ptr = new(std::nothrow) ListNode(val);
		if (ptr == nullptr)return false;
		count++;
		if (prev == nullptr) {
			head = ptr;
			head->next = curr;
			if (curr == nullptr) { tail = ptr; }
		}
		else {
			prev->next = ptr;
			ptr->next = curr;
			if (curr == nullptr) {
				tail = ptr;
			}

		}

		return true;
	}
	bool is_ascending()const noexcept {
		if (count < 2)return true;
		ListNode* ptr = head;
		while (ptr->next != nullptr) {
			if (ptr->data > ptr->next->data)return false;
			ptr = ptr->next;
		}
		return true;
	}
	bool is_descending()const noexcept {
		if (count < 2)return true;
		ListNode* ptr = head;
		while (ptr->next != nullptr) {
			if (ptr->data < ptr->next->data)return false;
			ptr = ptr->next;
		}
		return true;
	}
	bool is_sorted()const noexcept {
		if (count < 2)return true;

		ListNode* ptr = head;
		bool asc = true;
		bool desc = false;

		while (ptr->next != nullptr) {
			asc = asc && (ptr->data <= ptr->next->data);
			desc = desc && (ptr->data >= ptr->next->data);
			ptr = ptr->next;
		}
		return (asc || desc);
	}
	void remove(const _Ty& data) {
		if (count < 1)return;
		while (head != nullptr && head->data == data) {
			this->pop_front();
		}//empty or still have elements
		//if it is not empty kapou mesa elems h telos 
		if (head != nullptr)//still has
		{
			ListNode* prev{ head };// we no that head is no the elem
			ListNode* curr{ head->next };
			while (curr != nullptr) {
				if (curr->data == data) {
					prev->next = curr->next;
					count--;
					delete curr;
					curr = prev->next;
					if (curr == nullptr) {//delete the last
						tail = prev;
					}
				}
				else {
					prev = prev->next;
					curr = curr->next;
				}
			}

		}
	}
	template<typename _Pr1>
	void remove_if(_Pr1 pred) {
		if (count < 1)return;
		while (head != nullptr && pred(std::as_const(head->data))) {
			this->pop_front();
		}//empty or still have elements
		//if it is not empty kapou mesa elems h telos 
		if (head != nullptr)//still has
		{
			ListNode* prev{ head };// we no that head is no the elem
			ListNode* curr{ head->next };
			while (curr != nullptr) {
				if (pred(std::as_const(curr->data))) {
					prev->next = curr->next;
					count--;
					delete curr;
					curr = prev->next;
					if (curr == nullptr) {//delete the last
						tail = prev;
					}
				}
				else {
					prev = prev->next;
					curr = curr->next;
				}
			}

		}
	}
	void  merge(SingleLinkedList<_Ty> &other) {
		if (empty() && other.empty())return;
		if (!is_ascending() || !other.is_ascending()) {
			throw sequence_not_ordered{ "sequence_not_ordered" };
		}
		count += other.count;
		ListNode* Head{ new(std::nothrow) ListNode{} };
		if (Head == nullptr)return;
		ListNode* ptr{ Head };
		ListNode* curr1{ head };
		ListNode* curr2{ other.head };
		while (curr1 != nullptr && curr2 != nullptr) {
			if (curr1->data < curr2->data) {
				ptr->next = curr1;
				ptr = ptr->next;
				curr1 = curr1->next;
			}
			else {
				ptr->next = curr2;
				ptr = ptr->next;
				curr2 = curr2->next;
			}

		}
		if (curr1 == nullptr) {
			ptr->next = curr2;
			tail = other.tail;
		}
		if (curr2 == nullptr) {
			ptr->next = curr1;
		}
		other.head = nullptr;
		other.count = 0;
		other.tail = nullptr;
		ptr = Head;
		head = Head->next;
		delete ptr;
	}
	




};
#endif
_PANAGIOTIS_END

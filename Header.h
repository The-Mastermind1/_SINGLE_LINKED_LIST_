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
template<typename _Ty>
class SingleLinkedList {
private:
	class ListNode {
	public:
		_Ty data;
		ListNode* next;
		ListNode()noexcept :data{}, next{ nullptr }
		{

		}
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
			if (other.count != 0) {//exei na mas dosei kati o allos 
				ListNode* ptr = other.head;
				head = new (std::nothrow)ListNode(ptr->data);
				if (head != nullptr) {
					tail = head;
					count++;

					for (std::size_t i = 1; i < other.count; i++) {
						tail->next = new (std::nothrow)ListNode(ptr->next->data);
						if (tail->next != nullptr) {
							ptr = ptr->next;
							tail = tail->next;
							count++;
						}
						else {
							this->~SingleLinkedList();
							break;
						}
					}
				}

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

	bool push_back(const _Ty&data)
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
	bool push_back( _Ty&& data)
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
	bool push_front(const _Ty&data) {
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
	bool push_front(_Ty&&data) {
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
			
			
			for (std::size_t i = 0; i < count-1 ; i++) {
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
		ListNode* ptr ;
		for (std::size_t i = 0; i < count; i++) {
			
			ptr = head;
			head = head->next;
			delete ptr;
		}
		head = tail = nullptr;
		count = 0;
	}
	void clear()noexcept {
		this->~SingleLinkedList();
	}
	void swap(SingleLinkedList&other)noexcept {
		std::swap(head, other.head);
		std::swap(tail, other.tail);
		std::swap(count, other.count);
	}
	SingleLinkedList<_Ty>& operator =(const SingleLinkedList<_Ty>& other)&
	{//copy constructor 
		if (this == &other)return *this;
		if (other.count != 0) {
			if (count == 0) {
				ListNode* ptr = other.head;
				head = new (std::nothrow)ListNode(ptr->data);
				if (head != nullptr) {
					tail = head;
					count++;

					for (std::size_t i = 1; i < other.count; i++) {
						tail->next = new (std::nothrow)ListNode(ptr->next->data);
						if (tail->next != nullptr) {
							ptr = ptr->next;
							tail = tail->next;
							count++;
						}
						else {
							this->~SingleLinkedList();
							break;
						}
					}
				}
				return *this;
			}
			else {

				ListNode* ptr1 = head;
				ListNode* ptr2 = other.head;
				while (ptr1 != nullptr && ptr2 != nullptr) {

					while (ptr1->next != nullptr && ptr2->next != nullptr) {
						ptr1->data = ptr2->data;
						ptr1 = ptr1->next;
						ptr2 = ptr2->next;
					}
					ptr1->data = ptr2->data;
					break;

				}
				/*if (count == other.count) {
					Queue_Node* ptr1 = head;
					Queue_Node* ptr2 = other.head;
					while (ptr1 != nullptr && ptr2 != nullptr) {
						ptr1->data = ptr2->data;
						ptr1 = ptr1->next;
						ptr2 = ptr2->next;

					}
					return *this;
				}*/
				if (count < other.count) {
					/*Queue_Node* ptr1 = head;
					Queue_Node* ptr2 = other.head;
					while (ptr1 != nullptr && ptr2 != nullptr) {

						while (ptr1->next != nullptr && ptr2->next != nullptr) {
							ptr1->data = ptr2->data;
							ptr1 = ptr1->next;
							ptr2 = ptr2->next;
						}
						ptr1->data = ptr2->data;
						break;

					}*/
					ptr2 = ptr2->next;
					while (ptr2 != nullptr) {
						
						if (push_back(ptr2->data)) {
							ptr2 = ptr2->next;

						}
						else {
							this->~SingleLinkedList();
							break;
						}


					}
					return *this;
				}
				else if (count > other.count) {
					/*Queue_Node* ptr1 = head;
					Queue_Node* ptr2 = other.head;
					while (ptr1 != nullptr && ptr2 != nullptr) {

						while (ptr1->next != nullptr && ptr2->next != nullptr) {
							ptr1->data = ptr2->data;
							ptr1 = ptr1->next;
							ptr2 = ptr2->next;
						}
						ptr1->data = ptr2->data;
						break;

					}*/
					tail = ptr1;
					
					ptr1 = ptr1->next;
					while (ptr1 != nullptr) {

						ListNode* ptr = ptr1;
						ptr1 = ptr1->next;
						delete ptr;
						count--;
					}
					tail->next = nullptr;
					return *this;
					
				}
				return *this;
			}

		}
		else {
			this->~SingleLinkedList();
			return *this;
		}

	}

	SingleLinkedList<_Ty>& operator =(SingleLinkedList<_Ty>&& other) & noexcept
	{
		this->~SingleLinkedList();
		if (other.count != 0) {
			head = other.head;
			tail = other.tail;
			count = other.count;
			other.head = other.tail = nullptr;
			other.count = 0;
		}
		return *this;
	}

	std::size_t remove(const _Ty& val) {
		static_assert(Comparable<_Ty>,"type must support == operator");
		
		std::size_t nodeCount = 0;
		if (count == 1) {
			if (head->data == val)this->~SingleLinkedList();
			return 1;
		}//polla nodes ara
		while (head->data == val) {
			nodeCount++;
			this->pop_front();
			if (head == nullptr)break;
		}
		if (tail == nullptr)return nodeCount;
		while (tail->data == val) {
			nodeCount++;
			
			this->pop_back();
		}
		
		
		ListNode* ptr = head;
		
		while (ptr != nullptr) {
			while (ptr->next != nullptr) {
				
				if (ptr->next->data == val) {
					
					nodeCount++;
					ListNode* ptr2 = ptr->next;
					ptr->next = ptr->next->next;
					delete ptr2;
					count--;
				}
				else {
					ptr = ptr->next;
				}
			}
			break;
		}


		return nodeCount;
	}
	void reverse()noexcept {
		if (count > 1) {
			ListNode* ptr1 = head;
			ListNode* ptr2 = head->next;
			ListNode* newHead = nullptr;
			std::size_t count1 = 0;
			while (ptr2 != nullptr) {
				if (count1 == 0) {
					count1++;
					ListNode* ptr = ptr2->next;
					ptr2->next = ptr1;
					ptr1->next = ptr;
					newHead = ptr2;
					ptr2 = ptr1->next;
				}
				else {
					ListNode* ptr = ptr2->next;
					ptr2->next = newHead;
					ptr1->next = ptr;
					newHead = ptr2;
					ptr2 = ptr1->next;
				}



			}
			head = newHead;
			tail = newHead;
			if (tail == nullptr)return;
			for (std::size_t i = 0; i < count - 1; i++) {
				tail = tail->next;
			}

		}
	}

	void sort() 
	{
		static_assert(Can_Be_Sorted<_Ty>,
			"In order to use this func this type must be sortable");
		static_assert(std::is_move_constructible_v<_Ty> || 
			std::is_copy_constructible_v<_Ty>,
			"the type must be either copy constructible or move constructible");

		ListNode* ptr = head;
		std::priority_queue<_Ty,std::vector<_Ty>,std::greater<_Ty>>a;
		while (ptr != nullptr) {
			if constexpr (std::is_move_constructible_v<_Ty>) {
				a.push(std::move(ptr->data));
			}
			else if constexpr(std::is_copy_constructible_v<_Ty>) {
				a.push(ptr->data);
			}
			ptr = ptr->next;
		}
		ptr = head;
		while(ptr!=nullptr){
			ptr->data =a.top();
			a.pop();
			ptr = ptr->next;
		}
		
		return;
	}
	
	

};


_PANAGIOTIS_END

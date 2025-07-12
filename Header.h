//tasks: make emplace back and front, more type_trais use and iterators when ??
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

/*
#pragma once
#include<iostream>
#include<cstdlib>
#include<type_traits>
#include<algorithm>
#include<functional>
#include<utility>
#include<new>
#include<initializer_list>
#include"Macros.h"

_PANAGIOTIS_BEGIN
template<typename _Ty>
class single_linked_list final{
private:
	class list_node final {
	public:
		_Ty data;
		list_node* next;
		list_node() :data{}, next{}
		{}
		list_node(const _Ty& item)noexcept(noexcept(data=item)) :next{}
		{
			data = item;
		}
		list_node(_Ty&& item)noexcept(noexcept(data=std::move(item))) :next{}
		{
			data = std::move(item);
		}
	};
	list_node* head;
	list_node* tail;
	std::size_t count;
	template<class _Valty>
	bool push_back_node(_Valty &&_Val) {
		list_node* ptr{ new (std::nothrow)list_node(std::forward<_Valty>(_Val)) };
		if (ptr != nullptr) {
			if (head==nullptr) {
				head=ptr;
			}
			else {
				tail->next = ptr;
			}
			tail = ptr;
			count++;
			return true;
		}
		return false;
	}
	template<class _Valty>
	bool push_front_node(_Valty&& _Val) {
		list_node* ptr{ new (std::nothrow)list_node(std::forward<_Valty>(_Val)) };
		if (ptr != nullptr) {
			ptr->next = head;
			head = ptr;
			if (tail == nullptr)tail = ptr;
			count++;
			return true;
		}
		return false;
	}
	void pop_front_node() {
		if (count != 0) {
			list_node* ptr{ head };
			head = head->next;
			delete ptr;
			if (head == nullptr)tail = nullptr;
			count--;
			return;
		}
		throw pop_from_an_empty_list{ "tried to pop from an empty list" };
	}
	void pop_back_node() {
		if (count != 0) {
			count--;
			delete tail;
			if (count == 0) {
				head = tail = nullptr;
				return;
			}
			tail = head;
			for (std::size_t i = 0; i < count - 1; i++) {
				tail = tail->next;
			}
			tail->next = nullptr;
			return;
			
		}
		throw pop_from_an_empty_list{ "tried to pop from an empty list" };
	}
	void clear()noexcept {
		list_node* ptr{};
		while (head != nullptr) {
			ptr = head;
			head = head->next;
			delete ptr;
		}
		tail = nullptr;
		count = 0;
	}
	void reverse_linked_list()noexcept {
		if (count < 2)return;
		tail = head;
		list_node* ptr1{ head };
		list_node* ptr2{ head->next };
		while (ptr2 != nullptr) {
			ptr1->next = ptr2->next;
			ptr2->next = head;
			head = ptr2;
			ptr2 = ptr1->next;

		}
		return;
	}
	
	bool insert_element(const _Ty& data) {
		list_node* ptr{ new(std::nothrow)list_node(data) };
		if (ptr == nullptr)return false;
		list_node* prev{ nullptr };
		list_node* curr{ head };
		while (curr != nullptr && curr->data < data) {
			prev = curr;
			curr = curr->next;
		}
		
		count++;
		if (prev == nullptr) {
			ptr->next = head;
			head = ptr;
			if (tail == nullptr)tail = ptr;
				
		}
		else {
			ptr->next = curr;
			prev->next=ptr;
			if (curr == nullptr) {
				tail = ptr;
			}
		}
		return true;
	}
	bool add_unique_node(const _Ty&data) {
		list_node* ptr{ new(std::nothrow)list_node(data) };
		if (ptr == nullptr)return false;
		list_node* prev{ nullptr };
		list_node* curr{ head };
		while (curr != nullptr && curr->data < data) {//searching the position to put it 
			prev = curr;
			curr = curr->next;
		}
		list_node* ptr1{ curr };//an yparxei,eidh
		while (ptr1 != nullptr) {
			if (ptr1->data == data)return false;
			ptr1 = ptr1->next;
		}

		count++;
		if (prev == nullptr) {
			ptr->next = head;
			head = ptr;
			if (tail == nullptr)tail = ptr;

		}
		else {
			ptr->next = curr;
			prev->next = ptr;
			if (curr == nullptr) {
				tail = ptr;
			}
		}
		return true;


	}
	void erase_node(const _Ty&data)noexcept {
		while (head != nullptr && head->data == data) {
			pop_front_node();
		}
		if (head != nullptr) {
			list_node* prev{ head};
			list_node* curr{ head->next };
			while (curr != nullptr) {
				if (curr->data == data) {//exo match prepei na to kano delete
					count--;
					prev->next = curr->next;
					delete curr;
					curr = prev->next;
					
					
				}
				else {
					prev = curr;
					curr = curr->next;
				}
			}
			tail = prev;
		}
	}
	template<typename _Pr1>
	void erase_node_if(_Pr1 pred) {
		while (head != nullptr && pred(std::as_const(head->data))) {
			pop_front();
		}//empty or still have elements
		//if it is not empty kapou mesa elems h telos 
		if (head != nullptr)//still has
		{
			list_node* prev{ head };// we no that head is no the elem
			list_node* curr{ head->next };
			while (curr != nullptr) {
				if (pred(std::as_const(curr->data))) {//exo match prepei na to kano delete
					count--;
					prev->next = curr->next;
					delete curr;
					curr = prev->next;
				

				}
				else {
					prev = curr;
					curr = curr->next;
				}
			}
			tail = prev;

		}
	}
	void merge_in_place(single_linked_list<_Ty>& other) {
		if (this == &other)return;
		if (empty() && other.empty())return;
		if (!is_ascending() || !other.is_ascending())return;
		list_node* dummy{ new (std::nothrow)list_node{} };
		if (dummy == nullptr)return;
		list_node* ptr{ dummy };
		list_node* curr1{ head };
		list_node* curr2{ other.head };
		while (curr1 != nullptr && curr2 != nullptr) {
			if (curr1->data <= curr2->data) {
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
		if (curr1 == nullptr &&curr2!=nullptr) {
			ptr->next = curr2;
			tail = other.tail;
		}
		if (curr2 == nullptr && curr1!=nullptr) {
			ptr->next = curr1;
		}
		head = dummy->next;
		count = count + other.count;
		delete dummy;
		other.head = other.tail = nullptr;
		other.count = 0;
	}
	void delete_duplicates()noexcept {
		if (count < 2)return;
		list_node* prev{ head };
		list_node* curr{ head->next };
		while (curr != nullptr) {
			if (prev->data == curr->data) {//duplicate put out
				count--;
				prev->next = curr->next;
				delete curr;
				curr = prev->next;
			}
			else {
				prev = prev->next;
				curr = curr->next;
			}
		}
		tail = prev;
		return;
	}
	std::size_t  merge_in_place(const single_linked_list<_Ty>& other1,
		const single_linked_list<_Ty>& other2) {
		if (this == &other1 || this == &other2)return 0;
		if (other1.empty() && other2.empty())return 0;
		if (!other1.is_ascending() && !other2.is_ascending())return 0;
		
		clear();
		list_node* curr1{ other1.head };
		list_node* curr2{ other2.head };
		while (curr1 != nullptr &&curr2!=nullptr) {
			if(curr1->data<=curr2->data){
				if (!push_back(curr1->data)) {
					clear();
					return 0;
				}
				curr1 = curr1->next;
			}
			else {
				if (!push_back(curr2->data)) {
					clear();
					return 0 ;
				}
				curr2 = curr2->next;
			}
		}
		while (curr1 != nullptr) {
			if (!push_back(curr1->data)) {
				clear();
				return 0;
			}
			curr1 = curr1->next;
		}
		while (curr2 != nullptr) {
			if (!push_back(curr2->data)) {
				clear();
				return 0;
			}
			curr2 = curr2->next;
		}
		return count;//the count of nodes that merged
	}

	void merge_two_lists_unique_elements(const single_linked_list<_Ty>& other1,
		const single_linked_list<_Ty>& other2) {
		if (this == &other1 || this == &other2)return;
		if (other1.empty() && other2.empty())return;
		if (!other1.is_ascending() || !other2.is_ascending())return;
		
		clear();
		list_node* curr1{ other1.head };
		list_node* curr2{ other2.head };

		
	}

public:
	single_linked_list()noexcept :head{}, tail{}, count{}
	{}
	single_linked_list(const std::initializer_list<_Ty>& l) :head{}, tail{}, count{} {
		const _Ty* ptr{ l.begin() };
		for (std::size_t i = 0; i < l.size(); i++) {
			if (!push_back(*ptr)) {
				clear();
				break;
			}
			ptr++;
		}
	}
	single_linked_list(const single_linked_list<_Ty>& other) :head{}, tail{}, count{} {
		list_node* ptr{ other.head };
		while (ptr != nullptr) {
			if (!push_back(ptr->data)) {
				clear();
				break;
			}
			ptr = ptr->next;
		}
		
	}
	single_linked_list(single_linked_list<_Ty>&& other)noexcept :head{}, tail{}, count{} {
		std::swap(head, other.head);
		std::swap(count, other.count);
		std::swap(tail, other.tail);
	}
	bool push_back(const _Ty& data) {
		return push_back_node(data);
	}
	bool push_back(_Ty&& data) {
		return push_back_node(std::move(data));
	}
	bool push_front(const _Ty&data) {
		return push_front_node(data);
	}
	bool push_front(_Ty&&data){
		return push_front_node(std::move(data));
	}
	void pop_front() {
		pop_front_node();
	}
	void pop_back() {
		pop_back_node();
	}
	bool empty() const noexcept{
		return size() == 0;
	}
	std::size_t size()const noexcept {
		return count;
	}
	_NODISCARD _Ty&& back()&& {
		if (count == 0) {
			throw tried_to_access_an_empty_list_{ "tried to access  an empty list" };
		}
		return std::move(tail->data);

	}
	_NODISCARD const _Ty&& back()const&& {
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
	_NODISCARD _Ty&& front()&& {
		if (count == 0) {
			throw tried_to_access_an_empty_list_{ "tried to access  an empty list" };
		}
		return std::move(head->data);

	}
	_NODISCARD const _Ty&& front()const&& {
		if (count == 0) {
			throw tried_to_access_an_empty_list_{ "tried to access  an empty list" };
		}
		return std::move(head->data);

	}
	~single_linked_list()noexcept {
		clear();
	}
	void swap(single_linked_list<_Ty>& other)noexcept {
		std::swap(head, other.head);
		std::swap(tail, other.tail);
		std::swap(count, other.count);
	}
	single_linked_list<_Ty>& operator =(const single_linked_list<_Ty>& other)& {
		if (this != &other) {
			list_node* prev1{ nullptr };
			list_node* prev2{ nullptr };
			list_node* curr1{ head };
			list_node* curr2{ other.head };
			while (curr1 != nullptr && curr2 != nullptr) {
				curr1->data = curr2->data;
				prev1 = curr1;
				prev2 = curr2;
				curr1 = curr1->next;
				curr2 = curr2->next;
			}
			if (prev1 == nullptr && curr2 != nullptr ||curr2!=nullptr &&curr1==nullptr) {
				while (curr2 != nullptr) {
					if (!push_back(curr2->data)) {
						clear();
						break;
					}
					curr2 = curr2->next;
				}
				return *this;
			}
			if (prev2 == nullptr && curr1 != nullptr) {
				clear();
				return *this;
			}
			if (curr1 != nullptr && curr2 == nullptr) {//size>other.size,sbhse perittous
				prev1->next = nullptr;
				tail = prev1;
				while (curr1 != nullptr) {
					list_node* ptr{ curr1 };
					curr1 = curr1->next;
					delete ptr;
					count--;
				}
				return *this;
			}

		}
		return *this;
	}
	single_linked_list<_Ty>& operator =(single_linked_list<_Ty>&& other)& {
		clear();
		std::swap(head, other.head);
		std::swap(count, other.count);
		std::swap(tail, other.tail);
		return *this;
	}
	void show() {
		list_node* ptr{ head };
		while (ptr != nullptr) {
			std::cout << ptr->data << '\n';
			ptr = ptr->next;
		}

	}
	void reverse()noexcept {//head->1,2,3,4 tail<-
		reverse_linked_list();
	}
	bool is_ascending()const noexcept {
		if (count < 2)return true;
		list_node *prev{ head };
		list_node* curr{ head->next };
		while (curr != nullptr && prev->data <= curr->data) {
			prev = curr;
			curr = curr->next;
		}
		if (curr != nullptr)return false;
		return true;
	}
	bool is_descending()const noexcept {
		if (count < 2)return true;
		list_node* prev{ head };
		list_node* curr{ head->next };
		while (curr != nullptr && prev->data >= curr->data) {
			prev = curr;
			curr = curr->next;
		}
		if (curr != nullptr)return false;
		return true;
	}
	bool is_sorted()const noexcept {
		if (count < 2)return true;
		list_node* prev{ head };
		list_node* curr{ head->next };
		bool is_ascending = true;
		bool is_descending = true;
		while (curr != nullptr) {
			is_ascending = is_ascending && prev->data <= curr->data;
			is_descending = is_descending && prev->data >= curr->data;
			prev = curr;
			curr = curr->next;
		}
		return is_ascending || is_descending;
		
	}
	bool insert(const _Ty& data) {
		return insert_element(data);
		
	}
	bool add_unique(const _Ty& data) {
		return add_unique_node(data);
	}
	void erase(const _Ty& data)noexcept {
		erase_node(data);
	}
	void merge(single_linked_list<_Ty>& other) {
		merge_in_place(other);
	}
	void merge(const single_linked_list<_Ty>& other1
		, const single_linked_list<_Ty>& other2) {
		merge_in_place(other1, other2);

	}
	void merge_unique(const single_linked_list<_Ty>& other1
		, const single_linked_list<_Ty>& other2) {
		merge_two_lists_unique_elements(other1,other2);
	}
	template<typename _Pr1>
	void erase_if(_Pr1 pred) {
		erase_node_if(pred);
	}
	void erase_duplicates()noexcept {
		delete_duplicates();
	}
	std::size_t count_items()const noexcept {
		list_node* ptr{ head };
		std::size_t node_count = 0;
		while (ptr != nullptr) {
			node_count++;
			ptr = ptr->next;
		}
		return node_count;
	}
};



_PANAGIOTIS_END
*/




#pragma once 
#include<iostream>
#include<functional>
#include<type_traits>
#include<new>
#include<algorithm>
#include<cstdlib>
#include<utility>
#include<initializer_list>
#include<memory>
#include"Macros.h"

_PANAGIOTIS_BEGIN
template<typename _Ty>
class single_linked_list{
private:
	class list_node final{
	private:
		class secretClass {};
		template<class ..._Valty>
		list_node(secretClass, _Valty&& ..._Val) :data{ std::forward<_Valty>(_Val)... }, next{ nullptr }
		{
			//constructs the object in place
			static_assert(std::is_constructible_v<_Ty,_Valty...>,
				"the type must be constructible in place with these args");
		}
	public:	
		_Ty data;
		list_node* next;
		//template<typename t=_Ty>
		//requires(std::is_default_constructible_v<_Ty>)
		list_node()noexcept(std::is_nothrow_default_constructible_v<_Ty>) 
			:data{}, next{}
		{
			static_assert(std::is_default_constructible_v<_Ty>, "the type"
				"must be default constructible in order to use this constructor");
		}
		list_node(const _Ty& item)noexcept(std::is_nothrow_copy_assignable_v<_Ty>)
			:next{}
		{
			static_assert(std::is_copy_assignable_v<_Ty>,
				"the type must be copy assignable in order to use this constructor");
			data = item;
		}
		list_node(_Ty&& item)noexcept(std::is_nothrow_move_assignable_v<_Ty>)
			:next{}
		{
			static_assert(std::is_move_assignable_v<_Ty>
				, "the type must be move assignable in order to use this constructor");
			data = std::move(item);
		}
		template<class ..._Valty>
		static list_node* craft(_Valty&& ..._Val) {

			list_node* ptr = new list_node{ secretClass{},std::forward<_Valty>(_Val)... };
			return ptr;

		}

	};
	//thats my own iterator class
	//pretty much it is just a wrapper arroung my pointer 
	//it also has operator ++ in order to advance the pointer 
	//it also has += in order to progress it even further
	//it also has != and == for comparisons and operator *
	//with references qualifiers ,pretty much all that matters is where the pointer
	//shows 
	class list_node_iterator final {
	private:
		list_node* ptr;
	public:
		//
		list_node_iterator()noexcept :ptr{} {}
		//
		list_node_iterator(list_node* ptr1)noexcept :ptr{ ptr1 } {}
		//
		list_node_iterator(const list_node_iterator& other)noexcept = default;
		//
		list_node_iterator(list_node_iterator&& other)noexcept = default;
		//
		list_node_iterator operator ++() noexcept{
			if (ptr != nullptr) {
				ptr = ptr->next;
			}
			return list_node_iterator{ ptr };
		}
		//
		list_node_iterator operator ++(int)noexcept {
			list_node_iterator tmp{ ptr };
			if (ptr != nullptr) {
				ptr = ptr->next;
			}
			return tmp;
		}
		//
		bool operator !=(const list_node_iterator& other)const noexcept {
			return ptr != other.ptr;
		}
		//
		bool operator ==(const list_node_iterator& other)const noexcept {
			return ptr == other.ptr;
		}
		//
		const _Ty& operator *()const& {
			if (ptr != nullptr) {
				return ptr->data;
			}
			throw tried_to_access_an_empty_iterator_{ "tried to access an empty iterator" };
		}
		//
		_Ty& operator *()& {
			if (ptr != nullptr) {
				return ptr->data;
			}
			throw tried_to_access_an_empty_iterator_{ "tried to access an empty iterator" };
		}
		//
		const _Ty&& operator *()const&& {
			if (ptr != nullptr) {
				return std::move(ptr->data);
			}
			throw tried_to_access_an_empty_iterator_{ "tried to access an empty iterator" };
		}
		//
		_Ty&& operator *()&& {
			if (ptr != nullptr) {
				return std::move(ptr->data);
			}
			throw tried_to_access_an_empty_iterator_{ "tried to access an empty iterator" };
		}
		list_node_iterator operator +=(std::size_t counter)noexcept{
			for (std::size_t i = 0; i < counter; i++) {
				if (ptr != nullptr)ptr = ptr->next;
				else break;
			}
			return list_node_iterator{ ptr };
		}
		//
		const _Ty* operator ->()const&  {
			if (ptr != nullptr) {
				return std::addressof(ptr->data);
			}
			throw tried_to_access_an_empty_iterator_{ "tried to access an empty iterator" };
		}
		//
		_Ty* operator ->()& {
			if (ptr != nullptr) {
				return std::addressof(ptr->data);
			}
			throw tried_to_access_an_empty_iterator_{ "tried to access an empty iterator" };
		}
		//
		const _Ty* operator ->()const&& {
			if (ptr != nullptr) {
				return std::addressof(ptr->data);
			}
			throw tried_to_access_an_empty_iterator_{ "tried to access an empty iterator" };
		}
		//
		_Ty* operator ->()&& {
			if (ptr != nullptr) {
				return std::addressof(ptr->data);
			}
			throw tried_to_access_an_empty_iterator_{ "tried to access an empty iterator" };
		}
		//
		list_node_iterator operator +(std::size_t counter)noexcept {
			list_node* curr{ ptr };
			for (std::size_t i = 0; i < counter; i++) {
				if (curr != nullptr)curr = curr->next;
				else break;
			}
			return list_node_iterator{ curr };
		}
		//
		friend list_node_iterator operator +(std::size_t counter, const
			list_node_iterator& it)noexcept {
			list_node curr{ it.ptr };
			for (std::size_t i = 0; i < counter; i++) {
				if (curr != nullptr)curr = curr->next;
				else break;
			}
			return list_node_iterator{ curr };
		}
		//
		list_node_iterator operator =(const list_node_iterator& other)noexcept {
			ptr = other.ptr;
			return *this;
		}
		//
		list_node_iterator operator =(list_node_iterator&&other)noexcept{
			ptr = other.ptr;
			return *this;
		}
		//
		~list_node_iterator()noexcept {
			ptr = nullptr;
		}
	};
	list_node* head;
	list_node* tail;
	std::size_t count;
	//push_back_node done 
	template<typename _Valty>
	bool push_back_node(_Valty&& _Val) 
	{//push back node it simply creates a new node and
		//pushes it to the end of the list 
		//it also uses perfect forwarding
		list_node* ptr{ new (std::nothrow)list_node{std::forward<_Valty>(_Val)} };
		if (ptr != nullptr) {
			if (head == nullptr) {
				head = ptr;
			}
			else {
				tail->next = ptr;
			}
			tail = ptr;
			count++;
			return true;
		}
		return false;

	}
	//push_front_node done 
	template<typename _Valty>
	bool push_front_node(_Valty&& _Val) 
	{//it simply creates a new node and pushes it to the start of the list
		//if list is empty head=tail=ptr; count++;
		//else tail->next=ptr tail=ptr; count++;
		list_node* ptr{ new (std::nothrow)list_node{std::forward<_Valty>(_Val)} };
		if (ptr != nullptr) {
			ptr->next = head;
			head = ptr;
			if (tail == nullptr)tail = ptr;
			count++;
			return true;
		}
		return false;
	}
	//pop_front_node done
	void pop_front_node() 
	{
		//if count==0 throws
		//if count!=0
		//if the list has one node ->delete the node and then head=tail=nullptr count--;
		//if the list is not empty ->simply delete the first node and move head to point
		//to the next 
		static_assert(std::is_destructible_v<_Ty>,"the type must be destructible");
		if (count != 0) {
			list_node* ptr{ head };
			head = head->next;
			delete ptr;
			if (head == nullptr)tail = nullptr;
			count--;
			return;
		}
		throw pop_from_an_empty_list{ "tried to pop from an empty list" };
	}
	//pop_back_node done 
	void pop_back_node() 
	{//if the list is empty throws
		//if we have one node delete tail; head=tail=nullptr;l count --;
		//if we have more than one we have to put tail to point 
		//into the new last node using a for loop 
		static_assert(std::is_destructible_v<_Ty>, "the type must be destructible");
		if (count != 0) {
			count--;
			delete tail;
			if (count == 0) {
				head = tail = nullptr;
				return;
			}
			tail = head;
			for (std::size_t i = 0; i < count - 1; i++) {
				tail = tail->next;
			}
			tail->next = nullptr;
			return;

		}
		throw pop_from_an_empty_list{ "tried to pop from an empty list" };
	}
	//clear done 
	void clear()noexcept {//this deallocates the linked list 
		static_assert(std::is_nothrow_destructible_v<_Ty>,
			"the object must be destructible and must not throw exception");
		//it simply starts from the head and deletes the nodes
		//all the way to the end of the list 
		list_node* ptr{};
		while (head != nullptr) {
			ptr = head;
			head = head->next;
			delete ptr;
		}
		tail = nullptr;
		count = 0;
	}
	//reverse_list_node done 
	void reverse_linked_list()noexcept 
	{//if we have one node return 
		//if we have more than we are goint to reverse the listt
		//simply we have two pointers start at the head of the list and at the head->next
		//of the list 
		//simply we pretty much put the next nodes where the head was
		//it is like 1234 
		//1 iteration: 2134
		//2 iteration: 3214
		//3 iteration 4321
		//throw every time a node back where the head was 
		//if we are at the last node ptr1->next=ptr2->next=nullptr;
		//then we stop 
		//at the end of this tail points to the last element and head to the first
		if (count < 2)return;
		tail = head;
		list_node* ptr1{ head };
		list_node* ptr2{ head->next };
		while (ptr2 != nullptr) {
			ptr1->next = ptr2->next;
			ptr2->next = head;
			head = ptr2;
			ptr2 = ptr1->next;

		}
		return;
	}
	//insert_element done 
	bool insert_element(const _Ty &data) 
	{
		//a simple func that inserts an element to our linked list 
		//we first allocate a node and then if we have memory 
		//we try searching in the list the position that we should put this
		//element we consider in our interface that the list is sorted in ascending order
		//return true (if all goes good if we had memory)
		list_node* ptr{ new(std::nothrow)list_node(data) };
		if (ptr == nullptr)return false;
		list_node* prev{ nullptr };
		list_node* curr{ head };
		while (curr != nullptr && curr->data < data) {
			prev = curr;
			curr = curr->next;
		}
		//if prev==nullptr no elements or we have to put the new element at the start 
		count++;
		if (prev == nullptr) {
			ptr->next = head;
			head = ptr;
			if (tail == nullptr)tail = ptr;//if list empty update tail

		}//prev!=nullptr means we are somewhere inside the list or at the end 
		else {
			ptr->next = curr;
			prev->next = ptr;
			if (curr == nullptr) {//if we are at the end update tail
				tail = ptr;
			}
		}
		return true;
	}
	//add_unique_node done 
	bool add_unique_node(const _Ty& data) {
		//add_unique_node pretty much does the same thing 
		//except that it also 
		// checks in the list if the element we want to add already exists 

		list_node* ptr{ new(std::nothrow)list_node(data) };
		if (ptr == nullptr)return false;
		list_node* prev{ nullptr };
		list_node* curr{ head };
		while (curr != nullptr && curr->data < data) {//searching the position to put it 
			prev = curr;
			curr = curr->next;
		}
		list_node* ptr1{ curr };//we found where to put it with (curr,prev)
		//but we have to 
		//check if it is in our list 
		while (ptr1 != nullptr) {
			if (ptr1->data == data)return false;
			ptr1 = ptr1->next;
		}

		count++;
		if (prev == nullptr) {
			ptr->next = head;
			head = ptr;
			if (tail == nullptr)tail = ptr;//if list empty 

		}
		else {
			ptr->next = curr;
			prev->next = ptr;
			if (curr == nullptr) {//if we have to put it at the end 
				tail = ptr;
			}
		}
		return true;
	}
	//delete_duplicates func done 
	template<typename _Pred1>
	void delete_duplicates(_Pred1 _Pred)noexcept {
		//this func simply does two things
		//it is called by two funcs one that uses the std::equal<>() in order
		//to compare elements with the default 
		//and the other uses a pred to compare the elements if they are equal 
		static_assert(std::is_nothrow_destructible_v<_Ty>,
			"the object must be destructible and must not throw exception");
		static_assert(std::is_invocable_r_v<bool, _Pred1, const _Ty&,
		const _Ty&>,"you must be able to call the func pred for the curr->data");
		//static asserts to see if all goes good 
		static_assert(std::is_convertible_v<
			std::invoke_result_t<_Pred1, const _Ty&,const _Ty&>, bool>, 
			"the return type of"
			"the func must be a boolean");

		if (count < 2)return;//no duplicates 
		//we pretty much have to pointers prev,curr
		//prev show at head and curr to head ->next
		//and we compare those pointers until curr !=nullptr 
		//whenever we encouter two equal based on pred we delete them and progress only 
		//curr else we progress curr and prev this func only works good if the results 
		//are sorted in ascending order 
		list_node* prev{ head };
		list_node* curr{ head->next };
		while (curr != nullptr) {
			if (_Pred(std::as_const(prev->data),std::as_const(curr->data))) {//duplicate put out
				count--;
				prev->next = curr->next;
				delete curr;
				curr = prev->next;
			}
			else {
				prev = prev->next;
				curr = curr->next;
			}
		}
		tail = prev;//dont forget to update tail prev we always show to the tail after this 
		//because curr will be nullptr 
		return;
	}
	//erase_node_if func done 
	template<typename _Pr1>
	void erase_node_if(_Pr1 _Pred) {
		//this func in simple terms deletes an element from a list
		//if a certain condition is met 
		//this func is called by two methods 
		//the remove which removes all the instances of this object in the list
		//the remove_if whichs uses the pred
		//generally both methos calls the erase_node_if with a pred 
		//the remove passes a simple lambda that checks if they are simply equal
		static_assert(std::is_destructible_v<_Ty>, "the type must be destructible");
		static_assert(std::is_invocable_r_v<bool, _Pr1, const _Ty& > ,
			"you must be able to call the func pred for the curr->data");
		static_assert(std::is_convertible_v<
			std::invoke_result_t<_Pr1, const _Ty&>,bool>,"the return type of"
			"the func must be a boolean");
		//if you keep finding the element at the start delete it 
		while (head != nullptr && _Pred(std::as_const(head->data))) {
			pop_front();
		}//empty or still have elements 
		//if head !=nullptr we found element that is doesn't make the pred true 
		//or we have no elements (head==nullptr)
		if (head != nullptr)//still we have elements 
		{
			list_node* prev{ head };// we know that head doesn't satisfy pred 
			list_node* curr{ head->next };//with curr and the rest of list we check
			//if the pred is satisfied 
			while (curr != nullptr) {
				if (_Pred(std::as_const(curr->data))) {
					//the pred is satisfied we must erase the element 
					count--;
					prev->next = curr->next;
					delete curr;
					curr = prev->next;
				}
				else {
					prev = curr;
					curr = curr->next;
				}
			}//prev we always show at the tail if curr==nullptr 
			tail = prev;

		}
	}
	//emplace_back_node done 
	template<class ..._Valty>
	bool emplace_back_node(_Valty&&..._Val) {
		//emplace_back_node pretty much is the same with push
		//the only difference is that we construct the data hold by list_node in place
		list_node* ptr = list_node::craft(std::forward<_Valty>(_Val)...);
		if (ptr != nullptr) {
			if (head == nullptr) {
				head = ptr;
			}
			else {
				tail->next = ptr;
			}
			tail = ptr;
			count++;
			return true;
		}
		return false;
	}
	//emplace_front_node done 
	template<class..._Valty>
	bool emplace_front_node(_Valty&&..._Val) {
		//emplace_front_node pretty much is the same with push
		//the only difference is that we construct the data hold by list_node in place
		list_node* ptr = list_node::craft(std::forward<_Valty>(_Val)...);
		if (ptr != nullptr) {
			ptr->next = head;
			head = ptr;
			if (tail == nullptr)tail = ptr;
			count++;
			return true;
		}
		return false;
	}
	list_node_iterator start()const noexcept{
		//start of the list
		return head;
	}
	list_node_iterator start()noexcept {
		//start of the list 
		return head;
	}
	list_node_iterator finish()noexcept {
		//end of the list
		return nullptr;
	}
	list_node_iterator finish()const noexcept{
		//end of the list 
		return nullptr;
	}
	template<typename Compare>
	void merge_lists(single_linked_list<_Ty>&other,Compare comp) {
		if (this == &other)return;
		if (empty() && other.empty())return;
		if (!is_ascending() || !other.is_ascending()) {
			throw sequence_not_ordered{ "sequence_not_ordered in ascending order" };
		}
		count += other.count;
		list_node* Head{ new(std::nothrow) list_node {} };
		if (Head == nullptr)return;
		list_node* ptr{ Head };
		list_node* curr1{ head };
		list_node* curr2{ other.head };
		while (curr1 != nullptr && curr2 != nullptr) {
			if (comp(std::as_const(curr1->data), std::as_const(curr2->data))) {
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
		if (curr1 == nullptr && curr2 != nullptr) {
			ptr->next = curr2;
			tail = other.tail;
		}
		if (curr2 == nullptr && curr1 != nullptr) {
			ptr->next = curr1;
		}
		head = Head->next;
		delete Head;
		other.head = other.tail = nullptr;
		other.count = 0;
	}
public:
	using iterator = list_node_iterator;
	static_assert(std::is_object_v<_Ty>, "The C++ Standard forbids container adaptors of non-object types "
		"because of [container.requirements].");
	static_assert(!std::is_reference_v<_Ty>, "no references allowed");
	static_assert(!std::is_const_v<_Ty>, "no const types are allowed");
	static_assert(!std::is_volatile_v<_Ty>, "no volatile types are allowed");
	//
	single_linked_list()noexcept;
	//
	single_linked_list(const std::initializer_list<_Ty>& other);
	//
	~single_linked_list()noexcept;
	//
	single_linked_list(const single_linked_list<_Ty>& other);
	//
	single_linked_list(single_linked_list<_Ty>&& other)noexcept;
	//
	single_linked_list<_Ty>& operator =(const single_linked_list<_Ty>& other)&;
	//
	single_linked_list<_Ty>& operator =(single_linked_list<_Ty>&& other)&noexcept;
	//
	single_linked_list<_Ty>& operator=(const std::initializer_list<_Ty>& other)&;
	//
	bool is_ascending()const noexcept;
	//
	bool is_descending()const noexcept;
	//
	bool is_sorted()const noexcept;
	//
	bool push_back(const _Ty&data);
	//
	bool push_back(_Ty&& data);
	//
	template<class ..._Valty>
	bool emplace_back(_Valty&&..._Val);
	//
	template<class ..._Valty>
	bool emplace_front(_Valty&&..._Val);
	//
	bool push_front(const _Ty& data);
	//
	bool push_front( _Ty&& data);
	//
	void pop_front();
	//
	void pop_back();
	//
	void reverse()noexcept;
	//
	bool insert(const _Ty& data);
	//
	bool add_unique(const _Ty& data);
	//
	void unique();
	//
	template<typename _Pred1>
	void unique(_Pred1 _Pred);
	//
	void show();
	//
	void remove(const _Ty& val);
	//
	template<typename _Pred1>
	void remove_if(_Pred1 _Pred);
	//
	void merge(single_linked_list<_Ty>& other);
	//
	void merge(single_linked_list<_Ty>&& other);
	//
	template<typename Compare>
	void merge(single_linked_list<_Ty>& other, Compare comp);
	//
	template<typename Compare>
	void merge(single_linked_list<_Ty>&& other, Compare comp);
	//
	bool empty()const noexcept {
		//checks if list is empty 
		return count == 0;
	}
	//
	std::size_t size()const noexcept {
		//returns the size of the list 
		return count;
	}
	//
	void swap(single_linked_list<_Ty>& other)noexcept;
	//back func of the list returns the last element using reference qualifiers 
	_NODISCARD _Ty&& back()&& {
		if (count == 0) {
			throw tried_to_access_an_empty_list_{ "tried to access  an empty list" };
		}
		return std::move(tail->data);

	}
	//
	_NODISCARD const _Ty&& back()const&& {
		if (count == 0) {
			throw tried_to_access_an_empty_list_{ "tried to access  an empty list" };
		}
		return std::move(tail->data);

	}
	//
	_NODISCARD const _Ty& back()const& {
		if (count == 0) {
			throw tried_to_access_an_empty_list_{ "tried to access  an empty list" };
		}
		return tail->data;

	}
	//
	_NODISCARD _Ty& back()& {
		if (count == 0) {
			throw tried_to_access_an_empty_list_{ "tried to access  an empty list" };
		}
		return tail->data;

	}
	//front func of the list returns the front element using reference qualifiers
	_NODISCARD const _Ty& front()const& {
		if (count == 0) {
			throw tried_to_access_an_empty_list_{ "tried to access  an empty list" };
		}
		return head->data;

	}
	//
	_NODISCARD _Ty& front()& {
		if (count == 0) {
			throw tried_to_access_an_empty_list_{ "tried to access  an empty list" };
		}
		return head->data;

	}
	//
	_NODISCARD _Ty&& front()&& {
		if (count == 0) {
			throw tried_to_access_an_empty_list_{ "tried to access  an empty list" };
		}
		return std::move(head->data);

	}
	//
	_NODISCARD const _Ty&& front()const&& {
		if (count == 0) {
			throw tried_to_access_an_empty_list_{ "tried to access  an empty list" };
		}
		return std::move(head->data);

	}
	//
	iterator begin()const noexcept {
		
		return start();
	}
	//
	iterator begin()noexcept {
		
		return start();
	}
	//
	iterator end()const noexcept {
		
		return finish();
	}
	//
	iterator end() noexcept {
		return finish();
	}
	//
	
};
//default constructor done ,pretty much default state of the object 
template<typename _Ty>
single_linked_list<_Ty>::single_linked_list()noexcept 
	:head{}, tail{}, count{}
{}
//initializer list constructor done
template<typename _Ty>
single_linked_list<_Ty>::single_linked_list(const std::initializer_list<_Ty>& other)
	: head{}, tail{}, count{}
{//if the push_backs fails give back everything allocated and return 
	//the object in default state 
	const _Ty* ptr{ other.begin() };
	for(std::size_t i=0;i<other.size();i++){
		if (!push_back(*ptr)) {
			clear();
			break;
		}
		ptr++;
	}
}
//copy constructor done 
template<typename _Ty>
single_linked_list<_Ty>::single_linked_list(const single_linked_list<_Ty>& other)
	:head{}, tail{}, count{}
{//this simply copies the contets of the other object
	//if this==&other nothing happens
	//if this!=&other then i other empty nothing happens
	//otherwise copy the contents of the other object
	list_node* curr{ other.head };
	while (curr != nullptr) {
		if (!push_back(curr->data)) {
			clear();
			break;
		}
		curr = curr->next;
	}

}
//move constructor done 
template<typename _Ty>
single_linked_list<_Ty>::single_linked_list(single_linked_list<_Ty>&& other)noexcept
	:head{}, tail{}, count{}
{//if this==&other then swaps nullptr's and 0's no problem 
	//if this !=&other if other is empty same as case as previous
	//if this!=&other and other not empty simply steal his contets
	//and giving him the default state 
	std::swap(head, other.head);
	std::swap(tail, other.tail);
	std::swap(count, other.count);
}
//destructor done 
template<typename _Ty>
single_linked_list<_Ty>::~single_linked_list()noexcept {
	clear();
}
//push back done
template<typename _Ty>
bool single_linked_list<_Ty>::push_back(const _Ty& data) {
	return push_back_node(data);
}
//push back done
template<typename _Ty>
bool single_linked_list<_Ty>::push_back( _Ty&& data) {
	return push_back_node(std::move(data));
}
//push front done
template<typename _Ty>
bool single_linked_list<_Ty>::push_front(_Ty&& data) {
	return push_front_node(std::move(data));
}
//push front done
template<typename _Ty>
bool single_linked_list<_Ty>::push_front(const _Ty& data) {
	return push_front_node(data);
}
//pop front done 
template<typename _Ty>
void single_linked_list<_Ty>::pop_front() {
	pop_front_node();
}
//pop back done
template<typename _Ty>
void single_linked_list<_Ty>::pop_back() {
	pop_back_node();
}
//reverse a linked list done
template<typename _Ty>
void single_linked_list<_Ty>::reverse()noexcept {
	reverse_linked_list();
}
//insert element done
template<typename _Ty>
bool single_linked_list<_Ty>::insert(const _Ty&data) {
	return insert_element(data);
}
//add unique done
template<typename _Ty>
bool single_linked_list<_Ty>::add_unique(const _Ty& data) {
	return add_unique_node(data);
}
//unique func done 
template<typename _Ty>
void single_linked_list<_Ty>::unique(){
	delete_duplicates(std::equal_to<>{});
}
//unique func done
template<typename _Ty>
template<typename _Pred1>
void single_linked_list<_Ty>::unique(_Pred1 _Pred) {
	delete_duplicates(_Pred);
}
//show func done
template<typename _Ty>
void single_linked_list<_Ty>::show() {
	//use this func if the elements can be printed
	list_node* ptr{ head };
	while (ptr != nullptr) {
		std::cout << ptr->data << '\n';
		ptr = ptr->next;
	}
	std::cout << '\n';
}
//remove func done
template<typename _Ty>
void single_linked_list<_Ty>::remove(const _Ty&data) {
	erase_node_if([&](const _Ty& _Other) -> bool { return _Other == data; });
}
//remove_if func done
template<typename _Ty>
template<typename _Pred1>
void single_linked_list<_Ty>::remove_if(_Pred1 _Pred) {
	erase_node_if(_Pred);
}
//swap func done
template<typename _Ty>
void single_linked_list<_Ty>::swap(single_linked_list<_Ty>&other) noexcept{
	std::swap(head, other.head);
	std::swap(tail, other.tail);
	std::swap(count, other.count);
}
//emplace_back func done 
template<typename _Ty>
template<class ..._Valty>
bool single_linked_list<_Ty>::emplace_back(_Valty&&..._Val) {
	return emplace_back_node(std::forward<_Valty>(_Val)...);
}
//emplace_front func done 
template<typename _Ty>
template<class ..._Valty>
bool single_linked_list<_Ty>::emplace_front(_Valty&&..._Val) {
	return emplace_front_node(std::forward<_Valty>(_Val)...);
}
//copy operator func done 
template<typename _Ty>
single_linked_list<_Ty>& single_linked_list<_Ty>::operator=(const single_linked_list<_Ty>
	& other)& {
	//thats the copy operator it works with the method prev curr
	//in order to avoid unecessary allocations
	//there 5 scenarios
	//1 we are empty so nothing happens
	//one of the lists is empty 
	//if this empty prev1==nullptr and curr2!=nullptr and we go first if
	//if the other one is empty prev2==nullptr then we must call the destructor in the
	// second if 
	//if both have elements equally nothing happens we just copy the elements
	//if size>other.size we have extra nodes we should delete them so curr1!=nullptr
	//and curr2==nullptr show we go in the last if 
	//if size<other.size we have to allocate some new nodes so
	//curr1==nullptr and curr2!=nullptr show we go to the first if 
	static_assert(std::is_copy_assignable_v<_Ty>, "you must be able to make this operation"
		"curr1->data=curr2->data");
	if (this != &other) {
		list_node* prev1{ nullptr };
		list_node* prev2{ nullptr };
		list_node* curr1{ head };
		list_node* curr2{ other.head };
		while (curr1 != nullptr && curr2 != nullptr) {
			curr1->data = curr2->data;
			prev1 = curr1;
			prev2 = curr2;
			curr1 = curr1->next;
			curr2 = curr2->next;
		}
		if (prev1 == nullptr && curr2 != nullptr || curr2 != nullptr && curr1 == nullptr) {
			while (curr2 != nullptr) {
				if (!push_back(curr2->data)) {
					clear();
					break;
				}
				curr2 = curr2->next;
			}
			return *this;
		}
		if (prev2 == nullptr && curr1 != nullptr) {
			clear();
			return *this;
		}
		if (curr1 != nullptr && curr2 == nullptr) {//size>other.size,sbhse perittous
			prev1->next = nullptr;
			tail = prev1;
			while (curr1 != nullptr) {
				list_node* ptr{ curr1 };
				curr1 = curr1->next;
				delete ptr;
				count--;
			}
			return *this;
		}

	}
	return *this;
}
//move operator func done 
template<typename _Ty>
single_linked_list<_Ty>& single_linked_list<_Ty>::operator=(single_linked_list<_Ty>
	&& other)& noexcept{
	//we just swap the pointer pretty much we still the data
	//attention if we move to ourselves we lose our data 
	clear();
	std::swap(head, other.head);
	std::swap(count, other.count);
	std::swap(tail, other.tail);
	return *this;
}
//copy with initializer list done 
template<typename _Ty>
single_linked_list<_Ty>& single_linked_list<_Ty>::operator=(const std::initializer_list<_Ty>
	& other)& {
	//thats the copy with initializer list it works with the method prev curr
	//in order to avoid unecessary allocations
	//there 5 scenarios
	//1 we are empty so nothing happens
	//one of the lists is empty 
	//if this empty prev1==nullptr and other.size()!=0 and we go first if
	//if the other one is empty prev2==nullptr &&curr1!=nuullptr then we must call the destructor in the
	// second if 
	//if both have elements equally nothing happens we just copy the elements
	//if size>other.size we have extra nodes we should delete them so curr1!=nullptr
	//and curr2==other.end() show we go in the last if 
	//if size<other.size we have to allocate some new nodes so
	//curr1==nullptr and curr2!=other.end() show we go to the first if 
	static_assert(std::is_copy_assignable_v<_Ty>, "you must be able to do this:"
		"curr1->data=*curr2");
		list_node* prev1{ nullptr };
		list_node* curr1{ head };
		const _Ty* prev2{ nullptr };
		const _Ty* curr2{ other.begin() };
		while (curr1 != nullptr && curr2 != other.end()) {
			curr1->data = *curr2;
			prev1 = curr1;
			prev2 = curr2;
			curr1 = curr1->next;
			curr2++;
		}
		if (prev1 == nullptr && other.size() != 0|| curr1 == nullptr && curr2 != other.end()) {
			while (curr2 != other.end()) {
				if (!push_back(*curr2)) {
					clear();
					break;
				}
				curr2++;
			}
			return *this;
		}
		if (prev2 == nullptr && curr1 != nullptr) {
			clear();
			return *this;
		}
		if (curr1 != nullptr && curr2 == other.end()) {
			prev1->next = nullptr;
			tail = prev1;
			while (curr1 != nullptr) {
				list_node* ptr{ curr1 };
				curr1 = curr1->next;
				delete ptr;
				count--;
			}
			return *this;
		}
		
	
	return *this;
}
//is_ascending func done
template<typename _Ty>
bool single_linked_list<_Ty>::is_ascending() const noexcept {
	if (count < 2)return true;
	iterator prev = begin();
	iterator curr = begin() + 1;
	while (curr != end()&& *prev<=*curr) {	
		prev++;
		curr++;
	}
	if (curr != end())return false;
	return true;
}
//is_descending func done
template<typename _Ty>
bool single_linked_list<_Ty>::is_descending()const noexcept {
	if (count < 2)return true;
	iterator prev = begin();
	iterator curr = begin() + 1;
	while (curr != end()&&*prev>=*curr) {
		prev++;
		curr++;
	}
	if (curr != end())return false;
	return true;
}
//is_sorted func done 
template<typename _Ty>
bool single_linked_list<_Ty>::is_sorted()const noexcept {
	if (count < 2)return true;
	iterator prev = begin();
	iterator curr = begin() + 1;
	bool asc = true;
	bool desc = true;
	while (curr != end()) {
		asc = asc && *prev <= *curr;
		desc = desc && *prev >= *curr;
		prev++;
		curr++;
	}
	return asc || desc;
	
}
//
template<typename _Ty>
template<typename Compare>
void single_linked_list<_Ty>::merge(single_linked_list<_Ty>& other, Compare comp) {
	merge_lists(other, comp);
}
//
template<typename _Ty>
template<typename Compare>
void single_linked_list<_Ty>::merge(single_linked_list<_Ty>&& other, Compare comp) {
	merge_lists(other, comp);
}
//
template<typename _Ty>
void single_linked_list<_Ty>::merge(single_linked_list<_Ty>& other) {
	merge_lists(other, std::less<>{});
}
//
template<typename _Ty>

void single_linked_list<_Ty>::merge(single_linked_list<_Ty>&& other) {
	merge_lists(other, std::less<>{});
}
//
_PANAGIOTIS_END


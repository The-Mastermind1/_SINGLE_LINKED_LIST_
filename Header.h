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

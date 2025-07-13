# _SINGLE_LINKED_LIST_
# Simple SINGLE LINKED LIST  header that i created using C++
AN entrire single_linked_list with all the basic features:
<br>
1) an  inside class of the simple list node with constructors<br>
~the default<br>
~with const element<br>
~with move element<br>
~and a private constructor used to craft the object in place<br>
3) an inside class of simple iterator (pretty much a wrapper around a pointer that points to a node or to nothing nullptr)
   with the iterator you have access to change the element or to see it and advance
   operations supported: operator ++ ,!=,==,*,+=,->,+,copy operator and move operator and a simple destructor
   <br>
4) another inside class of a simple const iterator (pretty much a wrapper around a pointer that points to a node or to nothing nullptr)
    with this kind of iterator you can just get the element for reading not writing it also supports same operations with the other iterator <br>
<br>
5) 3 private members head,tail,count <br>
6)push_back function which put the element at the end <br>
7)push_front functions  which put the element at the start of the list <br>
8)pop_front which delete the first element of the list <br>
9)pop_back which delete the last element of the list<br>
10)clear func this just is the destructor the destructor uses it<br>
11)reverse func this just reverses the list<br>
12)insert element justs inserts the element but considers that the list is sorted because sorting a list is already a very expensive operation<br>
13)delete duplicates ,it just deletes the duplicates again we consider that the list is sorted if it is not we might want to use a hashset in order to keep track of the elements<br>
14)erase_node_if erase nodes of the list if a certain condition is met with the help of the passed function<br>
15)emplace_back same as push_back it just constructs the object in place the only difference<br>
16)emplace_front same as push_front it just constructs the object in place the only difference<br>
17)start funcs finish funcs and cstart funcs and cend funcs they just return an iterator or const iterator to the start of the list the head or the end we consider the end to be nullptr<br>
18)merge lists just combines two lists and leaves the other empty<br>
19) is_ascending_ checks if the elements are in ascending order using a compare func<br>
20) is_descending_ checks if the elements are in descending order using a compare func<br>
21) is sorted checks if the elements are either sorted in  ascending or descending order<br>

# 👥CONTRIBUTORS:

~The-Mastermind1

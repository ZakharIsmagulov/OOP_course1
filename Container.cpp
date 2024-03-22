#pragma once
#include "Container.h"
#include <iostream>

template<class T> std::ostream &operator<<(std::ostream &os, Cell<T> const &cell) {
	return os << *(cell.cell);
}

template<class T> Cell<T>::Cell(T *cl) {
	cell = cl;
	prev = nullptr;
	next = nullptr;
}

template<class T> Cell<T>::~Cell() {
	delete cell;
}

template<class T> Iterator<T>::Iterator(Cell<T> *cell) {
	current = cell;
}

template<class T> Iterator<T>& Iterator<T>::operator=(Iterator<T> const &other) {
	current = other.current;
	return *this;
}

template<class T> Iterator<T>& Iterator<T>::operator++() {
	if (current->next == nullptr) {
		throw std::out_of_range("Error: no more cells");
	}
	else {
		current = current->next;
	}
	return *this;
}

template<class T> Iterator<T> Iterator<T>::operator++(int i) {
	if (current->next == nullptr) {
		throw std::out_of_range("Error: no more cells");
	}
	else {
		Iterator temp(current);
		current = current->next;
		return temp;
	}
}

template<class T> T &Iterator<T>::operator*() const {
	return *(current->cell);
}

template<class T> T *Iterator<T>::operator&() const {
	return current->cell;
}

template<class T> T *Iterator<T>::operator->() const {
	return current->cell;
}

template<class T> bool Iterator<T>::operator==(Iterator<T> const &other) const {
	return current == other.current;
}

template<class T> bool Iterator<T>::operator!=(Iterator<T> const &other) const {
	return current != other.current;
}

template<class T> Container<T>::Container() {
	head = nullptr;
	tail = nullptr;
	count = 0;
}

template<class T> Container<T>::Container(T *unit) {
	head = new Cell<T>(unit);
	tail = head;
	count = 1;
}

template<class T> Container<T>::~Container() {
	Cell<T> *cur = head;
	while (cur != nullptr) {
		Cell<T> *temp_next = cur->next;
		delete cur;
		cur = temp_next;
	}
}

template<class T> void Container<T>::add_head(T *unit) {
	if (head == nullptr) {
		head = new Cell<T>(unit);
		head->prev = nullptr;
		head->next = nullptr;
		tail = head;
	}
	else {
		Cell<T> *temp = new Cell<T>(unit);
		head->prev = temp;
		temp->next = head;
		head = temp;
	}
	count++;
}

template<class T> void Container<T>::add_tail(T *unit) {
	if (head == nullptr) {
		head = new Cell<T>(unit);
		head->prev = nullptr;
		head->next = nullptr;
		tail = head;
	}
	else {
		Cell<T> *temp = new Cell<T>(unit);
		tail->next = temp;
		temp->prev = tail;
		tail = temp;
	}
	count++;
}

template<class T> void Container<T>::del_head() {
	if (head == nullptr) {
		throw std::out_of_range("Error: container is empty");
	}
	else if (head->next == nullptr) {
		delete head;
		head = nullptr;
		tail = nullptr;
		count--;
	}
	else {
		Cell<T> *temp = head->next;
		temp->prev = nullptr;
		delete head;
		head = temp;
		count--;
	}
}

template<class T> void Container<T>::del_tail() {
	if (head == nullptr) {
		throw std::out_of_range("Error: container is empty");
	}
	else if (head->next == nullptr) {
		delete head;
		head = nullptr;
		tail = nullptr;
		count--;
	}
	else {
		Cell<T> *temp = tail->prev;
		temp->next = nullptr;
		delete tail;
		tail = temp;
		count--;
	}
}

template<class T> void Container<T>::print_head() const {
	if (head != nullptr) {
		std::cout << *head << '\n';
	}
	else {
		throw std::out_of_range("Error: container is empty");
	}
}

template<class T> void Container<T>::print_tail() const {
	if (head != nullptr) {
		std::cout << *tail << '\n';
	}
	else {
		throw std::out_of_range("Error: container is empty");
	}
}

template<class T> Iterator<T> Container<T>::begin() const {
	if (head != nullptr) {
		return Iterator<T>(head);
	}
	else {
		throw std::out_of_range("Error: container is empty");
	}
}

template<class T> Iterator<T> Container<T>::end() const {
	if (head != nullptr) {
		return Iterator<T>(tail);
	}
	else {
		throw std::out_of_range("Error: container is empty");
	}
}

template<class T> int const Container<T>::get_count() const {
	return count;
}

template<class T> bool const Container<T>::is_empty() const {
	if (count == 0) {
		return true;
	}
	else {
		return false;
	}
}

template<class T> void Container<T>::swap(Container<T> &cont1, Container<T> &cont2) {
	Cell<T>* temp_head1 = cont1.head;
	Cell<T>* temp_tail1 = cont1.tail;
	int temp_count1 = cont1.count;
	cont1.head = cont2.head;
	cont1.tail = cont2.tail;
	cont1.count = cont2.count;
	cont2.head = temp_head1;
	cont2.tail = temp_tail1;
	cont2.count = temp_count1;
}

template<class T> void Container<T>::reverse() {
	if (head == nullptr) {
		std::cout << "container is empty\n";
	}
	else if (head->next == nullptr) {
		std::cout << "container length is 1\n";
	}
	else {
		Cell<T>* cur = head;
		Cell<T>* temp_next = cur->next;
		while (true) {
			cur->next = cur->prev;
			cur->prev = temp_next;
			cur = temp_next;
			if (cur != nullptr) {
				temp_next = cur->next;
			}
			else {
				break;
			}
		}
		Cell<T> *temp_head = head;
		head = tail;
		tail = temp_head;
	}
}

template<class T> void Container<T>::clear() {
	while (count != 0) {
		del_head();
	}
}

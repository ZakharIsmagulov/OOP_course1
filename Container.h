#pragma once

template<class T> struct Cell {
	Cell(T *cl);
	~Cell();
	T *cell;
	Cell *prev;
	Cell *next;
};

template<class T> class Iterator {
public:
	Iterator(Cell<T> *cell);
	Iterator &operator=(Iterator const &other);
	Iterator &operator++();
	Iterator operator++(int i);
	T &operator*() const;
	T *operator&() const;
	T *operator->() const;
	bool operator==(Iterator const &other) const;
	bool operator!=(Iterator const &other) const;
private:
	Cell<T> *current;
};

template<class T> class Container {
public:
	typedef Iterator<T> iterator;

	Container();
	Container(T *unit);
	~Container();

	void add_head(T *unit);
	void add_tail(T *unit);
	void del_head();
	void del_tail();
	void print_head() const;
	void print_tail() const;
	Iterator<T> begin() const;
	Iterator<T> end() const;
	int const get_count() const;
	bool const is_empty() const;
	static void swap(Container<T> &cont1, Container<T> &cont2);
	void reverse();
	void clear();

private:
	Cell<T> *head;
	Cell<T> *tail;
	int count;
};


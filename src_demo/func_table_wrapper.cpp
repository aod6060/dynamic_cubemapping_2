#include "library.h"

namespace ftw {
	ft::Table* _table;

	void init(ft::Table* table) {
		_table = table;
	}

	ft::Table* get() {
		return _table;
	}

	void release() {
		_table = nullptr;
	}

}
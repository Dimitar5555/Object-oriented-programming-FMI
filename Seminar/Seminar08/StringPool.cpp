#include "StringPool.h"
#include<cstring>
#include<exception>
#include<stdexcept>

StringPool::LittleString::LittleString() {
	data[0] = '\0';
}

StringPool::LittleString::LittleString(const char* arg) {
	if (strlen(arg) < 64) {
		strcpy_s(data, arg);
	}
}

const char* StringPool::LittleString::getString() const {
	return data;
}

bool StringPool::LittleString::operator==(const LittleString& other) const {
	return (strcmp(data, other.data) == 0);
}

bool StringPool::LittleString::operator<(const LittleString& other) const {
	return (strcmp(data, other.data) == -1);
}

void StringPool::free() {
	for (size_t i = 0; i < size; i++) {
		if (data[i]->refCount == 1)
			delete data[i];
		else
			data[i]->refCount--;
	}
	delete[] data;
}

void StringPool::copyFrom(const StringPool& other) {
	size = other.size;
	capacity = other.capacity;

	for (size_t i = 0; i < size; i++) {
		data[i] = new LittleString(*other.data[i]);
		data[i]->refCount = 1;

		while (i < size && *data[i] == *other.data[i + 1]) {
			data[i + 1] = data[i];
			data[i + 1]->refCount++;
			i++;
		}
	}
}

StringPool::StringPool(size_t cap) {
	size = 0;
	capacity = cap;

	data = new LittleString * [cap] {nullptr};
}

StringPool::StringPool(const StringPool& other) {
	copyFrom(other);
}

StringPool& StringPool::operator=(const StringPool& other) {
	if (this != &other) {
		free();
		copyFrom(other);
	}
	return *this;
}

size_t StringPool::mSize() const {
	return size;
}

void StringPool::insert(const LittleString& arg) {
	size_t indexAt = size;

	while (indexAt != 0 && data[indexAt - 1] != nullptr && (arg < *data[indexAt - 1])) {
		data[indexAt] = data[indexAt - 1];
		indexAt--;
	}

	if (indexAt != 0 && data[indexAt - 1] != nullptr && (arg == *data[indexAt - 1])) {
		data[indexAt] = data[indexAt - 1];
		data[indexAt]->refCount++;
	}
	else {
		data[indexAt] = new LittleString(arg);
		data[indexAt]->refCount = 1;
	}

	size++;
}

int StringPool::find(const LittleString& arg) const {
	int l = 0;
	int r = size - 1;

	int at = -1;

	while (l <= r) {
		int mid = l + (r - l) / 2;

		if (*data[mid] < arg) {
			l = mid + 1;
		}
		else {
			if (*data[mid] == arg)
				at = mid;

			r = mid - 1;
		}
	}

	return at;
}

const StringPool::LittleString& StringPool::operator[](size_t idx) const {
	if (idx >= size)
		throw std::runtime_error("Invalid index");

	return *data[idx];
}

StringPool::~StringPool() {
	free();
}

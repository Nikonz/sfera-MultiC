#include <stdexcept>
#include <string>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include "allocator.h"

Pointer::Pointer(struct ptr *newelem)
{
	this->data = newelem;
}

void *Pointer::get() const
{
	return this->data->base;
}

Allocator::Allocator(void *base, size_t size)
{
	this->base = base;
	this->size = size;
	ptr_list.clear();
}

Pointer Allocator::alloc(size_t N)
{
	if (this->ptr_list.empty()) {
		struct ptr *newone = (struct ptr *)malloc(sizeof(*newone));
		newone->base = base;
		newone->size = N;
        this->ptr_list.push_back(newone);
		return Pointer(newone);
	}

	list <ptr*>::iterator cur = this->ptr_list.begin();
	list <ptr*>::iterator next = this->ptr_list.begin();
	next++;

	while (next != this->ptr_list.end()) {
		if ((char *)(*next)->base - ((char*)(*cur)->base + (*cur)->size) >= N) {
			struct ptr *newone = (struct ptr *)malloc(sizeof(*newone));
			newone->base = (char*)(*cur)->base + (*cur)->size;
			newone->size = N;
			this->ptr_list.insert(next, newone);
			return Pointer(newone);
		}
        cur++;
		next++;
	}

	if (this->size + (char *)this->base - ((char*)(*cur)->base + (*cur)->size) >= N) {
		struct ptr *newone = (struct ptr *)malloc(sizeof(*newone));
		newone->base = (char*)(*cur)->base + (*cur)->size;
		newone->size = N;
		this->ptr_list.push_back(newone);
		return Pointer(newone);
	}
	throw AllocError(AllocErrorType::NoMemory, "Bad alloc");
}

void Allocator::free(Pointer &p)
{
	struct ptr *x = p.data;
    list <ptr*>::iterator cur = this->ptr_list.begin();
	while (cur != this->ptr_list.end() && (*cur) != x) {
		cur++;
	}
	this->ptr_list.erase(cur);
	p.data->base = 0;
}

void Allocator::realloc(Pointer &p, size_t N)
{
	struct ptr *x = p.data;
	if (x == NULL) {
		p = alloc(N);
		return;
	}

    list <ptr*>::iterator cur = this->ptr_list.begin();
	while (cur != this->ptr_list.end() && (*cur) != x) {
		cur++;
	}
	list <ptr*>::iterator next = cur;
    next++;

	if (cur == this->ptr_list.begin()) {
        if (next == this->ptr_list.end()) {
            if (this->size >= N) {
                for (int i = 0; i < (*cur)->size; i++) {
                    *((char *)this->base + i) = *((char*)(*cur)->base + i);
                }
                (*cur)->base = this->base;
                (*cur)->size = N;
            }
        } else {
            if ((char *)(*next)->base - (char *)this->base >= N) {
                for(int i = 0; i < (*cur)->size; i++) {
					*((char *)this->base + i) = *((char*)(*cur)->base + i);
				}
				(*cur)->base = this->base;
				(*cur)->size = N;
            } else {
                char *x = (char *)alloc(N).get();
				for(int i = 0; i < (*cur)->size; i++) {
					*((char *)x + i) = *((char*)(*cur)->base + i);
				}
				(*cur)->base = x;
				(*cur)->size = N;
            }
        }
	} else {
        if (next == this->ptr_list.end()) {
            if ((char *)base + size - ((char *)(*cur)->base) >= N) {
				(*cur)->size = N;
			}
        } else {
            if ((char *)(*next)->base - (char *)(*cur)->base >= N) {
				(*cur)->size = N;
			}
			else {
				char *x = (char *)alloc(N).get();
				for(int i = 0; i < (*cur)->size; i++) {
					*((char *)x + i) = *((char*)(*cur)->base + i);
				}
				(*cur)->base = x;
				(*cur)->size = N;
			}
        }
	}
}

void Allocator::defrag()
{
	list <ptr*>::iterator cur = this->ptr_list.begin();
	list <ptr*>::iterator next = this->ptr_list.begin();
	next++;

	if (!this->ptr_list.empty()) {
        if (next == this->ptr_list.end()) {
            if ((char *)(*cur)->base - (char *)base) {
                for (int i = 0; i < (*cur)->size; i++) {
                    *((char *)base + i) = *((char*)(*cur)->base + i);
                }
                (*cur)->base = this->base;
            }
        }
        while (next != this->ptr_list.end()) {
            if ((char *)(*next)->base - ((char*)(*cur)->base + (*cur)->size)) {
                for (int i = 0; i < (*next)->size; i++) {
                    *((char *)(*cur)->base + i) = *((char *)(*next)->base + i);
                }
                (*next)->base = (char*)(*cur)->base + (*cur)->size;
            }
            cur++;
            next++;
        }
	}
}

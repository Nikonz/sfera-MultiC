#include <stdexcept>
#include <string>
#include <vector>
#include <list>

using std::list;

enum class AllocErrorType {
    InvalidFree,
    NoMemory,
};

class AllocError: std::runtime_error {
private:
    AllocErrorType type;
public:
    AllocError(AllocErrorType _type, std::string message):
            runtime_error(message),
            type(_type)
    {}
    AllocErrorType getType() const { return type; }
};

struct ptr {
    void *base;
    size_t size;
};

class Pointer {
public:
    Pointer(struct ptr *newelem = NULL);
    void *get() const;
    struct ptr *data;
};

class Allocator {
public:
    Allocator(void *base, size_t size);
    Pointer alloc(size_t N);
    void realloc(Pointer &p, size_t N);
    void free(Pointer &p);
    void defrag();
    std::string dump() { return ""; }
private:
    void *base;
    size_t size;
    list <ptr*> ptr_list;
};


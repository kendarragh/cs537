typedef struct Tuple {
	void *ptr;
	size_t len;
	int mal;
}Tuple;

Tuple* createTuple(void* ptr, size_t len);
void setAlloc(Tuple* t);
void setFree(Tuple* t);

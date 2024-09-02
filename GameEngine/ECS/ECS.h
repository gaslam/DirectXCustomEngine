struct ComponentStore
{
	unsigned int TypeCount;
	unsigned int Cap;
	unsigned int Size;
	size_t* DataSizeArray;
	size_t* DataOffsetArray;
	void* data;
};


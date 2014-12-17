#pragma once




template <class T>
class ObjectManager
{
public:
	ObjectManager() { objects = NULL; objcount = 0; }
	~ObjectManager()
	{
		if ( objects )
		{
			for( unsigned int i = 0;i < objcount;i++ ) 
			{
				if( !objects[i] ) 
				{
					continue;
				} 
			
				delete objects[i];
				objects[i] = NULL;
			}

			delete []objects;
			objects = NULL;
		}
	}
	virtual T* add();
	virtual void add(T *);
	

	unsigned int objcount;
	T **objects;
};

template <class T>
T* ObjectManager<T>::add()
{
	T *new_obj = new T;

	objects = (T**) realloc( objects, ++objcount * sizeof(T*) );
	objects[objcount - 1] = new_obj;

	return new_obj;
}

template <class T>
void ObjectManager<T>::add(T *new_obj)
{
	objects = (T**) realloc( objects, ++objcount * sizeof(T*) );
	objects[objcount - 1] = new_obj;
}
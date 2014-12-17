#pragma once




template <class T>
class ObjectManager
{
public:
	ObjectManager() { objects = NULL; numobjs = 0; }
	~ObjectManager()
	{
		if ( objects && numobjs == 0)
		{
			for( unsigned int i = 0;i < numobjs;i++ ) 
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
			
			numobjs = 0;
		}
		else
		{
			printf ("ObjectManager: Really Weird.. no Objects but an numobjs != 0\n");
		}
	}
	//virtual T* add();
	virtual void add(T *);
	void RemoveAllObjects();
	SDL_bool hasa(T *);
	

	unsigned int numobjs;
	T **objects;
};

/*template <class T>
T* ObjectManager<T>::add()
{
	T *new_obj = new T;

	objects = (T**) realloc( objects, ++numobjs * sizeof(T*) );
	objects[numobjs - 1] = new_obj;

	return new_obj;
}*/

template <class T>
SDL_bool ObjectManager<T>::hasa(T *o)
{
	int i;
	for (i=0; i < numobjs; i++)
	{
		if (o == objects[i])
			return SDL_TRUE;
	}
	
	return SDL_FALSE;
}

template <class T>
void ObjectManager<T>::add(T *new_obj)
{
	objects = (T**) realloc( objects, ++numobjs * sizeof(T*) );
	objects[numobjs - 1] = new_obj;
}


template <class T>
void ObjectManager<T>::RemoveAllObjects()
{
	if ( objects )
	{
		free(objects);
		objects = NULL;
		numobjs = 0;
	}
}
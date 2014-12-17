#pragma once

#ifdef _DEBUG
	#include <typeinfo>
#endif

#define OM_SAVE_OBJS_AT_DESTROY				SDL_FALSE
#define OM_OBLITERATE_OBJS_AT_DESTROY		SDL_TRUE

#define OM_DELETE_OBJS						SDL_FALSE
#define OM_FREE_OBJS						SDL_TRUE



template <class T>
class ObjectManager
{
public:
	ObjectManager(SDL_bool delete_at_destroy=OM_SAVE_OBJS_AT_DESTROY, SDL_bool delete_or_free = OM_DELETE_OBJS)
	{
		objects = NULL; objcount = 0;
		obliterate_objects_at_destructor = delete_or_free;
		obliterate_objects_at_destructor = delete_at_destroy;
	}
	~ObjectManager()
	{
		if (obliterate_objects_at_destructor == SDL_TRUE)
		{
			//std::cout<<<<std::endl;
			if (delete_or_free_objects == OM_DELETE_OBJS)
			{
				#ifdef _DEBUG
					DEBUGLOG("Object Manager: Deleting OM<%s> contents\n", typeid(T).name());
				#endif
				DeleteAllObjectContents();
			}
			else // free them
			{
				#ifdef _DEBUG
					DEBUGLOG("Object Manager: Freeing OM<%s> contents\n", typeid(T).name());
				#endif
				FreeAllObjectContents();
			}
		}
		SDL_free(objects);
		objects= NULL;
		objcount = 0;
	}
	//virtual T* add();
	virtual void add(T *);
	void RemoveAllObjects();
	int hasa(T *);
	void Remove(int n);
	void DeleteAllObjectContents(); ///< uses Delete instead of Free()
	void FreeAllObjectContents();	///< uses free();
	

	SDL_bool obliterate_objects_at_destructor;	///< this sets to only delete the manager of the objects/ or should we also delete the objects too???
	unsigned int objcount;
	SDL_bool delete_or_free_objects;	///< delete them or free them? (depends of new was used or SDL_malloc-equiv)

	T **objects;
};

/*template <class T>
T* ObjectManager<T>::add()
{
	T *new_obj = new T;

	objects = (T**) SDL_realloc( objects, ++objcount * sizeof(T*) );
	objects[objcount - 1] = new_obj;

	return new_obj;
}*/

template <class T>
int ObjectManager<T>::hasa(T *o)
{
	unsigned int i;
	for (i=0; i < objcount; i++)
	{
		if (o == objects[i])
		{
			return (int)i;
		}
	}
	
	return -1;
}

template <class T>
void ObjectManager<T> :: Remove(int n)
{
	objects[n] = NULL;
	
	for (n=n; n<objcount-1; n++)
	{
		objects[n] = objects[n+1];
	}
	objects = (T**) SDL_realloc( objects, --objcount * sizeof(T*) );
}

template <class T>
void ObjectManager<T>::add(T *new_obj)
{
	objects = (T**) SDL_realloc( objects, ++objcount * sizeof(T*) );
	objects[objcount - 1] = new_obj;
}


/// Remove from OM but don't delete from memory
template <class T>
void ObjectManager<T>::RemoveAllObjects()
{
	if ( objects && objcount > 0 )
	{
		SDL_free(objects);
		objects = NULL;
		objcount = 0;
	}
}

template <class T>
void ObjectManager<T>::DeleteAllObjectContents()
{
	if (objects && objcount > 0)
	{
		DEBUGLOG("\tDeleting %d objects\n", objcount);
		
		for (unsigned int i=0; i < objcount; i++)
		{
			delete objects[i];
		}
		
		objcount = 0;
	}
	
	
}

template <class T>
void ObjectManager<T>::FreeAllObjectContents()
{
	if (objects && objcount > 0)
	{
		DEBUGLOG("\tFreeing %d objects\n", objcount);
		for (unsigned int i=0; i < objcount; i++)
		{
			SDL_free(objects[i]);
		}
		
		objcount = 0;
	}
}
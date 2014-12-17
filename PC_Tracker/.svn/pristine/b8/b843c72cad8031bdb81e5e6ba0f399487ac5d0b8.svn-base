#pragma once


#define OM_DELETE_OBJS					SDL_FALSE
#define OM_FREE_OBJS					SDL_TRUE

#define OM_DONT_OBLITERATE_OBJS_AT_DESTROY	SDL_FALSE
#define OM_OBLITERATE_OBJS_AT_DESTROY			SDL_TRUE

template <class T>
class ObjectManager
{
public:
	ObjectManager(SDL_bool delete_at_destroy=SDL_FALSE, SDL_bool delete_or_free = OM_DELETE_OBJS)
	{
		objects = NULL; numobjs = 0;
		obliterate_objects_at_destructor = delete_or_free;
		obliterate_objects_at_destructor = delete_at_destroy;
	}
	~ObjectManager()
	{
		if (obliterate_objects_at_destructor == SDL_TRUE)
		{
			if (delete_or_free_objects == OM_DELETE_OBJS)
			{
				DEBUGLOG("Object Manager: Deleting Object(s) contents\n");
				DeleteAllObjectContents();
			}
			else // free them
			{
				DEBUGLOG("Object Manager: Freeing Object(s) contents\n");
				FreeAllObjectContents();
			}
		}
		SDL_free(objects);
		objects= NULL;
		numobjs = 0;
	}
	//virtual T* add();
	virtual void add(T *);
	void RemoveAllObjects();
	SDL_bool hasa(T *);
	void DeleteAllObjectContents(); ///< uses Delete instead of Free()
	void FreeAllObjectContents();	///< uses free();
	

	SDL_bool obliterate_objects_at_destructor;	///< this sets to only delete the manager of the objects/ or should we also delete the objects too???
	unsigned int numobjs;
	SDL_bool delete_or_free_objects;	///< delete them or free them? (depends of new was used or SDL_malloc-equiv)
	T **objects;
};

/*template <class T>
T* ObjectManager<T>::add()
{
	T *new_obj = new T;

	objects = (T**) SDL_realloc( objects, ++numobjs * sizeof(T*) );
	objects[numobjs - 1] = new_obj;

	return new_obj;
}*/

template <class T>
SDL_bool ObjectManager<T>::hasa(T *o)
{
	unsigned int i;
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
	objects = (T**) SDL_realloc( objects, ++numobjs * sizeof(T*) );
	objects[numobjs - 1] = new_obj;
}


/// Remove from OM but don't delete from memory
template <class T>
void ObjectManager<T>::RemoveAllObjects()
{
	if ( objects && numobjs > 0 )
	{
		SDL_free(objects);
		objects = NULL;
		numobjs = 0;
	}
}

template <class T>
void ObjectManager<T>::DeleteAllObjectContents()
{
	if (objects && numobjs > 0)
	{
		DEBUGLOG("\tDeleting %d objects\n", numobjs);
		
		for (unsigned int i=0; i < numobjs; i++)
		{
			delete objects[i];
		}
		
		numobjs = 0;
	}
	
	
}

template <class T>
void ObjectManager<T>::FreeAllObjectContents()
{
	if (objects && numobjs > 0)
	{
		DEBUGLOG("\tFreeing %d objects\n", numobjs);
		for (unsigned int i=0; i < numobjs; i++)
		{
			SDL_free(objects[i]);
		}
		
		numobjs = 0;
	}
}
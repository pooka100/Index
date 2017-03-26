#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct word
{
	/* name of str */

	char *name;
	struct word *next;
	struct hashtable_file *htable;
} word;


typedef struct file_
{
	char *name;
	struct file_ *next;
	size_t count;
} file_;

/* Wish I had templating... */
typedef struct hashtable_str
{
	size_t size;
	size_t taken;
	word **table;

} hashtable_str;

typedef struct hashtable_file
{
	size_t size;
	size_t taken;
	char *name;
	file_ **table;
} hashtable_file;

/* Using voids so that I do not have to write this function again */
void hash_init_str(hashtable_str *htable)
{
	/* Start with 32 items */
	htable->size = 32;
	htable->taken = 0;
	htable->table = (word **)calloc(32, sizeof(word *));
}


void hash_init_file(hashtable_file *htable)
{
	/* Start with 8 items */
	htable->size = 8;
	htable->taken = 0;
	htable->table = (file_ **)calloc(8, sizeof(file_*));
}
/* FNV hash */

unsigned hash(char *str, size_t len)
{
	unsigned char *p = str;
	unsigned h = 0x811c9dc5;
	int i;

	for(i = 0; i < len; ++i)
	{
		h = (h ^ p[i]) * 0x01000193;
	}
	return h;
}

void hash_expand_word(hashtable_str *htable)
{
	
	int i;
	word *start;
	word *tmp;
	/* link up everything into one nice linked list */
	for(i = 0; i < htable->size; ++i)
	{
		if(htable->table[i])
		{
			start = htable->table[i];
			tmp   = htable->table[i];
			while(tmp->next != NULL)
			{
				tmp = tmp->next;
			}
			break;
		}

	}

	for(; i < htable->size; ++i)
	{
		if(htable->table[i])
		{
			tmp->next = htable->table[i];
			tmp = tmp->next;

			while(tmp->next != NULL)
			{
				tmp = tmp->next;
			}
		}
	}

	/* clear out the array */

	for(i = 0; i < htable->size; ++i)
		htable->table[i] = NULL;

	/* put them back in */

	tmp = start;

	/* free outer layer of hashtable and replace with bigger one */
	
	word **tmparray = (word **)calloc(htable->size * 2, sizeof(word *));
	word **tmptmparray = htable->table;
	htable->table = tmparray;

	/* insert back into bigger array */
	unsigned index;
	word *entry;
	htable->size *= 2;
	while(start)
	{
		index = hash(start->name, strlen(start->name));
		index = index % htable->size;
		
		entry = (htable->table)[index];	
		if(entry == NULL)
		{
			tmp = start->next;
			(htable->table)[index] = start;
			(htable->table)[index]->next = NULL;
			start = tmp;
		}
		else
		{
			while(entry)
			{
				if(entry->next == NULL)
				{
					tmp = start->next;
					entry->next = start;
					entry->next->next = NULL;
					start = tmp;
				}
				entry = entry->next;
			}
		}

			
	}
	free(tmptmparray);
}
	
void hash_expand_file(hashtable_file *htable)
{
	
	int i;
	file_ *start;
	file_ *tmp;
	/* link up everything into one nice linked list */
	for(i = 0; i < htable->size; ++i)
	{
		if(htable->table[i])
		{
			start = htable->table[i];
			tmp   = htable->table[i];
			while(tmp->next != NULL)
			{
				tmp = tmp->next;
			}
			break;
		}

	}

	for(; i < htable->size; ++i)
	{
		if(htable->table[i])
		{
			tmp->next = htable->table[i];
			tmp = tmp->next;

			while(tmp->next != NULL)
			{
				tmp = tmp->next;
			}
		}
	}

	/* clear out the array */

	for(i = 0; i < htable->size; ++i)
		htable->table[i] = NULL;

	/* put them back in */

	tmp = start;

	/* free outer layer of hashtable and replace with bigger one */
	
	file_ **tmparray = (file_ **)calloc(htable->size * 2, sizeof(file_ *));
	file_ **tmptmparray = htable->table;
	htable->table = tmparray;

	/* insert back into bigger array */
	unsigned index;
	file_ *entry;
	htable->size *= 2;
	while(start)
	{
		index = hash(start->name, strlen(start->name));
		index = index % htable->size;
		
		entry = (htable->table)[index];	
		if(entry == NULL)
		{
			tmp = start->next;
			(htable->table)[index] = start;
			(htable->table)[index]->next = NULL;
			start = tmp;
		}
		else
		{
			while(entry)
			{
				if(entry->next == NULL)
				{
					tmp = start->next;
					entry->next = start;
					entry->next->next = NULL;
					start = tmp;
				}
				entry = entry->next;
			}
		}

			
	}
	free(tmptmparray);
}

word *hash_get_word(char *str, hashtable_str *htable)
{
	if(htable->taken >= htable->size) hash_expand_word(htable);
	unsigned index = hash(str, strlen(str));
	index = index % htable->size;
	word *entry = (htable->table)[index];
	word *tmp;
	if(entry == NULL)
	{
		entry = (word *)malloc(sizeof(word));
		entry->name = str;
		entry->next = NULL;
		hash_init_file(entry->htable);
	}
	else
	{
		while(entry)
		{
			if(entry->name == str)
			{
				return entry;
			}
			tmp = entry;
			entry = entry->next;
		}
		tmp->next = (word *)malloc(sizeof(word));
		tmp->next->name = str;
		tmp->next->next = NULL;
		hash_init_file(tmp->next->htable);
	}
	++(htable->taken);
	return tmp;
}

void hash_put_file(char *str, word *w)
{
	if(w->htable->taken >= w->htable->size) hash_expand_file(w->htable);
	unsigned index = hash(str, strlen(str));
	index = index % w->htable->size;
	file_ *entry = (w->htable->table)[index];
	file_ *tmp;
	if(entry == NULL)
	{
		entry = (file_ *)malloc(sizeof(file_));
		entry->name = str;
		entry->next = NULL;
		entry->count = 1;
	}
	else
	{
		while(entry)
		{
			if(entry->name == str)
			{
				entry->count += 1;
				return;
			}
			tmp = entry;
			entry = entry->next;
		}
		tmp->next = (file_ *)malloc(sizeof(file_));
		tmp->next->name = str;
		tmp->next->next = NULL;
		tmp->next->count = 1;
	}
	++(w->htable->taken);
	return;
}

void hash_free_file(hashtable_file *htable)
{
	unsigned i;
	file_ *entry;
	file_ *tmp;
	for(i = 0; i < htable->size; ++i)
	{
		entry = (htable->table)[i];
		while(entry)
		{
			tmp = entry->next;
			free(entry);
			entry = tmp;
		}
	}
}

void hash_free_word(hashtable_str *htable)
{
	unsigned i;
	word *entry;
	word *tmp;
	for(i = 0; i < htable->size; ++i)
	{
		entry = (htable->table)[i];
		while(entry)
		{
			hash_free_file(entry->htable);
			tmp = entry->next;
			free(entry);
			entry = tmp;
		}
	}
	free(htable);
}

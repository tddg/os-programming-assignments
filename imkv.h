#ifndef __imkv_h__
#define __imkv_h__

// err code
typedef enum {
	HT_INIT_OK,
	HT_INIT_FAILED,
	HT_INSERT_OK,
	HT_INSERT_FAILED,
	HT_FOUND,
	HT_NOTFOUND,
	HT_DELETE_OK,
	HT_DELETE_FAILED,
} HT_RET;

/* External functions: these are what you must define */

// Initialize the data structure: 
// num_buckets specifies the maximum number of buckets in the bucket hash table
HT_RET ht_init(unsigned int num_buckets);

// Tries to insert a new key-value pair
HT_RET ht_insert(char *key, char *val);

// Tries to lookup a value given a key; found val should set the 2nd parameter
HT_RET ht_lookup(char *key, char **val);

// Tries to delete a key-value pair specified by the parameter
HT_RET ht_delete(char *key);

/* End of API declaration */

#endif // __imkv_h__

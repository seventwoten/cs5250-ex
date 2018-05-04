#include <stdio.h>

#define FILTER_SIZE 256

//  Bloom filter is a global variable
int bloom_filter[FILTER_SIZE/32];

/*  Function prototypes  */
unsigned int hash(unsigned char *key, int len);
void init_bloom();
int add_bloom(unsigned int key);
int check_bloom(unsigned int key);

void print_bloom();
void key_to_bitpos(unsigned int h, char *b1, char *b2, char *b3);


/*  Main function  */
int main()
{
    init_bloom();
    printf("Initialised bloom filter: \n");
    print_bloom();
    printf("\n");
    
    int test = 123;
    int present1 = add_bloom(test);
    printf("Check if %d has been seen before: %d\n", test, present1);
    printf("Current bloom filter: \n");
    print_bloom();
    printf("\n");
    
    int present2 = add_bloom(test);
    
    printf("Check if %d has been seen before: %d\n", test, present2);
    printf("Current bloom filter: \n");
    print_bloom();
    printf("\n");
    
    // char greeting[] = "Hello";
    // printf("%d\n", hash(greeting, 5));
}


// jenkins_one_at_a_time_hash:
// Given an array of length len bytes pointed to by key, 
// produce a 32 bit (unsigned int) of hash.
// https://stackoverflow.com/questions/7666509/hash-function-for-string

unsigned int hash(unsigned char *key, int len)
{
    unsigned int hash, i;
    for(hash = i = 0; i < len; ++i)
    {
        hash += key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}


//  Zeroes out the bloom filter structure
void init_bloom()
{
    int i;
    for(i = 0; i < FILTER_SIZE/32; i++)
    {
        bloom_filter[i] = 0;
    }
    
}

//  Adds key to the Bloom filter. 
//  Returns 0 on success, 1 if all three bits are set (collision)
int add_bloom(unsigned int key)
{
    // call hash on key
    char *pkey = (char *)&key;
    unsigned int h = hash(pkey, 4);
    int is_present = check_bloom(key);
    
    if (!is_present)
    {
        char b1, b2, b3;
        key_to_bitpos(h, &b1, &b2, &b3);
        
        bloom_filter[b1/32] = 1 << b1%32;
        bloom_filter[b2/32] = 1 << b2%32;
        bloom_filter[b3/32] = 1 << b3%32;
    }
    return is_present;
}



//  Returns 1 if key is in the Bloom filter, 0 otherwise.
int check_bloom(unsigned int key)
{
    char *pkey = (char *)&key;
    unsigned int h = hash(pkey, 4);
    
    char b1, b2, b3;
    key_to_bitpos(h, &b1, &b2, &b3);
    
    int is_present = bloom_filter[b1/32] >> b1%32 && 
                     bloom_filter[b2/32] >> b2%32 &&
                     bloom_filter[b3/32] >> b3%32;
                     
    return is_present;
}


//  Prints the bloom filter
void print_bloom()
{
    int i;
    for(i = 0; i < FILTER_SIZE/32; i++)
    {
        printf("%08x ", bloom_filter[i]);  
    }
    printf("\n");
    
}

// converts last three bytes of 32-bit hash to bit positions    
void key_to_bitpos(unsigned int h, char *b1, char *b2, char *b3)
{
    *b1 =  h        & 0x000000ff;
    *b2 = (h >> 8)  & 0x000000ff;
    *b3 = (h >> 16) & 0x000000ff;
}

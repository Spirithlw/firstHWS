#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

typedef long elem_t;
typedef long long canary_t;

const int DEFAULT_COUNT_OF_ELEMS = 100;
const double DELTA = 0.75;
const elem_t POISON = 3595322;
const int MAX_LENGTH_OF_NAME = 50;

#define for_dump __FILE__,__PRETTY_FUNCTION__,__LINE__
//#define crt_stk(name) stack_t name = {"#name"}

struct stack_t
    {
    const char name[MAX_LENGTH_OF_NAME];
    canary_t* bird1;
    char* buffer_ptr;
    unsigned long max_size;
    elem_t* data;
    unsigned long index_elem;
    long hash;
    canary_t* bird2;
    };

bool stack_message(const char message[], bool boolean)
    {
    printf("%s", message);
    return boolean;
    }

void stack_initial(stack_t* stk)
    {
    assert(stk);

    stk->buffer_ptr = nullptr;
    stk->data = nullptr;
    stk->bird1 = nullptr;
    stk->bird2 = nullptr;
    stk->hash = 0;
    stk->index_elem = 0;
    stk->max_size = 0;

    return ;
    }

long calculate_hash( stack_t* stk )
    {
    assert(stk);

    long h = 0;
    for ( int i = 0; i < 2*sizeof(canary_t) + (stk->max_size)*sizeof(elem_t) ; i++ )
        {
        h+=(i+1)*( *(stk->buffer_ptr+i) );
        }
    return h;
    }

void stack_dump( const stack_t* stk, const char s1[] = "", const char s2[] = "", int line = 0, const char error_description[] = "none")
    {
    assert(stk);

    printf("name of stack: %s\n",stk->name);
    printf("%s | %s | %d\n", s1, s2, line);
    printf("error description: %s\n",error_description);
    printf("buffer ptr [%p]\ndata massive ptr [%p]\n", stk->buffer_ptr, stk->data);
    printf("canary 1 ptr [%p]\ncanary 2 ptr [%p]\n", stk->bird1, stk->bird2);
    printf("canary 1 value %#llx\ncanary 2 value %#llx\n", *(stk->bird1), *(stk->bird2) );
    printf("max size %ld\nindex of element %ld\nhash %ld\nElements\n", stk->max_size, stk->index_elem, stk->hash);
    for (long i = 0; i < stk->max_size; i++)
        {
        if( i <= stk->index_elem-1)
            {
            printf("used ");
            }
        printf("data[%ld] = %ld\n", i, *(stk->data+i));
        }
    }

bool call_dump( stack_t* stk, const char message[], bool call)
    {
    assert(stk);

    if (call)
        {
        stack_dump( stk, "", "", 0,message );
        }

    return 0;
    }

bool stack_verify( stack_t* stk, bool DUMP_PRINT = 0 )
    {
    assert(stk);

    if ( stk->bird1 == nullptr )
        {
        return call_dump ( stk, "First canary ptr = null", DUMP_PRINT );
        }

    if ( stk->bird2 == nullptr )
        {
        return call_dump ( stk, "Second canary ptr = null", DUMP_PRINT );
        }


    if ( stk->data == nullptr )
        {
        return call_dump ( stk, "Data massive ptr = null", DUMP_PRINT );
        }

    if ( stk->buffer_ptr == nullptr )
        {
        return call_dump ( stk, "Buffer ptr = null", DUMP_PRINT );
        }

    if ( stk->bird1 == stk->bird2 )
        {
        return call_dump ( stk, "First canary ptr = Second canary ptr", DUMP_PRINT );
        }

    if ( (char*) stk->data == stk->buffer_ptr )
        {
        return call_dump ( stk, "Data massive ptr = Buffer ptr", DUMP_PRINT );
        }

    if ( stk->index_elem >= stk->max_size )
        {
        return call_dump ( stk, "index of new element bigger than max size of stack", DUMP_PRINT );
        }

    if ( stk->max_size == 0 )
        {
        return call_dump ( stk, "max size of stack = 0", DUMP_PRINT );
        }

    if ( *(stk->bird1) != 0xDEADBEEF )
        {
        return call_dump ( stk, "First canary has changed", DUMP_PRINT  );
        }

    if ( *(stk->bird2) != 0xBEDA )
        {
        return call_dump ( stk, "Second canary has changed", DUMP_PRINT  );
        }

    if ( stk->hash != calculate_hash(stk) )
        {
        return call_dump ( stk, " Unregistrered change of hash & stack", DUMP_PRINT  );
        }

    //stack_dump( stk, "ok");
    return 1;
    }

bool stack_create( stack_t* stk, int count_elem )
    {
    assert( stk );
    assert( !(count_elem == 0) );

    stk->max_size = count_elem;
    stk->index_elem = 0;
    stk->buffer_ptr = (char*) calloc( 2*sizeof(canary_t) + count_elem*sizeof(elem_t), sizeof(char) );
    if ( stk->buffer_ptr == nullptr )
        {
        return stack_message("not enough memory to create stack", 0);
        }
    stk->data = (elem_t*) ( stk->buffer_ptr + sizeof(canary_t) );
    stk->bird1 = (canary_t*) stk->buffer_ptr ;
    stk->bird2 = (canary_t*) (stk->buffer_ptr + sizeof(canary_t) + count_elem*sizeof(elem_t) );
    *(stk->bird1) = 0xDEADBEEF ;
    *(stk->bird2) = 0xBEDA ;
    for (unsigned long i = 0; i < stk->max_size; i++ )
        {
        *(stk->data + i) = POISON;
        }
    stk->hash = calculate_hash(stk);

    assert( stack_verify(stk) );
    return stack_verify(stk);
    }

void stack_memory( stack_t* stk , int type)
    {
    assert(stk);
    if ( type == 1)
        {
        if ( stk->index_elem == (stk->max_size - 1) )
            {
            stk->buffer_ptr = (char *) realloc(stk->buffer_ptr,2 * sizeof(canary_t) + 2 * (stk->max_size) * sizeof(elem_t));
            assert(stk->buffer_ptr);
            stk->max_size *= 2;
            stk->bird1 = (canary_t *) stk->buffer_ptr;
            stk->data = (elem_t *) (stk->buffer_ptr + sizeof(canary_t));
            stk->bird2 = (canary_t *) (stk->buffer_ptr + sizeof(canary_t) + (stk->max_size) * sizeof(elem_t));
            *(stk->bird2) = 0xBEDA;
            stk->hash = calculate_hash(stk);
            for (unsigned long i = stk->index_elem; i < stk->max_size; i++)
                {
                *(stk->data + i) = POISON;
                }
            return;
            }
        }

    if ( type == -1)
        {
        /*if ( (stk->index_elem <  round(0.75*(stk->max_size))) && (stk->index_elem > 100) )
            {
            char* pointer = stk->buffer_ptr;
            stk->buffer_ptr = (char*) realloc( stk->buffer_ptr, 2*sizeof(canary_t) + round(0.75*(stk->max_size))*sizeof(elem_t) );
            stk->max_size = round(0.75*(stk->max_size));
            stk->bird1 = (canary_t*) stk->buffer_ptr;
            *(stk->bird1) = 0xDEADBEEF;
            stk->bird2 = (canary_t*) (stk->buffer_ptr + sizeof(canary_t) + stk->max_size*sizeof(elem_t) );
            *(stk->bird2) = 0xBEDA;
            stk->data = (elem_t*) (stk->buffer_ptr + sizeof(canary_t) );
            elem_t* data_pointer = (elem_t*) (pointer + sizeof(canary_t) );
            for ( unsigned long i = 0; i < stk->index_elem ; i++)
                {
                *(stk->data + i) = *(data_pointer + i);
                }
            for ( unsigned long i = stk->index_elem; i < stk->max_size ; i++)
                {
                *(stk->data + i) = *(data_pointer + i);
                }
            stk->hash = calculate_hash( stk );
            return ;
            }*/
            if ( (stk->index_elem <  round(0.75*(stk->max_size))) && (stk->index_elem > 100) )
            {
            stk->buffer_ptr = (char*) realloc( stk->buffer_ptr, 2*sizeof(canary_t) + round(0.75*(stk->max_size))*sizeof(elem_t) );
            stk->max_size = round(0.75*(stk->max_size));
            stk->bird1 = (canary_t*) stk->buffer_ptr;
            stk->data = (elem_t*) ( stk->buffer_ptr + sizeof(canary_t) );
            stk->bird2 = (canary_t*) (stk->buffer_ptr + sizeof(canary_t) + stk->max_size*sizeof(elem_t) );
            *(stk->bird2) = 0xBEDA;
            stk->hash = calculate_hash( stk );
            return ;
            }
        }

    return ;
    }

bool stack_push( stack_t* stk, elem_t e)
    {
    assert(stk);
    assert( stack_verify(stk) );

    stack_memory( stk , 1);
    *(stk->data+stk->index_elem) = e;
    stk->index_elem++;
    stk->hash = calculate_hash( stk );

    assert( stack_verify(stk) );
    return 1;
    }

bool stack_pop( stack_t* stk, elem_t* e)
    {
    assert(stk);
    assert( stack_verify(stk) );

    stack_memory( stk , -1);
    if ( stk->index_elem == 0 )
        {
        return stack_message( "stack underflow\n",0);
        }
    *e = *(stk->data + stk->index_elem - 1);
    stk->index_elem--;
    *( stk->data + stk->index_elem ) = POISON ;
    stk->hash = calculate_hash( stk );

    assert( stack_verify(stk) );
    return 1;
    }

void stack_delete( stack_t* stk )
    {
    assert(stk);
    assert( stack_verify(stk) );

    free( stk->buffer_ptr );
    stack_initial(stk);
    return ;
    }

int main()
    {
    stack_t stk = {"stk"};
    stack_initial( &stk);
    stack_create( &stk, DEFAULT_COUNT_OF_ELEMS);
    for (int i = 0; i<1000 ; i++)
        {
        stack_push(&stk, 5*i);
        }
    elem_t ch = 0;
    for (int i = 0; i<700 ; i++)
        {
        stack_pop(&stk, &ch);
        }
    stack_pop( &stk, &ch);
    printf("%ld\n",ch);
    stack_dump(&stk,for_dump);
    stack_delete( &stk );
    return 0;
    }
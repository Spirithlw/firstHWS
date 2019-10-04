#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

const int DEFAULT_COUNT_OF_ELEMS = 100;

typedef int elem_t;
typedef int canary_t;

struct stack_t
    {
    canary_t* bird1;
    char* buffer_ptr;
    unsigned long max_size;
    elem_t* data;
    unsigned int index_elem;
    long hash;
    canary_t* bird2;
    };

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

    return;
    }

long calculate_hash( stack_t* stk )
    {

    }

void stack_dump( stack_t* stk, const char error_description[] = "")
    {

    }

bool call_dump( stack_t* stk, const char message[], bool call)
    {
    assert(stk);

    if (call)
        {
        stack_dump( stk, message );
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

        /*if ( *(stk->bird1) != 0xDEADBEAF )
        {
            if ( DUMP_PRINT )
            {
                stack_dump ( stk, "First canary has changed" );
            }
            return 0;
        }

        if ( *(stk->bird2) != 0xBEDA )
        {
            if ( DUMP_PRINT )
            {
                stack_dump ( stk, "First canary ptr = null" );
            }
            return 0;
        }

        if ( stk->hash != calculate_hash(stk) )
        {
            if ( DUMP_PRINT )
            {
                stack_dump ( stk, "Unregistrered change of stack" );
            }
            return 0;
        }*/

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
    stk->data = (elem_t*) ( stk->buffer_ptr + sizeof(canary_t) );
    stk->bird1 = (canary_t*) stk->buffer_ptr ;
    stk->bird2 = (canary_t*) (stk->buffer_ptr + sizeof(canary_t) + count_elem*sizeof(elem_t) );
    *(stk->bird1) = 0xDEADBEAF ;
    *(stk->bird2) = 0xBEDA ;
    //stk->hash = calculate_hash(stk);

    assert( stack_verify(stk) );
    return stack_verify(stk);
    }

void stack_delete( stack_t* stk )
    {
    assert(stk);
    assert( stack_verify(stk) );

    free( stk->buffer_ptr );
    stk->buffer_ptr = nullptr;
    stk->data = nullptr;
    stk->bird1 = nullptr;
    stk->bird2 = nullptr;
    stk->hash = 0;
    stk->index_elem = 0;
    stk->max_size = 0;
    }

int main()
    {
    stack_t stk ;
    stack_create( &stk, DEFAULT_COUNT_OF_ELEMS);
    stack_delete( &stk);
    return 0;
    }
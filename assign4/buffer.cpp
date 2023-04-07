/**
* Assignment 4: Producer Consumer Problem
 * @file buffer.cpp
 * @author Abraham Gomez and Tucker Shaw (TODO: your name)
 * @brief Implementation file for the buffer class
 * @version 0.1
 */

//You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient and clear comments to your code
#include "buffer.h"

Buffer::Buffer(int size)
{
    maxSize = size
    count = 0   
}

/*
Buffer::~Buffer()
{

}
*/

bool Buffer::insert_item(buffer_item item)
{


}

bool Buffer::remove_item(buffer_item *item)
{


}

int Buffer::get_size()
{
    return maxSize;
}

int Buffer::get_count()
{
    return count;
}

bool Buffer::is_empty()
{
    if(count == 0)
        return true;
    else
        return false;
}

bool Buffer::is_full()
{
    if(count == maxSize)
        return true;
    else
        return false;
}

void Buffer::print_buffer()
{


}
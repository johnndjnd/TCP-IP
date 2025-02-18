#include "reassembler.hh"
#include "debug.hh"

#include <cstdlib>
#include <iostream>
using namespace std;

void Reassembler::insert( uint64_t first_index, string data, bool is_last_substring )
{
  Writer& writer = output_.writer();
  if ( first_index + data.size() < first_unassemble_index_ )
    return;
  if ( first_index > first_unassemble_index_ + writer.available_capacity() )
    return;
  string useful_data;
  if ( first_index < first_unassemble_index_ ) {
    useful_data
      = data.substr( first_unassemble_index_ - first_index,
                     data.size() > writer.available_capacity() ? writer.available_capacity() : data.size() );
    first_index = first_unassemble_index_;

  } else {
    useful_data = data.substr( 0,
                               ( first_index + data.size() ) > first_unassemble_index_ + writer.available_capacity()
                                 ? writer.available_capacity() - ( first_index - first_unassemble_index_ )
                                 : data.size() );
    // 若有数据未写入，说明不能关闭
    if ( useful_data.size() < data.size() )
      is_last_substring = false;
  }
  while ( useful_data.size() > 0 ) {
    // 若有重复的数据，直接跳过
    if ( index_set_.find( first_index ) == index_set_.end() ){
    pending_bytes_.push( { first_index, useful_data[0] } );
    index_set_.insert( first_index );
    }
    useful_data = useful_data.substr( 1 );
    first_index++;
  }
  while ( !pending_bytes_.empty() && pending_bytes_.top().first == first_unassemble_index_ ) {
    writer.push( string( 1, pending_bytes_.top().second ) );
    pending_bytes_.pop();
    while ( pending_bytes_.size() > 0 && pending_bytes_.top().first == first_unassemble_index_ )
      pending_bytes_.pop();

    first_unassemble_index_++;
  }
  if ( is_last_substring ) {
    can_closed_ = true;
  }
  if ( can_closed_ && pending_bytes_.empty() ) {
    writer.close();
  }
}

// How many bytes are stored in the Reassembler itself?
// This function is for testing only; don't add extra state to support it.
uint64_t Reassembler::count_bytes_pending() const
{

  return pending_bytes_.size();
}

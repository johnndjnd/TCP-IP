#include "byte_stream.hh"

using namespace std;

ByteStream::ByteStream( uint64_t capacity ) : capacity_( capacity ) {}

void Writer::push( string data )
{
  if ( buffer_.size() + data.size() > capacity_ ) {
    total_buffer_ += data.substr( 0, capacity_ - buffer_.size() ).size();
    buffer_ += data.substr( 0, capacity_ - buffer_.size() );
  } else {
    total_buffer_ += data.size();
    buffer_ += data;
  }
}

void Writer::close()
{
  // Your code here.
  closed_ = true;
}

bool Writer::is_closed() const
{
  return closed_; // Your code here.
}

uint64_t Writer::available_capacity() const
{
  return capacity_ - buffer_.size(); // Your code here.
}

uint64_t Writer::bytes_pushed() const
{
  return total_buffer_; // Your code here.
}

string_view Reader::peek() const
{
  return buffer_; // Your code here.
}

void Reader::pop( uint64_t len )
{
  buffer_ = buffer_.substr( len, buffer_.size() - len ); // Your code here.
  total_pop_ += len;
}

bool Reader::is_finished() const
{
  return closed_ && buffer_.size() == 0; // Your code here.
}

uint64_t Reader::bytes_buffered() const
{
  return buffer_.size(); // Your code here.
}

uint64_t Reader::bytes_popped() const
{
  return total_pop_; // Your code here.
}

#include <iostream>
#include <cstring>
#include <fstream>
#include <stdlib.h>

using namespace std;

/* G
 *  A program that reads and writes itself :)
 *  zM - 11/07/2015
 *    E
 */

int main(int argc, char** argv)
{

  if ( argc > 1 && argv[1][0] == '-') {
    cout << "Usage: " << argv[0] << " [-h|--help|$new_data]\n";
    return 0;
  }

  string filename = argv[0];
  string chmod = "chmod 775 " + filename;

  ifstream ifile ( filename.c_str(), ios::in | ios::binary | ios::ate );
  ifstream::pos_type size, data_size;
  char *memblock, *child, *data;
  int flag[] = { 0x13, 0x37 };
  int flag_size = ( sizeof(flag)/sizeof(*flag) );
  int c = 0;

  // Read myself and save data
  if ( ifile.is_open() ) {

    size = ifile.tellg();
    memblock = new char [size];

    ifile.seekg( 0, ios::beg );
    ifile.read( memblock, size );

    for ( long i = 0; i < size; i++ ) {
      c = ( memblock[i] == flag[c] ? c+1 : 0 );

      if ( c >= flag_size ) {
        data_size = size-i-long(1);
        data = new char[data_size];
        memcpy( data, &memblock[i+1], data_size );
        break;
      }

    }
    cout << data << endl;

  } else {
    cout << "[!] ifile.open()\n";
    return 1;
  }

  ifile.close();

  // Do something with data here, put the next generation in new_data
  string new_data = ( argc > 1 ? argv[1] : "What? Evoli is evolving!\nCongratulations! Your Evoli evolved into Evoli !" );

  // Build a new binary file
  child = new char[size-data_size+new_data.length()];

  memcpy( child, memblock, size-data_size ); // Add program and flag
  memcpy( &child[size-data_size], new_data.c_str(), new_data.length() ); // Add data at the end

  // Remove the original binary
  remove(filename.c_str());

  ofstream ofile;
  ofile.open( filename.c_str(), ios::out | ios::ate );

  // And finally replace it by the next generation
  if ( ofile.is_open() ) {
    ofile.write( child, size-data_size+new_data.length() );
  } else {
    cout << "[!] ofile.open()\n";
    return 1;
  }

  ofile.close();
  system(chmod.c_str());

  delete[] memblock;
  delete[] child;
  delete[] data;

  return 0;
}


#include <iostream>

using namespace std;

int main()
{
  int size;
  cin >> size;
 
  unsigned int* a = new unsigned int[size / 16]; // <- input tab to encrypt
  unsigned int* b = new unsigned int[size / 16]; // <- output tab
 
  for (int i = 0; i < size / 16; i++) {   // Read size / 16 integers to a
    cin >> hex >> a[i];
    // cout << a[i] << "\n\n" ;
    // cout << sizeof( a[i] ) << "\n\n" ;
  }

  int tmp;
  tmp = size >> 5;
  cout << tmp << "\n" ;

  for (int i = 0; i < size / 16; i++) {   // Write size / 16 zeros to b
    b[i] = 0;
  }	
 
  for (int i = 0; i < size; i++)
    for (int j = 0; j < size; j++) {
      // b[(i + j) / 32] ^= ( (a[i / 32] >> (i % 32)) &
	  // (a[j / 32 + size / 32] >> (j % 32)) & 1 ) << ((i + j) % 32);   
      b[(i + j) / 32] ^= ( (a[i >> 5] >> (i % 32)) &
			   (a[(j >> 5) + (size >> 5)] >> (j % 32)) & 1 ) << ((i + j) % 32);
    }
  // Magic centaurian operation

  for (int i = 0; i < size; i++)
    for (int j = 0; j < size; j++)
      cout << ( (i + j) / 32 ) * 32 + ((i + j) % 32) << " % " 
	   << "i: " << i << " j: " << 32 + j << endl;
 
  for(int i = 0; i < size / 16; i++)
    cout << hex << b[i] << " ";       // print result

 /* 
    Good luck humans     
 */
  return 0;
}

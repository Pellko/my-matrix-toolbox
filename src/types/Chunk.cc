#include "Chunk.hh"
#include <vector>

namespace sciscript {

void Chunk::emitByte(uint8_t v) {
  bytecode.push_back(v);
}

void Chunk::emitDynamicBytes(int v) {
  std::vector<uint8_t> bytes = code::generateDynamicBytes(v);
  for(int i=0;i<bytes.size();i++) {
    bytecode.push_back(bytes[i]);
  }
}

namespace code {

std::vector<uint8_t> generateDynamicBytes(int value) {
  int i = value;
  unsigned char bytes = 1;
  unsigned char max = 128;

  while(max <= i) {
    i >>= 8;
    bytes++;
  }
  
  std::vector<uint8_t> result;
  result.push_back(bytes);
  for(int i=0;i<bytes;i++) {
    result.push_back((value >> i * 8) & 0xFF);
  }
  
  return result;
}

}
}
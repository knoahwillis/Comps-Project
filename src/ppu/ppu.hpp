#pragma once

#include <array>
#include <cstdint>
#include <utility>
#include <vector>

enum Mirroring {
  vert,
  horiz,
  fourScreen,
};

class PPU {
  int cycles = 0;
  int scanline = 0;

 public:
  PPU();
  PPU(std::vector<uint8_t> chr, Mirroring m);
  ~PPU();

  bool clock(int cycs);

  std::vector<uint8_t> chrROM;
  std::array<uint8_t, 2048> vram;
  std::array<uint8_t, 32> pallet;

  std::array<uint8_t, 256> oamData;
  uint8_t oamAddr;

  Mirroring mirroring;
  uint8_t dataBuffer;

  void oamDataWrite(uint8_t val);
  void oamAddrWrite(uint8_t val);
  uint8_t oamDataRead();
  void oamDataOverride(std::array<uint8_t, 256> data);

  uint8_t nmiInterrupt = 0x00;

  // Registers

  struct Addr {
    std::pair<uint8_t, uint8_t> val = std::make_pair(0x00, 0x00);
    bool hi = true;

    void increment(uint8_t rhs);
    void set(uint16_t rhs);
    void update(uint8_t rhs);
    void reset();
    uint16_t get();
  };

  Addr addrReg;

  void addrWrite(uint8_t val);
  uint8_t addrRead();
  uint8_t incrementAddr();

  uint8_t controlReg = 0b00000000;
  void controlWrite(uint8_t val);
  uint16_t mirrorAddr(uint16_t addr);

  uint8_t maskReg = 0b00000000;
  void maskWrite(uint8_t val);

  uint8_t statusReg = 0b00000000;
  uint8_t readStatus();

  void vramWrite(uint8_t val);
  uint8_t vramRead();
};

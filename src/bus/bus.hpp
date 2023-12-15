#pragma once

#include <array>
#include <cstdint>
#include <functional>
#include <vector>

#include "../ppu/ppu.hpp"

struct CartRom {
  std::vector<uint8_t> prgROM = {};
  std::vector<uint8_t> chrROM = {};
  uint8_t mapper = 0x00;
  Mirroring mirroring = Mirroring::vert;

  void loadRom(std::vector<uint8_t> data);
};

class Bus {
 public:
  Bus();
  ~Bus();

  void clearRAM();
  std::array<uint8_t, 2048> cpuVRAM;
  std::vector<uint8_t> prgROM = {};

  void connectFunction(std::function<void()> f);
  std::function<void()> fn;

  PPU ppu;
  void connectPPU(CartRom c);

  uint8_t read(uint16_t addr);
  void write(uint16_t addr, uint8_t d);

  uint8_t readPRGROM(uint16_t addr);

  int cycles = 0;
  void clock(int cycs);

  bool getNMIStatus() { return ppu.nmiInterrupt != 0x00; }
};

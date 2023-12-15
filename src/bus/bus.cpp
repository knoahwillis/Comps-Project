#include "bus.hpp"

#include <iostream>

void CartRom::loadRom(std::vector<uint8_t> data) {
  mapper = (data[7] & 0b11110000) | (data[6] >> 4);

  uint8_t iNES = (data[7] >> 2) & 0b11;
  if ((data[6] & 0b1000) != 0) {
    mirroring = fourScreen;
  } else if ((data[6] & 0b1) != 0) {
    mirroring = vert;
  } else {
    mirroring = horiz;
  }

  int prgROMSize = static_cast<int>(data[4]) * 16384;
  int chrROMSize = static_cast<int>(data[5]) * 8192;
  int prgROMStart = 16;
  if ((data[6] & 0b100) != 0) {
    prgROMStart += 512;
  }
  std::vector<uint8_t> prg;
  for (int i = prgROMStart; i < (prgROMStart + prgROMSize); i++) {
    prg.push_back(data[i]);
  }
  std::vector<uint8_t> chr;
  int chrROMStart = prgROMSize + prgROMStart;
  for (int i = chrROMStart; i < (chrROMStart + chrROMSize); i++) {
    chr.push_back(data[i]);
  }
  prgROM = prg;
  chrROM = chr;
}

Bus::Bus() {
  for (int i = 0; i < 2048; i++) {
    cpuVRAM[i] = 0x00;
  }
}

Bus::~Bus() {}

void Bus::clearRAM() {
  for (int i = 0; i < 2048; i++) {
    cpuVRAM[i] = 0x00;
  }
}

void Bus::connectFunction(std::function<void()> f) { fn = f; }

void Bus::connectPPU(CartRom c) {
  ppu = PPU(c.chrROM, c.mirroring);
  prgROM = c.prgROM;
}

uint8_t Bus::read(uint16_t addr) {
  if (addr >= 0x0000 && addr <= 0x1FFF) {
    return cpuVRAM[addr & 0b0000011111111111];
  } else if (addr == 0x2002) {
    return ppu.readStatus();
  } else if (addr == 0x2004) {
    return ppu.oamDataRead();
  } else if (addr == 0x2007) {
    return ppu.vramRead();
  } else if (addr >= 0x2008 && addr <= 0x3FFF) {
    return this->read(addr & 0b0010000000000111);
  } else if (addr >= 0x8000 && addr <= 0xFFFF) {
    return readPRGROM(addr);
  } else {
    return 0x00;
  }
}

void Bus::write(uint16_t addr, uint8_t d) {
  if (addr >= 0x0000 && addr <= 0x1FFF) {
    cpuVRAM[addr & 0b0000011111111111] = d;
  } else if (addr == 0x2000) {
    ppu.controlWrite(d);
  } else if (addr == 0x2001) {
    ppu.maskWrite(d);
  } else if (addr == 0x2003) {
    ppu.oamAddrWrite(d);
  } else if (addr == 0x2004) {
    ppu.oamDataWrite(d);
  } else if (addr == 0x2005) {
  } else if (addr == 0x2006) {
    ppu.addrWrite(d);
  } else if (addr == 0x2007) {
    // std::cout << "ers";
    ppu.vramWrite(d);
  } else if (addr >= 2008 && addr <= 0x3FFF) {
    this->write(addr & 0b0010000000000111, d);
  }
}

uint8_t Bus::readPRGROM(uint16_t addr) {
  addr -= 0x8000;
  if (prgROM.size() == 0x4000 && addr >= 0x4000) {
    addr %= 0x4000;
  }
  return prgROM[addr];
}

void Bus::clock(int cycs) {
  cycles += cycs;

  bool frame = ppu.clock(cycles * 3);

  if (frame) {
    fn();
  }
}

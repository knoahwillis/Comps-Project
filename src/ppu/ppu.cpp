#include "ppu.hpp"

#include <iostream>

PPU::PPU() {}

PPU::PPU(std::vector<uint8_t> chr, Mirroring m) {
  chrROM = chr;
  mirroring = m;
  for (int i = 0; i < 2048; i++) {
    vram[i] = 0;
  }
  for (int i = 0; i < 256; i++) {
    oamData[i] = 0;
  }
}

PPU::~PPU() {}

bool PPU::clock(int cycs) {
  cycles += cycs;
  if (cycles >= 341) {
    cycles -= 341;
    scanline++;

    if (scanline == 241) {
      statusReg |= 0b10000000;
      statusReg &= 0b10111111;
      if ((controlReg & 0b1000000) > 0) {
        nmiInterrupt = 0x01;
      }
    }

    if (scanline >= 262) {
      scanline = 0;
      nmiInterrupt = 0x00;
      statusReg &= 0b10111111;
      statusReg &= 0b01111111;
      return true;
    }
  }
  return false;
}

void PPU::addrWrite(uint8_t val) { addrReg.update(val); }

uint8_t PPU::addrRead() {
  uint16_t addr = addrReg.get();
  incrementAddr();

  if (addr >= 0x0000 && addr <= 0x1FFF) {
    uint8_t temp = dataBuffer;
    dataBuffer = chrROM[addr];
    return temp;
  } else if (addr >= 0x2000 && addr <= 0x2FFF) {
    uint8_t temp = dataBuffer;
    dataBuffer = vram[mirrorAddr(addr)];
    return temp;
  } else if (addr >= 0x3F00 && addr <= 0x3FFF) {
    return pallet[addr - 0x3F00];
  }
}

void PPU::controlWrite(uint8_t val) {
  bool nmiStatus = ((controlReg & 0b10000000) > 0) ? true : false;
  controlReg = val;
  if (!nmiStatus && ((controlReg & 0b10000000) > 0) &&
      (statusReg & 0b10000000) > 0) {
    nmiInterrupt = 0x01;
  }
}

uint8_t PPU::incrementAddr() {
  if ((controlReg << 2) != 0) {
    return 1;
  } else {
    return 32;
  }
}

uint16_t PPU::mirrorAddr(uint16_t addr) {
  uint16_t mirroredAddr = addr & 0b10111111111111;
  uint16_t vramIndex = mirroredAddr - 0x2000;
  uint16_t vramTable = vramIndex / 0x400;

  if (mirroring == Mirroring::vert && (vramTable == 2 || vramTable == 3)) {
    return vramIndex - 0x800;
  } else if (mirroring == horiz && (vramTable == 2 || vramTable == 1)) {
    return vramIndex - 0x400;
  } else if (mirroring == horiz && vramTable == 3) {
    return vramIndex - 0x800;
  } else {
    return vramIndex;
  }
}

void PPU::Addr::set(uint16_t rhs) {
  val.first = static_cast<uint8_t>(rhs >> 8);
  val.second = static_cast<uint8_t>(rhs & 0xFF);
}

void PPU::Addr::increment(uint8_t rhs) {
  uint8_t lo = val.second;
  val.second = lo + rhs;
  if (lo > val.second) {
    val.first++;
  }
  if (this->get() > 0x3FFF) {
    this->set(this->get() & 0b11111111111111);
  }
}

void PPU::Addr::update(uint8_t rhs) {
  if (hi) {
    val.first = rhs;
  } else {
    val.second = rhs;
  }

  if (this->get() > 0x3FFF) {
    this->set(this->get() & 0b11111111111111);
  }

  hi = !hi;
}

void PPU::Addr::reset() { hi = true; }

uint16_t PPU::Addr::get() {
  return (static_cast<uint16_t>(val.first) << 8) |
         static_cast<uint16_t>(val.second);
}

void PPU::maskWrite(uint8_t val) { maskReg = val; }

uint8_t PPU::readStatus() {
  uint8_t val = statusReg;
  statusReg &= 0b01111111;
  addrReg.reset();
  return val;
}

void PPU::oamDataWrite(uint8_t val) {
  oamData[oamAddr] = val;
  oamAddr++;
}

void PPU::oamAddrWrite(uint8_t val) { oamAddr = val; }

uint8_t PPU::oamDataRead() { return oamData[oamAddr]; }

void PPU::oamDataOverride(std::array<uint8_t, 256> data) {
  for (const auto& d : data) {
    oamData[oamAddr] = d;
    oamAddr++;
  }
}

void PPU::vramWrite(uint8_t val) {
  uint16_t addr = addrReg.get();
  if (addr >= 0x2000 && addr <= 0x2FFF) {
    // std::cout << mirrorAddr(addr) << " " << val << "|";
    vram[mirrorAddr(addr)] = val;
  } else if (addr == 0x3F10 || addr == 0x3F14 || addr == 0x3F18 ||
             addr == 0x3F1C) {
    uint16_t temp = addr - 0x10;
    pallet[(temp - 0x3F00)] = val;
  } else if (addr >= 0x3F00 && addr <= 0x3FFF) {
    pallet[addr - 0x3F00] = val;
  }
  incrementAddr();
  std::cout << addr << " ";
}

uint8_t PPU::vramRead() {
  uint16_t addr = addrReg.get();
  incrementAddr();

  if (addr >= 0x0000 && addr <= 0x1FFF) {
    uint8_t temp = dataBuffer;
    dataBuffer = chrROM[addr];
    return temp;
  } else if (addr >= 0x2000 && addr <= 0x2FFF) {
    uint8_t temp = dataBuffer;
    dataBuffer = chrROM[mirrorAddr(addr)];
    return temp;
  } else if (addr == 0x3F10 || addr == 0x3F14 || addr == 0x3F18 ||
             addr == 0x3F1C) {
    uint16_t temp = addr - 0x10;
    return pallet[temp - 0x3F00];
  } else if (addr >= 0x3F00 && addr <= 0x3FFF) {
    return pallet[addr - 0x3F00];
  }
}

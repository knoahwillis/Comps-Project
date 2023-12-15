#include "cpu.hpp"

#include <iostream>
#include <vector>

CPU::CPU() {
  opcodes = {
      {0x00, Ins{0x00, "BRK", 1, 7, AddressingMode::xxx}},
      {0xEA, Ins{0xEA, "NOP", 1, 2, AddressingMode::xxx}},

      {0x69, Ins{0x69, "ADC", 2, 2, AddressingMode::imm}},
      {0x65, Ins{0x65, "ADC", 2, 3, AddressingMode::zp0}},
      {0x75, Ins{0x75, "ADC", 2, 4, AddressingMode::zpx}},
      {0x6D, Ins{0x6D, "ADC", 3, 4, AddressingMode::abs}},
      {0x7D, Ins{0x7D, "ADC", 3, 4, AddressingMode::abx}},
      {0x79, Ins{0x79, "ADC", 3, 4, AddressingMode::aby}},
      {0x61, Ins{0x61, "ADC", 2, 6, AddressingMode::idx}},
      {0x71, Ins{0x71, "ADC", 2, 5, AddressingMode::idy}},

      {0xE9, Ins{0xE9, "SBC", 2, 2, AddressingMode::imm}},
      {0xE5, Ins{0xE5, "SBC", 2, 3, AddressingMode::zp0}},
      {0xF5, Ins{0xF5, "SBC", 2, 4, AddressingMode::zpx}},
      {0xED, Ins{0xED, "SBC", 3, 4, AddressingMode::abs}},
      {0xFD, Ins{0xFD, "SBC", 3, 4, AddressingMode::abx}},
      {0xF9, Ins{0xF9, "SBC", 3, 4, AddressingMode::aby}},
      {0xE1, Ins{0xE1, "SBC", 2, 6, AddressingMode::idx}},
      {0xF1, Ins{0xF1, "SBC", 2, 5, AddressingMode::idy}},

      {0x29, Ins{0x29, "AND", 2, 2, AddressingMode::imm}},
      {0x25, Ins{0x25, "AND", 2, 3, AddressingMode::zp0}},
      {0x35, Ins{0x35, "AND", 2, 4, AddressingMode::zpx}},
      {0x2D, Ins{0x2D, "AND", 3, 4, AddressingMode::abs}},
      {0x3D, Ins{0x3D, "AND", 3, 4, AddressingMode::abx}},
      {0x39, Ins{0x39, "AND", 3, 4, AddressingMode::aby}},
      {0x21, Ins{0x21, "AND", 2, 6, AddressingMode::idx}},
      {0x31, Ins{0x31, "AND", 2, 5, AddressingMode::idy}},

      {0x49, Ins{0x49, "EOR", 2, 2, AddressingMode::imm}},
      {0x45, Ins{0x45, "EOR", 2, 3, AddressingMode::zp0}},
      {0x55, Ins{0x55, "EOR", 2, 4, AddressingMode::zpx}},
      {0x4D, Ins{0x4D, "EOR", 3, 4, AddressingMode::abs}},
      {0x5D, Ins{0x5D, "EOR", 3, 4, AddressingMode::abx}},
      {0x59, Ins{0x59, "EOR", 3, 4, AddressingMode::aby}},
      {0x41, Ins{0x41, "EOR", 2, 6, AddressingMode::idx}},
      {0x51, Ins{0x51, "EOR", 2, 5, AddressingMode::idy}},

      {0x09, Ins{0x09, "ORA", 2, 2, AddressingMode::imm}},
      {0x05, Ins{0x05, "ORA", 2, 3, AddressingMode::zp0}},
      {0x15, Ins{0x15, "ORA", 2, 4, AddressingMode::zpx}},
      {0x0D, Ins{0x0D, "ORA", 3, 4, AddressingMode::abs}},
      {0x1D, Ins{0x1D, "ORA", 3, 4, AddressingMode::abx}},
      {0x19, Ins{0x19, "ORA", 3, 4, AddressingMode::aby}},
      {0x01, Ins{0x01, "ORA", 2, 6, AddressingMode::idx}},
      {0x11, Ins{0x11, "ORA", 2, 5, AddressingMode::idy}},

      {0x0A, Ins{0x0A, "ASL", 1, 2, AddressingMode::xxx}},
      {0x06, Ins{0x06, "ASL", 2, 5, AddressingMode::zp0}},
      {0x16, Ins{0x16, "ASL", 2, 6, AddressingMode::zpx}},
      {0x0E, Ins{0x0E, "ASL", 3, 6, AddressingMode::abs}},
      {0x1E, Ins{0x1E, "ASL", 3, 7, AddressingMode::abx}},

      {0x4A, Ins{0x4A, "LSR", 1, 2, AddressingMode::xxx}},
      {0x46, Ins{0x46, "LSR", 2, 5, AddressingMode::zp0}},
      {0x56, Ins{0x56, "LSR", 2, 6, AddressingMode::zpx}},
      {0x4E, Ins{0x4E, "LSR", 3, 6, AddressingMode::abs}},
      {0x5E, Ins{0x5E, "LSR", 3, 7, AddressingMode::abx}},

      {0x2A, Ins{0x2A, "ROL", 1, 2, AddressingMode::xxx}},
      {0x26, Ins{0x26, "ROL", 2, 5, AddressingMode::zp0}},
      {0x36, Ins{0x36, "ROL", 2, 6, AddressingMode::zpx}},
      {0x2E, Ins{0x2E, "ROL", 3, 6, AddressingMode::abs}},
      {0x3E, Ins{0x3E, "ROL", 3, 7, AddressingMode::abx}},

      {0x6A, Ins{0x6A, "ROR", 1, 2, AddressingMode::xxx}},
      {0x66, Ins{0x66, "ROR", 2, 5, AddressingMode::zp0}},
      {0x76, Ins{0x76, "ROR", 2, 6, AddressingMode::zpx}},
      {0x6E, Ins{0x6E, "ROR", 3, 6, AddressingMode::abs}},
      {0x7E, Ins{0x7E, "ROR", 3, 7, AddressingMode::abx}},

      {0xE6, Ins{0xE6, "INC", 2, 5, AddressingMode::zp0}},
      {0xF6, Ins{0xF6, "INC", 2, 6, AddressingMode::zpx}},
      {0xEE, Ins{0xEE, "INC", 3, 6, AddressingMode::abs}},
      {0xFE, Ins{0xFE, "INC", 3, 7, AddressingMode::abx}},

      {0xE8, Ins{0xE8, "INX", 1, 2, AddressingMode::xxx}},
      {0xC8, Ins{0xC8, "INY", 1, 2, AddressingMode::xxx}},

      {0xC6, Ins{0xC6, "DEC", 2, 5, AddressingMode::zp0}},
      {0xD6, Ins{0xD6, "DEC", 2, 6, AddressingMode::zpx}},
      {0xCE, Ins{0xCE, "DEC", 3, 6, AddressingMode::abs}},
      {0xDE, Ins{0xDe, "DEC", 3, 7, AddressingMode::abx}},

      {0xCA, Ins{0xCA, "DEX", 1, 2, AddressingMode::xxx}},
      {0x88, Ins{0x88, "DEY", 1, 2, AddressingMode::xxx}},

      {0xC9, Ins{0xC9, "CMP", 2, 2, AddressingMode::imm}},
      {0xC5, Ins{0xC5, "CMP", 2, 3, AddressingMode::zp0}},
      {0xD5, Ins{0xD5, "CMP", 2, 4, AddressingMode::zpx}},
      {0xCD, Ins{0xCD, "CMP", 3, 4, AddressingMode::abs}},
      {0xDD, Ins{0xDD, "CMP", 3, 4, AddressingMode::abx}},
      {0xD9, Ins{0xD9, "CMP", 3, 4, AddressingMode::aby}},
      {0xC1, Ins{0xC1, "CMP", 2, 6, AddressingMode::idx}},
      {0xD1, Ins{0xD1, "CMP", 2, 5, AddressingMode::idy}},

      {0xC0, Ins{0xC0, "CPY", 2, 2, AddressingMode::imm}},
      {0xC4, Ins{0xC4, "CPY", 2, 3, AddressingMode::zp0}},
      {0xCC, Ins{0xCC, "CPY", 3, 4, AddressingMode::abs}},

      {0xE0, Ins{0xE0, "CPX", 2, 2, AddressingMode::imm}},
      {0xE4, Ins{0xE4, "CPX", 2, 3, AddressingMode::zp0}},
      {0xEC, Ins{0xEC, "CPX", 3, 4, AddressingMode::abs}},

      {0x4C, Ins{0x4C, "JMP", 3, 3, AddressingMode::xxx}},
      {0x6C, Ins{0x6C, "JMP", 3, 5, AddressingMode::xxx}},

      {0x20, Ins{0x20, "JSR", 3, 6, AddressingMode::xxx}},
      {0x60, Ins{0x60, "RTS", 1, 6, AddressingMode::xxx}},

      {0x40, Ins{0x40, "RTI", 1, 6, AddressingMode::xxx}},

      {0xD0, Ins{0xD0, "BNE", 2, 2, AddressingMode::xxx}},
      {0x70, Ins{0x70, "BVS", 2, 2, AddressingMode::xxx}},
      {0x50, Ins{0x50, "BVC", 2, 2, AddressingMode::xxx}},
      {0x30, Ins{0x30, "BMI", 2, 2, AddressingMode::xxx}},
      {0xF0, Ins{0xF0, "BEQ", 2, 2, AddressingMode::xxx}},
      {0xB0, Ins{0xB0, "BCS", 2, 2, AddressingMode::xxx}},
      {0x90, Ins{0x90, "BCC", 2, 2, AddressingMode::xxx}},
      {0x10, Ins{0x10, "BPL", 2, 2, AddressingMode::xxx}},

      {0x24, Ins{0x24, "BIT", 2, 3, AddressingMode::zp0}},
      {0x2C, Ins{0x2C, "BIT", 3, 4, AddressingMode::abs}},

      {0xA9, Ins{0xA9, "LDA", 2, 2, AddressingMode::imm}},
      {0xA5, Ins{0xA5, "LDA", 2, 3, AddressingMode::zp0}},
      {0xB5, Ins{0xB5, "LDA", 2, 4, AddressingMode::zpx}},
      {0xAD, Ins{0xAD, "LDA", 3, 4, AddressingMode::abs}},
      {0xBD, Ins{0xBD, "LDA", 3, 4, AddressingMode::abx}},
      {0xB9, Ins{0xB9, "LDA", 3, 4, AddressingMode::aby}},
      {0xA1, Ins{0xA1, "LDA", 2, 6, AddressingMode::idx}},
      {0xB1, Ins{0xB1, "LDA", 2, 5, AddressingMode::idy}},

      {0xA2, Ins{0xA2, "LDX", 2, 2, AddressingMode::imm}},
      {0xA6, Ins{0xA6, "LDX", 2, 3, AddressingMode::zp0}},
      {0xB6, Ins{0xB6, "LDX", 2, 4, AddressingMode::zpy}},
      {0xAE, Ins{0xAE, "LDX", 3, 4, AddressingMode::abs}},
      {0xBE, Ins{0xBE, "LDX", 3, 4, AddressingMode::aby}},

      {0xA0, Ins{0xA0, "LDY", 2, 2, AddressingMode::imm}},
      {0xA4, Ins{0xA4, "LDY", 2, 3, AddressingMode::zp0}},
      {0xB4, Ins{0xB4, "LDY", 2, 4, AddressingMode::zpx}},
      {0xAC, Ins{0xAC, "LDY", 3, 4, AddressingMode::abs}},
      {0xBC, Ins{0xBC, "LDY", 3, 4, AddressingMode::abx}},

      {0x85, Ins{0x85, "STA", 2, 3, AddressingMode::zp0}},
      {0x95, Ins{0x95, "STA", 2, 4, AddressingMode::zpx}},
      {0x8D, Ins{0x8D, "STA", 3, 4, AddressingMode::abs}},
      {0x9D, Ins{0x9D, "STA", 3, 5, AddressingMode::abx}},
      {0x99, Ins{0x99, "STA", 3, 5, AddressingMode::aby}},
      {0x81, Ins{0x81, "STA", 2, 6, AddressingMode::idx}},
      {0x91, Ins{0x91, "STA", 2, 6, AddressingMode::idy}},

      {0x86, Ins{0x86, "STX", 2, 3, AddressingMode::zp0}},
      {0x96, Ins{0x96, "STX", 2, 4, AddressingMode::zpy}},
      {0x8E, Ins{0x8E, "STX", 3, 4, AddressingMode::abs}},

      {0x84, Ins{0x84, "STY", 2, 3, AddressingMode::zp0}},
      {0x94, Ins{0x94, "STY", 2, 4, AddressingMode::zpx}},
      {0x8C, Ins{0x8C, "STY", 3, 4, AddressingMode::abs}},

      {0xD8, Ins{0xD8, "CLD", 1, 2, AddressingMode::xxx}},
      {0x58, Ins{0x58, "CLI", 1, 2, AddressingMode::xxx}},
      {0xB8, Ins{0xB8, "CLV", 1, 2, AddressingMode::xxx}},
      {0x18, Ins{0x18, "CLC", 1, 2, AddressingMode::xxx}},
      {0x38, Ins{0x38, "SEC", 1, 2, AddressingMode::xxx}},
      {0x78, Ins{0x78, "SEI", 1, 2, AddressingMode::xxx}},
      {0xF8, Ins{0xF8, "SED", 1, 2, AddressingMode::xxx}},

      {0xAA, Ins{0xAA, "TAX", 1, 2, AddressingMode::xxx}},
      {0xA8, Ins{0xA8, "TAY", 1, 2, AddressingMode::xxx}},
      {0xBA, Ins{0xBA, "TSX", 1, 2, AddressingMode::xxx}},
      {0x8A, Ins{0x8A, "TXA", 1, 2, AddressingMode::xxx}},
      {0x9A, Ins{0x9A, "TXS", 1, 2, AddressingMode::xxx}},
      {0x98, Ins{0x98, "TYA", 1, 2, AddressingMode::xxx}},

      {0x48, Ins{0x48, "PHA", 1, 3, AddressingMode::xxx}},
      {0x68, Ins{0x68, "PLA", 1, 4, AddressingMode::xxx}},
      {0x08, Ins{0x08, "PHP", 1, 3, AddressingMode::xxx}},
      {0x28, Ins{0x28, "PLP", 1, 4, AddressingMode::xxx}},
  };
}

CPU::~CPU() {}

void CPU::connectBus(Bus b) { bus = b; }

void CPU::reset() {
  a = 0x00;
  x = 0x00;
  p = 0b100100;
  sp = 0xFD;
  pc = 0x0600;
  closed = false;
}

uint8_t CPU::read(uint16_t addr) { return bus.read(addr); }

uint16_t CPU::readInt(uint16_t addr) {
  uint8_t lo = read(addr);
  uint8_t hi = read(addr + 1);
  return static_cast<uint16_t>((hi << 8) | lo);
}

void CPU::write(uint16_t addr, uint8_t d) { bus.write(addr, d); }

void CPU::writeInt(uint16_t addr, uint16_t d) {
  uint8_t hi = static_cast<uint8_t>((d >> 8));
  uint8_t lo = static_cast<uint8_t>(d & 0xff);
  write(addr, lo);
  write(addr + 1, hi);
}

void CPU::load() {
  bus.clearRAM();
  for (int i = 0; i < snakeCode.size(); i++) {
    write(0x0600 + i, snakeCode[i]);
  }
  writeInt(0xFFFC, 0x0600);
}

// helps create the log file when running nestest

void CPU::runWithFunction(std::function<void()> fn) {
  while (!closed) {
    fn();
    uint8_t opcode = read(pc);
    pc += 1;
    uint16_t pcState = pc;
    if (opcodes.find(opcode) == opcodes.end()) {
      std::cout << "Illegal opcode!";
      return;
    }
    Ins curr = opcodes[opcode];
    if (opcode == 0x04) {
      break;
    } else if (opcode == 0xA5 || opcode == 0xA9 || opcode == 0xB5 ||
               opcode == 0xAD || opcode == 0xBD || opcode == 0xB9 ||
               opcode == 0xA1 || opcode == 0xB1) {
      lda(curr.mode);
    } else if (opcode == 0x3C) {
    } else if (opcode == 0xAA) {
      tax();
    } else if (opcode == 0xE8) {
      inx();
    } else if (opcode == 0xD8 || opcode == 0xF8) {
      p &= 0b11110111;
    } else if (opcode == 0x58 || opcode == 0x78) {
      p &= 0b11111011;
    } else if (opcode == 0xB8) {
      p &= 0b10111111;
    } else if (opcode == 0x18) {
      clearCarryFlag();
    } else if (opcode == 0x38) {
      setCarryFlag();
    } else if (opcode == 0x48) {
      push(a);
    } else if (opcode == 0x68) {
      pla();
    } else if (opcode == 0x08) {
      php();
    } else if (opcode == 0x28) {
      plp();
    } else if (opcode == 0x69 || opcode == 0x65 || opcode == 0x75 ||
               opcode == 0x6D || opcode == 0x7D || opcode == 0x79 ||
               opcode == 0x61 || opcode == 0x71) {
      adc(curr.mode);
    } else if (opcode == 0xE9 || opcode == 0xE5 || opcode == 0xF5 ||
               opcode == 0xED || opcode == 0xFD || opcode == 0xF9 ||
               opcode == 0xE1 || opcode == 0xF1) {
      sbc(curr.mode);
    } else if (opcode == 0x29 || opcode == 0x25 || opcode == 0x35 ||
               opcode == 0x2D || opcode == 0x3D || opcode == 0x39 ||
               opcode == 0x21 || opcode == 0x31) {
      adn(curr.mode);
    } else if (opcode == 0x49 || opcode == 0x45 || opcode == 0x55 ||
               opcode == 0x4D || opcode == 0x5D || opcode == 0x59 ||
               opcode == 0x41 || opcode == 0x51) {
      eor(curr.mode);
    } else if (opcode == 0x09 || opcode == 0x05 || opcode == 0x15 ||
               opcode == 0x0D || opcode == 0x1D || opcode == 0x19 ||
               opcode == 0x01 || opcode == 0x11) {
      ora(curr.mode);
    } else if (opcode == 0x4A) {
      lsrAccum();
    } else if (opcode == 0x46 || opcode == 0x56 || opcode == 0x4E ||
               opcode == 0x5E) {
      lsr(curr.mode);
    } else if (opcode == 0x0A) {
      aslAccum();
    } else if (opcode == 0x06 || opcode == 0x16 || opcode == 0x0E ||
               opcode == 0x1E) {
      asl(curr.mode);
    } else if (opcode == 0x2A) {
      rolAccum();
    } else if (opcode == 0x26 || opcode == 0x36 || opcode == 0x2E ||
               opcode == 0x3E) {
      rol(curr.mode);
    } else if (opcode == 0x6A) {
      rorAccum();
    } else if (opcode == 0x66 || opcode == 0x76 || opcode == 0x6E ||
               opcode == 0x7E) {
      ror(curr.mode);
    } else if (opcode == 0xE6 || opcode == 0xF6 || opcode == 0xEE ||
               opcode == 0xFE) {
      inc(curr.mode);
    } else if (opcode == 0xC8) {
      iny();
    } else if (opcode == 0xC6 || opcode == 0xD6 || opcode == 0xCE ||
               opcode == 0xDE) {
      dec(curr.mode);
    } else if (opcode == 0xCA) {
      dex();
    } else if (opcode == 0x88) {
      dey();
    } else if (opcode == 0xC9 || opcode == 0xC5 || opcode == 0xD5 ||
               opcode == 0xCD || opcode == 0xDD || opcode == 0xD9 ||
               opcode == 0xC1 || opcode == 0xD1) {
      compare(curr.mode, a);
    } else if (opcode == 0xC0 || opcode == 0xC4 || opcode == 0xCC) {
      compare(curr.mode, y);
    } else if (opcode == 0xE0 || opcode == 0xE4 || opcode == 0xEC) {
      compare(curr.mode, x);
    } else if (opcode == 0x4C) {
      uint16_t addr = readInt(pc);
      pc = addr;
    } else if (opcode == 0x6C) {
      uint16_t addr = readInt(pc);
      uint16_t indirectRef;
      if ((addr & 0x00FF) == 0x00FF) {
        uint8_t lo = read(addr);
        uint8_t hi = read(addr & 0xFF00);
        indirectRef =
            (static_cast<uint16_t>(hi) << 8) | static_cast<uint16_t>(lo);
      } else {
        indirectRef = readInt(addr);
      }
      pc = indirectRef;
    } else if (opcode == 0x20) {
      pushInt(pc + 2 - 1);
      uint16_t addr = readInt(pc);
      pc = addr;
    } else if (opcode == 0x60) {
      pc = popInt() + 1;
    } else if (opcode == 0x40) {
      p = pop();
      p &= 0b11101111;
      p |= 0b00100000;
      pc = popInt();
    } else if (opcode == 0xD0) {
      branch(!(p & (1 << 1)));
    } else if (opcode == 0x70) {
      branch(p & (1 << 6));
    } else if (opcode == 0x50) {
      branch(!(p & (1 << 6)));
    } else if (opcode == 0x10) {
      branch((p & 0b10000000) == 0);
    } else if (opcode == 0x30) {
      branch(p & (1 << 7));
    } else if (opcode == 0xF0) {
      branch(p & (1 << 1));
    } else if (opcode == 0xB0) {
      branch(p & 1);
    } else if (opcode == 0x90) {
      branch((p & 0b00000001) <= 0);
    } else if (opcode == 0x24 || opcode == 0x2C) {
      bit(curr.mode);
    } else if (opcode == 0x85 || opcode == 0x95 || opcode == 0x8D ||
               opcode == 0x9D || opcode == 0x99 || opcode == 0x81 ||
               opcode == 0x91) {
      sta(curr.mode);
    } else if (opcode == 0x86 || opcode == 0x96 || opcode == 0x8E) {
      write(getAddressingMode(curr.mode), x);
    } else if (opcode == 0x84 || opcode == 0x94 || opcode == 0x8C) {
      write(getAddressingMode(curr.mode), y);
    } else if (opcode == 0xA2 || opcode == 0xA6 || opcode == 0xB6 ||
               opcode == 0xAE || opcode == 0xBE) {
      ldx(curr.mode);
    } else if (opcode == 0xA0 || opcode == 0xA4 || opcode == 0xB4 ||
               opcode == 0xAC || opcode == 0xBC) {
      ldy(curr.mode);
    } else if (opcode == 0xA8) {
      y = a;
      setZeroFlag(y);
      setNegFlag(y);
    } else if (opcode == 0xBA) {
      x = sp;
      setZeroFlag(x);
      setNegFlag(x);
    } else if (opcode == 0x8A) {
      a = x;
      setZeroFlag(a);
      setNegFlag(a);
    } else if (opcode == 0x9A) {
      sp = x;
    } else if (opcode == 0x98) {
      a = y;
      setZeroFlag(a);
      setNegFlag(a);
    } else if (opcode == 0x00) {
      break;
    } else if (opcode == 0x9B) {
      uint8_t data = a & x;
      sp = data;
      uint16_t addr = readInt(pc) + static_cast<uint16_t>(y);
      data = (static_cast<uint8_t>(addr >> 8) + 1) & sp;
      write(addr, data);
    } else if (opcode == 0xC7 || opcode == 0xD7 || opcode == 0xCF ||
               opcode == 0xDF || opcode == 0xDB || opcode == 0xD3 ||
               opcode == 0xC3) {
      uint8_t data = read(getAddressingMode(curr.mode));
      data--;
      write(getAddressingMode(curr.mode), data);
      if (data <= a) {
        p |= 0b00000001;
      }
      setZeroFlag(a - data);
      setNegFlag(a - data);
    } else if (opcode == 0x27 || opcode == 0x37 || opcode == 0x2F ||
               opcode == 0x3F || opcode == 0x3B || opcode == 0x33 ||
               opcode == 0x23) {
      uint8_t data = rol(curr.mode);
      a = data & a;
    } else if (opcode == 0x07 || opcode == 0x17 || opcode == 0x0F ||
               opcode == 0x1F || opcode == 0x1B || opcode == 0x03 ||
               opcode == 0x13) {
      uint8_t data = asl(curr.mode);
      a = data | a;
    } else if (opcode == 0x47 || opcode == 0x57 || opcode == 0x4F ||
               opcode == 0x5F || opcode == 0x5B || opcode == 0x43 ||
               opcode == 0x53) {
      uint8_t data = lsr(curr.mode);
      a = data ^ a;
    } else if (opcode == 0x6B) {
      uint8_t data = read(getAddressingMode(curr.mode));
      a = data & a;
      rorAccum();
      uint8_t result = a;
      uint16_t bit5 = (result >> 5) & 1;
      uint16_t bit6 = (result >> 6) & 1;

      if (bit6 == 1) {
        p |= 0b00000001;
      } else {
        p &= 0b11111110;
      }

      if ((bit5 ^ bit6) == 1) {
        p |= 0b01000000;
      } else {
        p &= 0b10111111;
      }
    }

    if (pcState == pc) {
      pc += static_cast<uint16_t>(curr.length - 1);
    }
  }
}

// runs the 6502 normally

void CPU::run() {
  if (bus.getNMIStatus()) {
    nmiInterrupt();
  }

  uint8_t opcode = read(pc);
  pc += 1;
  uint16_t pcState = pc;
  if (opcodes.find(opcode) == opcodes.end()) {
    std::cout << "Illegal opcode!";
    return;
  }
  Ins curr = opcodes[opcode];

  if (opcode == 0xA5 || opcode == 0xA9 || opcode == 0xB5 || opcode == 0xAD ||
      opcode == 0xBD || opcode == 0xB9 || opcode == 0xA1 || opcode == 0xB1) {
    lda(curr.mode);
  } else if (opcode == 0x3C) {
  } else if (opcode == 0xAA) {
    tax();
  } else if (opcode == 0xE8) {
    inx();
  } else if (opcode == 0xD8 || opcode == 0xF8) {
    p &= 0b11110111;
  } else if (opcode == 0x58 || opcode == 0x78) {
    p &= 0b11111011;
  } else if (opcode == 0xB8) {
    p &= 0b10111111;
  } else if (opcode == 0x18) {
    clearCarryFlag();
  } else if (opcode == 0x38) {
    setCarryFlag();
  } else if (opcode == 0x48) {
    push(a);
  } else if (opcode == 0x68) {
    pla();
  } else if (opcode == 0x08) {
    php();
  } else if (opcode == 0x28) {
    plp();
  } else if (opcode == 0x69 || opcode == 0x65 || opcode == 0x75 ||
             opcode == 0x6D || opcode == 0x7D || opcode == 0x79 ||
             opcode == 0x61 || opcode == 0x71) {
    adc(curr.mode);
  } else if (opcode == 0xE9 || opcode == 0xE5 || opcode == 0xF5 ||
             opcode == 0xED || opcode == 0xFD || opcode == 0xF9 ||
             opcode == 0xE1 || opcode == 0xF1) {
    sbc(curr.mode);
  } else if (opcode == 0x29 || opcode == 0x25 || opcode == 0x35 ||
             opcode == 0x2D || opcode == 0x3D || opcode == 0x39 ||
             opcode == 0x21 || opcode == 0x31) {
    adn(curr.mode);
  } else if (opcode == 0x49 || opcode == 0x45 || opcode == 0x55 ||
             opcode == 0x4D || opcode == 0x5D || opcode == 0x59 ||
             opcode == 0x41 || opcode == 0x51) {
    eor(curr.mode);
  } else if (opcode == 0x09 || opcode == 0x05 || opcode == 0x15 ||
             opcode == 0x0D || opcode == 0x1D || opcode == 0x19 ||
             opcode == 0x01 || opcode == 0x11) {
    ora(curr.mode);
  } else if (opcode == 0x4A) {
    lsrAccum();
  } else if (opcode == 0x46 || opcode == 0x56 || opcode == 0x4E ||
             opcode == 0x5E) {
    lsr(curr.mode);
  } else if (opcode == 0x0A) {
    aslAccum();
  } else if (opcode == 0x06 || opcode == 0x16 || opcode == 0x0E ||
             opcode == 0x1E) {
    asl(curr.mode);
  } else if (opcode == 0x2A) {
    rolAccum();
  } else if (opcode == 0x26 || opcode == 0x36 || opcode == 0x2E ||
             opcode == 0x3E) {
    rol(curr.mode);
  } else if (opcode == 0x6A) {
    rorAccum();
  } else if (opcode == 0x66 || opcode == 0x76 || opcode == 0x6E ||
             opcode == 0x7E) {
    ror(curr.mode);
  } else if (opcode == 0xE6 || opcode == 0xF6 || opcode == 0xEE ||
             opcode == 0xFE) {
    inc(curr.mode);
  } else if (opcode == 0xC8) {
    iny();
  } else if (opcode == 0xC6 || opcode == 0xD6 || opcode == 0xCE ||
             opcode == 0xDE) {
    dec(curr.mode);
  } else if (opcode == 0xCA) {
    dex();
  } else if (opcode == 0x88) {
    dey();
  } else if (opcode == 0xC9 || opcode == 0xC5 || opcode == 0xD5 ||
             opcode == 0xCD || opcode == 0xDD || opcode == 0xD9 ||
             opcode == 0xC1 || opcode == 0xD1) {
    compare(curr.mode, a);
  } else if (opcode == 0xC0 || opcode == 0xC4 || opcode == 0xCC) {
    compare(curr.mode, y);
  } else if (opcode == 0xE0 || opcode == 0xE4 || opcode == 0xEC) {
    compare(curr.mode, x);
  } else if (opcode == 0x4C) {
    uint16_t addr = readInt(pc);
    pc = addr;
  } else if (opcode == 0x6C) {
    uint16_t addr = readInt(pc);
    uint16_t indirectRef;
    if ((addr & 0x00FF) == 0x00FF) {
      uint8_t lo = read(addr);
      uint8_t hi = read(addr & 0xFF00);
      indirectRef =
          (static_cast<uint16_t>(hi) << 8) | static_cast<uint16_t>(lo);
    } else {
      indirectRef = readInt(addr);
    }
    pc = indirectRef;
  } else if (opcode == 0x20) {
    pushInt(pc + 2 - 1);
    uint16_t addr = readInt(pc);
    pc = addr;
  } else if (opcode == 0x60) {
    pc = popInt() + 1;
  } else if (opcode == 0x40) {
    p = pop();
    p &= 0b11101111;
    p |= 0b00100000;
    pc = popInt();
  } else if (opcode == 0xD0) {
    branch(!(p & (1 << 1)));
  } else if (opcode == 0x70) {
    branch(p & (1 << 6));
  } else if (opcode == 0x50) {
    branch(!(p & (1 << 6)));
  } else if (opcode == 0x10) {
    branch((p & 0b10000000) == 0);
  } else if (opcode == 0x30) {
    branch(p & (1 << 7));
  } else if (opcode == 0xF0) {
    branch(p & (1 << 1));
  } else if (opcode == 0xB0) {
    branch(p & 1);
  } else if (opcode == 0x90) {
    branch((p & 0b00000001) != 0x0001);
  } else if (opcode == 0x24 || opcode == 0x2C) {
    bit(curr.mode);
  } else if (opcode == 0x85 || opcode == 0x95 || opcode == 0x8D ||
             opcode == 0x9D || opcode == 0x99 || opcode == 0x81 ||
             opcode == 0x91) {
    sta(curr.mode);
  } else if (opcode == 0x86 || opcode == 0x96 || opcode == 0x8E) {
    write(getAddressingMode(curr.mode), x);
  } else if (opcode == 0x84 || opcode == 0x94 || opcode == 0x8C) {
    write(getAddressingMode(curr.mode), y);
  } else if (opcode == 0xA2 || opcode == 0xA6 || opcode == 0xB6 ||
             opcode == 0xAE || opcode == 0xBE) {
    ldx(curr.mode);
  } else if (opcode == 0xA0 || opcode == 0xA4 || opcode == 0xB4 ||
             opcode == 0xAC || opcode == 0xBC) {
    ldy(curr.mode);
  } else if (opcode == 0xA8) {
    y = a;
    setZeroFlag(y);
    setNegFlag(y);
  } else if (opcode == 0xBA) {
    x = sp;
    setZeroFlag(x);
    setNegFlag(x);
  } else if (opcode == 0x8A) {
    a = x;
    setZeroFlag(a);
    setNegFlag(a);
  } else if (opcode == 0x9A) {
    sp = x;
  } else if (opcode == 0x98) {
    a = y;
    setZeroFlag(a);
    setNegFlag(a);
  } else if (opcode == 0x00) {
    return;
  } else if (opcode == 0x9B) {
    uint8_t data = a & x;
    sp = data;
    uint16_t addr = readInt(pc) + static_cast<uint16_t>(y);
    data = (static_cast<uint8_t>(addr >> 8) + 1) & sp;
    write(addr, data);
  } else if (opcode == 0xC7 || opcode == 0xD7 || opcode == 0xCF ||
             opcode == 0xDF || opcode == 0xDB || opcode == 0xD3 ||
             opcode == 0xC3) {
    uint8_t data = read(getAddressingMode(curr.mode));
    data--;
    write(getAddressingMode(curr.mode), data);
    if (data <= a) {
      p |= 0b00000001;
    }
    setZeroFlag(a - data);
    setNegFlag(a - data);
  } else if (opcode == 0x27 || opcode == 0x37 || opcode == 0x2F ||
             opcode == 0x3F || opcode == 0x3B || opcode == 0x33 ||
             opcode == 0x23) {
    uint8_t data = rol(curr.mode);
    a = data & a;
  } else if (opcode == 0x07 || opcode == 0x17 || opcode == 0x0F ||
             opcode == 0x1F || opcode == 0x1B || opcode == 0x03 ||
             opcode == 0x13) {
    uint8_t data = asl(curr.mode);
    a = data | a;
  } else if (opcode == 0x47 || opcode == 0x57 || opcode == 0x4F ||
             opcode == 0x5F || opcode == 0x5B || opcode == 0x43 ||
             opcode == 0x53) {
    uint8_t data = lsr(curr.mode);
    a = data ^ a;
  } else if (opcode == 0x6B) {
    uint8_t data = read(getAddressingMode(curr.mode));
    a = data & a;
    rorAccum();
    uint8_t result = a;
    uint16_t bit5 = (result >> 5) & 1;
    uint16_t bit6 = (result >> 6) & 1;

    if (bit6 == 1) {
      p |= 0b00000001;
    } else {
      p &= 0b11111110;
    }

    if ((bit5 ^ bit6) == 1) {
      p |= 0b01000000;
    } else {
      p &= 0b10111111;
    }
  }

  bus.clock(curr.cycles);

  if (pcState == pc) {
    pc += static_cast<uint16_t>(curr.length - 1);
  }
}

void CPU::setZeroFlag(uint8_t d) {
  if (d == 0) {
    p |= 0b00000010;
  } else {
    p &= 0b11111101;
  }
}

void CPU::setNegFlag(uint8_t d) {
  if ((d >> 7) == 1) {
    p |= 0b10000000;
  } else {
    p &= 0b01111111;
  }
}

void CPU::setCarryFlag() { p |= 0b00000001; }

void CPU::clearCarryFlag() { p &= 0b11111110; }

void CPU::setRegA(uint8_t d) {
  a = d;
  setZeroFlag(a);
  setNegFlag(a);
}

void CPU::addRegA(uint8_t d) {
  uint16_t sum = static_cast<uint16_t>(a) + static_cast<uint16_t>(d);
  if ((p & (1 << 0)) > 0) {
    sum++;
  }
  if (sum > 0xFF) {
    p |= 0b00000001;
  } else {
    p &= 0b11111110;
  }

  if (((d ^ static_cast<uint8_t>(sum)) & (static_cast<uint8_t>(sum) ^ a) &
       0x80) != 0) {
    p |= 0b01000000;
  } else {
    p &= 0b10111111;
  }

  setRegA(static_cast<uint8_t>(sum));
}

uint8_t CPU::pop() {
  sp++;
  return read(static_cast<uint16_t>(0x0100) + static_cast<uint16_t>(sp));
}

uint16_t CPU::popInt() {
  uint16_t lo = static_cast<uint16_t>(pop());
  uint16_t hi = static_cast<uint16_t>(pop());
  return (hi << 8) | lo;
}

void CPU::push(uint8_t d) {
  write(static_cast<uint16_t>(0x0100) + static_cast<uint16_t>(sp), d);
  sp--;
}

void CPU::pushInt(uint16_t d) {
  uint8_t hi = static_cast<uint8_t>(d >> 8);
  uint8_t lo = static_cast<uint8_t>(d & 0xFF);
  push(hi);
  push(lo);
}

void CPU::compare(AddressingMode m, uint8_t code) {
  uint8_t temp = read(getAddressingMode(m));
  if (temp <= code) {
    p |= 0b00000001;
  } else {
    p &= 0b11111110;
  }
  uint8_t t = code - temp;
  setZeroFlag(t);
  setNegFlag(t);
}

void CPU::branch(bool con) {
  if (con) {
    bus.clock(1);
    int jump = (read(pc));
    if (jump > 127) {
      jump = 255 - jump + 1;
      jump *= -1;
    }
    uint16_t jumpAddr = pc + 1 + jump;
    if (((pc + 1) & 0xFF00) != (jumpAddr & 0xFF00)) {
      bus.clock(1);
    }
    pc = jumpAddr;
  }
}

uint16_t CPU::getAddressingMode(AddressingMode opcode) {
  if (opcode == imm) {
    return pc;
  } else {
    switch (opcode) {
      case zp0: {
        return static_cast<uint16_t>(read(pc));
        break;
      }
      case zpx: {
        return static_cast<uint16_t>(read(pc) + x);
        break;
      }
      case zpy: {
        return static_cast<uint16_t>(read(pc) + y);
        break;
      }
      case abs: {
        return readInt(pc);
        break;
      }
      case abx: {
        return readInt(pc) + static_cast<uint16_t>(x);
        break;
      }
      case aby: {
        return readInt(pc) + static_cast<uint16_t>(y);
        break;
      }
      case idx: {
        uint8_t base = read(pc);
        uint8_t ptr = base + x;
        uint8_t lo = read(static_cast<uint16_t>(ptr));
        uint8_t hi = read(static_cast<uint16_t>(ptr + 1));
        return (static_cast<uint16_t>(hi) << 8) | static_cast<uint16_t>(lo);
      }
      case idy: {
        uint8_t base = read(pc);
        uint8_t lo = read(static_cast<uint16_t>(base));
        uint8_t hi = read(static_cast<uint16_t>(base + 1));
        return ((static_cast<uint16_t>(hi) << 8) | static_cast<uint16_t>(lo)) +
               y;
      }
    }
  }
}

uint16_t CPU::getAddress(AddressingMode m, uint16_t addr) {
  if (m == imm) {
    return pc;
  } else {
    switch (m) {
      case zp0: {
        return static_cast<uint16_t>(read(addr));
        break;
      }
      case zpx: {
        return static_cast<uint16_t>(read(addr) + x);
        break;
      }
      case zpy: {
        return static_cast<uint16_t>(read(addr) + y);
        break;
      }
      case abs: {
        return readInt(addr);
        break;
      }
      case abx: {
        return readInt(addr) + static_cast<uint16_t>(x);
        break;
      }
      case aby: {
        return readInt(addr) + static_cast<uint16_t>(y);
        break;
      }
      case idx: {
        uint8_t base = read(addr);
        uint8_t ptr = base + x;
        uint8_t lo = read(static_cast<uint16_t>(ptr));
        uint8_t hi = read(static_cast<uint16_t>(ptr + 1));
        return (static_cast<uint16_t>(hi) << 8) | static_cast<uint16_t>(lo);
      }
      case idy: {
        uint8_t base = read(addr);
        uint8_t lo = read(static_cast<uint16_t>(base));
        uint8_t hi = read(static_cast<uint16_t>(base + 1));
        return ((static_cast<uint16_t>(hi) << 8) | static_cast<uint16_t>(lo)) +
               y;
      }
    }
  }
}

void CPU::adc(AddressingMode m) { addRegA(read(getAddressingMode(m))); }

void CPU::adn(AddressingMode m) { setRegA(read(getAddressingMode(m)) & a); }

void CPU::aslAccum() {
  uint16_t temp = a;
  if ((temp >> 7) == 1) {
    setCarryFlag();
  } else {
    clearCarryFlag();
  }
  temp = temp << 1;
  setRegA(temp);
}

uint8_t CPU::asl(AddressingMode m) {
  uint16_t temp = read(getAddressingMode(m));
  if (temp >> 7 == 1) {
    setCarryFlag();
  } else {
    clearCarryFlag();
  }
  temp = temp << 1;
  write(getAddressingMode(m), temp);
  setZeroFlag(temp);
  setNegFlag(temp);
  return temp;
}

void CPU::bit(AddressingMode m) {
  uint8_t temp = read(getAddressingMode(m));
  uint8_t tempAnd = temp & a;
  if (tempAnd == 0) {
    p |= 0b00000010;
  } else {
    p &= 0b11111101;
  }

  if ((tempAnd & 0b10000000) > 0) {
    p |= 0b10000000;
  } else {
    p &= 0b01111111;
  }

  if ((tempAnd & 0b01000000) > 0) {
    p |= 0b01000000;
  } else {
    p &= 0b10111111;
  }
}

void CPU::dex() {
  x--;
  setZeroFlag(x);
  setNegFlag(x);
}

uint8_t CPU::dec(AddressingMode m) {
  uint8_t temp = read(getAddressingMode(m));
  temp--;
  write(getAddressingMode(m), temp);
  setZeroFlag(temp);
  setNegFlag(temp);
  return temp;
}

void CPU::dey() {
  y--;
  setZeroFlag(y);
  setNegFlag(y);
}

void CPU::eor(AddressingMode m) { setRegA(read(getAddressingMode(m)) ^ a); }

uint8_t CPU::inc(AddressingMode m) {
  uint8_t temp = read(getAddressingMode(m));
  write(getAddressingMode(m), temp + 1);
  temp++;
  setZeroFlag(temp);
  setNegFlag(temp);
  return temp;
}

void CPU::inx() {
  x += 1;
  setZeroFlag(x);
  setNegFlag(x);
}

void CPU::iny() {
  y += 1;
  setZeroFlag(y);
  setNegFlag(y);
}

void CPU::lda(AddressingMode m) { setRegA(read(getAddressingMode(m))); }

void CPU::ldx(AddressingMode m) {
  x = read(getAddressingMode(m));
  setZeroFlag(x);
  setNegFlag(x);
}

void CPU::ldy(AddressingMode m) {
  y = read(getAddressingMode(m));
  setZeroFlag(y);
  setNegFlag(y);
}

void CPU::lsrAccum() {
  uint8_t temp = a;
  if ((temp & 1) == 1) {
    setCarryFlag();
  } else {
    clearCarryFlag();
  }
  temp = temp >> 1;
  setRegA(temp);
}

uint8_t CPU::lsr(AddressingMode m) {
  uint8_t temp = read(getAddressingMode(m));
  if ((temp & 1) == 1) {
    setCarryFlag();
  } else {
    clearCarryFlag();
  }
  temp = temp >> 1;
  write(getAddressingMode(m), temp);
  setZeroFlag(temp);
  setNegFlag(temp);
  return temp;
}

void CPU::ora(AddressingMode m) { setRegA(read(getAddressingMode(m)) | a); }

void CPU::pla() { setRegA(pop()); }

void CPU::plp() {
  p = pop();
  p &= 0b11101111;
  p |= 0b00100000;
}

void CPU::php() {
  uint8_t temp = p;
  temp |= 0b00010000;
  temp |= 0b00100000;
  push(temp);
}

void CPU::rolAccum() {
  uint8_t temp = a;
  if ((temp >> 7) == 1) {
    setCarryFlag();
  } else {
    clearCarryFlag();
  }
  temp = temp >> 1;
  if (p << 8 == 1) {
    temp |= 1;
  }
  setRegA(temp);
}

uint8_t CPU::rol(AddressingMode m) {
  uint8_t temp = read(getAddressingMode(m));
  if ((temp >> 7) == 1) {
    setCarryFlag();
  } else {
    clearCarryFlag();
  }
  temp = temp >> 1;
  if (p << 8 == 1) {
    temp |= 1;
  }
  write(getAddressingMode(m), temp);
  setNegFlag(temp);
  return temp;
}

void CPU::rorAccum() {
  uint8_t temp = a;
  if ((temp & 1) == 1) {
    setCarryFlag();
  } else {
    clearCarryFlag();
  }
  temp = temp >> 1;
  if (p << 8 == 1) {
    temp |= 0b10000000;
  }
  setRegA(temp);
}

uint8_t CPU::ror(AddressingMode m) {
  uint8_t temp = read(getAddressingMode(m));
  if ((temp & 1) == 1) {
    setCarryFlag();
  } else {
    clearCarryFlag();
  }
  temp = temp >> 1;
  if (p << 8 == 1) {
    temp |= 0b10000000;
  }
  write(getAddressingMode(m), temp);
  setNegFlag(temp);
  return temp;
}

void CPU::sbc(AddressingMode m) {
  int data = read(getAddressingMode(m));
  data *= -1;
  if (data == -255) {
    data = 0;
  } else {
    data--;
  }
  addRegA(data);
}

void CPU::sta(AddressingMode m) {
  uint16_t addr = getAddressingMode(m);
  write(addr, a);
}

void CPU::tax() {
  x = a;
  setZeroFlag(x);
  setNegFlag(x);
}

void CPU::nmiInterrupt() {
  pushInt(pc);
  uint8_t temp = p;
  p &= 0b11101111;
  p |= 0b00100000;
  push(temp);
  p |= 0b00000100;

  bus.clock(2);
  pc = readInt(0xFFFA);
}

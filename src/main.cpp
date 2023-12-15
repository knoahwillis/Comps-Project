#include <SDL2/SDL.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>

#include "bus/bus.hpp"
#include "cpu/cpu.hpp"

// functions for constructing the log of the nestest.nes rom

std::string makeHex(uint8_t c) {
  std::stringstream ret;
  ret << std::uppercase << std::hex << (c & 0xff);
  return ret.str();
}

std::string makeHexInt(uint16_t i) {
  std::stringstream ret;
  ret << std::uppercase << std::hex << (i & 0xffff);
  return ret.str();
}

std::string trace(CPU& cpu) {
  uint8_t code = cpu.read(cpu.pc);
  CPU::Ins curr = cpu.opcodes[code];

  uint16_t begin = cpu.pc;
  std::vector<uint8_t> hexDump;
  hexDump.push_back(code);

  uint16_t memAddr;
  uint8_t val;

  if (curr.mode == CPU::AddressingMode::imm ||
      curr.mode == CPU::AddressingMode::xxx) {
    memAddr = 0x00;
    val = 0x00;
  } else {
    memAddr = cpu.getAddress(curr.mode, begin + 1);
    val = cpu.read(memAddr);
  }

  std::string temp;

  if (curr.length == 1) {
    if (curr.opcode == 0x0A || curr.opcode == 0x4A || curr.opcode == 0x2A ||
        curr.opcode == 0x6A) {
      temp = "A";
    } else {
      temp = "";
    }
  } else if (curr.length == 2) {
    uint8_t addr = cpu.read(begin + 1);
    hexDump.push_back(addr);

    switch (curr.mode) {
      case CPU::imm: {
        std::string ret = "#$";
        ret += makeHex(addr);
        temp = ret;
        break;
      }
      case CPU::zp0: {
        std::string ret = "$";
        ret += makeHex(memAddr);
        ret += "=";
        ret += makeHex(val);
        temp = ret;
        break;
      }
      case CPU::zpx: {
        std::string ret = "$";
        ret += makeHex(addr);
        ret += ",X@";
        ret += makeHex(memAddr);
        ret += "=";
        ret += makeHex(val);
        temp = ret;
        break;
      }
      case CPU::zpy: {
        std::string ret = "$";
        ret += makeHex(addr);
        ret += ",Y@";
        ret += makeHex(memAddr);
        ret += "=";
        ret += makeHex(val);
        temp = ret;
        break;
      }
      case CPU::idx: {
        std::string ret = "($";
        ret += makeHex(addr);
        ret += ",X)@";
        ret += makeHex(addr + cpu.x);
        ret += "=";
        ret += makeHex(memAddr);
        ret += "=";
        ret += makeHex(val);
        temp = ret;
        break;
      }
      case CPU::idy: {
        std::string ret = "($";
        ret += makeHex(addr);
        ret += "),Y=";
        ret += makeHex(memAddr - static_cast<uint16_t>(cpu.y));
        ret += "@";
        ret += makeHex(memAddr);
        ret += "=";
        ret += makeHex(val);
        temp = ret;
        break;
      }
      case CPU::xxx: {
        int address = (begin + 2) + addr;
        std::string ret = "$";
        ret += makeHexInt(address);
        temp = ret;
        break;
      }
    }
  } else if (curr.length == 3) {
    uint8_t lo = cpu.read(begin + 1);
    uint8_t hi = cpu.read(begin + 2);
    hexDump.push_back(lo);
    hexDump.push_back(hi);

    uint16_t addr = cpu.readInt(begin + 1);

    switch (curr.mode) {
      case CPU::AddressingMode::abs: {
        std::string ret = "$";
        ret += makeHex(memAddr);
        ret += "=";
        ret += makeHex(val);
        temp = ret;
        break;
      }
      case CPU::AddressingMode::abx: {
        std::string ret = "$";
        ret += makeHex(addr);
        ret += ",X @";
        ret += makeHex(memAddr);
        ret += "=";
        ret += makeHex(val);
        temp = ret;
        break;
      }
      case CPU::AddressingMode::aby: {
        std::string ret = "$";
        ret += makeHex(addr);
        ret += ",Y@";
        ret += makeHex(memAddr);
        ret += "=";
        ret += makeHex(val);
        temp = ret;
        break;
      }
      case CPU::AddressingMode::xxx: {
        if (curr.opcode == 0x6C) {
          uint16_t jumpAddr;
          if ((addr & 0x00FF) == 0x00FF) {
            uint8_t lo = cpu.read(addr);
            uint8_t hi = cpu.read(addr & 0xFF00);
            jumpAddr =
                (static_cast<uint16_t>(hi) << 8) | static_cast<uint16_t>(lo);
          } else {
            jumpAddr = cpu.readInt(addr);
          }
          std::string ret = "($";
          ret += makeHex(addr);
          ret += ")=";
          ret += makeHex(jumpAddr);
          temp = ret;
        } else {
          std::string ret = "$";
          ret += makeHex(addr);
          temp = ret;
        }
        break;
      }
    }
  }

  std::string hexString;
  for (const auto& h : hexDump) {
    hexString += makeHex(h);
  }

  std::string asmString = makeHexInt(begin);
  asmString += hexString;
  asmString += curr.name;
  asmString += temp;
  asmString += "A:";
  asmString += makeHex(cpu.a);
  asmString += "X:";
  asmString += makeHex(cpu.x);
  asmString += "Y:";
  asmString += makeHex(cpu.y);
  asmString += "P:";
  asmString += makeHexInt(cpu.p);
  asmString += "SP:";
  asmString += makeHex(cpu.sp);
  asmString += "\n";
  return asmString;
}

float compareFiles(std::string first, std::string second) {
  int total = 0;
  int diff_one = 0;
  int diff_two = 0;
  std::ifstream f;
  f.open(first, std::ios::in);
  std::ifstream s;
  s.open(second, std::ios::in);
  while (f && s) {
    total++;
    if (f.get() != s.get()) {
      diff_one++;
    }
  }
  float correct = total - diff_one;
  float percent = (correct / total) * 100;
  return percent;
}

void nestest() {
  std::ifstream ifs;
  ifs.open("nestest.nes", std::ios::binary | std::ios::in);
  std::ofstream out;
  out.open("output.log", std::ios::out);
  std::vector<uint8_t> data;
  while (ifs) {
    data.push_back(ifs.get());
  }
  CPU cpu;
  CartRom c;
  Bus b;
  c.loadRom(data);
  b.connectPPU(c);
  cpu.connectBus(b);
  cpu.reset();
  cpu.pc = 0xC000;

  std::string temp;

  cpu.runWithFunction([&cpu, &temp]() { temp += trace(cpu); });

  out << temp;
  std::cout << "Test accuracy when compared to another emulator: "
            << compareFiles("nestest.log", "output.log") << "%\n";
}

// functions to help draw the screen for snake

SDL_Color getColor(uint8_t d) {
  if (d == 0) {
    return SDL_Color{0, 0, 0, 255};
  } else if (d == 1) {
    return SDL_Color{255, 255, 255, 255};
  } else if (d == 2 || d == 9) {
    return SDL_Color{125, 125, 125, 255};
  } else if (d == 3 || d == 10) {
    return SDL_Color{255, 0, 0, 255};
  } else if (d == 4 || d == 11) {
    return SDL_Color{0, 255, 0, 255};
  } else if (d == 5 || d == 12) {
    return SDL_Color{0, 0, 255, 255};
  } else if (d == 6 || d == 13) {
    return SDL_Color{255, 0, 255, 255};
  } else if (d == 7 || d == 14) {
    return SDL_Color{255, 255, 0, 255};
  } else {
    return SDL_Color{0, 255, 255, 255};
  }
}

bool readScreenState(CPU cpu, std::array<uint8_t, 3072>& screen) {
  int screenIndex = 0;
  bool changed = false;
  for (uint16_t i = 0x0200; i < 0x0600; i++) {
    uint8_t colorIndex = cpu.read(i);
    SDL_Color c = getColor(colorIndex);
    if (screen[screenIndex] != c.r || screen[screenIndex + 1] != c.g ||
        screen[screenIndex + 2] != c.b) {
      screen[screenIndex] = c.r;
      screen[screenIndex + 1] = c.g;
      screen[screenIndex + 2] = c.b;
      changed = true;
    }
    screenIndex += 3;
  }
  return changed;
}

void snake() {
  CartRom c;
  Bus b;

  SDL_Window* window =
      SDL_CreateWindow("snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       600, 600, SDL_WINDOW_FULLSCREEN);
  SDL_Renderer* rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_Texture* text = SDL_CreateTexture(rend, SDL_PIXELFORMAT_RGB24,
                                        SDL_TEXTUREACCESS_STREAMING, 32, 32);

  std::ifstream ifs;
  ifs.open("donkey kong.nes", std::ios::binary | std::ios::in);
  std::vector<uint8_t> data;
  CPU cpu;
  while (ifs) {
    data.push_back(ifs.get());
  }
  c.loadRom(data);
  b.connectPPU(c);
  b.connectFunction([]() {});
  cpu.connectBus(b);

  std::array<uint8_t, 3072> blank_screen;

  std::srand(std::time(nullptr));

  // clang-format off
  const int snake[5][20] = {
      {1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1},

      {1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1},

      {0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1},

      {1, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1},
      {1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1}};
  // clang-format on
  //
  int scale = 10;

  bool close = false;
  while (!close) {
    SDL_RenderClear(rend);

    SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
    int x_offset = 30;
    int y_offset = 30;
    for (int i = 0; i < 6; i++) {
      for (int y = 0; y < 5; y++) {
        for (int x = 0; x < 4; x++) {
          if (snake[i][4 * y + x] == 1) {
            SDL_Rect r;
            r.x = x_offset + x * scale;
            r.y = y_offset + y * scale;
            r.w = scale;
            r.h = scale;

            SDL_RenderFillRect(rend, &r);
          }
        }
      }
      x_offset += 6 * scale;
    }
    SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
    SDL_RenderPresent(rend);
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        close = true;
      } else if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.scancode) {
          case SDL_SCANCODE_ESCAPE:
            close = true;
            break;
          case SDL_SCANCODE_C:
            SDL_RenderClear(rend);
            SDL_RenderPresent(rend);
            break;

          case SDL_SCANCODE_SPACE: {
            std::array<uint8_t, 3072> screen;
            for (int i = 0; i < 3072; i++) {
              screen[i] = 0x00;
            }
            cpu.load();
            cpu.reset();
            cpu.runWithFunction([&cpu, &screen, &rend, &text]() {
              SDL_RenderClear(rend);
              SDL_Event e;
              while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                  cpu.closed = true;
                } else if (e.type == SDL_KEYDOWN) {
                  switch (e.key.keysym.scancode) {
                    case SDL_SCANCODE_ESCAPE:
                      cpu.closed = true;
                      break;
                    case SDL_SCANCODE_A:
                      cpu.write(0xFF, 0x61);
                      break;
                    case SDL_SCANCODE_S:
                      cpu.write(0xFF, 0x73);
                      break;
                    case SDL_SCANCODE_D:
                      cpu.write(0xFF, 0x64);
                      break;
                    case SDL_SCANCODE_W:
                      cpu.write(0xFF, 0x77);
                      break;
                  }
                }
              }
              cpu.write(0xFE, (std::rand() % 16) + 1);

              if (readScreenState(cpu, screen)) {
                SDL_UpdateTexture(text, NULL, screen.data(), 32 * 3);
                SDL_RenderCopy(rend, text, NULL, NULL);
                SDL_RenderPresent(rend);
              }
              std::this_thread::sleep_for(std::chrono::nanoseconds(4000));
            });

            break;
          }
        }
      }
    }
  }
}
// nes color pallette
std::array<SDL_Color, 64> pallete = {
    SDL_Color{0x80, 0x80, 0x80}, SDL_Color{0x00, 0x3D, 0xA6},
    SDL_Color{0x00, 0x12, 0xB0}, SDL_Color{0x44, 0x00, 0x96},
    SDL_Color{0xA1, 0x00, 0x5E}, SDL_Color{0xC7, 0x00, 0x28},
    SDL_Color{0xBA, 0x06, 0x00}, SDL_Color{0x8C, 0x17, 0x00},
    SDL_Color{0x5C, 0x2F, 0x00}, SDL_Color{0x10, 0x45, 0x00},
    SDL_Color{0x05, 0x4A, 0x00}, SDL_Color{0x00, 0x47, 0x2E},
    SDL_Color{0x00, 0x41, 0x66}, SDL_Color{0x00, 0x00, 0x00},
    SDL_Color{0x05, 0x05, 0x05}, SDL_Color{0x05, 0x05, 0x05},
    SDL_Color{0xC7, 0xC7, 0xC7}, SDL_Color{0x00, 0x77, 0xFF},
    SDL_Color{0x21, 0x55, 0xFF}, SDL_Color{0x82, 0x37, 0xFA},
    SDL_Color{0xEB, 0x2F, 0xB5}, SDL_Color{0xFF, 0x29, 0x50},
    SDL_Color{0xFF, 0x22, 0x00}, SDL_Color{0xD6, 0x32, 0x00},
    SDL_Color{0xC4, 0x62, 0x00}, SDL_Color{0x35, 0x80, 0x00},
    SDL_Color{0x05, 0x8F, 0x00}, SDL_Color{0x00, 0x8A, 0x55},
    SDL_Color{0x00, 0x99, 0xCC}, SDL_Color{0x21, 0x21, 0x21},
    SDL_Color{0x09, 0x09, 0x09}, SDL_Color{0x09, 0x09, 0x09},
    SDL_Color{0xFF, 0xFF, 0xFF}, SDL_Color{0x0F, 0xD7, 0xFF},
    SDL_Color{0x69, 0xA2, 0xFF}, SDL_Color{0xD4, 0x80, 0xFF},
    SDL_Color{0xFF, 0x45, 0xF3}, SDL_Color{0xFF, 0x61, 0x8B},
    SDL_Color{0xFF, 0x88, 0x33}, SDL_Color{0xFF, 0x9C, 0x12},
    SDL_Color{0xFA, 0xBC, 0x20}, SDL_Color{0x9F, 0xE3, 0x0E},
    SDL_Color{0x2B, 0xF0, 0x35}, SDL_Color{0x0C, 0xF0, 0xA4},
    SDL_Color{0x05, 0xFB, 0xFF}, SDL_Color{0x5E, 0x5E, 0x5E},
    SDL_Color{0x0D, 0x0D, 0x0D}, SDL_Color{0x0D, 0x0D, 0x0D},
    SDL_Color{0xFF, 0xFF, 0xFF}, SDL_Color{0xA6, 0xFC, 0xFF},
    SDL_Color{0xB3, 0xEC, 0xFF}, SDL_Color{0xDA, 0xAB, 0xEB},
    SDL_Color{0xFF, 0xA8, 0xF9}, SDL_Color{0xFF, 0xAB, 0xB3},
    SDL_Color{0xFF, 0xD2, 0xB0}, SDL_Color{0xFF, 0xEF, 0xA6},
    SDL_Color{0xFF, 0xF7, 0x9C}, SDL_Color{0xD7, 0xE8, 0x95},
    SDL_Color{0xA6, 0xED, 0xAF}, SDL_Color{0xA2, 0xF2, 0xDA},
    SDL_Color{0x99, 0xFF, 0xFC}, SDL_Color{0xDD, 0xDD, 0xDD},
    SDL_Color{0x11, 0x11, 0x11}, SDL_Color{0x11, 0x11, 0x11}};

// functions for displaying tiles

std::array<uint8_t, 256 * 240 * 3> getTiles(std::vector<uint8_t> chr, int b) {
  b *= 4096;
  std::array<uint8_t, 256 * 240 * 3> pixels;
  int x = 0;
  int y = 0;

  for (int i = 0; i < 255; i++) {
    if (i != 0 && i % 20 == 0) {
      y += 10;
      x = 0;
    }
    std::array<uint8_t, 16> tile;

    int pos = 0;
    for (int j = b + i * 16; j <= b + i * 16 + 15; j++) {
      tile[pos] = chr[j];
      pos++;
    }
    for (int k = 0; k < 8; k++) {
      uint8_t hi = tile[k];
      uint8_t lo = tile[k + 8];
      for (int j = 7; j >= 0; j--) {
        int val = (1 & hi) << 1 | (1 & lo);
        hi = hi >> 1;
        lo = lo >> 1;
        SDL_Color c;
        if (val == 0) {
          c = pallete[1];
        } else if (val == 1) {
          c = pallete[10];
        } else if (val == 2) {
          c = pallete[23];
        } else if (val == 3) {
          c = pallete[35];
        }
        pixels[(k + y) * 3 * 256 + (j + x) * 3] = c.r;
        pixels[(k + y) * 3 * 256 + (j + x) * 3 + 1] = c.g;
        pixels[(k + y) * 3 * 256 + (j + x) * 3 + 2] = c.b;
      }
    }
    x += 10;
  }
  return pixels;
}

void pputest() {
  CartRom c;
  Bus b;

  SDL_Window* window = SDL_CreateWindow("PPU Preview", SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED, 512, 480, NULL);
  SDL_Renderer* rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_Texture* text = SDL_CreateTexture(rend, SDL_PIXELFORMAT_RGB24,
                                        SDL_TEXTUREACCESS_STREAMING, 256, 250);

  std::ifstream ifs;
  ifs.open("donkey kong.nes", std::ios::binary | std::ios::in);
  std::vector<uint8_t> data;
  CPU cpu;
  while (ifs) {
    data.push_back(ifs.get());
  }
  c.loadRom(data);
  b.connectPPU(c);
  cpu.connectBus(b);

  std::array<uint8_t, 256 * 240 * 3> bank = getTiles(c.chrROM, 0);
  SDL_UpdateTexture(text, NULL, bank.data(), 256 * 3);
  SDL_RenderCopy(rend, text, NULL, NULL);
  SDL_RenderPresent(rend);
  bool closed = false;

  while (!closed) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        closed = true;
      } else if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.scancode) {
          case SDL_SCANCODE_ESCAPE:
            closed = true;
            break;
        }
      }
    }
  }
}

int main(int argc, char* argv[]) {
  std::string test = argv[1];
  if (test == "nestest") {
    nestest();
  } else if (test == "snake") {
    snake();
  } else if (test == "ppu") {
    pputest();
  }
  return 0;
}

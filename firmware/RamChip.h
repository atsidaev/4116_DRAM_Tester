#ifndef __RAMCHIP_H
#define __RAMCHIP_H

#include <Arduino.h>
#include "pinout.h"

template <uint8_t Rows, uint8_t Cols>
class RamChip {
    public:
        static constexpr uint8_t BUS_WIDTH = (Rows > Cols ? Rows : Cols);
        static constexpr uint8_t BUS_WIDTH_RAS = Rows;
        static constexpr uint8_t BUS_WIDTH_CAS = Cols;

    private:
        const uint8_t bus[BUS_WIDTH] = { XA0, XA1, XA2, XA3, XA4, XA5, XA6 };

    public:
        void init() {
            for (uint8_t i = 0; i < this->BUS_WIDTH; i++)
                pinMode(this->bus[i], OUTPUT);

            pinMode(DI, OUTPUT);
            pinMode(DO, INPUT);

            pinMode(nCAS, OUTPUT);
            pinMode(nRAS, OUTPUT);
            pinMode(nWRITE, OUTPUT);

            digitalWrite(nWRITE, HIGH);
            digitalWrite(nRAS, HIGH);
            digitalWrite(nCAS, HIGH);
        }

        void setBus(uint8_t addr) {
            #ifdef __AVR_ATmega328P__
            // Optimized code (10x speed increase)
            
            // PORTB: D8, D9, D13
            PORTB = (PORTB & ~((1<<0)|(1<<1)|(1<<5))) |
                    ((addr & 0x01) << 1) |   // XA0 -> D9
                    ((addr & 0x04) >> 2) |   // XA2 -> D8
                    ((addr & 0x20));         // XA5 -> D13

            // PORTD: D3, D4, D7
            PORTD = (PORTD & ~((1<<2)|(1<<3)|(1<<4)|(1<<7))) |
                    ((addr & 0x10) >> 2) |   // XA4 -> D2
                    ((addr & 0x08)) |        // XA3 -> D3
                    ((addr & 0x40) >> 2) |   // XA6 -> D4
                    ((addr & 0x02) << 6);    // XA1 -> D7
            #else
            // Much slower, but compatible with all Arduino platforms
            for (uint8_t i = 0; i < this->BUS_WIDTH; i++) {
                digitalWrite(bus[i], addr & 1);
                addr >>= 1;
            }
            #endif
        }

        void writeAddress(uint8_t ras, uint8_t cas, uint8_t val) {
            setBus(ras);
            digitalWrite(nRAS, LOW);

            // According to DS, nWRITE should be on only after RAS
            digitalWrite(nWRITE, LOW);

            digitalWrite(DI, val & 0x01);

            setBus(cas);
            digitalWrite(nCAS, LOW);

            digitalWrite(nCAS, HIGH);
            digitalWrite(nWRITE, HIGH);
            digitalWrite(nRAS, HIGH);
        }

        uint8_t readAddress(uint8_t ras, uint8_t cas) {
            uint8_t ret = 0;

            setBus(ras);
            digitalWrite(nRAS, LOW);

            setBus(cas);
            digitalWrite(nCAS, LOW);

            ret = digitalRead(DO);

            digitalWrite(nCAS, HIGH);
            digitalWrite(nRAS, HIGH);

            return ret;
        }

        void fastClean() {
            // Writing 0 to all cells
            digitalWrite(DI, LOW);

            for (uint8_t ras = 0; ras < (1 << Rows); ras++) {
                setBus(ras);
                digitalWrite(nRAS, LOW);
                digitalWrite(nWRITE, LOW);

                // Making 128 CASes for single RAS
                for (uint8_t cas = 0; cas < (1 << Cols); cas++) {
                    setBus(cas);
                    digitalWrite(nCAS, LOW);
                    digitalWrite(nCAS, HIGH);
                }

                digitalWrite(nWRITE, HIGH);
                digitalWrite(nRAS, HIGH);
            }
        }
};

#endif

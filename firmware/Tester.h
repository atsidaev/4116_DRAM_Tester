#include "RamChip.h"
#include "pinout.h"

template <uint8_t Rows, uint8_t Cols>
class Tester {
    private:
        RamChip<Rows, Cols> *chip;
    public:
        Tester(RamChip<Rows, Cols> *chip) {
            this->chip = chip;
        }
        void init();

        bool RasCasTest()
        {
            // Find interconnected CAS lines if any

            chip->fastClean();
            uint8_t ras = 0x55;
            uint8_t cas = 0x08;

            // Only 1 bit in whole chip is set
            chip->writeAddress(ras, cas, 1);

            // Iterate over all CAS, 1 must be at one pos only
            for (uint8_t c = 0; c < (1 << chip->BUS_WIDTH_CAS); c++)
            {
                uint8_t v = chip->readAddress(ras, c);
                if ((v != 0 && c != cas) || (v == 0 && c == cas))
                    return false;
            }

            // Find interconnected RAS lines if any

            chip->fastClean();
            cas = 0x55;
            ras = 0x04;
            chip->writeAddress(ras, cas, 1);

            // Iterate over all RAS, 1 must be at one pos only
            for (uint8_t r = 0; r < (1 << chip->BUS_WIDTH_RAS); r++)
            {
                uint8_t v = chip->readAddress(r, cas);
                if ((v != 0 && r != ras) || (v == 0 && r == ras))
                    return false;
            }

            return true;
        }

        bool IoShortCircuitTest()
        {
            uint8_t v1 = digitalRead(DO);

            // Write the opposite value to DI
            digitalWrite(DI, v1 ^ 1);

            // Output should not change
            return v1 == digitalRead(DO);
        }

        bool CellTest(uint8_t evenCellPattern, uint8_t oddCellPattern)
        {
            bool isEven = true;
            for (uint8_t c = 0; c < (1 << chip->BUS_WIDTH_CAS); c++) {
                for (uint8_t r = 0; r < (1 << chip->BUS_WIDTH_RAS); r++) {
                    uint8_t v = isEven ? evenCellPattern : oddCellPattern;
                    chip->writeAddress(r, c, v);
                    
                    uint8_t v_read = chip->readAddress(r, c);
                    if (v_read != v)
                        return false;
                    isEven = !isEven;
                }
            }
            return true;
        }
};

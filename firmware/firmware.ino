#include "StateReporter.h"
#include "RamChip.h"
#include "Tester.h"

StateReporter state;

RamChip<7,7> chip;
Tester<7,7> tester(&chip);

typedef struct
{
    uint8_t v1;
    uint8_t v2;
} test;

test tests[] = { 
  { 0, 1 },
  { 0, 0 }, 
  { 1, 1 }, 
  { 1, 0 },
};

void setup() {
    state.init();
    state.println(F("4116\nTests"));

    chip.init();
}

void loop() {
    bool result = true;
    uint8_t i;

    state.print("IO ");
    result = state.logTestResult(tester.IoShortCircuitTest());

    state.print("RC ");
    bool rasCasResult = state.logTestResult(tester.RasCasTest());
    result &= rasCasResult;

    for (i = 0; i < sizeof(tests) / sizeof(test); i++)
    {
        test t = tests[i];
        state.write('0' + t.v1);
        state.write('0' + t.v2);
        state.write(' ');
        state.refresh();

        bool test_res = state.logTestResult(tester.CellTest(t.v1, t.v2));

        // Blink at the end of the test
        state.ledBlink();

        // Blink twice if test passed
        if (test_res)
            state.ledBlink();

        result &= test_res;
    }

    state.logTestResult(result, F("GOOD!"), F("FAIL!"));

    while (1) {
        if (result)
            state.ledOn();
        else
            for (i = 0; i < 5; i++)
                state.ledBlink(); 

        delay(600);
    }
}

//
// Created by bardanik on 04/05/24.
//

#ifndef PA2_BIG_TASK_CTESTER_H
#define PA2_BIG_TASK_CTESTER_H

#include <cassert>
#include <cfloat>
#include "../src/CSpreadsheet.h"

bool valueMatch(const CValue &r,
                const CValue &s) {
    if (r.index() != s.index())
        return false;
    if (r.index() == 0)
        return true;
    if (r.index() == 2)
        return std::get<std::string>(r) == std::get<std::string>(s);
    if (std::isnan(std::get<double>(r)) && std::isnan(std::get<double>(s)))
        return true;
    if (std::isinf(std::get<double>(r)) && std::isinf(std::get<double>(s)))
        return (std::get<double>(r) < 0 && std::get<double>(s) < 0)
               || (std::get<double>(r) > 0 && std::get<double>(s) > 0);
    return fabs(std::get<double>(r) - std::get<double>(s)) <= 1e8 * DBL_EPSILON * fabs(std::get<double>(r));
}


struct Tester {

    static void layout() {
        cout << '\n' << __func__ << " -> START" << endl;

        cout << __func__ << " ->    OK" << '\n' << endl;
    }

    static void runAll() {
        basicTest();
        cycleDetectionTest();
        loaderTest();
        functionsTest();
    }

    static void basicTest() {
        cout << '\n' << __func__ << " -> START" << endl;

        CSpreadsheet x0, x1;
        std::ostringstream oss;
        std::istringstream iss;
        std::string data;
        assert(x0.setCell(CPos("A1"), "10"));
        assert(x0.setCell(CPos("A2"), "20.5"));
        assert(x0.setCell(CPos("A3"), "3e1"));
        assert(x0.setCell(CPos("A4"), "=40"));
        assert(x0.setCell(CPos("A5"), "=5e+1"));
        assert(x0.setCell(CPos("A6"), "raw text with any characters, including a quote \" or a newline\n"));
        assert(x0.setCell(CPos("A7"),
                          "=\"quoted string, quotes must be doubled: \"\". Moreover, backslashes are needed for C++.\""));
        assert(valueMatch(x0.getValue(CPos("A1")), CValue(10.0)));
        assert(valueMatch(x0.getValue(CPos("A2")), CValue(20.5)));
        assert(valueMatch(x0.getValue(CPos("A3")), CValue(30.0)));
        assert(valueMatch(x0.getValue(CPos("A4")), CValue(40.0)));
        assert(valueMatch(x0.getValue(CPos("A5")), CValue(50.0)));
        assert(valueMatch(x0.getValue(CPos("A6")),
                          CValue("raw text with any characters, including a quote \" or a newline\n")));
        assert(valueMatch(x0.getValue(CPos("A7")),
                          CValue("quoted string, quotes must be doubled: \". Moreover, backslashes are needed for C++.")));
        assert(valueMatch(x0.getValue(CPos("A8")), CValue()));
        assert(valueMatch(x0.getValue(CPos("AAAA9999")), CValue()));
        assert(x0.setCell(CPos("B1"), "=A1+A2*A3"));
        assert(x0.setCell(CPos("B2"), "= -A1 ^ 2 - A2 / 2   "));
        assert(x0.setCell(CPos("B3"), "= 2 ^ $A$1"));
        assert(x0.setCell(CPos("B4"), "=($A1+A$2)^2"));
        assert(x0.setCell(CPos("B5"), "=B1+B2+B3+B4"));
        assert(x0.setCell(CPos("B6"), "=B1+B2+B3+B4+B5"));
        assert(valueMatch(x0.getValue(CPos("B1")), CValue(625.0)));
        assert(valueMatch(x0.getValue(CPos("B2")), CValue(-110.25)));
        assert(valueMatch(x0.getValue(CPos("B3")), CValue(1024.0)));
        assert(valueMatch(x0.getValue(CPos("B4")), CValue(930.25)));
        assert(valueMatch(x0.getValue(CPos("B5")), CValue(2469.0)));
        assert(valueMatch(x0.getValue(CPos("B6")), CValue(4938.0)));
        assert(x0.setCell(CPos("A1"), "12"));
        assert(valueMatch(x0.getValue(CPos("B1")), CValue(627.0)));
        assert(valueMatch(x0.getValue(CPos("B2")), CValue(-154.25)));
        assert(valueMatch(x0.getValue(CPos("B3")), CValue(4096.0)));
        assert(valueMatch(x0.getValue(CPos("B4")), CValue(1056.25)));
        assert(valueMatch(x0.getValue(CPos("B5")), CValue(5625.0)));
        assert(valueMatch(x0.getValue(CPos("B6")), CValue(11250.0)));
        x1 = x0;
        assert(x0.setCell(CPos("A2"), "100"));
        assert(x1.setCell(CPos("A2"), "=A3+A5+A4"));
        assert(valueMatch(x0.getValue(CPos("B1")), CValue(3012.0)));
        assert(valueMatch(x0.getValue(CPos("B2")), CValue(-194.0)));
        assert(valueMatch(x0.getValue(CPos("B3")), CValue(4096.0)));
        assert(valueMatch(x0.getValue(CPos("B4")), CValue(12544.0)));
        assert(valueMatch(x0.getValue(CPos("B5")), CValue(19458.0)));
        assert(valueMatch(x0.getValue(CPos("B6")), CValue(38916.0)));
        assert(valueMatch(x1.getValue(CPos("B1")), CValue(3612.0)));
        assert(valueMatch(x1.getValue(CPos("B2")), CValue(-204.0)));
        assert(valueMatch(x1.getValue(CPos("B3")), CValue(4096.0)));
        assert(valueMatch(x1.getValue(CPos("B4")), CValue(17424.0)));
        assert(valueMatch(x1.getValue(CPos("B5")), CValue(24928.0)));
        assert(valueMatch(x1.getValue(CPos("B6")), CValue(49856.0)));
        oss.clear();
        oss.str("");
        assert(x0.save(oss));
        data = oss.str();
        iss.clear();
        iss.str(data);
        assert(x1.load(iss));
        assert(valueMatch(x1.getValue(CPos("B1")), CValue(3012.0)));
        assert(valueMatch(x1.getValue(CPos("B2")), CValue(-194.0)));
        assert(valueMatch(x1.getValue(CPos("B3")), CValue(4096.0)));
        assert(valueMatch(x1.getValue(CPos("B4")), CValue(12544.0)));
        assert(valueMatch(x1.getValue(CPos("B5")), CValue(19458.0)));
        assert(valueMatch(x1.getValue(CPos("B6")), CValue(38916.0)));
        assert(x0.setCell(CPos("A3"), "4e1"));
        assert(valueMatch(x1.getValue(CPos("B1")), CValue(3012.0)));
        assert(valueMatch(x1.getValue(CPos("B2")), CValue(-194.0)));
        assert(valueMatch(x1.getValue(CPos("B3")), CValue(4096.0)));
        assert(valueMatch(x1.getValue(CPos("B4")), CValue(12544.0)));
        assert(valueMatch(x1.getValue(CPos("B5")), CValue(19458.0)));
        assert(valueMatch(x1.getValue(CPos("B6")), CValue(38916.0)));
        oss.clear();
        oss.str("");
        assert(x0.save(oss));
        data = oss.str();
        for (size_t i = 0; i < std::min<size_t>(data.length(), 10); i++)
            data[i] ^= 0x5a;
        iss.clear();
        iss.str(data);
        assert(!x1.load(iss));
        assert(x0.setCell(CPos("D0"), "10"));
        assert(x0.setCell(CPos("D1"), "20"));
        assert(x0.setCell(CPos("D2"), "30"));
        assert(x0.setCell(CPos("D3"), "40"));
        assert(x0.setCell(CPos("D4"), "50"));
        assert(x0.setCell(CPos("E0"), "60"));
        assert(x0.setCell(CPos("E1"), "70"));
        assert(x0.setCell(CPos("E2"), "80"));
        assert(x0.setCell(CPos("E3"), "90"));
        assert(x0.setCell(CPos("E4"), "100"));
        assert(x0.setCell(CPos("F10"), "=D0+5"));
        assert(x0.setCell(CPos("F11"), "=$D0+5"));
        assert(x0.setCell(CPos("F12"), "=D$0+5"));
        assert(x0.setCell(CPos("F13"), "=$D$0+5"));
        x0.copyRect(CPos("G11"), CPos("F10"), 1, 4);
        assert(valueMatch(x0.getValue(CPos("F10")), CValue(15.0)));
        assert(valueMatch(x0.getValue(CPos("F11")), CValue(15.0)));
        assert(valueMatch(x0.getValue(CPos("F12")), CValue(15.0)));
        assert(valueMatch(x0.getValue(CPos("F13")), CValue(15.0)));
        assert(valueMatch(x0.getValue(CPos("F14")), CValue()));
        assert(valueMatch(x0.getValue(CPos("G10")), CValue()));
        assert(valueMatch(x0.getValue(CPos("G11")), CValue(75.0)));
        assert(valueMatch(x0.getValue(CPos("G12")), CValue(25.0)));
        assert(valueMatch(x0.getValue(CPos("G13")), CValue(65.0)));
        assert(valueMatch(x0.getValue(CPos("G14")), CValue(15.0)));
        x0.copyRect(CPos("G11"), CPos("F10"), 2, 4);
        assert(valueMatch(x0.getValue(CPos("F10")), CValue(15.0)));
        assert(valueMatch(x0.getValue(CPos("F11")), CValue(15.0)));
        assert(valueMatch(x0.getValue(CPos("F12")), CValue(15.0)));
        assert(valueMatch(x0.getValue(CPos("F13")), CValue(15.0)));
        assert(valueMatch(x0.getValue(CPos("F14")), CValue()));
        assert(valueMatch(x0.getValue(CPos("G10")), CValue()));
        assert(valueMatch(x0.getValue(CPos("G11")), CValue(75.0)));
        assert(valueMatch(x0.getValue(CPos("G12")), CValue(25.0)));
        assert(valueMatch(x0.getValue(CPos("G13")), CValue(65.0)));
        assert(valueMatch(x0.getValue(CPos("G14")), CValue(15.0)));
        assert(valueMatch(x0.getValue(CPos("H10")), CValue()));
        assert(valueMatch(x0.getValue(CPos("H11")), CValue()));
        auto res = x0.getValue(CPos("H12"));
        assert(valueMatch(x0.getValue(CPos("H12")), CValue()));
        assert(valueMatch(x0.getValue(CPos("H13")), CValue(35.0)));
        assert(valueMatch(x0.getValue(CPos("H14")), CValue()));
        assert(x0.setCell(CPos("F0"), "-27"));
        assert(valueMatch(x0.getValue(CPos("H14")), CValue(-22.0)));
        x0.copyRect(CPos("H12"), CPos("H13"), 1, 2);
        assert(valueMatch(x0.getValue(CPos("H12")), CValue(25.0)));
        assert(valueMatch(x0.getValue(CPos("H13")), CValue(-22.0)));
        assert(valueMatch(x0.getValue(CPos("H14")), CValue(-22.0)));

        cout << __func__ << " ->    OK" << '\n' << endl;
    }

    static void cycleDetectionTest() {
        cout << '\n' << __func__ << " -> START" << endl;

        CSpreadsheet x3;
        assert(x3.setCell(CPos("A1"), "=5+A2"));
        assert(x3.setCell(CPos("A2"), "=A1"));
        assert(valueMatch(x3.getValue(CPos("A1")), CValue()));
        assert(x3.setCell(CPos("B1"), "42"));
        assert(x3.setCell(CPos("B2"), "42"));

        cout << __func__ << " ->    OK" << '\n' << endl;
    }

    static void loaderTest() {
        cout << '\n' << __func__ << " -> START" << endl;


        std::ostringstream oss;
        std::istringstream iss;
        std::string data;

        CSpreadsheet x0;
        assert(x0.setCell(CPos("A0"), ",s,;tr,,;ing\n\n A0,;"));
        assert(x0.setCell(CPos("A1"), ",s,;tr,,;ing\n\n A1,;"));
        assert(x0.setCell(CPos("A2"), ",s,;tr,,;ing\n\n A2,;"));
        assert(x0.setCell(CPos("A3"), ",s,;tr,,;ing\n\n A3,;"));
        assert(x0.setCell(CPos("A4"), ",s,;tr,,;ing\n\n A4,;"));
        assert(x0.setCell(CPos("B0"), ",s,;tr,,;ing\n\n B0,;"));
        assert(x0.setCell(CPos("B1"), ",s,;tr,,;ing\n\n B1,;"));
        assert(x0.setCell(CPos("B2"), ",s,;tr,,;ing\n\n B2,;"));
        assert(x0.setCell(CPos("B3"), ",s,;tr,,;ing\n\n B3,;"));
        assert(x0.setCell(CPos("B4"), ",s,;tr,,;ing\n\n B4,;"));
        assert(x0.setCell(CPos("C0"), ",s,;tr,,;ing\n\n C0,;"));
        assert(x0.setCell(CPos("C1"), ",s,;tr,,;ing\n\n C1,;"));
        assert(x0.setCell(CPos("C2"), ",s,;tr,,;ing\n\n C2,;"));
        assert(x0.setCell(CPos("C3"), ",s,;tr,,;ing\n\n C3,;"));
        assert(x0.setCell(CPos("C4"), ",s,;tr,,;ing\n\n C4,;"));

        CSpreadsheet x1;
        assert(x1.setCell(CPos("B0"), "=\",s,;tr,,;ing\n\n B0,;\""));
        assert(x1.setCell(CPos("B1"), "=\",s,;tr,,;ing\n\n B1,;\""));
        assert(x1.setCell(CPos("B2"), "=\",s,;tr,,;ing\n\n B2,;\""));
        assert(x1.setCell(CPos("B3"), "=\",s,;tr,,;ing\n\n B3,;\""));
        assert(x1.setCell(CPos("B4"), "=\",s,;tr,,;ing\n\n B4,;\""));
        assert(x1.setCell(CPos("C0"), "=\",s,;tr,,;ing\n\n C0,;\""));
        assert(x1.setCell(CPos("C1"), "=\",s,;tr,,;ing\n\n C1,;\""));
        assert(x1.setCell(CPos("C2"), "=\",s,;tr,,;ing\n\n C2,;\""));
        assert(x1.setCell(CPos("C3"), "=\",s,;tr,,;ing\n\n C3,;\""));
        assert(x1.setCell(CPos("C4"), "=\",s,;tr,,;ing\n\n C4,;\""));
        assert(x1.setCell(CPos("D0"), "=\",s,;tr,,;ing\n\n D0,;\""));
        assert(x1.setCell(CPos("D1"), "=\",s,;tr,,;ing\n\n D1,;\""));
        assert(x1.setCell(CPos("D2"), "=\",s,;tr,,;ing\n\n D2,;\""));
        assert(x1.setCell(CPos("D3"), "=\",s,;tr,,;ing\n\n D3,;\""));
        assert(x1.setCell(CPos("D4"), "=\",s,;tr,,;ing\n\n D4,;\""));

        oss.clear();
        oss.str("");
        assert(x0.save(oss));
        data = oss.str();
        iss.clear();
        iss.str(data);
        assert(x1.load(iss));

        assert(valueMatch(x0.getValue(CPos("A0")), CValue(",s,;tr,,;ing\n\n A0,;")));
        assert(valueMatch(x0.getValue(CPos("A1")), CValue(",s,;tr,,;ing\n\n A1,;")));
        assert(valueMatch(x0.getValue(CPos("A2")), CValue(",s,;tr,,;ing\n\n A2,;")));
        assert(valueMatch(x0.getValue(CPos("A3")), CValue(",s,;tr,,;ing\n\n A3,;")));
        assert(valueMatch(x0.getValue(CPos("A4")), CValue(",s,;tr,,;ing\n\n A4,;")));
        assert(valueMatch(x0.getValue(CPos("B0")), CValue(",s,;tr,,;ing\n\n B0,;")));
        assert(valueMatch(x0.getValue(CPos("B1")), CValue(",s,;tr,,;ing\n\n B1,;")));
        assert(valueMatch(x0.getValue(CPos("B2")), CValue(",s,;tr,,;ing\n\n B2,;")));
        assert(valueMatch(x0.getValue(CPos("B3")), CValue(",s,;tr,,;ing\n\n B3,;")));
        assert(valueMatch(x0.getValue(CPos("B4")), CValue(",s,;tr,,;ing\n\n B4,;")));
        assert(valueMatch(x0.getValue(CPos("C0")), CValue(",s,;tr,,;ing\n\n C0,;")));
        assert(valueMatch(x0.getValue(CPos("C1")), CValue(",s,;tr,,;ing\n\n C1,;")));
        assert(valueMatch(x0.getValue(CPos("C2")), CValue(",s,;tr,,;ing\n\n C2,;")));
        assert(valueMatch(x0.getValue(CPos("C3")), CValue(",s,;tr,,;ing\n\n C3,;")));
        assert(valueMatch(x0.getValue(CPos("C4")), CValue(",s,;tr,,;ing\n\n C4,;")));

        assert(valueMatch(x1.getValue(CPos("A0")), CValue(",s,;tr,,;ing\n\n A0,;")));
        assert(valueMatch(x1.getValue(CPos("A1")), CValue(",s,;tr,,;ing\n\n A1,;")));
        assert(valueMatch(x1.getValue(CPos("A2")), CValue(",s,;tr,,;ing\n\n A2,;")));
        assert(valueMatch(x1.getValue(CPos("A3")), CValue(",s,;tr,,;ing\n\n A3,;")));
        assert(valueMatch(x1.getValue(CPos("A4")), CValue(",s,;tr,,;ing\n\n A4,;")));
        assert(valueMatch(x1.getValue(CPos("B0")), CValue(",s,;tr,,;ing\n\n B0,;")));
        assert(valueMatch(x1.getValue(CPos("B1")), CValue(",s,;tr,,;ing\n\n B1,;")));
        assert(valueMatch(x1.getValue(CPos("B2")), CValue(",s,;tr,,;ing\n\n B2,;")));
        assert(valueMatch(x1.getValue(CPos("B3")), CValue(",s,;tr,,;ing\n\n B3,;")));
        assert(valueMatch(x1.getValue(CPos("B4")), CValue(",s,;tr,,;ing\n\n B4,;")));
        assert(valueMatch(x1.getValue(CPos("C0")), CValue(",s,;tr,,;ing\n\n C0,;")));
        assert(valueMatch(x1.getValue(CPos("C1")), CValue(",s,;tr,,;ing\n\n C1,;")));
        assert(valueMatch(x1.getValue(CPos("C2")), CValue(",s,;tr,,;ing\n\n C2,;")));
        assert(valueMatch(x1.getValue(CPos("C3")), CValue(",s,;tr,,;ing\n\n C3,;")));
        assert(valueMatch(x1.getValue(CPos("C4")), CValue(",s,;tr,,;ing\n\n C4,;")));

        assert(valueMatch(x1.getValue(CPos("D0")), CValue()));
        assert(valueMatch(x1.getValue(CPos("D1")), CValue()));
        assert(valueMatch(x1.getValue(CPos("D2")), CValue()));
        assert(valueMatch(x1.getValue(CPos("D3")), CValue()));
        assert(valueMatch(x1.getValue(CPos("D4")), CValue()));

        cout << __func__ << " ->    OK" << '\n' << endl;

    }

    static void functionsTest() {
        cout << '\n' << __func__ << " -> START" << endl;

        CSpreadsheet x0;
        assert(x0.setCell(CPos("A0"), "42"));
        assert(x0.setCell(CPos("A1"), "42"));
        assert(x0.setCell(CPos("A3"), "just a string"));
        assert(x0.setCell(CPos("A4"), "=\"just an expression string\""));
        assert(x0.setCell(CPos("A5"), "=1"));
        assert(x0.setCell(CPos("A6"), "=A0 + A1"));

        assert(valueMatch(x0.getValue(CPos("A4")), CValue("just an expression string")));

        assert(x0.setCell(CPos("A10"), "=sum(A0:A6)"));
        CValue result = x0.getValue(CPos("A10"));
        assert(valueMatch(result, CValue(169.0)));


        cout << __func__ << " ->    OK" << '\n' << endl;
    }

};

#endif //PA2_BIG_TASK_CTESTER_H

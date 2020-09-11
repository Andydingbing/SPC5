#ifndef RD_TYPES_H
#define RD_TYPES_H

/*
 * RD(RF Driver) Types.
 *
 * 1, All types begin with the prefix "RD_".
 * 2, All enum variables begin with the prefix "RD_".
 * 3, Project related types or variables begin with "RD_##Project_".
 */

enum RD_IO_Mode {
    RD_IO,
    RD_Output,
    RD_Loop,
    RD_Input,
    RD_Close
};

enum RD_TX_Source {
    RD_Phy,
    RD_ARB,
    RD_CW
};

#endif

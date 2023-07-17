#include "geometry.h"
#include <string.h>
#include "utils.h"

const char* place_to_string(enum color_t c, enum sort_t s) {
    switch (c) {
        case WHITE:
            switch (s) {
                case PAWN:
                    return "♙";
                    break;
                case TOWER:
                    return "♖";
                    break;
                case ELEPHANT:
                    return "♘";
                    break;
                // Autres pions
                default:
                    return "•";
                    break;
            }
            break;
        case BLACK:
            switch (s) {
                case PAWN:
                    return "♟";
                    break;
                case TOWER:
                    return "♜";
                    break;
                case ELEPHANT:
                    return "♞";
                    break;
                // Autres pions
                default:
                    return "•";
                    break;
            }
            break;
        default:
            return "•";
            break;
    }

    return "•";
}

const char* dir_to_string(enum dir_t d) {
    switch (d) {
        case NO_DIR:
            return "No direction";
        case EAST:
            return "East";
        case NEAST:
            return "North east";
        case NORTH:
            return "North";
        case NWEST:
            return "North west";
        case WEST:
            return "West";
        case SWEST:
            return "South west";
        case SOUTH:
            return "South";
        case SEAST:
            return "South east";
        case MAX_DIR: // To handle warning
            break;
    }
    return "Unknown";
}

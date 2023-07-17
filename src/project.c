#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "game.h"

struct user_args {
    int rdm_seed;
    int max_turns;
    int seed_neighbors;
    int config_id;
    enum game_mode game_mode;
};

void usage() {
    printf("Basic usage : ./project\n");
    printf("With parameters :\n");
    printf("Parameters -h or -u : Help/Usage\n");
    printf("Parameter -s : Initialization of random seed\n");
    printf("Parameter -n : Init neighbors seed\n");
    printf("Parameter -m : Max turns\n");
    printf("Parameter -t : s to simple win | c to complex win\n");
}

void display_game_summary(game g) {
    game_next_player(g);
    if (game_get_winning_status(g) == DRAW)
        printf("The game ends in a tie\n");
    else {
        printf("The game ends with a %s victory. Player %d wins.\n",
               game_get_winning_status(g) == SIMPLE_WIN ? "simple" : "complex",
               game_get_current_player(g));
    }
}

void display_game_parameters(uint p, uint ns, int seed, uint in) {
    printf("Game parameters :\n- Piece number : %d\n- Number of players : %d\n- Seed : %d\n- Init neighbors seed : %d\n\n",
           ns,
           p,
           seed,
           in);
}

int main(int argc, char* argv[]) {
    int opt;
    game g;

    struct user_args ua = {-1, -1, get_random_number(MAX_RELATIONS), -1, SIMPLE_WIN};

    while ((opt = getopt(argc, argv, "s:m:t:hun:c:")) != -1) {
        switch (opt) {
            case 'h':
            case 'u':
                usage();
                return EXIT_SUCCESS;
            case 's':
                ua.rdm_seed = atoi(optarg);
                break;
            case 'n':
                ua.seed_neighbors = atoi(optarg);
                break;
            case 'm':
                ua.max_turns = atoi(optarg);
                break;
            case 'c':
                ua.config_id = atoi(optarg);
                break;
            case 't':
                if (!strcmp(optarg, "c"))
                    ua.game_mode = COMPLEX_WIN;
                if (!strcmp(optarg, "s"))
                    ua.game_mode = SIMPLE_WIN;
                break;
            default:
                break;
        }
    }

    int rdm_seed = time(NULL);
    srand(ua.rdm_seed != -1 ? ua.rdm_seed : rdm_seed);

    init_neighbors(ua.seed_neighbors);
    struct ens init_p = ens_create(UINT_MAX);

    if (ua.config_id == -1)
        g = game_new(ua.game_mode, ua.max_turns, -1, true, init_p);
    else
        g = game_new_with_config(game_get_configuration(ua.config_id));

    display_game_parameters(MAX_PLAYERS, HEIGHT, ua.rdm_seed != -1 ? ua.rdm_seed : rdm_seed, get_neighbors_seed());

    while (!game_is_over(g)) {
        game_move_random(g);
        game_next_player(g);
    }

    display_game_summary(g);

    game_delete(g);

    return EXIT_SUCCESS;
}

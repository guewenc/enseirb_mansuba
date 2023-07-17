#include "include/colors.h"
#include "include/tst_ens.h"
#include "include/tst_game.h"
#include "include/tst_geometry.h"
#include "include/tst_neighbors.h"
#include "include/tst_position.h"
#include "include/tst_world.h"
#include "include/utils.h"

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int test_passed = 0;
int total_tests = 0;

void running_tests() { TRACE(TST_CYAN "%s\n" TST_RESET, "Running tests..."); }

void print_success() { TRACE(TST_GREEN "%s\n" TST_RESET, "Test passed !"); }

void print_failed() { TRACE(TST_RED "%s\n" TST_RESET, "Test failed !"); }

// On lance le programme dans un processus différent pour chaque fonction pour
// avoir un environnement vierge à chaque exécution
void launch_tests(tests t) {
    TRACE("%s", "\n");
    for (int i = 0; i < t.test_count; i++) {
        int pid = fork();
        if (!pid) {
            TRACE(">> Run test on [%s] ... ", t.list[i].test_name);
            fclose(stderr);
            fclose(stdout);
            t.list[i].f();
            exit(EXIT_SUCCESS);
        }

        int status;
        if (waitpid(pid, &status, 0) != -1) {
            if (WIFEXITED(status)) {
                int es = WEXITSTATUS(status);
                if (es == EXIT_SUCCESS) {
                    print_success();
                    test_passed++;
                } else {
                    print_failed();
                }
            } else {
                print_failed();
            }
        }
        total_tests++;
    }
}

void print_summary() {
    TRACE("%s", "\n");
    if (test_passed == total_tests)
        TRACE(TST_GREEN "100%% %s" TST_RESET, "tests passed");
    else {
        TRACE(TST_RED "%d%s test passed" TST_RESET,
              (int)(test_passed * 100) / total_tests,
              "%");
    }
    TRACE(", %d tests failed out of %d.\n", total_tests - test_passed, total_tests);
}

int main() {
    running_tests();

    tests tst_list[TESTS_CAT_COUNT] = {
        tst_world_list(),
        tst_position_list(),
        tst_neighbors_list(),
        tst_geometry_list(),
        tst_ens_list(),
        tst_game_list()};

    for (int i = 0; i < TESTS_CAT_COUNT; i++)
        launch_tests(tst_list[i]);

    print_summary();

    return test_passed == total_tests ? EXIT_SUCCESS : EXIT_FAILURE;
}

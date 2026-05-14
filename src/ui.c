#include <stdio.h>
#include <stdlib.h>
#include "pardus_health_check.h"

/* Define Pardus ASCII Art Colors */
#define LOGO_COLOUR_1 MAGENTA_FG
#define LOGO_COLOUR_2 CYAN_FG

#define C(c, text) c text RESET

void ui_print_banner(void) {
    printf("\n");
    printf("%s┓ ┏┓┏┓┏┓┏┓┳┓┳┓    \n",LOGO_COLOUR_1);
    printf("%s┃ ┣ ┃┃┃┃┣┫┣┫┃┃    \n",LOGO_COLOUR_2);
    printf("%s┗┛┗┛┗┛┣┛┛┗┛┗┻┛    \n",LOGO_COLOUR_1);
    printf("%s┓ ┏┏┓┏┳┓┏┓┓┏┳┓┏┓┏┓\n",LOGO_COLOUR_2);
    printf("%s┃┃┃┣┫ ┃ ┃ ┣┫┃┃┃┃┃┓\n",LOGO_COLOUR_1);
    printf("%s┗┻┛┛┗ ┻ ┗┛┛┗┻┛┗┛┗┛%s\n",LOGO_COLOUR_2,RESET);
    printf("\n");
}

void ui_print_header(const char *title) {
    printf("%s%s╔═══════════════════════════════════════════════════════════╗%s\n",RESET, BOLD, RESET);
    printf("%s║%s  %-55s  %s%s║%s\n", BOLD, BLUE_FG, title, RESET, BOLD, RESET);
    printf("%s╚═══════════════════════════════════════════════════════════╝%s\n", BOLD, RESET);
}

void ui_print_module_start(const char *module) {
    printf("%s▶%s %s\n", BLUE_FG, RESET, module);
}

void ui_print_progress(const char *message, int percent) {
    printf("\r%s[", BLUE_FG);
    for (int i = 0; i < 30; i++) {
        if (i < (percent * 30 / 100)) {
            printf("█");
        } else {
            printf("░");
        }
    }
    printf("] %d%% - %s%s", percent, message, RESET);
    fflush(stdout);
}

void ui_print_spacing(void) {
    printf("\n");
}

void ui_print_help(const char *program_name) {
    printf("\n%sUsage:%s %s [OPTIONS]\n\n", BOLD, RESET, program_name);
    printf("%sOptions:%s\n", BOLD, RESET);
    printf("  -h, --help           Show this help message\n");
    printf("  -v, --verbose        Enable verbose logging\n");
    printf("  -q, --quiet          Disable verbose logging\n");
    printf("  -c, --cli            Output as colored CLI (default)\n");
    printf("  -j, --json FILE      Output as JSON to FILE\n");
    printf("  -w, --html FILE      Output as HTML to FILE\n");
    printf("  -a, --all            Run all checks\n");
    printf("  -p, --packages       Check packages only\n");
    printf("  -n, --network        Check open ports only\n");
    printf("  -d, --hardware       Check hardware only\n");
    printf("  -s, --system         Check system health only\n");
    printf("\n%sExamples:%s\n", BOLD, RESET);
    printf("  %s --all                    # Full system check\n", program_name);
    printf("  %s --json report.json       # Save report as JSON\n", program_name);
    printf("  %s --html report.html       # Save report as HTML\n", program_name);
    printf("\n");
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "pardus_health_check.h"

static const char *risk_string(RiskLevel_t level) {
    switch (level) {
        case RISK_CRITICAL: return "CRITICAL";
        case RISK_HIGH: return "HIGH";
        case RISK_MEDIUM: return "MEDIUM";
        case RISK_LOW: return "LOW";
        case RISK_INFO: return "INFO";
        default: return "UNKNOWN";
    }
}

inline static const char *risk_color(RiskLevel_t level) {
    switch (level) {
        case RISK_CRITICAL: return RED_FG;
        case RISK_HIGH: return RED_FG;
        case RISK_MEDIUM: return YELLOW_FG;
        case RISK_LOW: return YELLOW_FG;
        case RISK_INFO: return BLUE_FG;
        default: return RESET;
    }
}

/* CLI Report - human readable with colors */
int generate_cli_report(Finding_t *findings, int count, HealthScore_t *score) {
    printf("\n%s═-═-═-═-═-═-═-═-═-═-═-═-═-═-═-═-═-═-═-═-═-═-═-═-═-═-═-═-═-═-═-%s\n", 
           BOLD, RESET);
    printf("%s                PARDUS HEALTH CHECK REPORT                   %s\n", 
           BOLD, RESET);
    printf("%s═-═-═-═-═-═-═-═-═-═-═-═-═-═-═-═-═-═-═-═-═-═-═-═-═-═-═-═-═-═-═-%s\n\n", 
           BOLD, RESET);
    
    /* Overall Score */
    printf("%sOVERALL SECURITY SCORE: %.1f/100%s\n", BOLD, score->overall_score, RESET);
    
    if (score->overall_score >= 80) {
        printf("%s✓ System is in GOOD condition%s\n\n", GREEN_FG, RESET);
    } else if (score->overall_score >= 60) {
        printf("%s⚠ System needs attention%s\n\n", YELLOW_FG, RESET);
    } else {
        printf("%s✗ CRITICAL ISSUES DETECTED%s\n\n", RED_FG, RESET);
    }
    
    /* Score Breakdown */
    printf("%sScore Breakdown:%s\n", BOLD, RESET);
    printf("  Package Health:  %.1f/100\n", score->package_score);
    printf("  Network Safety:  %.1f/100\n", score->network_score);
    printf("  Hardware:        %.1f/100\n", score->hardware_score);
    printf("  System Health:   %.1f/100\n\n", score->system_score);
    
    /* Finding Summary */
    printf("%sFinding Summary:%s\n", BOLD, RESET);
    printf("  %sCritical: %d%s  ", RED_FG, score->critical_count, RESET);
    printf("  %sHigh: %d%s  ", RED_FG, score->high_count, RESET);
    printf("  %sMedium: %d%s  ", YELLOW_FG, score->medium_count, RESET);
    printf("  %sLow: %d%s\n\n", YELLOW_FG, score->low_count, RESET);
    
    /* Detailed Findings */
    if (count > 0) {
        printf("%s─────────────────────────────────────────────────────────────────%s\n", 
               BOLD, RESET);
        printf("%sDetailed Findings:%s\n", BOLD, RESET);
        printf("%s─────────────────────────────────────────────────────────────────%s\n\n", 
               BOLD, RESET);
        
        for (int i = 0; i < count; i++) {
            printf("%s[%s%s%s] %s\n", BOLD, risk_color(findings[i].severity), 
                   risk_string(findings[i].severity), RESET, findings[i].title);
            printf("Description: %s\n", findings[i].description);
            printf("Recommendation: %s\n\n", findings[i].recommendation);
        }
    } else {
        printf("%sNo findings! System appears healthy.%s\n\n", GREEN_FG, RESET);
    }
    
    printf("%s═══════════════════════════════════════════════════════════════%s\n", 
           BOLD, RESET);
    time_t now = time(NULL);
    printf("Report generated: %s", ctime(&now));
    printf("%s═══════════════════════════════════════════════════════════════%s\n\n", 
           BOLD, RESET);
    
    return 0;
}

/* JSON Report - machine readable */
int generate_json_report(Finding_t *findings, int count, HealthScore_t *score, 
                        const char *output_file) {
    FILE *f = fopen(output_file, "w");
    if (!f) {
        LOG_msg(RISK_HIGH, "Could not open file for JSON report: %s", output_file);
        return -1;
    }
    
    fprintf(f, "{\n");
    fprintf(f, "  \"timestamp\": %ld,\n", time(NULL));
    fprintf(f, "  \"overall_score\": %.1f,\n", score->overall_score);
    fprintf(f, "  \"scores\": {\n");
    fprintf(f, "    \"package\": %.1f,\n", score->package_score);
    fprintf(f, "    \"network\": %.1f,\n", score->network_score);
    fprintf(f, "    \"hardware\": %.1f,\n", score->hardware_score);
    fprintf(f, "    \"system\": %.1f\n", score->system_score);
    fprintf(f, "  },\n");
    fprintf(f, "  \"summary\": {\n");
    fprintf(f, "    \"total_findings\": %d,\n", count);
    fprintf(f, "    \"critical\": %d,\n", score->critical_count);
    fprintf(f, "    \"high\": %d,\n", score->high_count);
    fprintf(f, "    \"medium\": %d,\n", score->medium_count);
    fprintf(f, "    \"low\": %d\n", score->low_count);
    fprintf(f, "  },\n");
    fprintf(f, "  \"findings\": [\n");
    
    for (int i = 0; i < count; i++) {
        fprintf(f, "    {\n");
        fprintf(f, "      \"severity\": \"%s\",\n", risk_string(findings[i].severity));
        fprintf(f, "      \"title\": \"%s\",\n", findings[i].title);
        fprintf(f, "      \"description\": \"%s\",\n", findings[i].description);
        fprintf(f, "      \"recommendation\": \"%s\"\n", findings[i].recommendation);
        fprintf(f, "    }%s\n", i < count - 1 ? "," : "");
    }
    
    fprintf(f, "  ]\n");
    fprintf(f, "}\n");
    
    fclose(f);
    LOG_msg(RISK_INFO, "JSON report saved to: %s", output_file);
    return 0;
}

/* HTML Report - web viewable */
int generate_html_report(Finding_t *findings, int count, HealthScore_t *score, 
                        const char *output_file) {
    FILE *f = fopen(output_file, "w");
    if (!f) {
        LOG_msg(RISK_HIGH, "Could not open file for HTML report: %s", output_file);
        return -1;
    }
    
    fprintf(f, "<!DOCTYPE html>\n<html>\n<head>\n");
    fprintf(f, "<meta charset='utf-8'>\n");
    fprintf(f, "<title>Pardus Health Check Report</title>\n");
    fprintf(f, "<style>\n");
    fprintf(f, "body { font-family: Arial, sans-serif; margin: 20px; background: #89d6ff; }\n");
    fprintf(f, ".container { max-width: 900px; margin: 0 auto; background: white; padding: 20px; border-radius: 8px; box-shadow: 0 2px 4px rgba(0,0,0,0.1); }\n");
    fprintf(f, ".header { text-align: center; border-bottom: 3px solid #333; padding-bottom: 20px; }\n");
    fprintf(f, ".score { font-size: 32px; font-weight: bold; margin: 10px 0; }\n");
    fprintf(f, ".score-good { color: #27ae60; }\n");
    fprintf(f, ".score-medium { color: #f39c12; }\n");
    fprintf(f, ".score-bad { color: #e74c3c; }\n");
    fprintf(f, ".finding { margin: 15px 0; padding: 15px; border-left: 4px solid; border-radius: 4px; }\n");
    fprintf(f, ".critical { border-left-color: #e74c3c; background: #fadbd8; }\n");
    fprintf(f, ".high { border-left-color: #e74c3c; background: #fadbd8; }\n");
    fprintf(f, ".medium { border-left-color: #f39c12; background: #fef5e7; }\n");
    fprintf(f, ".low { border-left-color: #f39c12; background: #fef5e7; }\n");
    fprintf(f, ".info { border-left-color: #3498db; background: #ebf5fb; }\n");
    fprintf(f, ".finding-title { font-weight: bold; font-size: 16px; margin-bottom: 5px; }\n");
    fprintf(f, ".finding-desc { margin: 5px 0; color: #555; }\n");
    fprintf(f, ".finding-rec { margin: 5px 0; color: #27ae60; font-style: italic; }\n");
    fprintf(f, "</style>\n");
    fprintf(f, "</head>\n<body>\n");
    fprintf(f, "<div class='container'>\n");
    fprintf(f, "<div class='header'>\n");
    fprintf(f, "<h1>Pardus System Health Check Report</h1>\n");
    fprintf(f, "<div class='score ");
    
    if (score->overall_score >= 80) {
        fprintf(f, "score-good");
    } else if (score->overall_score >= 60) {
        fprintf(f, "score-medium");
    } else {
        fprintf(f, "score-bad");
    }
    
    fprintf(f, "'>%.1f / 100</div>\n", score->overall_score);
    fprintf(f, "<p>Generated: %s</p>\n", ctime(&(time_t){time(NULL)}));
    fprintf(f, "</div>\n");
    
    fprintf(f, "<h2>Score Breakdown</h2>\n");
    fprintf(f, "<ul>\n");
    fprintf(f, "<li>Package Health: %.1f</li>\n", score->package_score);
    fprintf(f, "<li>Network Safety: %.1f</li>\n", score->network_score);
    fprintf(f, "<li>Hardware: %.1f</li>\n", score->hardware_score);
    fprintf(f, "<li>System Health: %.1f</li>\n", score->system_score);
    fprintf(f, "</ul>\n");
    
    fprintf(f, "<h2>Findings (%d total)</h2>\n", count);
    
    for (int i = 0; i < count; i++) {
        fprintf(f, "<div class='finding %s'>\n", 
               findings[i].severity == RISK_CRITICAL ? "critical" :
               findings[i].severity == RISK_HIGH ? "high" :
               findings[i].severity == RISK_MEDIUM ? "medium" :
               findings[i].severity == RISK_LOW ? "low" : "info");
        fprintf(f, "<div class='finding-title'>%s</div>\n", findings[i].title);
        fprintf(f, "<div class='finding-desc'>%s</div>\n", findings[i].description);
        fprintf(f, "<div class='finding-rec'>→ %s</div>\n", findings[i].recommendation);
        fprintf(f, "</div>\n");
    }
    
    fprintf(f, "</div>\n</body>\n</html>\n");
    fclose(f);
    LOG_msg(RISK_INFO, "HTML report saved to: %s", output_file);
    return 0;
}

int generate_report(Finding_t *findings, int count, HealthScore_t *score, 
                   ReportFormat_t format, const char *output_file) {
    
    /* Calculate overall score */
    score->overall_score = (score->package_score + score->network_score + 
                           score->hardware_score + score->system_score) / 4.0f;
    
    switch (format) {
        case REPORT_CLI:
            return generate_cli_report(findings, count, score);
        case REPORT_JSON:
            return generate_json_report(findings, count, score, output_file);
        case REPORT_HTML:
            return generate_html_report(findings, count, score, output_file);
        default:
            return -1;
    }
}

// NX Portal - Web Injection Test
// Tests launching the system browser to two different URLs (Google
// and Xbox Cloud Gaming) to figure out whether "This feature is not
// available" is a general whitelist/config issue, or something
// specific to xbox.com refusing to run in this browser.

#include <switch.h>
#include <cstdio>
#include <cstring>

static void launchBrowser(const char* url, const char* label) {
    WebCommonConfig config;
    WebCommonReply reply;

    printf("\nLaunching browser to %s...\n", label);
    consoleUpdate(NULL);

    Result rc = webPageCreate(&config, url);
    if (R_FAILED(rc)) {
        printf("webPageCreate failed: 0x%x\n", rc);
        consoleUpdate(NULL);
        return;
    }

    webConfigSetJsExtension(&config, true);
    webConfigSetPageCache(&config, true);
    webConfigSetBootLoadingIcon(&config, true);
    webConfigSetWhitelist(&config, ".*");

    rc = webConfigShow(&config, &reply);
    if (R_FAILED(rc)) {
        printf("webConfigShow failed: 0x%x\n", rc);
    } else {
        printf("Browser closed normally after showing %s.\n", label);
    }
    consoleUpdate(NULL);
}

int main(int argc, char* argv[]) {
    consoleInit(NULL);

    PadState pad;
    padConfigureInput(1, HidNpadStyleSet_NpadStandard);
    padInitializeDefault(&pad);

    bool running = true;
    while (appletMainLoop() && running) {
        padUpdate(&pad);
        u64 kDown = padGetButtonsDown(&pad);

        consoleClear();
        printf("NX Portal - Web Injection Test\n");
        printf("--------------------------------\n");
        printf("A: Test Google (control)\n");
        printf("X: Test Xbox Cloud Gaming\n");
        printf("+: Exit\n");
        consoleUpdate(NULL);

        if (kDown & HidNpadButton_Plus) {
            running = false;
            break;
        }

        if (kDown & HidNpadButton_A) {
            launchBrowser("https://www.google.com", "Google (control test)");
        }

        if (kDown & HidNpadButton_X) {
            launchBrowser("https://www.xbox.com/play", "Xbox Cloud Gaming");
        }
    }

    consoleExit(NULL);
    return 0;
}

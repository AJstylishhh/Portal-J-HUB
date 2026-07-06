// NX Portal - Web Injection Test
// Minimal test: launch the system browser to xbox.com/play and try
// injecting a harmless test script via the JsExtension flag.
//
// Goal: confirm whether libnx's WebCommonConfig JsExtension actually
// works for general (non-offline) web browsing on real hardware.
// If the alert box below appears when the page loads, injection works
// and we can move on to injecting the real better-xcloud script.

#include <switch.h>
#include <cstdio>
#include <cstring>

int main(int argc, char* argv[]) {
    consoleInit(NULL);

    printf("NX Portal - Web Injection Test\n");
    printf("--------------------------------\n");
    printf("Launching browser to xbox.com/play...\n");
    printf("Press A to launch, + to exit without launching.\n\n");

    PadState pad;
    padConfigureInput(1, HidNpadStyleSet_NpadStandard);
    padInitializeDefault(&pad);

    bool launched = false;
    while (appletMainLoop() && !launched) {
        padUpdate(&pad);
        u64 kDown = padGetButtonsDown(&pad);

        if (kDown & HidNpadButton_Plus) {
            consoleExit(NULL);
            return 0;
        }

        if (kDown & HidNpadButton_A) {
            WebCommonConfig config;
            WebCommonReply reply;

            Result rc = webPageCreate(&config, "https://www.xbox.com/play");
            if (R_FAILED(rc)) {
                printf("webPageCreate failed: 0x%x\n", rc);
                consoleUpdate(NULL);
                launched = true;
                break;
            }

            // Try enabling JS extension / injection support.
            // NOTE: this may not be supported outside the offline/local
            // web applet mode - this test exists specifically to find out.
            webConfigSetJsExtension(&config, true);

            // Simple test payload: just show an alert so we can visually
            // confirm on-device whether injected JS actually executed.
            const char* testScript =
                "alert('NX Portal injection test OK');";

            // Some libnx versions expose webConfigSetInjectionScript or
            // similar - if this call doesn't exist in your libnx version,
            // comment it out and just test whether the page loads at all
            // first.
            // webConfigSetInjectionScript(&config, testScript);
            (void)testScript;

            webConfigSetWhitelist(&config, "^https?://.*");

            rc = webConfigShow(&config, &reply);
            if (R_FAILED(rc)) {
                printf("webConfigShow failed: 0x%x\n", rc);
            } else {
                printf("Browser closed normally.\n");
            }

            launched = true;
        }

        consoleUpdate(NULL);
    }

    printf("\nDone. Press + to exit.\n");
    while (appletMainLoop()) {
        padUpdate(&pad);
        u64 kDown = padGetButtonsDown(&pad);
        if (kDown & HidNpadButton_Plus) break;
        consoleUpdate(NULL);
    }

    consoleExit(NULL);
    return 0;
}
